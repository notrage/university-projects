#include "terrain.h"
#include <stdio.h>

int main(int argc, char **argv) {
  FILE *f;
  Terrain t;
  int x, y;

  if (argc < 2) {
    printf("Usage : %s <fichier>\n", argv[0]);
    return 1;
  }

  f = fopen(argv[1], "r");
  
  erreur_terrain error = lire_terrain(f, &t, &x, &y) ;

  switch(error) {
    case ERREUR_FICHIER:
      printf("Erreur de fichier : %s \n", argv[1]);
      printf("Entrez un nouveau fichier \n");
      scanf("%s", argv[1]);
      main(argc, argv);
      break;
    case ERREUR_TERRAIN:
      printf("ERREUR_TERRAIN \n");
      break;
    case ERREUR_LECTURE_LARGEUR: 
      printf("ERREUR_LECTURE_LARGEUR \n");
      break;
    case ERREUR_LECTURE_HAUTEUR:
      printf("ERREUR_LECTURE_HAUTEUR \n");
      break;
    case ERREUR_LARGEUR_INCORRECTE:
      printf("ERREUR_LARGEUR_INCORRECTE \n");
      break; 
    case ERREUR_HAUTEUR_INCORRECTE:
      printf("ERREUR_HAUTEUR_INCORRECTE \n");
      break; 
    case ERREUR_CARACTERE_INCORRECT:
      printf("ERREUR_CARACTERE_INCORRECT \n");
      break; 
    case ERREUR_LIGNE_TROP_LONGUE:
      printf("ERREUR_LIGNE_TROP_LONGUE \n");
      break; 
    case ERREUR_LIGNE_TROP_COURTE:
      printf("ERREUR_LIGNE_TROP_COURTE \n");
      break; 
    case ERREUR_LIGNES_MANQUANTES:
      printf("ERREUR_LIGNES_MANQUANTES \n");
      break; 
    case ERREUR_LIGNES_EN_TROP:
      printf("ERREUR_LIGNES_EN_TROP \n");
      break; 
    case ERREUR_POSITION_ROBOT_MANQUANTE:
      printf("ERREUR_POSITION_ROBOT_MANQUANTE \n");
      fclose(f);
      break;
    case OK:
      afficher_terrain(&t);
      printf("Position initiale du robot : (%d, %d)\n", x, y);
      fclose(f);
      break;
    default:
      printf("je ss pas \n zizi \n");
      break;
  }
}