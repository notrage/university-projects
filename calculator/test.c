#include <stdio.h>
#include <stdlib.h>

typedef struct Element Element;

struct Element{
	int a;
	Element *suivant;
} ;

typedef struct {
	Element *head;
} ListeC;

ListeC *maliste;

int main(void) {
	void insert(ListeC *maliste, int value);
	void afficherL(ListeC *maliste);
	void metLafin(Element **element);
	maliste = (ListeC *) malloc(sizeof(ListeC));
	maliste->head = (Element *) malloc(sizeof(Element));
	maliste->head->a = 0;
	maliste->head->suivant = NULL;
	insert(maliste, 7);

	Element *actuel = maliste->head;
	printf("%d \n", actuel->a);
	metLafin(&actuel);
	printf("%d \n", actuel->a);
	afficherL(maliste);

	return 0;
}


void insert(ListeC *maliste, int value) {
	Element *actuel = maliste->head;
	while (actuel->suivant != NULL) {
		actuel = actuel->suivant;
	}
	Element *nouveau = (Element *) malloc(sizeof(Element));
	nouveau->a = value;
	nouveau->suivant = NULL;
	actuel->suivant = nouveau;
}

void metLafin(Element **element) {
	Element *actuel = maliste->head;
	while (actuel->suivant != NULL) {
		actuel = actuel->suivant;
	}
	*element = actuel;
}

void afficherL(ListeC *maliste) {
	printf("affichage Liste \n");
	Element *actuel = maliste->head;
	while (actuel != NULL)
	{
		printf("%d \n", actuel->a);
		actuel = actuel->suivant;
	}
	
} 