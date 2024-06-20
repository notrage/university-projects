CC=gcc -g -Wall

PROGRAMMES=calculator test_lexeme test_ast test_table_symbole

all: $(PROGRAMMES)

calculator: analyse_lexicale.o analyse_syntaxique.o lecture_caracteres.o calculator.o ast_parcours.o ast_construction.o table_symbole.o
	$(CC) -o calculator analyse_syntaxique.o analyse_lexicale.o  lecture_caracteres.o ast_construction.o ast_parcours.o table_symbole.o calculator.o

test_lexeme: analyse_lexicale.o  lecture_caracteres.o  test_lexeme.o
	$(CC) -o test_lexeme analyse_lexicale.o  lecture_caracteres.o  test_lexeme.o

test_ast: ast_construction.o ast_parcours.o test_ast.o table_symbole.o
	$(CC) -o $@ $^

test_table_symbole: table_symbole.o test_table_symbole.o
	$(CC) -o test_table_symbole table_symbole.o test_table_symbole.o

#####################	dependencies	#####################

analyse_syntaxique.o: analyse_syntaxique.c analyse_lexicale.h analyse_syntaxique.h ast_parcours.h  type_ast.h table_symbole.h
	$(CC) -c analyse_syntaxique.c

analyse_lexicale.o: analyse_lexicale.c analyse_lexicale.h lecture_caracteres.h 
	$(CC) -c analyse_lexicale.c

lecture_caracteres.o: lecture_caracteres.c lecture_caracteres.h 
	$(CC) -c lecture_caracteres.c

test_lexeme.o: analyse_lexicale.h
	$(CC) -c test_lexeme.c

calculator.o: analyse_syntaxique.h lecture_caracteres.h 
	$(CC) -c calculator.c

#####################	makefile -> ast	#####################
test_ast.o : test_ast.c ast_construction.h ast_parcours.h table_symbole.h
	$(CC) -c test_ast.c

ast_construction.o: ast_construction.c type_ast.h
	$(CC) -c ast_construction.c

ast_parcours.o: ast_parcours.c type_ast.h
	$(CC) -c ast_parcours.c



#####################  makefile -> table_symbole #####################
test_table_symbole.o : test_table_symbole.c table_symbole.h
	$(CC) -c test_table_symbole.c

table_symbole.o: table_symbole.c table_symbole.h 
	$(CC) -c table_symbole.c



#######################################################################
clean:
	rm -f  test_ast test_table_symbole test_lexeme calculator *.o

