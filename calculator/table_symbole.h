#ifndef ___TS___
#define ___TS___

#define TAILLE_MAX 256

typedef struct {
    char * nom;
    float valeur;
} Identificateur ;

typedef struct ElementTableIdf ElementTableIdf ;

struct ElementTableIdf {
    Identificateur idf;
    ElementTableIdf *suivant;
} ;

typedef struct {
    ElementTableIdf *head;
    int cmp;
} tableIdf ;

void initialisationTable(void);

void ModificationCouple(char * nom, float valeur) ;

float rechercheValeur(char * A);

void afficherTable(void);

int estDansTableExtern(char * nom);

#endif