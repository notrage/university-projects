#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "table_symbole.h"


#define TAILLE_MAX 256
tableIdf *tableauIdentificateur;

void initialisationTable(void) {
    tableauIdentificateur = malloc(sizeof(tableIdf));
    ElementTableIdf *element = malloc(sizeof(ElementTableIdf));
    element->idf.nom = malloc(sizeof(char)*TAILLE_MAX);
    element->suivant = NULL;
    tableauIdentificateur->head = element;
    tableauIdentificateur->cmp = 0;

}

int estDansTable(char * nom, ElementTableIdf **element) {
    ElementTableIdf *actuel = tableauIdentificateur->head;
    
    if (tableauIdentificateur->cmp == 0) {
        printf("le tableau est vide ! \n");
        element = &actuel;
        return 0;
    }
    while (actuel != NULL)
    {
        *element = actuel;
        if (strcmp(actuel->idf.nom, nom) == 0){
            return 1;
        }
        actuel = actuel->suivant;
    }
    
    
    return 0;
    
}


int estDansTableExtern(char * nom) {
    ElementTableIdf *actuel = tableauIdentificateur->head;
    
    if (tableauIdentificateur->cmp == 0) {
        printf("le tableau est vide ! \n");
        return 0;
    }
    while (actuel != NULL)
    {
        if (strcmp(actuel->idf.nom, nom) == 0){
            return 1;
        }
        actuel = actuel->suivant;
    }
    
    
    return 0;
    
}




/* insérer / remplacer un couple (IDF, valeur) */
/* (A : 2)  (B : 2) */
void ModificationCouple(char * nom, float valeur) {
    ElementTableIdf *actuel = tableauIdentificateur->head;
    if (tableauIdentificateur->cmp == 0) {
        strcpy(actuel->idf.nom, nom);
        actuel->idf.valeur = valeur;
        tableauIdentificateur->cmp++;
    }
    else {
        if (estDansTable(nom, &actuel) ==  1) {
            actuel->idf.valeur = valeur;
            }
        else {
            ElementTableIdf *element = malloc(sizeof(ElementTableIdf));
            element->idf.nom = nom;
            element->idf.valeur = valeur;
            element->suivant = NULL;
            actuel->suivant = element;
        }
    }


}
/*
    actuel->idf.valeur
    actuel.idf.valeur

    -> <=> *()
*/

float rechercheValeur(char * nom) {
    ElementTableIdf *actuel = tableauIdentificateur->head;
    if (estDansTable(nom, &actuel) == 1) return actuel->idf.valeur;
    else  {
        printf("element inexistant \n");
        exit(0);
    }
}




void afficherTable(void) {
    ElementTableIdf *actuel = tableauIdentificateur->head;
    if (tableauIdentificateur->cmp == 0) {
        printf("tableau vide \n");
        return ;
    }
    while(actuel != NULL) {
        printf("(%s : %g) \t", actuel->idf.nom, actuel->idf.valeur);
        actuel = actuel->suivant;
    }
    printf("\n");
}