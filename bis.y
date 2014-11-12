%{
#include <stdio.h>
#include <stdlib.h>

int yylex();
int yyparse();
FILE *yyin;
extern int yylineno;

void yyerror(const char *s);
%}

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

%%
		
start: sentences										{printf("start\n");};

sentences: inst sentences | inst 						{printf("recursive sentences\n");};
sentences: label sentences | label						{printf("label rule\n");};

inst: 	rinst | iinst | jinst							;

rinst: 	RTYPE1 REG COMA REG COMA REG					{printf("rtype1 %s %s, %s, %s\n", $1, $2, $4, $6);};
rinst:  RTYPE2 REG COMA REG COMA INT 					{printf("rtype2 %s %s, %s, %d\n", $1, $2, $4, $6);};
rinst:  RTYPE3 REG COMA REG 							{printf("rtype3 %s %s, %s\n", $1, $2, $4);};
rinst:  RTYPE4 REG 										{printf("rtype4 %s %s\n", $1, $2);};

iinst:  ITYPE1 REG COMA REG COMA INT 					{printf("itype1 %s %s, %s, %d\n", $1, $2, $4, $6);};
iinst:  ITYPE2 REG COMA REG COMA ID 					{printf("itype2 %s %s, %s, %s\n", $1, $2, $4, $6);};
iinst:  ITYPE3 REG COMA INT LPAR REG RPAR 				{printf("itype3 %s %s, %d(%s)\n", $1, $2, $4, $6);};
iinst:  ITYPE4 REG COMA INT 							{printf("itype3 %s %s, %d\n", $1, $2, $4);};

jinst: JTYPE ID 										{printf("jtype %s %s\n", $1, $2);};

label: ID COLON											{printf("label found: %s\n", $1);}

%%

main(int argc, char *argv[]){

	if(argc == 2){
		FILE *myfile = fopen(argv[1], "r");
		if(myfile == NULL){
			fprintf(stderr, "Can not open %s file.\n", argv[1]);
			exit(1);
		}
		yyin = myfile;
	}
	
	do{
		yyparse();
	}while(!feof(yyin));

}

void yyerror(const char *s){

	fprintf(stderr, "Parse error on line %d! Message: %s\n", yylineno, s);
	exit(1);

}
