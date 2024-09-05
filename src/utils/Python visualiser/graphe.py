import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import random
from matplotlib.patches import Circle, RegularPolygon
import sys
import math

def couleur_aleatoire():
    return (random.randint(0, 255) / 255, random.randint(0, 255) / 255, random.randint(0, 255) / 255)

class Graphe:
    def __init__(self):
        self.noeuds = {}  # {"1" : {1, 2}}
        self.lignes = []
        self.depot_node = 0
        self.matrice_temps = []

    def ajouter_noeud(self, identifiant, x, y):   # l'identifiant (commence à 1) du noeud et ses coordonnées 
        self.noeuds[int(identifiant)] = {'x': x, 'y': y}

    def ajouter_ligne(self, freq, start_time, end_time, noeuds):
        self.lignes.append({'freq': freq, 'start_time': start_time, 'end_time': end_time, 'noeuds': noeuds}) 
        
    def ajouter_matrice(self, temps):
        self.matrice_temps.append(temps)
        
    def get_distance(self, id_noeud1, id_noeud2):
        index_noeud1 = id_noeud1 - 1
        index_noeud2 = id_noeud2 - 1
        distance = self.matrice_temps[index_noeud1][index_noeud2]
        return distance
    
    def get_coordonnees(self, identifiant):
        if identifiant in self.noeuds:
            return self.noeuds[identifiant]['x'], self.noeuds[identifiant]['y']
        else:
            raise ValueError(f"Le noeud avec l'identifiant {identifiant} n'existe pas.")   
        
    def lire_fichier(self, chemin_fichier):
        with open(chemin_fichier, 'r') as fichier:
            bool_noeuds = False 
            bool_ligne = False
            bool_matrice = False
            lignes_a_sauter = 0

            for ligne in fichier:
                ligne = ligne.strip()  # Supprime les espaces blancs
                if lignes_a_sauter > 0:
                    lignes_a_sauter -= 1
                    continue
                if ligne.startswith("#Params"):
                    lignes_a_sauter = 1
                    continue
                if ligne.startswith("#Nodes format"):
                    bool_noeuds = True
                    bool_ligne = False
                    bool_matrice = False
                    continue
                if ligne.startswith("#Matrix"):
                    bool_noeuds = False 
                    bool_ligne = False
                    bool_matrice = True
                if ligne.startswith("#PT line format"):
                    bool_noeuds = False
                    bool_ligne = True
                    bool_matrice = False
                    continue 
                if ligne.startswith("#Depot node index"):
                    bool_noeuds = False
                    bool_ligne = False
                    bool_matrice = False
                    self.depot_node = int(fichier.readline().strip())
                    continue 
                
                if bool_ligne:
                    parties = ligne.split(',') 
                    parties = [str(x) for x in parties]
                    freq = parties[0]
                    start_time = parties[1]
                    end_time = parties[2]
                    noeuds = parties[3:]
                    self.ajouter_ligne(freq, start_time, end_time, noeuds)

                if bool_noeuds:
                    _, x, y = ligne.split(',')   
                    identifiant = len(self.noeuds) + 1  # commence à 1 
                    self.ajouter_noeud(identifiant, int(x), int(y))
                    
                if bool_matrice:
                    for i in range(len(self.noeuds)):
                        ligne_matrice = list(map(int, fichier.readline().strip().split(',')))
                        self.ajouter_matrice(ligne_matrice)
                        continue
                        
    def tracer_noeuds(self, fig, ax):
        for identifiant, coordonnees in self.noeuds.items():
            x = coordonnees['x']
            y = coordonnees['y']
            if identifiant == self.depot_node + 1: # tracer le noeud de dépôt avec un triangle noir
                triangle = RegularPolygon((x, y), numVertices=3, radius=0.1, color='black')
                ax.add_patch(triangle)
            else:
                ax.scatter(x, y, color='blue', label=f'Noeud {identifiant}')   
                circle = Circle((x, y), radius=0.1, color="red")   # pour que les stations soient représentées par des ronds sur la figure 
                ax.add_patch(circle)
                
                ax.annotate(f'Noeud {identifiant}', (x, y))
                
        return fig, ax
    
    def tracer_lignes(self, fig, ax):
        for ligne in self.lignes:
            liste_noeuds_ligne = list(map(int, ligne["noeuds"]))
            couleur_aleatoire = couleur_aleatoire()
            for i in range(len(liste_noeuds_ligne) - 1): 
                noeud_depart = int(liste_noeuds_ligne[i])
                noeud_arrive = int(liste_noeuds_ligne[i + 1])
                x_depart = self.noeuds[noeud_depart]['x']
                y_depart = self.noeuds[noeud_depart]['y']
                x_arrive = self.noeuds[noeud_arrive]['x']
                y_arrive = self.noeuds[noeud_arrive]['y']
                temps = self.matrice_temps[noeud_depart - 1][noeud_arrive - 1] 
                ax.plot([x_depart, x_arrive], [y_depart, y_arrive], color=couleur_aleatoire)
                milieu_x = (x_depart + x_arrive) / 2
                milieu_y = (y_depart + y_arrive) / 2
                
          
                if x_depart < x_arrive:
                    h = 'top'  
                else:
                    h = 'left'
                ax.text(milieu_x, milieu_y, f"{temps}", ha=h, va='center')
                
        return fig, ax
    
    def tracer_arrete(self, ax, x1, y1, x2, y2, couleur):
        ax.plot([x1, x2], [y1, y2], color=couleur)
                        
    def sauvegarder_graphe(self):
        fig, ax = plt.subplots() 
        
        fig, ax = tracer_noeuds(fig, ax)
        fig, ax = tracer_lignes(fig, ax)
    
        # pour que la figure contienne toutes les stations (que ça ne dépasse pas) : 
        min_x = min(coordonnees['x'] for coordonnees in self.noeuds.values())
        max_x = max(coordonnees['x'] for coordonnees in self.noeuds.values())
        min_y = min(coordonnees['y'] for coordonnees in self.noeuds.values())
        max_y = max(coordonnees['y'] for coordonnees in self.noeuds.values())
        ax.set_xlim(min_x - 1, max_x + 1) 
        ax.set_ylim(min_y - 1, max_y + 1)

        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.set_title('Graphe')
        fig.savefig("graphe.png")
        plt.close(fig)
        
    def afficher_graphe(self):
        print("Les stations :")
        for identifiant, coordonnees in self.noeuds.items():
            print(f"Identifiant : {identifiant}, Coordonnées : ({coordonnees['x']}, {coordonnees['y']})")

        print("\nLes lignes :")
        for ligne in self.lignes:
            print(f"Fréquence : {ligne['freq']} min, Début : {ligne['start_time']} min, Fin : {ligne['end_time']} min, Stations : {ligne['noeuds']}")

        if self.depot_node is not None:
            print(f"\nNoeud de dépôt : {self.depot_node}")
            