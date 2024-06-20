#ifndef __AST_PARCOURS__
#define __AST_PARCOURS__

void afficherAst(Ast expr) ;
// affiche l'arbre abstrait de l'expression arithmetique expr

float evaluation_op(Ast expr) ;
// calcule la valeur de l'expression arithmetique expr
// FONCTION A COMPLETER !

void evaluation_inst(Ast expr) ;

void evaluation_debug(void);
// active mode debug de l'evaluation 

void testEvaluationInst(Ast expr);
#endif

