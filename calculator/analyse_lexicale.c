/* ------------------------------------------------------------------------
-- paquetage analyse_lexicale
--
-- analyse lexicale d'une expression arithmetique
--
-- J-P. Peyrin, G. Serasset (version initiale) : janvier 1999
-- P.Habraken - 18/01/2010
--
-- 10 juin 2006 - PH :
-- remplacement record a champs variants par record "plat"
-- remplacement traitement iteratif par automate
--
-- Aout 2016 - LM : version C
------------------------------------------------------------------------ */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lecture_caracteres.h"
#include "analyse_lexicale.h"



/* --------------------------------------------------------------------- */

/* Les variables et fonctions locales au module */

Lexeme lexeme_en_cours; /* le lexeme courant */



char tableau_MOT[NB_MOT][TAILLE_MOT] = {"lire", "ecrire", "if", "then", "else", "fi", "tantque", "faire", "fait"};

char tableau_OP_BOOL[NB_OP_BOOL][TAILLE_OP_BOOL] = {"et", "ou", "non"};

void ajouter_caractere(char *s, char c);
Nature_Caractere nature_caractere(char c);
int est_separateur(char c);
int est_chiffre(char c);
int est_symbole(char c);
void reconnaitre_lexeme();

/* --------------------------------------------------------------------- */

void demarrer(char *nom_fichier)
{
	demarrer_car(nom_fichier);
	avancer();
}

/* --------------------------------------------------------------------- */

void avancer()
{
	reconnaitre_lexeme();
}

/* --------------------------------------------------------------------- */

Lexeme lexeme_courant()
{
	return lexeme_en_cours;
}

/* --------------------------------------------------------------------- */

int fin_de_sequence()
{
	return lexeme_en_cours.nature == FIN_SEQUENCE;
}

/* --------------------------------------------------------------------- */

void arreter()
{
	arreter_car();
}

/* --------------------------------------------------------------------- */
void reinitialiser() {
	while (!fin_de_sequence() && caractere_courant() != '\n') avancer();
}


// reconnaissance d'un nouveau lexeme
// etat initial : le caractere courant est soit separateur
//                soit le 1er caractere d'un lexeme
// etat final :
//       - un nouveau lexeme est reconnu dans lexeme_en_cours
//       - le caractere courant est soit la fin de fichier,
//		soit un separateur,  soit le 1er caractere d'un lexeme


/* condition → eag OPCOMP eag */
/* OPCOMP -> != | == | >= | <= | < | >  */
void reconnaitre_lexeme()
{
	typedef enum
	{
		E_INIT,
		E_ENTIER,
		E_FIN,
		E_REEL,

		E_IDF,
		E_IDF_E,

		E_OPCOMP,
		E_EXCLA

	} Etat_Automate;
	
	Etat_Automate etat = E_INIT;

	// on commence par lire et ignorer les separateurs
	while (est_separateur(caractere_courant()))
	{
		avancer_car();
	};

	lexeme_en_cours.chaine[0] = '\0';

	// on utilise ensuite un automate pour reconnaitre et construire le prochain lexeme
	
	int j = 1; // compteur utilisé lors de la construction de la partie fractionnaire d'un réel
	while (etat != E_FIN)
	{

		switch (etat)
		{
		case E_INIT: // etat initial

			switch (nature_caractere(caractere_courant()))
			{

			case C_FIN_SEQUENCE:
				lexeme_en_cours.nature = FIN_SEQUENCE;
				etat = E_FIN;
				break;

			case CHIFFRE:
				lexeme_en_cours.nature = ENTIER;
				lexeme_en_cours.ligne = numero_ligne();
				lexeme_en_cours.colonne = numero_colonne();
				ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
				lexeme_en_cours.valeur = caractere_courant() - '0';
				etat = E_ENTIER;
				avancer_car();
				break;

			case SYMBOLE:
				lexeme_en_cours.ligne = numero_ligne();
				lexeme_en_cours.colonne = numero_colonne();
				ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
				switch (caractere_courant())
				{
				case '+':
					lexeme_en_cours.nature = PLUS;
					etat = E_FIN;
					break;
				case '-':
					lexeme_en_cours.nature = MOINS;
					etat = E_FIN;
					break;
				case '*':
					lexeme_en_cours.nature = MUL;
					etat = E_FIN;
					break;
				case '/':
					lexeme_en_cours.nature = DIV;
					etat = E_FIN;
					break;
				case '(':
					lexeme_en_cours.nature = PARO;
					etat = E_FIN;
					break;
				case ')':
					lexeme_en_cours.nature = PARF;
					etat = E_FIN;
					break;
				case '.':
					lexeme_en_cours.nature = REEL;
					etat = E_REEL;
					break;
				default:
					printf("Erreur_Lexicale SYMBOLE inconue \n");
					exit(0);
				};
				avancer_car();
				break;
			case CARACTERE:
				lexeme_en_cours.ligne = numero_ligne();
				lexeme_en_cours.colonne = numero_colonne();
				ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
				lexeme_en_cours.nature = IDF;
				etat = E_IDF;
				avancer_car();
				break;
			case EXCLA:
				lexeme_en_cours.ligne = numero_ligne();
				lexeme_en_cours.colonne = numero_colonne();
				ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
				lexeme_en_cours.nature = C_EXCLA;
				etat = E_EXCLA;
				avancer_car();
				break;
				
			case C_FIN_AFFECTATION:
				lexeme_en_cours.ligne = numero_ligne();
				lexeme_en_cours.colonne = numero_colonne();
				ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
				switch (caractere_courant())
				{
				case ';':
				case '\n':
					lexeme_en_cours.nature = SEPINST;
					etat = E_FIN;
					avancer_car();
					break;
				default:
					printf("Erreur_Lexicale C_FIN_AFFECTATION non gerer \n");
					break;
				}
				break;
			case OPCOMP:
				lexeme_en_cours.ligne = numero_ligne();
				lexeme_en_cours.colonne = numero_colonne();
				ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
				switch (caractere_courant())
				{
				case '>':
					lexeme_en_cours.nature = OP_COMP_SUP;
					break;
				case '<':
					lexeme_en_cours.nature = OP_COMP_INF;
					break;
				case '=':
					lexeme_en_cours.nature = AFF;

					break;
				default:
					printf("ERREUR LEXICAL: operateur conditionnel non gerer \n");
					exit(0);
					break;
				}
				etat = E_OPCOMP;
				avancer_car();
				break;
			default:
				printf("Erreur_Lexicale 2\n");
				exit(0);
			};
			break;

		case E_ENTIER: // reconnaissance d'un entier
			switch (nature_caractere(caractere_courant()))
			{
			case CHIFFRE:
				ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
				lexeme_en_cours.valeur = lexeme_en_cours.valeur * 10 + caractere_courant() - '0';
				etat = E_ENTIER;
				avancer_car();
				break;

			case SYMBOLE:
				switch (caractere_courant())
				{
				case '.':
					lexeme_en_cours.nature = REEL;
					ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
					lexeme_en_cours.valeur = lexeme_en_cours.valeur;
					etat = E_REEL;
					avancer_car();
					break;
				default:
					etat = E_FIN;
					break;
				}
				break;


			case CARACTERE:
				lexeme_en_cours.nature = IDF_E;
				ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
				etat = E_IDF_E;
				avancer_car();
				break;

			default:
				etat = E_FIN;
				break;
			};
			break;
		case E_REEL:
			switch (nature_caractere(caractere_courant()))
			{
			case CHIFFRE:
				ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
				lexeme_en_cours.valeur = lexeme_en_cours.valeur + ((float)(caractere_courant() - '0') / (power(10, j)));j++;
				etat = E_REEL;
				avancer_car();
				break;
			
			default:
				etat = E_FIN;
				break;
			}
			break;
		case E_IDF:
			switch (nature_caractere(caractere_courant()))
			{
			
			case CARACTERE:
			case CHIFFRE:
				ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
				etat = E_IDF;
				avancer_car();
				break;
			default:
				for (int i = 0; i<NB_MOT; i++) {
					if (strcmp(tableau_MOT[i], lexeme_en_cours.chaine) == 0) {
						switch (i)
						{
						case 0:
							lexeme_en_cours.nature = LIRE;
							break;
						case 1:
							lexeme_en_cours.nature = ECRIRE;
							break;
						case 2:
							lexeme_en_cours.nature = IF;
							break;
						case 3:
							lexeme_en_cours.nature = THEN;
							break;
						case 4:
							lexeme_en_cours.nature = ELSE;
							break;
						case 5:
							lexeme_en_cours.nature = FI;
							break;
						case 6:
							lexeme_en_cours.nature = TANTQUE;
							break;
						case 7:
							lexeme_en_cours.nature = FAIRE;
							break;
						case 8:
							lexeme_en_cours.nature = FAIT;
							break;
						default:
							break;
						}
					}
				}
				for (int i = 0; i<NB_OP_BOOL; i++) {
					if (strcmp(tableau_OP_BOOL[i], lexeme_en_cours.chaine) == 0) {
						switch (i)
						{
						case 0:
							lexeme_en_cours.nature = OP_BOOL_ET;
							break;
						case 1:
							lexeme_en_cours.nature = OP_BOOL_OU;
							break;
						case 2:
							lexeme_en_cours.nature = OP_BOOL_NON;
						default:
							break;
						}
					}
				}
				etat = E_FIN;
				break;
			}
			break;
		case E_IDF_E:
			switch (nature_caractere(caractere_courant()))
			{
			
			case CARACTERE:
			case CHIFFRE:
				ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
				etat = E_IDF_E;
				avancer_car();
				break;
			default:
				etat = E_FIN;
				break;
			}
			break;
		case E_OPCOMP:
			switch (nature_caractere(caractere_courant()))
			{
			case OPCOMP:
				switch (caractere_courant())
				{
				case '=':
					ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
					switch (lexeme_en_cours.nature)
					{
					case AFF:
						lexeme_en_cours.nature = OP_COMP_EGAL;
						break;
					case OP_COMP_INF:
						lexeme_en_cours.nature = OP_COMP_INF_EGAL;
						break; 
					case OP_COMP_SUP:
						lexeme_en_cours.nature = OP_COMP_SUP_EGAL;
						break;
					default:
						break;
					}
					etat = E_FIN;
					avancer_car();
					break;
				
				default:
					etat = E_FIN;
					break;
				}
				break;
			
			default:
				etat = E_FIN;
				break;
			}
			break;
		case E_EXCLA:

			if (nature_caractere(caractere_courant()) == OPCOMP && caractere_courant() == '=') {
					ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
					lexeme_en_cours.nature = OP_COMP_DIFF;
					etat = E_FIN;
					avancer_car();
			}
			else {
				printf("ERREUR LEXICAL operateur non gerer \n");
				exit(0);
			}
			break;
		case E_FIN: // etat final
			break;

		}; // fin du switch(etat)
	};	   // fin du while (etat != fin)
}

/* --------------------------------------------------------------------- */

// cette fonction ajoute le caractere c a la fin de la chaine s
// (la chaine s est donc modifiee)

void ajouter_caractere(char *s, char c)
{
	int l;

	l = strlen(s);
	s[l] = c;
	s[l + 1] = '\0';
};

/* --------------------------------------------------------------------- */

// vaut vrai ssi c designe un caractere correspondant a un caractere aplhabetique
int est_caractere(char c)
{
	if ((c >= 65 && c <= 90)  || (c >= 97 && c <= 122)) return 1;
	return 0;
}


// vaut vrai ssi c designe un caractere d'affectation
int est_caractere_aff(char c) {
	if (c == '=') return 1;
	return 0;
}

// vaut vrai ssi c designe un caractere fin d'affactation
int est_caractere_fin_aff(char c) {
	if (c == ';' || c == '\n') return 1;
	return 0;
}

int est_operateur_conditionnel(char c) {
	switch (c)
	{
	case '>':
	case '<':
	case '!':
	case '=':
		return 1;
	
	default:
		return 0;
		
	}
}


int est_excla(char c) {
	if (c == '!') return 1;
	return 0;
}

/* --------------------------------------------------------------------- */

Nature_Caractere nature_caractere(char c)
{
	if (fin_de_sequence_car(c))
		return C_FIN_SEQUENCE;
	if (est_chiffre(c))
		return CHIFFRE;
	if (est_symbole(c)) 
		return SYMBOLE; 
	if (est_caractere(c))
		return CARACTERE;
	if (est_excla(c))
		return EXCLA;
	/*if (est_caractere_aff(c))
		return C_AFFECTATION;*/
	if (est_caractere_fin_aff(c))
		return C_FIN_AFFECTATION;
	if (est_operateur_conditionnel(c))
		return OPCOMP;
	return ERREUR_CAR;
}
/* --------------------------------------------------------------------- */

// vaut vrai ssi c designe un caractere separateur
int est_separateur(char c)
{
	return c == ' ' || c == '\t' || c == '\n';
}

/* --------------------------------------------------------------------- */

// vaut vrai ssi c designe un caractere chiffre
int est_chiffre(char c)
{
	return c >= '0' && c <= '9';
}

/* --------------------------------------------------------------------- */

// vaut vrai ssi c designe un caractere correspondant a un symbole arithmetique
int est_symbole(char c)
{
	switch (c)
	{
	case '+':
	case '-':
	case '*':
	case '/':
	case '.':
	case '(':
	case ')':
		return 1;

	default:
		return 0;
	}
}



/* --------------------------------------------------------------------- */

// renvoie la chaine de caracteres correspondant a la nature du lexeme
char *Nature_vers_Chaine(Nature_Lexeme nature)
{
	switch (nature)
	{
	case ENTIER:
		return "ENTIER";
	case REEL:
		return "REEL";

	case PLUS:
		return "PLUS";
	case MOINS:
		return "MOINS";
	case MUL:
		return "MUL";
	case DIV:
		return "DIV";
	case PARO:
		return "PARO";
	case PARF:
		return "PARF";

	case IDF:
		return "IDF";
	case IDF_E:
		return "IDF_E";
	case AFF:
		return "AFF";
	case SEPINST:
		return "SEPINST";

	case LIRE:
		return "LIRE";
	case ECRIRE:
		return "ECRIRE";

	case IF:
		return "IF";
	case THEN:
		return "THEN";
	case ELSE:
		return "ELSE";
	case FI:
		return "FI";


	case OP_COMP_DIFF:
		return "OP_COMP_DIFF";
	case OP_COMP_EGAL:
		return "OP_COMP_EGAL";
	case OP_COMP_INF:
		return "OP_COMP_INF";
	case OP_COMP_INF_EGAL:
		return "OP_COMP_INF_EGAL";
	case OP_COMP_SUP:
		return "OP_COMP_SUP";
	case OP_COMP_SUP_EGAL:
		return "OP_COMP_SUP_EGAL";

	case OP_BOOL_ET:
		return "OP_BOOL_ET";
	case OP_BOOL_OU:
		return "OP_BOOL_OU";
	case OP_BOOL_NON:
		return "OP_BOOL_NON";

	case TANTQUE:
		return "TANTQUE";
	case FAIRE:
		return "FAIRE";
	case FAIT:
		return "FAIT";

	case FIN_SEQUENCE:
		return "FIN_SEQUENCE";
	default:
		return "ERREUR";
	};
}

/* --------------------------------------------------------------------- */

// affiche a l'ecran le lexeme l
void afficher(Lexeme l)
{

	switch (l.nature)
	{
	case FIN_SEQUENCE:
		break;
	default:
		printf("(ligne %d, ", l.ligne);
		printf("colonne %d) : ", l.colonne);
		printf("[");
		printf("nature = %s", Nature_vers_Chaine(l.nature));
		printf(", chaine : %s, ", l.chaine);
		switch (l.nature)
		{
		case ENTIER:
		case REEL:
			printf("valeur = %g", l.valeur);
			break;
		case IDF:
		case IDF_E:
			printf("affichage non gere");	
			break;
		default:
			break;
		};
		printf("]");
	};
}

int power(int a, int b) {
	int resultat = a;
	if (b > 0){
		for (int i = 1; i<b; i++) {
			resultat *= a;
		}
	}
	else {
		printf("cas pas gerer \n");
	}
	return resultat;
}

/* --------------------------------------------------------------------- */
