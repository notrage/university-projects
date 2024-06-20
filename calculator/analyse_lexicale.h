/* ------------------------------------------------------------------------
-- paquetage machine_lexemes
--
-- analyse lexicale d'une expression arithmetique
--
-- P.Habraken - 18/01/2010
--
-- 2 aout 2006 - PH :
-- remplacement record a champs variants par record "plat"
------------------------------------------------------------------------ */



   // definition des constantes ;
   
   #define TAILLE_MOT 16
   #define NB_MOT 9

   #define TAILLE_OP_BOOL 5
   #define NB_OP_BOOL 3


   typedef enum {
	  ERREUR_CAR,         // caractere incorrect
	  CHIFFRE,		 	    // chiffre 
	  SYMBOLE,			    // symbole arithmetique
     C_FIN_SEQUENCE,     // caractere de fin de sequence 

     CARACTERE,          // Caractere aphabetique 
     C_AFFECTATION,      // Caractere d'affectation
     C_FIN_AFFECTATION,  // Catactere de fin d'affectation

     OPCOMP,  // operateur de comparaison
     OPBOOL, // operateur bool

     EXCLA // Caractere bizzard ...

   } Nature_Caractere ;

   typedef enum { 
      ENTIER,           // sequence de chiffres
      REEL,             // (sequence de chiffres).caractere('.').(sequence de chiffres)
      PLUS,             // +
      MOINS,            // -
      MUL,              // *
      DIV,              // /
      PARO,             // (
      PARF,             // )
      FIN_SEQUENCE,     // pseudo lexeme ajoute en fin de sequence


      IDF, // identificateur (un caractere alphabetique)
      IDF_E, // identificateur errone, non conforme.  par exemple "un idf commencant par un chiffre..."
      AFF, // Affectation (=)
      SEPINST, // separateur d'affectation  (';','\n')

      // ########## mot-clès ##########

      // Entre-Sortie
      LIRE, 
      ECRIRE,

      // Instruction Conditionnelle
      IF,
      THEN,
      ELSE,
      FI,

      // Operateur conditionnel
      OP_COMP_SUP_EGAL, // >=
      OP_COMP_SUP, // >
      OP_COMP_INF_EGAL, // <=
      OP_COMP_INF, // <
      OP_COMP_DIFF, // !=
      OP_COMP_EGAL, // ==

      // Caractere de merde 
      C_EXCLA, // !

      // Caractere booleen
      OP_BOOL_ET,
      OP_BOOL_OU,
      OP_BOOL_NON,


      // Instruction Iterative

      TANTQUE,
      FAIRE,
      FAIT
   } Nature_Lexeme ;

   typedef struct { 
      Nature_Lexeme nature;   // nature du lexeme
      unsigned int ligne;     // numero de ligne
      unsigned int colonne;   // numero de colonne
      char chaine[256];       // chaine de caracteres
      float valeur;             // valeur d'un entier
   } Lexeme ; 

   void afficher(Lexeme l);

   void demarrer(char *nom_fichier);
   // e.i. : indifferent
   // e.f. : la sequence de lexemes est lue dans le fichier designe par
   //        nom_fichier
   //        fin_de_sequence <=> lexeme_courant.nature = FIN_SEQUENCE
   //        (non fin_de_sequence) => lexeme courant est le premier
   //        lexeme de la sequence
   //        l'exception Erreur_Lexicale est levee en cas d'erreur

   void avancer();
   // pre-condition : la machine sequentielle est demarree
   // e.i. : on pose i = rang du lexeme_courant :
   //        (non fin_de_sequence)
   //        et (non lexeme_courant.nature = FIN_SEQUENCE)
   // e.f. : fin_de_sequence <=> lexeme_courant.nature = FIN_SEQUENCE
   //        (non fin_de_sequence) => lexeme_courant est le lexeme i+1
   //        l'exception Erreur_Lexicale est levee en cas d'erreur

   Lexeme lexeme_courant();
   // pre-condition : la machine sequentielle est demarree
   // lexeme_courant est :
   // . le pseudo lexeme FIN_SEQUENCE si fin_de_sequence
   // . le pseudo lexeme ERREUR si une erreur a ete detectee
   // . le lexeme de rang i sinon

   int fin_de_sequence();
   // pre-condition : la machine sequentielle est demarree
   // fin_de_sequence vaut :
   // . vrai si tous les lexemes de la sequence ont ete reconnus
   // . faux sinon

   void arreter();
   // e.i. : la machine sequentielle est demarree
   // e.f. : la machine sequentielle est arretee

   int power(int a, int b);

   void reinitialiser(void);