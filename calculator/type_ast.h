
#ifndef __TYPE_AST__
#define __TYPE_AST__ 

typedef enum {OPERATION, INSTRUCTION, VALEUR, IDENTIFICATEUR, OPERATEUR_COMPARAISON, OPERATEUR_BOOL} TypeAst ;
typedef enum {N_PLUS, N_MUL, N_MOINS, N_DIV} TypeOperateur ;
typedef enum {N_LIRE, N_ECRIRE, N_AFF, N_SEPINST, N_IF, N_TANTQUE} TypeInstruction;
typedef enum {N_OP_COMP_SUP, N_OP_COMP_SUP_EGAL, N_OP_COMP_INF, N_OP_COMP_INF_EGAL, N_OP_COMP_EGAL, N_OP_COMP_DIFF} TypeOperateurComparaison;
typedef enum {N_OP_BOOL_ET, N_OP_BOOL_OU, N_OP_BOOL_NON} TypeOperateurBool;

typedef struct noeud { 
	TypeAst nature ;
	TypeOperateur operateur ;
	TypeInstruction inst;
	TypeOperateurComparaison operateurComparaison;
	TypeOperateurBool operateurBool;
	struct noeud *gauche, *central,*droite ;
	char nom[20];
    float valeur ;
} NoeudAst ;

typedef NoeudAst * Ast ;

#endif  
