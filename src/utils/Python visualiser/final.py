from graphe import Graphe
from voiture import lire_fichier_routes, afficher_routes, tracer_routes, recuperer_identifiants_noeuds
import matplotlib.pyplot as plt
import random
import numpy as np

def couleur_aleatoire():
    return (random.randint(0, 255) / 255, random.randint(0, 255) / 255, random.randint(0, 255) / 255)

def afficher_routes_et_graphe(graphe, routes):
    fig, ax = plt.subplots()
    ax.axis('off')
    
    graphe.tracer_noeuds(fig, ax)

    liste_identifiants_noeuds = recuperer_identifiants_noeuds(routes)
    liste_coordonnees = [] 
    for route in liste_identifiants_noeuds:
        route_coords = []
        for identifiant in route:
            x, y = graphe.get_coordonnees(identifiant + 1)
            route_coords.append((x, y))
        liste_coordonnees.append(route_coords)
        
    rayon = 0.1 
    for route in liste_coordonnees:
        couleur = couleur_aleatoire()
        for i in range(len(route) - 1):  
            x_start, y_start = route[i][0], route[i][1]
            x_end, y_end = route[i + 1][0], route[i + 1][1]
            
            """
            dx = x_end - x_start
            dy = y_end - y_start
            distance = np.sqrt(dx**2 + dy**2)
            if distance != 0:
                cosinus = dx = distance
                dy /= distance
                
            # normaliser le vecteur = le même vecteur mais avec une norme 1 
            
            x_start_adj = x_start + dx * rayon  # on ajoute au début de la flèche le début du vecteur 
            y_start_adj = y_start + dy * rayon
            x_end_adj = x_end - dx * rayon  # idem mais on soustrait 
            y_end_adj = y_end - dy * rayon 
            """
            ax.annotate("", xy=(x_end, y_end), xytext=(x_start, y_start), arrowprops=dict(arrowstyle="->", color=couleur))
            
            # xy=(x_end, y_end) = position de la pointe de la flèche.
            # xytext=(x_start, y_start) = position de la base de la flèche
            
    ax.set_title('Réseau de transport et routes')
    fig.savefig("graphe_avec_routes.png")
    plt.close(fig)
