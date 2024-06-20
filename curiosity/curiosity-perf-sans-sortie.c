#include "environnement.h"
#include "interprete.h"
#include "generation_terrains.h"
#include "programme.h"

#include <stdio.h>
#include <stdlib.h>

void gestion_erreur_terrain(erreur_terrain e) {
  switch (e) {
  case OK:
    break;
  case ERREUR_FICHIER:
    printf("Erreur lecture du terrain : erreur d'ouverture du fichier\n");
    exit(1);
  case ERREUR_TERRAIN:
    printf("Erreur lecture du terrain : terrain invalide\n");
    exit(1);
  case ERREUR_LECTURE_LARGEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la largeur\n");
    exit(1);
  case ERREUR_LECTURE_HAUTEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la hauteur\n");
    exit(1);
  case ERREUR_LARGEUR_INCORRECTE:
    printf("Erreur lecture du terrain : largeur incorrecte\n");
    exit(1);
  case ERREUR_HAUTEUR_INCORRECTE:
    printf("Erreur lecture du terrain : hauteur incorrecte\n");
    exit(1);
  case ERREUR_CARACTERE_INCORRECT:
    printf("Erreur lecture du terrain : caractère incorrect\n");
    exit(1);
  case ERREUR_LIGNE_TROP_LONGUE:
    printf("Erreur lecture du terrain : ligne trop longue\n");
    exit(1);
  case ERREUR_LIGNE_TROP_COURTE:
    printf("Erreur lecture du terrain : ligne trop courte\n");
    exit(1);
  case ERREUR_LIGNES_MANQUANTES:
    printf("Erreur lecture du terrain : lignes manquantes\n");
    exit(1);
  case ERREUR_LIGNES_EN_TROP:
    printf("Erreur lecture du terrain : ligne en trop\n");
    exit(1);
  case ERREUR_POSITION_ROBOT_MANQUANTE:
    printf(
        "Erreur lecture du terrain : position initiale du robot manquante\n");
    exit(1);
  }
}
void affichage_position_programme(erreur_programme e) {
  int i;
  printf("Ligne %d, colonne %d :\n", e.num_ligne, e.num_colonne);
  printf("%s\n", e.ligne);
  /* Impression de e.num_colonne-1 espaces */
  for (i = 1; i < e.num_colonne; i++) {
    printf(" ");
  }
  /* Impression d'un curseur de position */
  printf("^\n");
}

void gestion_erreur_programme(erreur_programme e) {
  switch (e.type_err) {
  case OK_PROGRAMME:
    break;
  case ERREUR_FICHIER_PROGRAMME:
    printf("Erreur lecture du programme : erreur d'ouverture du fichier\n");
    exit(2);
  case ERREUR_BLOC_NON_FERME:
    printf("Erreur lecture du programme : bloc non fermé\n");
    exit(2);
  case ERREUR_FERMETURE_BLOC_EXCEDENTAIRE:
    printf("Erreur lecture du programme : fermeture de bloc excédentaire\n");
    affichage_position_programme(e);
    exit(2);
  case ERREUR_COMMANDE_INCORRECTE:
    printf("Erreur lecture du programme : commande incorrecte\n");
    affichage_position_programme(e);
    exit(2);
  }
}

int main(int argc, char **argv) {
  Environnement envt;
  Programme prog;
  erreur_terrain errt;
  erreur_programme errp;
  etat_inter etat;
  resultat_inter res;
  Terrain T;
	char * CaseToChar(Case unecase);

  int N, l, h, nbPas = 0, nbPasTotal = 0;
  int nbRobotSortis = 0, nbRobotObstacle = 0, nbRobotBloque = 0;
  float dObst;

  if (argc <= 8) {
    printf("Usage: %s <programme> <N> <L> <H> <d> <graine> <nb_step_max> <fichier_res> \n", argv[0]);
    return 1;
  }
  N = strtol(argv[2], NULL, 10);
  l = strtol(argv[3], NULL, 10);
  h = strtol(argv[4], NULL, 10);
  dObst = strtof(argv[5], NULL);

	FILE * fileRes = fopen(argv[8], "w");
	if (fileRes == NULL) {
		printf("ERROR : Erreur d'adressage \n");
		exit(0);
	}
	fprintf(fileRes, "%i\n", N);
	srand(strtol(argv[6], NULL, 10));
	for (int k = 0; k<N;k++) {
		FILE *fileTerrain = fopen("buffer.txt", "w");
		if (fileTerrain == NULL) {
			printf("ERROR : Erreur d'Adressage !!!\n");
			exit(0);
		}

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

	
		// Génération aléatoire des terrains
		generation_aleatoire(&T, l, h, dObst);
		while (existe_chemin_vers_sortie(&T)) {
			generation_aleatoire(&T, l, h, dObst);
		}
		// Écriture des terrains générés dans le fichier resFile
		fprintf(fileTerrain, "%i\n%i\n",T.largeur, T.hauteur);
		for (int i = 0; i<T.hauteur;i++) {
			for (int j = 0; j<T.largeur;j++) {
				if (i == T.hauteur/2 && j == T.largeur/2) {
					fprintf(fileTerrain, "C");
					continue;
				}
				fprintf(fileTerrain, "%s", CaseToChar(T.tab[i][j]));
			}
			fprintf(fileTerrain, "\n");
		}
		fclose(fileTerrain);

		nbPas = 0;

		/* Initialisation de l'environnement : lecture du terrain,
			initialisation de la position du robot */
		errt = initialise_environnement(&envt, "buffer.txt");
		gestion_erreur_terrain(errt);

		/* Lecture du programme */
		errp = lire_programme(&prog, argv[1]);
		gestion_erreur_programme(errp);

		/* Initialisation de l'état */
		init_etat(&etat);
		do {
			res = exec_pas(&prog, &envt, &etat);
			exec_pas(&prog, &envt, &etat);
			nbPas++;
			/* Affichage du terrain et du robot */
			//afficher_envt(&envt);
		} while (res == OK_ROBOT && nbPas < atoi(argv[7]));

		/* Affichage du résultat */
		switch (res) {
		case OK_ROBOT:
			nbRobotBloque++;
			fprintf(fileRes, "-1\n");
			//printf("Robot sur une case libre, programme non terminé (ne devrait pas "
			//			"arriver)\n");
			break;
		case SORTIE_ROBOT:
			fprintf(fileRes, "%i\n", nbPas);
			nbRobotSortis++;
			nbPasTotal += nbPas;
			//printf("Le robot est sorti :-)\n");
			break;
		case ARRET_ROBOT:
			//printf("Robot sur une case libre, programme terminé :-/\n");
			break;
		case PLOUF_ROBOT:
			fprintf(fileRes, "-2\n");
			nbRobotObstacle++;
			//printf("Le robot est tombé dans l'eau :-(\n");
			break;
		case CRASH_ROBOT:
		  fprintf(fileRes, "-3\n");
			nbRobotObstacle++;
			//printf("Le robot s'est écrasé sur un rocher X-(\n");
			break;
		case ERREUR_PILE_VIDE:
			//printf("ERREUR : pile vide\n");
			break;
		case ERREUR_ADRESSAGE:
			//printf("ERREUR : erreur d'adressage\n");
			break;
		case ERREUR_DIVISION_PAR_ZERO:
			//printf("ERREUR : division par 0\n");
			break;
		}
	}
	if (nbRobotSortis == 0) printf("Nombre Robot Sortis : %i\t%.2f %c\tmoyene de pas : %.2f\n", nbRobotSortis, ((float) nbRobotSortis/N )* 100 ,'%', (float) 0);
	else printf("Nombre Robot Sortis : %i\t%.2f %c\tmoyene de pas : %f\n", nbRobotSortis, ((float) nbRobotSortis/N )* 100 ,'%', (float) nbPasTotal/nbRobotSortis);
	printf("Nombre Robot Bloque : %i\t%.2f %c\n", nbRobotBloque,((float) nbRobotBloque/N )* 100, '%');
	printf("Nombre Robot ecrasé : %i\t%.2f %c\n", nbRobotObstacle,((float) nbRobotObstacle/N )* 100, '%');

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
