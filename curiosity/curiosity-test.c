#include "environnement.h"
#include "interprete.h"
#include "programme.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

  if (argc < 2) {
    printf("Usage: %s <fichier test>\n", argv[0]);
    return 1;
  }

  char **initialisation_tableau_test( char *);
  bool programme_test_oracle(char **, int, resultat_inter ,Environnement), test_case_finale(char *, char), test_position(char *, Environnement ), test_nb_pas(char *, int ), test_orientation(char *, Environnement);


  int nb_pas , debug = 0; 
  char **tab_test = NULL;


  for (int i = 1; i<=5; i++) {
    printf("******************************************\n");
    printf("test : %s \n", argv[i]);
    nb_pas = 0;
    tab_test = (char **) initialisation_tableau_test(argv[i]);

    /* Initialisation de l'environnement : lecture du terrain,
    ** initialisation de la position du robot                 */
    errt = initialise_environnement(&envt, tab_test[0]);
    gestion_erreur_terrain(errt);

    /* Lecture du programme */
    errp = lire_programme(&prog, tab_test[1]);
    gestion_erreur_programme(errp);

    /* Initialisation de l'état */
    init_etat(&etat);
    do {
      res = exec_pas(&prog, &envt, &etat);
      /* Affichage du terrain et du robot */
      if (debug) afficher_envt(&envt);
      nb_pas++;
    } while (res == OK_ROBOT && nb_pas < atoi(tab_test[2]));

    /* Affichage du résultat */
    switch (res) {
    case OK_ROBOT:
      printf("Robot sur une case libre, programme non terminé \n");

      break;
    case SORTIE_ROBOT:
      printf("Le robot est sorti :-)\n");
      break;
    case ARRET_ROBOT:
      printf("Robot sur une case libre, programme terminé :-/\n");
      break;

    case PLOUF_ROBOT:
      printf("Le robot est tombé dans l'eau :-(\n");
      break;
    case CRASH_ROBOT:
      printf("Le robot s'est écrasé sur un rocher X-(\n");
      break;
    case ERREUR_PILE_VIDE:
      printf("ERREUR : pile vide\n");
      break;
    case ERREUR_ADRESSAGE:
      printf("ERREUR : erreur d'adressage\n");
      break;
    case ERREUR_DIVISION_PAR_ZERO:
      printf("ERREUR : division par 0\n");
      break;
    }
    if (programme_test_oracle(tab_test, nb_pas, res, envt)) printf("Resultat : OK \n");
    printf("******************************************\n\n");
  }
}

char ** initialisation_tableau_test(char *fichier) {
  char **tab_test = NULL;
  int k = 0, lenFichier(FILE *);
  FILE *f;
  char *temp = calloc(256, sizeof(char));
  f = fopen(fichier, "r");
  if (f == NULL) {
    printf("Erreur lecture du terrain : erreur d'ouverture du fichier\n");
    exit(EXIT_FAILURE);
  }

  tab_test = (char **) malloc(lenFichier(f)*sizeof(tab_test));

  while (fgets(temp, 256, f) != NULL) {
    *(tab_test + k) = (char *) malloc(sizeof(char)*strlen(temp)-1);
    strncpy(tab_test[k], temp, strlen(temp) - 1);
    // printf("%s, len : %li \n", *(tab_test + k), strlen(*(tab_test + k)));
    k++;
    ;
  };
  fclose(f);
  return tab_test;
}

bool test_nb_pas(char *ligne, int nb_pas) {
    // test nb_pas
    if (atoi(ligne) >= nb_pas) return true;
    return false;
}

bool test_case_finale(char *chaine, char res) {
    // test de nature de case finale

    if (chaine[0] == res) return true; 
    if (res == 'F' && chaine[0] == 'N') return true;
    return false;
}

char caseFinaleToChar(resultat_inter res) {
  switch (res)
  {
  case OK_ROBOT:
    return 'N';
  case ARRET_ROBOT:
    return 'F';
  case CRASH_ROBOT:
    return 'O';
  case SORTIE_ROBOT:
    return 'S';
  case PLOUF_ROBOT:
    return 'P';
  
  default:
    return 'E';
    break;
  }
}

bool test_position(char *ligne, Environnement envt) {
    // test de la position
    if ((int) ligne[0] - 48 == envt.r.x && (int) ligne[2] - 48 == envt.r.y ) return true;
    return false;
}



bool test_orientation(char *ligne, Environnement envt) {
    // test de l'orientation
    switch (envt.r.o) {
      case Nord:
        if (strcmp(ligne, "N") == 0 || strcmp(ligne, "n") == 0 ) return true;
        break;
      case Est:
        if (strcmp(ligne, "E") == 0 || strcmp(ligne, "e") == 0) return true;
        break;  
      case Sud:
        if (strcmp(ligne, "S") == 0 || strcmp(ligne, "s") == 0) return true;
        break;
      case Ouest:
        if (strcmp(ligne, "O") == 0 || strcmp(ligne, "o") == 0) return true;
        break;
      default:
        printf("orientation non definie \n");
        break;
    } 
    return false;

}

bool programme_test_oracle(char **tab_test, int nb_pas, resultat_inter res, Environnement envt) {

  char caseFinaleToChar(resultat_inter res), caseFinaleChar = caseFinaleToChar(res);
  int nb_test = 2, nb_ok = 0;

  // test 1 : test nb_pas
  if (test_nb_pas(tab_test[2], nb_pas)) nb_ok++;
  else {
    printf("mauvais nb de pas \n");
    printf("Expected : %i \n", nb_pas);
    printf("Got : %i \n", atoi(tab_test[2]));
  }

  // test 2 : test case finale
  if (test_case_finale(tab_test[3], caseFinaleChar)) nb_ok++;
  else {
    printf("case finale incorrecte \n");
    printf("Expected : %c \n", caseFinaleChar);
    printf("Got : %c \n", atoi(&tab_test[3][0]));
    }

  if (caseFinaleChar == 'N' || caseFinaleChar == 'F') {
    nb_test += 2;
    // test 3 : test position finale
    if (test_position(tab_test[4], envt)) nb_ok++; 
    else {
      printf("position incorrecte !\n");
      printf("Expected : (%i, %i) \n", tab_test[4][0] - 48, tab_test[4][2] - 48);
      printf("Got : (%i, %i) \n", envt.r.x, envt.r.y); 
    }

    // test 4 : test de l'orientation
    if (test_orientation(tab_test[5], envt)) nb_ok++;
    else {
      char tabO[4] = "NESO";
      printf("mauvaise orientation \n");
      printf("Expected : %c \n", tab_test[5][0]);
      printf("Got : %c \n", tabO[envt.r.o]); 
      }
  }
  if (nb_test == nb_ok) return true;
  return false;
}

int lenFichier(FILE *f) {
  int k = 0;
  char * temp = calloc(256, sizeof(temp));
  while (fgets(temp, 256, f) != NULL) {
    k++;
  }
  fseek(f, 0, SEEK_SET);
  return k;
}
