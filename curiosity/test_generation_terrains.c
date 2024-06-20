#include "generation_terrains.h"
#include "terrain.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int nb_cases_occupees(Terrain *T) {
  int occupee = 0;
  int x, y;
  for (x = 0; x < largeur(T); x++) {
    for (y = 0; y < hauteur(T); y++) {
      if (T->tab[x][y] != LIBRE)
        occupee++;
    }
  }
  return occupee;
}

// Test de generation aléatoire de terrains
// Le programme génère n terrains de largeur et hauteur fixes
// avec largeur et hauteur impaires et inférieures a dimension_max de terrain.h
// avec densité d'obstacle dObst
// autre que la case centrale soit occupee
// l'appel du programme se fait avec 5 arguments :
// generation_terrains N largeur hauteur dObstacle fichier_res
// la sortie se fait dans le fichier resultat

int main(int argc, char **argv) {
  int N, i, l, h, densiteCmp, terrainValideCmp = 0;
  float dObst, densiteTotal = 0.;
  FILE *resFile;
  Terrain T;
  char * CaseToChar(Case unecase);

  if (argc < 6) {
    printf(
        "Usage: %s <N> <largeur> <hauteur> <densite_obstacle> <fichier_res> \n",
        argv[0]);
    return 1;
  }

  N = strtol(argv[1], NULL, 10);
  l = strtol(argv[2], NULL, 10);
  h = strtol(argv[3], NULL, 10);
  dObst = strtof(argv[4], NULL);

  // test de l et h
  if (l > DIM_MAX || l % 2 == 0) {
    printf("Largeur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if (h > DIM_MAX || h % 2 == 0) {
    printf("Hauteur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if ((dObst > 1) || (dObst < 0)) {
    printf("Densité incorrecte : doit être comprise entre 0 et 1\n");
    return 1;
  }

  // Ouverture du fichier résultat
  resFile = fopen(argv[5], "w");
  // Écriture du nombre de terrains
  fprintf(resFile, "%d\n", N);

  // Initialisation de la fonction random
  // A compléter
  srand(time(NULL));

  // A compléter
  for (i = 0; i<N; i++) {
    fprintf(resFile, "terrain : %i\n", i);

    // Génération aléatoire des terrains
    generation_aleatoire(&T, l, h, dObst);
    densiteCmp = 0;
    while (!existe_chemin_vers_sortie(&T)) {
      generation_aleatoire(&T, l, h, dObst);
    }
    // Écriture des terrains générés dans le fichier resFile
    fprintf(resFile, "h : %i \nl : %i \ndObst : %f\n\n", T.hauteur, T.largeur, dObst);
    if (existe_chemin_vers_sortie(&T)) terrainValideCmp += 1;
    for (int i = 0; i<T.hauteur;i++) {
        for (int j = 0; j<T.largeur;j++) {
            if (T.tab[i][j] != LIBRE) densiteCmp++;
            if (i == T.hauteur/2 && j == T.largeur/2) {
              fprintf(resFile, "C");
              continue;
            }
            fprintf(resFile, "%s", CaseToChar(T.tab[i][j]));
        }
        fprintf(resFile, "\n");
    }
    fprintf(resFile, "\n%f\n\n",(float) densiteCmp/(T.hauteur*T.largeur));
    densiteTotal +=(float) densiteCmp/(T.hauteur*T.largeur);
  }
  // Écriture/Affichage des statistiques
  fprintf(resFile, "\n densité moyenne : %f \n nombre terrain valide : %d",(densiteTotal/N), terrainValideCmp);
  // fermeture des fichiers
  fclose(resFile);
  return 0;
}



char * CaseToChar(Case uneCase) {
    switch (uneCase)
    /*{LIBRE, EAU, ROCHER}*/
    {
    case LIBRE:
        return ".";
        break;
    case EAU:
        return "~";
        break;
    case ROCHER:
        return "#";
        break;
    default:
        return "EREUR";
        break;
    }
}