%{
#include <stdio.h>
#include <stdlib.h>
#include "asm.h"

#define YYERROR_VERBOSE 1

%}

%locations

%union {
	int ival;
	char *s;	
}

%token <s> 	  RTYPE1
%token <s> 	  RTYPE2
%token <s> 	  RTYPE3
%token <s> 	  RTYPE4
%token <s> 	  ITYPE1
%token <s> 	  ITYPE2
%token <s> 	  ITYPE3
%token <s> 	  ITYPE4
%token <s> 	  JTYPE
%token <s>    ID
%token <s>    REG
%token <ival> INT
%token LPAR
%token RPAR
%token COMA
%token COLON
%token END

%start sentences

%%
		


sentences
	: inst sentences
	| label sentences
	| inst 						
	| label
	{
		yyerrok;
	}						

inst
	: rinst
	| iinst
	| jinst
	;

rinst
	: RTYPE1 REG COMA REG COMA REG
		{
			printf("rtype1 %s %s, %s, %s\n", $1, $2, $4, $6);
		}
	;
rinst
	: RTYPE2 REG COMA REG COMA INT
		{
			printf("rtype2 %s %s, %s, %d\n", $1, $2, $4, $6);
		}
	;
rinst
	: RTYPE3 REG COMA REG
	 	{
	 		printf("rtype3 %s %s, %s\n", $1, $2, $4);
	 	}
	 ;
rinst
	: RTYPE4 REG
		{
			printf("rtype4 %s %s\n", $1, $2);
		}
	;

iinst
	: ITYPE1 REG COMA REG COMA INT
		{
			printf("itype1 %s %s, %s, %d\n", $1, $2, $4, $6);
		}
	;
iinst
	: ITYPE2 REG COMA REG COMA ID
		{
			printf("itype2 %s %s, %s, %s\n", $1, $2, $4, $6);
		}
	;
iinst
	: ITYPE3 REG COMA INT LPAR REG RPAR
		{
			printf("itype3 %s %s, %d(%s)\n", $1, $2, $4, $6);
		}
	;
iinst
	: ITYPE4 REG COMA INT
		{
			printf("itype3 %s %s, %d\n", $1, $2, $4);
		}
	;

jinst
	: JTYPE ID
		{
			printf("jtype %s %s\n", $1, $2);
		}
	;

label
	: ID COLON
		{
			printf("label found: %s\n", $1);
		}
	;

%%

extern
void yyerror(const char *s){

	PrintError(s);

}
