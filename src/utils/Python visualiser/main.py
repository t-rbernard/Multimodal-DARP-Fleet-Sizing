from graphe import Graphe
from voiture import lire_fichier_routes, afficher_routes, tracer_routes, recuperer_identifiants_noeuds
from final import afficher_routes_et_graphe

def main():
    graphe = Graphe()
    
    graphe.lire_fichier("graph.txt")
    routes = lire_fichier_routes("saev_routes.out")

    afficher_routes_et_graphe(graphe, routes)

main()
