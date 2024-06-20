#include <stdio.h>
#include <stdlib.h>
#include "type_ast.h"
#include "table_symbole.h"


int count = 0;
int lvl_x = 0;
int lvl_y = 0;
float temp = 0;

int mode_debug_evaluation = 0;
int evaluationSeqComparaison(Ast expr) ;

void evaluation_debug (void) {
	mode_debug_evaluation += 1;
}


void aff_operateur(TypeOperateur op)
{
	switch (op)
	{
	case N_PLUS:
		printf("+");
		break;
	case N_MOINS:
		printf("-");
		break;
	case N_MUL:
		printf("*");
		break;
	case N_DIV:
		printf("/");
		break;
	}
}

void aff_instruction(TypeInstruction inst)
{
	switch (inst)
	{
	case N_LIRE:
		printf("Lire");
		break;
	case N_ECRIRE:
		printf("Ecrire");
		break;
	case N_AFF:
		printf(" = ");
		break;
	case N_SEPINST:
		printf("; \n");
		break;
	case N_IF:
		printf("if ");
		break;
	case N_TANTQUE:
		printf("tantque ");
		break;
	default:
		printf("ERROR aff_instruction : autre instruction non gerer \n");
		break;
	}
}


void aff_op_comparaison(TypeOperateurComparaison op_comp)
{
	switch (op_comp)
	{
	case N_OP_COMP_SUP:
		printf(" > ");
		break;
	case N_OP_COMP_SUP_EGAL:
		printf(" >= ");
		break;
	case N_OP_COMP_INF:
		printf(" < ");
		break;
	case N_OP_COMP_INF_EGAL:
		printf(" <= ");
		break;
	case N_OP_COMP_EGAL:
		printf(" == ");
		break;
	case N_OP_COMP_DIFF:
		printf(" != ");
		break;
	default:
		printf("Autre OP_COMP non gerer \n");
		break;
	}
}

void aff_op_bool(TypeOperateurBool op_bool)
{
	switch (op_bool)
	{
	case N_OP_BOOL_ET:
		printf(" et ");
		break;
	case N_OP_BOOL_OU:
		printf(" ou ");
		break;
	case N_OP_BOOL_NON:
		printf(" non ");
		break;
	default:
		printf("Autre OP_BOOL non gerer \n");
		break;
	}
}

void afficherAst(Ast expr)
{
	if (expr == NULL) {
		return ;
	}
	switch (expr->nature)
	{
	case OPERATION:
		printf("(");
		if (expr->gauche != NULL) afficherAst(expr->gauche);
		aff_operateur(expr->operateur);
		if (expr->droite != NULL) afficherAst(expr->droite);
		printf(")");
		break;
	case INSTRUCTION:
		switch (expr->inst)
		{
		case N_AFF:
			if (expr->gauche != NULL) afficherAst(expr->gauche);
			aff_instruction(expr->inst);
			if (expr->droite != NULL) afficherAst(expr->droite);
			break;
		case N_ECRIRE:
		case N_LIRE:
			if (lvl_x != 0) printf("  ");
			aff_instruction(expr->inst);
			printf("(");
			if (expr->gauche != NULL) afficherAst(expr->gauche);
			printf(")");
			break;
		case N_SEPINST:

			if (expr->gauche != NULL) afficherAst(expr->gauche);
			aff_instruction(expr->inst);

			if (expr->droite != NULL) {
				afficherAst(expr->droite);
			}
			break;
		case N_IF:
			aff_instruction(expr->inst);
			afficherAst(expr->gauche);
			printf(" then\n");
			lvl_y = 0;
			lvl_x = 1;
			if (expr->central != NULL) {afficherAst(expr->central);lvl_y = 0;}
			if (expr->droite != NULL) {
				printf("else\n");
				afficherAst(expr->droite);
			}
			printf("fi");
			lvl_x = 0;
			break;
		case N_TANTQUE:
			aff_instruction(expr->inst);
			afficherAst(expr->gauche);
			printf(" faire\n");
			lvl_y = 0;
			lvl_x = 1;
			if (expr->droite != NULL) {
				afficherAst(expr->droite);
			}
			printf("fait");
			lvl_x = 0;
			break;
		default:
			break;
		}

		break;
	case OPERATEUR_COMPARAISON:
		afficherAst(expr->gauche);
		aff_op_comparaison(expr->operateurComparaison);
		afficherAst(expr->droite);
		break;
	case OPERATEUR_BOOL:
		switch (expr->operateurBool)
		{
		case N_OP_BOOL_NON:
			aff_op_bool(expr->operateurBool);
			afficherAst(expr->gauche);
			break;
		case N_OP_BOOL_ET:
		case N_OP_BOOL_OU:
			afficherAst(expr->gauche);
			aff_op_bool(expr->operateurBool);
			afficherAst(expr->droite);
			break;
		default:
			break;
		}

		break;
	case IDENTIFICATEUR:
		if (lvl_x == 0 || ((lvl_x == 1) && (lvl_y == 1))) printf("%s", expr->nom);
		else {
			lvl_y = 1;
			printf("  %s", expr->nom);
		}
		break;
	case VALEUR:
		printf("%g", expr->valeur);
		break;
	default:
		break;
	}
}


void active_evaluation_debug (Ast expr) {
	char buffer;
	printf("instruction evalue : ");
	afficherAst(expr);
	printf("\n");
	afficherTable();
	scanf("%c", &buffer);
}

float evaluation_op(Ast expr)
{
	switch (expr->nature)
	{
	case OPERATION:

		switch (expr->operateur)
		{
		case N_PLUS:
			return evaluation_op(expr->gauche) + evaluation_op(expr->droite);
			break;
		case N_MOINS:
			return evaluation_op(expr->gauche) - evaluation_op(expr->droite);
			break;
		case N_MUL:
			return evaluation_op(expr->gauche) * evaluation_op(expr->droite);
			break;
		case N_DIV:
			if (evaluation_op(expr->droite) != 0) 
				return evaluation_op(expr->gauche) / evaluation_op(expr->droite);
			else {
				printf("ERREUR : Evaluation, division par zéro interdite ! \n");
				exit(0);

			}
			break;
		}
		break;


	case IDENTIFICATEUR:

		if (estDansTableExtern(expr->nom)) return rechercheValeur(expr->nom);
		else printf("element non existant dans table ... \n");
		break;
	case VALEUR:
		return expr->valeur;
		break;
	default:
		printf("ERROR : Evaluation operation \n");
		break;
	}
	return -1;
}


void evaluation_inst(Ast expr) {
	switch (expr->nature) {
		case INSTRUCTION:
			switch (expr->inst)
			{
			case N_SEPINST:
				if (expr->gauche != NULL) evaluation_inst(expr->gauche);
				if (expr->droite != NULL) evaluation_inst(expr->droite);
				break;
			case N_AFF:
				if (expr->gauche->nature != IDENTIFICATEUR) 
				{
					printf("ERREUR_Evaluation_instruction : affectation commence par un identificateur valide ! \n");
					exit(0);
				}
				expr->gauche->valeur = evaluation_op(expr->droite);
				ModificationCouple(expr->gauche->nom, expr->gauche->valeur);
				if (mode_debug_evaluation != 0) {
					active_evaluation_debug(expr);
				}
				break;
			case N_ECRIRE:
				printf(">>> %g \n", evaluation_op(expr->gauche));
				break;
			case N_LIRE:
				printf("=> ");
				scanf("%f", &temp);
				expr->gauche->valeur = temp;
				ModificationCouple(expr->gauche->nom, temp);
				if (mode_debug_evaluation != 0) {
					active_evaluation_debug(expr);
				}
				break;
			case N_IF:
				if (evaluationSeqComparaison(expr->gauche)) evaluation_inst(expr->central);
				else evaluation_inst(expr->droite);
				break;
			case N_TANTQUE:
				while (evaluationSeqComparaison(expr->gauche)) evaluation_inst(expr->droite);
				break;
			default:
				printf("ERROR: Nature d'expression non geree\n");
				break;
			}
		break;
		default:
			printf("ERROR : Evaluation instruction \n");
			break;
	}
}

int evaluationComparaison(Ast expr) {
	switch (expr->operateurComparaison)
	{
	case N_OP_COMP_DIFF:
		return evaluation_op(expr->gauche) != evaluation_op(expr->droite); 
	case N_OP_COMP_SUP:
		return evaluation_op(expr->gauche) > evaluation_op(expr->droite);
	case N_OP_COMP_SUP_EGAL:
		return evaluation_op(expr->gauche) >= evaluation_op(expr->droite); 
	case N_OP_COMP_INF:
		return evaluation_op(expr->gauche) < evaluation_op(expr->droite); 
	case N_OP_COMP_INF_EGAL:
		return evaluation_op(expr->gauche) <= evaluation_op(expr->droite);
	case N_OP_COMP_EGAL:
		return evaluation_op(expr->gauche) == evaluation_op(expr->droite);  
	default:
		printf("Error_Evaluation_Comparaison : autre cas pas encore gere :) \n");
		return -1;
		break;
	}
}




int evaluationBool(Ast expr) {
	switch (expr->operateurBool)
	{
	case N_OP_BOOL_ET:
		return evaluationSeqComparaison(expr->gauche) && evaluationSeqComparaison(expr->droite); 
	case N_OP_BOOL_OU:
		return evaluationSeqComparaison(expr->gauche) || evaluationSeqComparaison(expr->droite);
	case N_OP_BOOL_NON:
		return !(evaluationSeqComparaison(expr->gauche)); 
	default:
		printf("Error_Evaluation_Bool : autre cas pas encore gere :) \n");
		return -1;
		break;
	}
}

int evaluationSeqComparaison(Ast expr) {
	switch (expr->nature)
	{
	case OPERATEUR_COMPARAISON:
		return evaluationComparaison(expr);
		break;
	case OPERATEUR_BOOL:
		return evaluationBool(expr);
	default:
		printf("ERREOR EVALUATION SEQ COMPARAISON \n");
		exit(0);
		break;
	}
}

void testEvaluationInst(Ast arbre) {
	initialisationTable();
	evaluation_inst(arbre);
}