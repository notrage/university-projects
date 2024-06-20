
#ifndef __AST_CONSTRUCTION__
#define __AST_CONSTRUCTION__

#include "type_ast.h"

Ast creer_operation(TypeOperateur opr , Ast opde_gauche , Ast opde_droit) ;
// renvoie un arbre abstrait de nature OPERATEUR,  d'operateur opr 
// et ayant opde_gauche et opde_droit pour fils gauche et droit


Ast creer_idf(char *nom) ;
// renvoie un arbre abstrait de nature IDENTIFICATEUR
// d'identificateur nom


Ast creer_valeur(float val) ; 
// renvoie un arbre abstrait "feuille", de nature VALEUR  
// et de valeur val


Ast creer_aff(Ast opde_gauche, Ast opde_droit) ;
// renvoie un arbre abstrait de nature INSTRUCTION,  d'instruction aff 
// et ayant opde_gauche et opde_droit pour fils gauche et droit


Ast creer_sepinst(Ast opde_gauche, Ast opde_droit);
// renvoie un arbre abstrait de nature INSTRUCTION,  d'instruction sepinst
// et ayant opde_gauche et opde_droit pour fils gauche et droit


Ast creer_lire(Ast opde_gauche);
// renvoie un arbre abstrait de nature INSTRUCTION,  d'instruction lire 
// et ayant opde_gauche et opde_droit pour fils gauche et droit


Ast creer_ecire(Ast opde_gauche) ;
// renvoie un arbre abstrait de nature INSTRUCTION,  d'instruction ecrire 
// et ayant opde_gauche et opde_droit pour fils gauche et droit


Ast creer_if(Ast opde_gauche, Ast opde_central, Ast opde_droite) ;
// renvoie un arbre abstrait de nature INSTRUCTION, d'instruction IF


Ast creer_tantque(Ast opde_gauche, Ast opde_droite) ;
// renvoie un arbre abstrait de nature INSTRUCTION, d'instruction TANTQUE


Ast creer_op_comp(int type_opcomp, Ast opde_gauche, Ast opde_droite) ;
// renvoie un arbre abstrait de nature OP_COMP, d'OPCOMP type_opcomp



Ast creer_op_bool(int type_opbool, Ast opde_gauche, Ast opde_droite) ;
// renvoie un arbre abstrait de nature OP_BOOL, d'OPBOOL type_opbool



#endif
