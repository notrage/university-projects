#include "table_symbole.h"
#include <stdio.h>

int main(void) {
    initialisationTable();
    ModificationCouple("AB", 2);
    afficherTable();
    ModificationCouple("Y", 1);
    afficherTable();
    

    return 0;
}