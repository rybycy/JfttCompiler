%option noyywrap
%option yylineno
%{
#include "bison.tab.h"
int yylex();

%}
%x comment

%%

"(*"			BEGIN(comment);
<comment>[^*\n]*        ;
<comment>"*"+[^*)\n]*   ;
<comment>\n             ;
<comment>"*"+")"        BEGIN(INITIAL);

[ \t\r\n]+	{} 	
CONST			{ return(CONST); }
"="				{ return(CASSIGN); }
VAR				{ return(VAR); }
BEGIN			{ return(START); }
END				{ return(END); }
":="			{ return(ASSIGN); }
WHILE			{ return(WHILE); }
DO				{ return(DO); }
IF				{ return(IF); }
THEN			{ return(THEN); }
ELSE			{ return(ELSE); }
WRITE			{ return(WRITE); }
READ			{ return(READ); }
"=="			{ return(EQ); } 
"<"				{ return(LT); }
"<="			{ return(LET); }
">"				{ return(GT); }
">="			{ return(GET); }
"!="			{ return(NEQ); }
"+"				{ return(PLUS); }
"-"				{ return(MINUS); }
"*"				{ return(MULT); }
"/"				{ return(DIV); }
"%"				{ return(MOD); }
[0-9]+	{    
	     yylval.numu = (char *) strdup(yytext);
				return(NUM);
	}
[_a-z]+	{ 
	     yylval.stru = (char *) strdup(yytext);
				return(IDENTIFIER);  
	}
";"				{ return(SEMICOLON); }
.	;
%%
