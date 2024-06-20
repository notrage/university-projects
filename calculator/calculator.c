

/* ########################################## */
/* ###########      Author        ########### */
/* ###########   Mohamed Badzi    ########### */
/* ###########   Dylan Grousson   ########### */
/* ########################################## */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "analyse_syntaxique.h"
#include "lecture_caracteres.h"



Ast mon_arbre;

int main(int argc, char *argv[])
{
    switch (argc)
    {
    case 1:
        // demarre l'analyse lexicale sur stdin
        printf("nombre d'arguments incorrects ! il faut le nom du fichier a interpreter en argument par exemple './calculate entree1.txt'\n");
        exit(0);
        break;
    case 2:
        // demarre l'analyse lexicale sur le fichier transmis en argument
        if (strcmp("-d", argv[1]) == 0) {
            printf("nombre d'arguments incorrects !\n");
            exit(0);
        }
        analyser(argv[1], &mon_arbre);
        break;
    case 3:
        if (strcmp("-d", argv[1]) != 0) {
            printf("option non gerer choisissez entre ces options : '-d'\n");
            exit(0);
        }
        debug();
        analyser(argv[2], &mon_arbre);
        break;
    default:
        printf("nombre d'arguments incorrects !\n");
        exit(0);
    }
    return 0;
}