import matplotlib.pyplot as plt
import random
import math
import time

from graphe import Graphe

def couleur_aleatoire():
    return (random.randint(0, 255) / 255, random.randint(0, 255) / 255, random.randint(0, 255) / 255)

def lire_fichier_routes(chemin_fichier):
    routes = []
    with open(chemin_fichier, 'r') as fichier:
        for ligne in fichier:
            if ligne.startswith("#SAEV Route format"):
                continue
            points_passage = ligne.strip().split(';')
            route = []
            for point in points_passage:
                contenu = point.strip('()').split(',')
                identifiant_noeud = contenu[0]
                identifiant_requete = contenu[1]
                fenetre_temps_min = contenu[2]
                fenetre_temps_max = contenu[3]
                route.append((identifiant_noeud, identifiant_requete, fenetre_temps_min, fenetre_temps_max))
            routes.append(route)
    return routes

def afficher_routes(routes):
    for i in range(len(routes)):
        route = routes[i]
        for point in route:
            identifiant_noeud = int(point[0]) + 1
            fenetre_temps_min = point[2]
            fenetre_temps_max = point[3]
            print(f"Noeud {identifiant_noeud} Fenêtre de temps ({fenetre_temps_min}, {fenetre_temps_max})")
        print()

def tracer_routes(routes, graphe, chemin_fichier):
    fig, ax = plt.subplots()
    ax.axis('off')
    for i in range(len(routes)):
        route = routes[i]
        for j in range(len(route)):    # j est un point de passage 
            noeud, id_requete, tw_min, tw_max = route[j]

            if id_requete:  # Utilisation d'une graine aléatoire spécifique pour les couleurs
                random.seed(int(id_requete))
            else:
                random.seed(int(time.time()))  # Utilisation du timestamp actuel comme graine aléatoire

            couleur_aleatoire = couleur_aleatoire()

            noeud = int(noeud)
            label = f"({tw_min},{tw_max})"
            ax.scatter(j, i+1, color=couleur_aleatoire, s=10)
            ax.text(j, i+1 - 0.2, label, ha='center', va='top', fontsize=8)
            ax.text(j, i+1 + 0.1, f"Nœud {noeud}", ha='center', va='bottom', fontsize=8)
            
            if j < len(route) - 1:
                noeud_suivant = int(route[j + 1][0])   # point de passage suivant 
                distance = graphe.get_distance(noeud+1, noeud_suivant+1)
                milieu_x = (j + (j + 1)) / 2
                milieu_y = (i + 1 + (i + 1)) / 2
                ax.text(milieu_x, milieu_y - 0.1, f"{distance:.2f}", ha='center', va='center', fontsize=8)
                ax.plot([j, j+1], [i+1, i+1], color='black')

    fig.savefig(chemin_fichier)
    plt.close(fig)
    
def recuperer_identifiants_noeuds(routes):
    identifiants_noeuds = []
    for route in routes:
        noeuds_route=[]
        for point in route:
            noeuds_route.append(int(point[0]))
        identifiants_noeuds.append(noeuds_route)
    return identifiants_noeuds
