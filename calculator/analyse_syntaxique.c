
/* Exemple d'utilisation de l'analyse lexicale :
    Affichage d'une sequence de lexemes            */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "analyse_lexicale.h"
#include "analyse_syntaxique.h"

#include "ast_construction.h"
#include "ast_parcours.h"

#include "table_symbole.h"

typedef enum
{
    E_INIT,
    E_ENTIER,
    E_OP
} Etat_Automate;
Etat_Automate etat = E_INIT;

Lexeme precedent;


int error = 0;
int mode_debug = 0;

void Rec_seq_inst(Ast * );
void Rec_seq_cond(Ast *);

void debug() {
    mode_debug += 1;
}

int est_entier(float a) {
    return a == (int) a;
}


int Evaluer(int a, char op, int b) {
    switch (op)
    {
    case '+':
        return a + b;
        break;
    case '-':
        return a - b;
        break;
    case '*':
        return a * b;
        break;
    case '/':
        if (b == 0) {
            printf("ERREUR SEMANTIQUE : division par zero non autorise \n");
            exit(0);
        }
        return a / b;
        break;
    default:
        printf("ne devrais pas arriver ! \n");
        exit(0);
        break;
    }
}



void test_Evaluer(int a, char b, int c) {
    printf("test evaluer \n");
    printf("arg \t a: 5, op = +, b: 10 \n");
    printf("%i \n", Evaluer(a, b, c));
}


void Raise_op_2_error(void) {
    printf("ERROR SYNTAXIQUE : OP_2 \n");
    exit(0);
}

int Rec_op_2(void) {
    /*switch case ...*/
    switch (lexeme_courant().nature)
    {
    case MUL:
        avancer();
        return 1;
        break;
    case DIV:
        avancer();
        return 2;
    default:
        return 0;
        break;
    }
}

void Raise_op_1_error(void) {
    printf("ERROR SYNTAXIQUE : OP_2 \n");
    exit(0);
}

int Rec_op_1(void) {
    switch (lexeme_courant().nature)
    {
    case PLUS:
        avancer();
        return 1;
        break;
    case MOINS:
        avancer();
        return 2;
        break;
    default:
        return 0;
        break;
    }
}


void Raise_rec_facteur(void) {
    printf("ERROR SYNTAXIQUE : Rec_facteur \n");
    error += 1;
    
}


/*FACTEUR => ENTIER | PARO EAG PARF | IDF*/
/* 1 + x */
void Rec_facteur(Ast *arbre) {
    switch (lexeme_courant().nature)
    {
    case ENTIER:
    case REEL:
        *arbre = creer_valeur(lexeme_courant().valeur);
        avancer();
        break;
    case PARO:
        avancer();
        EAG(arbre);
        if (lexeme_courant().nature == PARF) avancer();
        else Raise_rec_facteur();
        break;
    case IDF:
        *arbre = creer_idf(lexeme_courant().chaine);
        avancer();
        break;
    default:
        *arbre = NULL;
        break;
    }

}

/*SUITE_SEQ_FACTEUR => OP1 TERME SUITE_SEQ_FACTEUR */
void Rec_suite_seq_facteur(Ast *arbre,Ast *arbreSuite) {
    switch (Rec_op_2()) {
        case 1:
            Rec_facteur(arbreSuite);
            *arbre = creer_operation(N_MUL, *arbre, *arbreSuite);
            Rec_suite_seq_facteur(arbre, arbreSuite);
            break;
        case 2:
            Rec_facteur(arbreSuite);
            *arbre = creer_operation(N_DIV, *arbre, *arbreSuite);
            Rec_suite_seq_facteur(arbre, arbreSuite);
            break;
        default:
            break;
    }
} 

/* SEQ_FACTEUR => FACTEUR  SUITE_SEQ_FACTEUR */
Ast Rec_seq_facteur(void) {
    Ast arbre, arbreSuite;
    /* besoins d'un switch case !!! pour retourner epsilon ???*/
    Rec_facteur(&arbre);
    Rec_suite_seq_facteur(&arbre, &arbreSuite);
    return arbre;
}

/* TERME => SEQ_FACTEUR*/
void Rec_terme(Ast *arbre) {
    *arbre = Rec_seq_facteur();
}

/* SUITE_SEQ_TERME => OP1 TERME SUITE_SEQ_TERME | epsilon */
void Rec_suite_seq_terme(Ast *arbre, Ast *arbreSuite) {
    /* besoins d'un switch case !!! pour retourner epsilon ???*/
    switch (Rec_op_1())
    {
    case 1:
        Rec_terme(arbreSuite); 
        *arbre = creer_operation(N_PLUS, *arbre, *arbreSuite);
        Rec_suite_seq_terme(arbre, arbreSuite);
        break;
    case 2:
        Rec_terme(arbreSuite);

        if (*arbre != NULL && *arbreSuite != NULL) *arbre = creer_operation(N_MOINS, *arbre, *arbreSuite);
        else { 
            if (*arbre == NULL && *arbreSuite != NULL) {
                switch ((*arbreSuite)->nature)
                {
                case IDENTIFICATEUR:
                    *arbre = creer_operation(N_MOINS, NULL, creer_idf((*arbreSuite)->nom));
                    break;
                case VALEUR:
                    *arbre = creer_valeur(-(*arbreSuite)->valeur);
                    break;
                default:
                    break;
                }

            }
        }
        Rec_suite_seq_terme(arbre, arbreSuite);
        break;
    default:
        /* Epsilon */
        break;
    }
    
    
    
}

/* SEQ_TERME => TERME SUITE_SEQ_TERME */
void Rec_seq_terme(Ast *arbre) {
    Ast arbreSuite;
    Rec_terme(arbre);
    Rec_suite_seq_terme(arbre, &arbreSuite);
}

/* EAG -> Seq-terme */
void EAG(Ast *arbre) {
    Rec_seq_terme(arbre);
}

int est_OPCOMP(int *op) {
    switch (lexeme_courant().nature)
    {
    case OP_COMP_DIFF: 
        *op = 0;
         break;
    case OP_COMP_SUP:
        *op = 1;
        break;
    case OP_COMP_SUP_EGAL:
        *op = 2;
        break;
    case OP_COMP_INF:
        *op = 3;
        break;
    case OP_COMP_INF_EGAL:
        *op = 4;
        break;
    case OP_COMP_EGAL:
        *op = 5;
        break;
    default:
        return -1;
    }
    return 1;
}
/* x == 2 et y > 2 */
/* seq_condition -> condition suite_seq_condition */
/* suite_seq_condition -> OPBOOL seq_condition | epsilon */





int est_OPBOOL(int *opbool) {
    switch (lexeme_courant().nature)
    {
    case OP_BOOL_ET:
        *opbool = 0;
        break;
    case OP_BOOL_OU:
        *opbool = 1;
        break;
    case OP_BOOL_NON:
        *opbool = 2;
        break;
    
    default:
        return 0;
        break;
    }
    return 1;
}




/* condition -> eag OPCOMP eag */
void Rec_cond(Ast *arbre_resultat) {
    Ast arbre_g = (Ast) malloc(sizeof(NoeudAst));
    Ast arbre_d = (Ast) malloc(sizeof(NoeudAst));
    int op;

    EAG(&arbre_g);
    if (est_OPCOMP(&op)) avancer();
    EAG(&arbre_d);
    *arbre_resultat = creer_op_comp(op, arbre_g, arbre_d); 
} 


/* suite_seq_condition -> OPBOOL seq_condition | epsilon */
void Rec_suite_seq_cond(Ast arbre_donne, Ast *arbre_resultat) {
    Ast arbre_g;
    int op;
    if (est_OPBOOL(&op)) {
        if (op == 2) {
            printf("ERREUR ANALYSE SYNTAXIQUE : suite seq cond doit commencer par OPBOOL 'et' ou 'ou' \n");
            exit(0);
        }
        avancer();
        Rec_seq_cond(&arbre_g);
        *arbre_resultat = creer_op_bool(op, arbre_donne, arbre_g);
    }
    else {
        *arbre_resultat = arbre_donne;
    }
}

/* seq_condition -> condition suite_seq_condition | OPBOOL_non condition suite_seq_condition */
void Rec_seq_cond(Ast *arbre_resultat) {
    Ast arbre_g;
    switch (lexeme_courant().nature)
    {
    case OP_BOOL_NON:
        avancer();
        Rec_seq_cond(arbre_resultat);
        *arbre_resultat = creer_op_bool(2, *arbre_resultat, NULL);
        break;
    case IDF:
    case ENTIER:
        Rec_cond(&arbre_g);
        Rec_suite_seq_cond(arbre_g, arbre_resultat);
        break;
    default:
        break;
    }
}



/* aff -> IDF AFF eag SEPAFF */

/* x = 3 + 5 */

/* inst -> IDF AFF eag | LIRE PARO IDF PARF | ECRIRE PARO IDF PARF |IF seq cond THEN seq inst ELSE seq inst FI | TANTQUE seq cond FAIRE seq inst FAIT | autres forme d'inst ...*/
void Rec_inst(Ast * arbre_resultat) {
    Ast arbre_op_g, arbre_op_c, arbre_op_d;
    char * save = (char *) malloc(sizeof(char)*TAILLE_MAX);

    switch (lexeme_courant().nature)
    {
    case IDF:
        strcpy(save, lexeme_courant().chaine);
        arbre_op_g = creer_idf(save);
        avancer();
        if (lexeme_courant().nature == AFF) {
        avancer();
        }
        else {
            printf("ERREUR Rec_inst : on attend le symbole '=' \n");
            error += 1;
        }
        EAG(&arbre_op_d);
        arbre_op_g = creer_aff(arbre_op_g, arbre_op_d);
        break;

    case LIRE:
        avancer();
        if (lexeme_courant().nature == PARO) avancer();
        else printf("ERREUR : on attend PARO \n");
        if (lexeme_courant().nature == IDF) 
        {   
            arbre_op_g = creer_idf(lexeme_courant().chaine);
            avancer();
        }
        else printf("ERREUR : on attend IDF");
        if (lexeme_courant().nature == PARF) avancer();
        else printf("ERREUR : on attend PARF \n");

        arbre_op_g = creer_lire(arbre_op_g);
        break;

    case ECRIRE:
        avancer();
        if (lexeme_courant().nature == PARO) avancer();
        else printf("ERREUR : on attend PARO \n");
        arbre_op_g = (Ast) malloc(sizeof(NoeudAst));
        EAG(&arbre_op_g);
        if (lexeme_courant().nature == PARF) avancer();
        else printf("ERREUR : on attend PARF \n");
        arbre_op_g = creer_ecire(arbre_op_g);

        break;

    /* ajouter ici les autres fonctions */  
    /* IF seq cond THEN seq inst ELSE seq inst FI */
    case IF:
        avancer();
        Rec_seq_cond(&arbre_op_g);
        if (lexeme_courant().nature == THEN) avancer();
        else {
            error += 1;
            printf("ERREUR : on attend THEN \n");
            exit(0);
        }
        Rec_seq_inst(&arbre_op_c);
        if (lexeme_courant().nature == ELSE) avancer();
        else {
            error += 1;
            printf("ERREUR : on attend ELSE \n");
            exit(0);
        }
        Rec_seq_inst(&arbre_op_d);
        if (lexeme_courant().nature == FI) avancer();
        else {
            error += 1;
            printf("ERREUR : on attend FI \n");
            exit(0);
        }
        arbre_op_g = creer_if(arbre_op_g, arbre_op_c, arbre_op_d);
        break;
    // TANTQUE condition FAIRE seq inst FAIT
    case TANTQUE:
        avancer();
        Rec_seq_cond(&arbre_op_g);
        if (lexeme_courant().nature == FAIRE) avancer();
        else {
            error += 1;
            printf("ERREUR : on attend FAIRE \n");
            exit(0);
        }

        Rec_seq_inst(&arbre_op_d);
       
        if (lexeme_courant().nature == FAIT) avancer();
        else {
            error += 1;
            printf("ERREUR : on attend FAIT \n");
            exit(0);
        }
        arbre_op_g = creer_tantque(arbre_op_g, arbre_op_d);
        break;
        
    default:
        arbre_op_g = NULL;
        break;
    }  
    if (lexeme_courant().nature != SEPINST && arbre_op_g != NULL) {
        printf("ERROR MANQUE SEPINST \n");
        exit(0);
    }

    *arbre_resultat = arbre_op_g ;
}




/* suite_seq_inst -> SEPINST  seq_inst | Epsilon */

void Rec_suite_seq_inst(Ast * arbre_op_donne, Ast * arbre_op_resultat) {

    Ast arbre_op_2 ;
    switch (lexeme_courant().nature)
    {
    case SEPINST:
        avancer();
        Rec_seq_inst(&arbre_op_2);
        *arbre_op_resultat = creer_sepinst(*arbre_op_donne, arbre_op_2);
        break;
    default:

        *arbre_op_resultat = * arbre_op_donne;
        break;
    }
}


int estInstruction(void) {
    switch (lexeme_courant().nature)
    {
    case IDF:
    case IF:
    case TANTQUE:
    case ECRIRE:
    case LIRE:
        return 1;
        break;
    
    default:
        return 0;
        break;
    }
}

/* seq_inst -> inst suite_seq_inst */
void Rec_seq_inst(Ast * arbre_resultat) {
    Ast arbre_op_1;
    if (estInstruction()) {
        Rec_inst(&arbre_op_1);
        Rec_suite_seq_inst(&arbre_op_1, arbre_resultat);
    }
    else {
        *arbre_resultat = NULL;
        return;
    }

}






/* pgm  -> seq_inst */
void Rec_pgm(Ast * arbre) {
    Rec_seq_inst(arbre);
}






void analyser(char *nom_fichier, Ast *arbre) {
    if (mode_debug != 0) {
        evaluation_debug();
        printf("*** mode debug ***\n");
    }
    if (strcmp(nom_fichier, "") == 0) demarrer("");
    else demarrer(nom_fichier);
    /* initialisation de la sequence de table pour stocker les identificateurs .*/
    initialisationTable();
    while (!fin_de_sequence()) {
        switch (lexeme_courant().nature)
        {
        case LIRE:
        case ECRIRE:
        case IDF:
            Rec_pgm(arbre);

            break;
        case IDF_E:
            error += 1;
            printf("ERROR : IDF Errone \n");
            break;
        case ENTIER:
            EAG(arbre);
        default:
            printf("ERREUR SYNTAXIQUE : instruction errone \n");
            exit(0);
            break;
        }
       
    }
    if (error != 0) {
        printf("Error syntaxique \n");
        exit(0);
    }
    else printf("Analyse syntaxique : PASS \n");
   
    printf("\n ******* Affichage de L'arbre abstrait *******\n");
    afficherAst(*arbre); 
    
    printf("\n");
    evaluation_inst(*arbre);
    if (mode_debug == 0) {
        printf("******* Affichage de La table de valeur *******  \n");
        afficherTable();
    }


}
/*void ErrorSyntaxique() {
    printf("ERROR Syntaxique");
    exit(0);
}
*/

/*

void Rec_op(char *operateur) {
    switch (lexeme_courant().nature)
    {
    case PLUS:
    case MOINS:
    case MUL:
    case DIV:
        strcpy(operateur, lexeme_courant().chaine);
        avancer();
        break;
    
    default:
        ErrorSyntaxique();
        break;
    }
}



void Rec_eaep(int *resultat) {
    int vald, valg;
    char op;

    switch (lexeme_courant().nature)
    {
    case ENTIER:
        *resultat = lexeme_courant().valeur;
        avancer();
        break;
    case PARO:
        avancer();
        Rec_eaep(&valg);
        Rec_op(&op);
        Rec_eaep(&vald);
        *resultat = Evaluer(valg, op, vald);
        if (lexeme_courant().nature == PARF) {
            avancer();
        }
        else ErrorSyntaxique();
        break;
    default:
        ErrorSyntaxique();
        break;
    }
    
}

void analyser(int *resultat) {

    Rec_eaep(resultat);
}
*/

/*void analyser(float *resultat)
{
    Lexeme courant = lexeme_courant();
    switch (etat)
    {
    case E_INIT:
        switch (courant.nature)
        {
        case ENTIER:
            etat = E_ENTIER;
            *resultat = courant.valeur;
            break;
        case REEL:
            etat = E_ENTIER;
            *resultat = courant.valeurReel;
            break;
        default:
            printf("ERREUR SYNTAXIQUE :EAS ne commence pas par une operation ! \n");
            reinitialiser();
            etat = E_OP;
            break;
        }
        break;
    case E_ENTIER:
        switch (courant.nature)
        {
        case PLUS:
        case MOINS:
        case MUL:
        case DIV:
            etat = E_OP;
            break;

        default:
            printf("ERREUR SYNTAXIQUE :dans tout EAS entre deux entier il y'a une operation \n");
            break;
        }
        break;
    case E_OP:
        switch (courant.nature)
        {
        case ENTIER:
            etat = E_ENTIER;
            switch (precedent.nature)
            {
            case PLUS:
                *resultat += courant.valeur;
                break;
            case MOINS:
                *resultat -= courant.valeur;
                break;
            case MUL:
                *resultat *= courant.valeur;
                break;
            case DIV:
                if (courant.valeur == 0) {
                    printf("ERREUR SEMANTIQUE : division par zero non autorisée \n");
                    reinitialiser();
                    etat = E_OP;
                }
                *resultat /= courant.valeur;
            default:
                break;
            }
            break;
        case REEL:
            etat = E_ENTIER;
            switch (precedent.nature)
            {
            case PLUS:
                *resultat += courant.valeurReel;
                break;
            case MOINS:
                *resultat -= courant.valeurReel;
                break;
            case MUL:
                *resultat *= courant.valeurReel;
                break;
            case DIV:
                *resultat /= courant.valeurReel;
            default:
                break;
            }
            break;
        default:
            printf("ERREUR SYNTAXIQUE \n");
            reinitialiser();
            etat = E_OP;
            break;
        }
        break;
    default:
        printf("OK \n");
        break;
    }
    precedent = courant;
}
*/

void initialiser_etat(void) {
    etat = E_INIT;
}

void verdicte(float resultat) {
    switch (etat)
    {
    case E_INIT:
    case E_ENTIER:
        printf("%g \n", resultat);
        break;
    default:
        printf("hmm \n");
        break;
    }
}
