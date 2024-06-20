#include <stdio.h>

#include "ast_construction.h"
#include "ast_parcours.h"

int main()
{

    Ast ast1, ast2, ast3, ast4, ast5, ast6, ast7;
    /*
        lire (X) ;
        Y := X+1 ;
        ecrire (Y * 2) ;
    */
    // lire (X)
    /*
    ast1 = creer_idf("X");
    ast7 = creer_lire(ast1);


    ast1 = creer_idf("Y");

    ast2 = creer_idf("X");
    ast3 = creer_valeur(1);
    
    ast2 = creer_operation(N_PLUS, ast2, ast3); 

    ast4 = creer_aff(ast1, ast2);  */




    // Y := X+1 ; ecrire (Y * 2) ; NULL


    /* ecrire (Y * 2) ; NULL*/
    /*
    ast1 = creer_idf("Y");
    ast2 = creer_valeur(2);
    ast1 = creer_operation(N_MUL, ast1, ast2);

    ast1 = creer_ecire(ast1);

    ast1 = cree_sepinst(ast1, NULL);


    // Y := X+1 ; ecrire (Y * 2) ; NULL
    ast3 = cree_sepinst(ast4, ast1); */



    /*
        lire (X) ;
        Y := X+1 ;
    */
    /*
    ast6 = cree_sepinst(ast7, ast3);
    
    printf("########### Arbre abstrait de l'expression ###########\n");
    afficherAst(ast6);
    printf("####################################################### \n");

    printf(" affichage evaluation \n");
    testEvaluation(ast6);
    */



    /*
    x = 5;
    if x > 2 then
      x = 0; 
    else
      x = 1; 
    fi; */
    
    // #######################################################
    // #######################################################
    // #########  test ast instruction conditionnel  #########
    // #######################################################
    // #######################################################
    /*
    ast5 = creer_aff(creer_idf("x"), creer_valeur(10.5));

    ast1 = creer_idf("x");
    ast2 = creer_valeur(2);
    ast1 = creer_op_comp_sup(ast1, ast2);
    // afficherAst(ast1);
    ast2 = creer_idf("x");
    ast3 = creer_valeur(-100);
    ast2 = creer_aff(ast2, ast3);
    ast2 = creer_sepinst(ast2, NULL);
    // printf("\n");
    // afficherAst(ast2);
    ast3 = creer_idf("x");
    ast4 = creer_valeur(100);
    ast3 = creer_aff(ast3, ast4);
    ast3 = creer_sepinst(ast3, NULL);
    // afficherAst(ast3);
    ast1 = creer_if(ast1, ast2, ast3);
    ast1 = creer_sepinst(ast1, NULL);
    ast1 = creer_sepinst(ast5, ast1);
    // printf("\n");

    printf("affichage arbre abstrait \n");
    afficherAst(ast1); */

    

    /*
    x = 0;
    tantque x < 3 faire
      x = x + 1; 
      ecrire(x); 
    fait; 
    */
    
    // #######################################################
    // #######################################################
    // ######  test ast instruction iterative tantque  #######
    // #######################################################
    // #######################################################

    /*

    ast1 = creer_op_comp(3, creer_idf("x"), creer_valeur(3)); // x < 3

    ast2 = creer_aff(creer_idf("x"), creer_operation(N_PLUS, creer_idf("x"), creer_valeur(1))); // x = x + 1
    ast3 = creer_sepinst(creer_ecire(creer_idf("x")), NULL); // ecrire(x);
    ast2 = creer_sepinst(ast2, ast3);  

    ast1 = creer_sepinst(creer_tantque(ast1, ast2), NULL);
    ast1 = creer_sepinst(creer_aff(creer_idf("x"), creer_valeur(0)), ast1);
    printf("affichage arbre abstrait \n");
    afficherAst(ast1);

    printf("affichage evaluation \n");
    testEvaluationInst(ast1); 
    */


    // #######################################################
    // #######################################################
    // ####  test ast instruction conditionnel avec opbool ###
    // #######################################################
    // #######################################################
    
    // x > 2 et y > 2

    ast1 = creer_op_comp(1, creer_idf("x"), creer_valeur(2)); // x > 2
    ast2 = creer_op_comp(1, creer_idf("y"), creer_valeur(2)); // y > 2
    ast1 = creer_op_bool(0, ast1, ast2);
    ast1 = creer_sepinst(ast1, NULL);
    // ast2 = creer_op_bool(2, ast2, NULL);
    printf("Arbre abstrait de l'expression\n");
    afficherAst(ast1);

    printf("affichage evaluation \n");
    testEvaluationInst(ast1); 
    return 0;
}
