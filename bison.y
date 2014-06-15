/**
	@author: Michal Robaszynski 194165
*/

%{

#include <stdio.h>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>

int yylex(void);
extern int yylineno;

int yyerror(const char *error) 
{ 
    printf("ERROR: Line %d - %s\n", yylineno, error ); 
}

void catch_error(int line, const char* err);
void err(int line, int no);

#include "translate.c"

%}

%union{ char *stru; char *numu; }
%token <stru> CONST CASSIGN /* blok przypisywania stalych*/
%token <stru> VAR /* blok deklaracji zmiennych */
%token <stru> START END /* blok kodu */
%token <stru> LCOMMENT RCOMMENT
%token <stru> IF THEN ELSE
%token <stru> WHILE DO
%token <stru> READ WRITE
%token <stru> ASSIGN
%token <stru> PLUS MINUS MULT DIV MOD
%token <stru> EQ LT LET GT GET NEQ
%token <numu> NUM
%token <stru> IDENTIFIER
%token <stru> SEMICOLON

%%
program : 	| 
		CONST cdeclarations VAR vdeclarations START commands END
		{
		    endOfProgram();
		}
;

cdeclarations : cdeclarations IDENTIFIER CASSIGN NUM
		{
			int result=declareConstant($<stru>2, $<stru>4);
			switch(result){
				case 1:
					{
					str err = "Redeklaracja stalej ";
					err += $<stru>2;
					catch_error(yylineno,  err.c_str());
					}
					break;
				case 0:
					if(DEBUG)printf("\tUdana deklaracja stalej\n");
					break;
			}
		}
		| { if(DEBUG)printf("\n****** BLOK DEKLARACJI STALYCH ******\n\n\n"); }
;
		
vdeclarations : vdeclarations IDENTIFIER
		{
		    int result=declareVariable($<stru>2);
			switch(result){
				case 2:
					{
					str err = "Istnieje juz stala o takiej nazwie = ";
					err += $<stru>2;
					catch_error(yylineno, err.c_str());
					}
					break;
				case 1:
					{
					str err = "Redeklaracja zmiennej ";
					err += $<stru>2;
					catch_error(yylineno, err.c_str());
					}
					break;
				case 0:
					if(DEBUG)printf("\tUdana deklaracja zmiennej\n");
					break;
			}
		}
		| { if(DEBUG)printf("\n\n****** BLOK DEKLARACJI ZMIENNYCH ******\n\n\n"); }
;

commands : commands command 
      	| { if(DEBUG)printf("\n\n****** BLOK KOMENDY ******\n\n\n"); }
;

command : IDENTIFIER ASSIGN expression SEMICOLON
	{
	    int result=generateVariableAssign($<stru>1, $<stru>3);    
		switch(result){
			case 1:
				{
				str err = "Niezadeklarowana zmienna ";
				err += $<stru>1;
				catch_error(yylineno, err.c_str());
				}
				break;
			case 0:
				if(DEBUG)printf("\tUdane przypisanie do zmiennej\n");
				break;
			case 2:
				{
				str err = "Nie mozna przypisac do stalej ";
				err += $<stru>1;
				catch_error(yylineno, err.c_str());
				}
				break;
		}
	}
    | IF 
	{
	    if(DEBUG)printf("IF");
		generateIf();
	}
	condition THEN commands
	{
		if(DEBUG)printf("Warunki then ...\n");
		generateThen();
	}	
	ELSE commands END
	{
		generateElse();
	    if(DEBUG)printf("ELSE!\n");
	}
    | WHILE 
	{
	    generateWhile();
		if(DEBUG)printf("Obsluga while\n");
	}	
	condition DO commands END
	{	
		generateDo();
	    if(DEBUG)printf("Condition do while\n");
	}	
        | READ IDENTIFIER SEMICOLON
	{
	    int result=generateRead($<stru>2);
		if(result!=0){
			err(yylineno, result); //fixit
		}
	}
        | WRITE VALUE SEMICOLON
	{ 
	    int result=generateWrite($<stru>2);
		if(result!=0){
			err(yylineno, result);
		}
	}
;

arith_op : 
	PLUS | MINUS | MULT | DIV | MOD
	;
	
bool_op :
	EQ | NEQ | LT | GT | LET | GET
	;
	
expression: VALUE
			{	
				if(DEBUG)printf("Generuje wartosc %s\n", $<stru>1);
				/*if(isNumber($<stru>1)){
					generateValue($<stru>1);
				}else{
					int ind=constantManager.getItemIndex($<stru>1);
					if(ind!=-1){
						generateValue(constantManager.getItemValue(ind));
					}else{
					}
				}*/
				int result=generateP_A($<stru>1);
				if(result){err(yylineno, result);}
			}
			| VALUE PLUS VALUE 
			{
				if(DEBUG)printf("Operacja arytmetyczna %s %s\n", $<stru>1, $<stru>3);
				
				int result=generateArithOp("+", $<stru>1, $<stru>3);
				if(result){err(yylineno, result);}
			}
			| VALUE MINUS VALUE
			{
				if(DEBUG)printf("Operacja arytmetyczna %s %s\n", $<stru>1, $<stru>3);
				
				int result=generateArithOp("-", $<stru>1, $<stru>3);
				if(result){err(yylineno, result);}
			}
			| VALUE MULT VALUE
			{
				int result=generateArithOp(S_MULT, $<stru>1, $<stru>3);
				if(result){err(yylineno, result);}
			}
			| VALUE DIV VALUE
			{
				int result=generateArithOp(S_DIV, $<stru>1, $<stru>3);
				if(result){err(yylineno, result);}
			}
			| VALUE MOD VALUE
			{
				int result=generateArithOp(S_MOD, $<stru>1, $<stru>3);
				if(result){err(yylineno, result);}
			}
			
	;


	
condition :  VALUE
	{
		int result=generateP_A($<stru>1);
		if(result){err(yylineno, result);}
	}
	| VALUE EQ VALUE
	{
		int result=generateBoolOp(S_EQ, $<stru>1, $<stru>3);
		if(result){err(yylineno, result);}
	}	
	| VALUE NEQ VALUE
	{
		int result=generateBoolOp(S_NEQ, $<stru>1, $<stru>3);
		if(result){err(yylineno, result);}
	}
	| VALUE LT VALUE
	{
		int result=generateBoolOp(S_GT, $<stru>3, $<stru>1);
		if(result){err(yylineno, result);}
	}
	| VALUE GT VALUE
	{
		int result=generateBoolOp(S_GT, $<stru>1, $<stru>3);
		if(result){err(yylineno, result);}
	}
	| VALUE GET VALUE 
	{	
		int result=generateBoolOp(S_GET, $<stru>1, $<stru>3);
		if(result){err(yylineno, result);}
	}
	| VALUE LET VALUE
	{
		int result=generateBoolOp(S_GET, $<stru>3, $<stru>1); //odwracamy argumenty, bo (a>=b) <=> (b<=a)
		if(result){err(yylineno, result);}
	}
	
;

VALUE: IDENTIFIER
	| NUM
%%

void err(int line, int no){
	char txt[50];
	switch(no){
		case(-1):
			sprintf(txt, "Nieznana zmienna");
			break;
		case(-3):
			sprintf(txt, "Proba zmiany stalej");
			break;
		case(-5):
			sprintf(txt, "Niezainicjalizowana zmienna");
			break;
		default:
			sprintf(txt, "BLAD NUMER %d", no);
			break;
	}
	
	
	catch_error(line, txt);
}

void catch_error(int linia, const char* err){
	printf("BLAD: [Linia %d] %s\n", linia, err);
	exit(0); //kompiluje do pierwszego napotkanego bledu
}



int main()
{ 
	yyparse(); 

	return 0;
}