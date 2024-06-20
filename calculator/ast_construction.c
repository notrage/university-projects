#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "type_ast.h"

Ast creer_operation(TypeOperateur opr, Ast opde_gauche, Ast opde_droit)
{
    Ast expr;
    expr = (Ast) malloc(sizeof(NoeudAst));
    expr->nature = OPERATION;
    expr->operateur = opr;
    if (opde_gauche == NULL || opde_droit == NULL)
    {
        printf("ERREUR_CONSTRUCTION : OPERATION\n");
        exit(1);
    }
    else
    {
        expr->gauche = opde_gauche;
        expr->droite = opde_droit;
    };
    return expr;
}


Ast creer_lire(Ast opde_gauche) {
    Ast expr;
    expr = (Ast) malloc(sizeof(NoeudAst));
    expr->nature = INSTRUCTION;
    expr->inst = N_LIRE;
    if (opde_gauche == NULL) 
    {
        printf("ERREUR_CONSTRUCTION : LIRE \n");
        exit(1);
    }
    else 
    {
        expr->gauche = opde_gauche;
    }
    return expr;
}

Ast creer_ecire(Ast opde_gauche) {
    Ast expr;
    expr = (Ast) malloc(sizeof(NoeudAst));
    expr->nature = INSTRUCTION;
    expr->inst = N_ECRIRE;
    if (opde_gauche == NULL) 
    {
        printf("ERREUR_CONSTRUCTION : ECRIRE \n");
        exit(1);
    }
    else 
    {
        expr->gauche = opde_gauche;
    }
    return expr;
}

Ast creer_aff(Ast opde_gauche, Ast opde_droit) {
    Ast expr;
    expr = (Ast) malloc(sizeof(NoeudAst));
    expr->nature = INSTRUCTION;
    expr->inst = N_AFF;

    if (opde_gauche == NULL || opde_droit == NULL)
    {
        printf("ERREUR_CONSTRUCTION : AFFECTATION \n");
        exit(1);
    }
    else
    {
        expr->gauche = opde_gauche;
        expr->droite = opde_droit;
    }
    return expr;
}

Ast creer_sepinst(Ast opde_gauche, Ast opde_droit) {
    Ast expr;
    expr = (Ast) malloc(sizeof(NoeudAst));
    expr->nature = INSTRUCTION;
    expr->inst = N_SEPINST;
    if (opde_gauche == NULL && opde_droit == NULL)
    {
        printf("ERREUR_CONSTRUCTION : SEPINST\n");
        /* exit(1); */
    }
    else
    {
        expr->gauche = opde_gauche;
        expr->droite = opde_droit;
    }
    return expr;
}


Ast creer_if(Ast opde_gauche, Ast opde_central, Ast opde_droite) {
    Ast expr;
    expr = (Ast) malloc(sizeof(NoeudAst));
    expr->nature = INSTRUCTION;
    expr->inst = N_IF;
    if (opde_gauche == NULL)
    {
        printf("ERREUR_CONSTRUCTION : IF 'condition ne peut etre null...' \n");
        exit(0);
    }
    if (opde_central == NULL && opde_droite == NULL) 
    {
        printf("ERREUR_CONSTRUCTION : IF 'sans instruction ...' \n");
        exit(0);   
    }
    expr->gauche = opde_gauche;
    expr->central = opde_central;
    expr->droite = opde_droite;    

    return expr;
    
}

Ast creer_tantque(Ast opde_gauche, Ast opde_droite) {
    Ast expr;
    expr = (Ast) malloc(sizeof(NoeudAst));
    expr->nature = INSTRUCTION;
    expr->inst = N_TANTQUE;
    if (opde_gauche == NULL)
    {
        printf("ERREUR_CONSTRUCTION : TANTQUE 'condition ne peut etre null...' \n");
        exit(0);
    }
    if (opde_droite == NULL) 
    {
        printf("ERREUR_CONSTRUCTION : TANTQUE 'sans instruction ...' \n");
        exit(0);   
    }
    expr->gauche = opde_gauche;
    expr->droite = opde_droite;    

    return expr;
    
}

Ast creer_idf(char *nom) {
    Ast expr = (Ast) malloc(sizeof(NoeudAst));
    expr->nature = IDENTIFICATEUR;
    strcpy(expr->nom, nom);
    expr->gauche = NULL;
    expr->droite = NULL;
    return expr;
}


Ast creer_op_comp(int type_opcomp, Ast opde_gauche, Ast opde_droite) {
    Ast expr;
    expr = (Ast) malloc(sizeof(NoeudAst));
    expr->nature = OPERATEUR_COMPARAISON;
    switch (type_opcomp) {
    case 0: // !=
        expr->operateurComparaison = N_OP_COMP_DIFF;
        break;
    case 1: // >
        expr->operateurComparaison = N_OP_COMP_SUP;
        break;
    case 2: // >=
        expr->operateurComparaison = N_OP_COMP_SUP_EGAL;
        break;
    case 3: // <
        expr->operateurComparaison = N_OP_COMP_INF;
        break;
    case 4: // <=
        expr->operateurComparaison = N_OP_COMP_INF_EGAL;
        break;
    case 5: // ==
        expr->operateurComparaison = N_OP_COMP_EGAL;
        break;
    default:
        printf("ERREUR_CONSTRUCTION = OP_COMP non reconnu... \n");
        exit(0);
    }
    if (opde_gauche == NULL || opde_droite == NULL) 
    {
        printf("ERREUR_CONSTRUCTION : OP_COMP l'un des comparée est vide ...  \n");
        exit(0);   
    }
    expr->gauche = opde_gauche;
    expr->droite = opde_droite;
    return expr;

}

Ast creer_op_bool(int type_opbool, Ast opde_gauche, Ast opde_droite) {
    Ast expr;
    expr = (Ast) malloc(sizeof(NoeudAst));
    expr->nature = OPERATEUR_BOOL;
    switch (type_opbool)
    {
    case 0: // et
        expr->operateurBool = N_OP_BOOL_ET;
        break;
    case 1: // ou
        expr->operateurBool = N_OP_BOOL_OU;
        break;
    case 2: // non
        expr->operateurBool = N_OP_BOOL_NON;
        break;
    default:
        printf("ERREUR CREER OP BOOL : operateur bool non gerer \n ");
        exit(0);
        break;
    }
    switch (expr->operateurBool)
    {
    case N_OP_BOOL_ET:
    case N_OP_BOOL_OU:
        if (opde_gauche == NULL || opde_droite == NULL) {
            printf("ERREUR CREER OP BOOL : l'un des comparée est null \n");
            exit(0);
        }
        expr->gauche = opde_gauche;
        expr->droite = opde_droite;
        break;
    case N_OP_BOOL_NON:
        if (opde_gauche == NULL) {
            printf("ERREUR CREE OP BOOL : partie gauche de non ne peut etre null \n");
            exit(0);
        }
        expr->gauche = opde_gauche;
        expr->droite = NULL;
        break;
    default:
        printf("ERREUR CREE OP BOOL : Operateur non gerer \n");
        break;
    }
    return expr;

}

Ast creer_valeur(float val)
{
    Ast expr;
    expr = (Ast) malloc(sizeof(NoeudAst));
    expr->nature = VALEUR; // type de ast
    expr->valeur = val;
    return expr;
}
