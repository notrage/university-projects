
#include "terrain.h"
#include "robot.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* typedef enum {OK, ERREUR_LECTURE_LARGEUR, ERREUR_LECTURE_HAUTEUR, ERREUR_LARGEUR_INCORRECTE, ERREUR_HAUTEUR_INCORRECTE, ERREUR_CARACTERE_INCORRECT, ERREUR_LIGNE_TROP_LONGUE, ERREUR_LIGNE_TROP_COURTE, ERREUR_LIGNES_MANQUANTES, ERREUR_POSITION_ROBOT_MANQUANTE} erreur_terrain; */

erreur_terrain lire_terrain(FILE *f, Terrain *t, int *x, int *y) {
  int l, h;   // Dimensions du terrain
  int rx = -1, ry = -1; // Coordonnées initiales du robot

  if (f == NULL) return ERREUR_FICHIER;
  if (t == NULL) return ERREUR_TERRAIN;

  // Lecture de la largeur
  // À compléter
  if (fscanf(f, "%i", &l) == EOF) return ERREUR_LECTURE_LARGEUR;
  if (l > DIM_MAX) return ERREUR_LARGEUR_INCORRECTE;
  t->largeur = l;
  // Lecture de la hauteur
  if (fscanf(f, "%i", &h) == EOF ) return ERREUR_LECTURE_HAUTEUR;
  if (h > DIM_MAX) return ERREUR_HAUTEUR_INCORRECTE;
  t->hauteur = h;

  // À compléter
  fscanf(f, "\n");
  // Lecture du terrain
  char c;
  for (int i = 0; i<h;i++) {
    for (int j = 0; j<l;j++) {
      if (fscanf(f, "%c", &c) == EOF) return ERREUR_LIGNES_MANQUANTES;
      switch (c) {
      case '.':
        t->tab[j][i] = LIBRE;
        break;
      case '#':
        t->tab[j][i] = ROCHER;
        break;
      case '~':
        t->tab[j][i] = EAU;
        break;
      case 'C':
        t->tab[j][i] = LIBRE;
        rx = j;
        ry = i;
        break;
      case '\n':
        return ERREUR_LIGNE_TROP_COURTE;
        break;
      default:
        return ERREUR_CARACTERE_INCORRECT;
        break;
      }
    }
    fscanf(f, "%c", &c); if (c != '\n') return ERREUR_LIGNE_TROP_LONGUE;
  }
  if (fscanf(f, "%c", &c) != EOF) return ERREUR_LIGNES_EN_TROP;
  // À compléter

  // Initialisation de la position du robot
  if (rx == -1 || ry==-1) return ERREUR_POSITION_ROBOT_MANQUANTE;
  *x = rx;
  *y = ry;
  // À compléter
  return OK;
}

/* À compléter */
/* Largeur d'un terrain */
int largeur(Terrain *t) {
  return t->largeur;
}

/* Hauteur d'un terrain */
int hauteur(Terrain *t) {
  return t->hauteur;
}

/* Indique si la case de coordonnées (x,y) est libre
   Renvoie vrai ssi les 3 conditions suivantes sont vraies :
    - 0 <= x < largeur
    - 0 <= y < hauteur
    - t.tab[x][y] = LIBRE
 */
int est_case_libre(Terrain *t, int x, int y) {
  if ((x >= 0 && x< t->largeur) && (x >= 0 && x< t->largeur) && t->tab[y][x] == LIBRE) return 1;
  return 0;
}

/* Affichage d'un terrain t sur la sortie standard */
void afficher_terrain(Terrain *t) {
  printf("%i\n", t->largeur);
  printf("%i\n", t->hauteur);
  for (int i = 0; i < t->largeur;i++) {
    for (int j = 0; j < t->hauteur;j++) {
      switch (t->tab[j][i]) {
      case LIBRE:
        printf(".");
        break;
      case ROCHER:
        printf("#");
        break;
      case EAU:
        printf("~");
        break;
      default:
        break;
      }
    }
    putchar('\n');
  }
}

/* Écriture d'un terrain t dans un fichier f ouvert en écriture.
   x et y contiennent les coordonnées du robot
   Le terrain est écrit au format lisible par lire_terrain */
void ecrire_terrain(FILE *f, Terrain *t, int x, int y) {
  fprintf(f, "%i\n", t->largeur);
  fprintf(f, "%i\n", t->hauteur);
  for (int i = 0; i<t->largeur; i++) {
    for (int j = 0; j<t->hauteur; j++) {
      switch (t->tab[j][i]) {
        case LIBRE:
          fprintf(f, "%c" ,'.');
          break;
        case ROCHER:
          fprintf(f, "%c", '#');
          break;
        case EAU:
          fprintf(f, "%c", '~');
          break;
        default:
          break;
      }
      if (x == i && y == j) fprintf(f, "%c", 'C');
    }
    fprintf(f, "%c", '\n');
  }
}
