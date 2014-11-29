%{
#include <stdio.h>
#include <stdlib.h>
#include "asm.h"

#define YYERROR_VERBOSE 1

#define lower(i) ((i) & (0x0000FFFF))

extern int yylineno;

inst_t *node = NULL;

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
			node = (inst_t*)malloc(sizeof(inst_t));
			memset(node, 0x00, sizeof(inst_t));
			node->line = yylineno;
			node->type = R;
			node->name = $1;
			node->rd = $2; node->rs = $4; node->rt = $6;
			add_inst(node);
			node = NULL;
		}
	| RTYPE1 error REG error COMA error REG error COMA error REG
	;
rinst
	: RTYPE2 REG COMA REG COMA INT
		{
			node = (inst_t*)malloc(sizeof(inst_t));
			memset(node, 0x00, sizeof(inst_t));
			node->line = yylineno;
			node->type = R;
			node->name = $1;
			node->rd = $2; node->rt = $4;
			if($6 > 31)
				PrintError("immediate exceeds maximum value of 31\n");
			else
				node->shamt = lower($6);
			add_inst(node);
			node = NULL;
		}
	| RTYPE2 error REG error COMA error REG error COMA error INT
	;
rinst
	: RTYPE3 REG COMA REG
	 	{
	 		node = (inst_t*)malloc(sizeof(inst_t));
			memset(node, 0x00, sizeof(inst_t));
			node->line = yylineno;
			node->type = R;
			node->name = $1;
			node->rs = $2; node->rt = $4;
			add_inst(node);
			node = NULL;
	 	}
	| RTYPE3 error REG error COMA error REG
	 ;
rinst
	: RTYPE4 REG
		{
			node = (inst_t*)malloc(sizeof(inst_t));
			memset(node, 0x00, sizeof(inst_t));
			node->line = yylineno;
			node->type = R;
			node->name = $1;
			if($1[0] == 'j')
				node->rs = $2;
			else
				node->rd = $2;
			add_inst(node);
			node = NULL;
		}
	| RTYPE4 error REG
	;

iinst
	: ITYPE1 REG COMA REG COMA INT
		{
			node = (inst_t*)malloc(sizeof(inst_t));
			memset(node, 0x00, sizeof(inst_t));
			node->line = yylineno;
			node->type = I;
			node->name = $1;
			node->rt = $2; node->rs = $4;
			if((($6 & 0x80000000) && (-$6 > 65535)) || ($6 > 65535))
				PrintError("immediate exceeds maximum value of 65535\n");
			else
				node->imm = lower($6);
			add_inst(node);
			node = NULL;
		}
	| ITYPE1 error REG error COMA error REG error COMA error INT
	;
iinst
	: ITYPE2 REG COMA REG COMA ID
		{
			node = (inst_t*)malloc(sizeof(inst_t));
			memset(node, 0x00, sizeof(inst_t));
			node->line = yylineno;
			node->type = I;
			node->name = $1;
			node->rs = $2; node->rt = $4;
			node->label = strdup($6);
			add_inst(node);
			node = NULL;
		}
	| ITYPE2 REG COMA REG COMA INT
		{
			node = (inst_t*)malloc(sizeof(inst_t));
			memset(node, 0x00, sizeof(inst_t));
			node->line = yylineno;
			node->type = I;
			node->name = $1;
			node->rs = $2; node->rt = $4;
			if((($6 & 0x80000000) && (-$6 > 65535)) || ($6 > 65535))
				PrintError("immediate exceeds maximum value of 65535\n");
			else
				node->imm = lower($6);
			add_inst(node);
			node = NULL;
		}
	| ITYPE2 error REG error COMA error REG error COMA error ID
	| ITYPE2 error REG error COMA error REG error COMA error INT
	;
iinst
	: ITYPE3 REG COMA INT LPAR REG RPAR
		{
			node = (inst_t*)malloc(sizeof(inst_t));
			memset(node, 0x00, sizeof(inst_t));
			node->line = yylineno;
			node->type = I;
			node->name = $1;
			node->rt = $2; node->rs = $6;
			if((($4 & 0x80000000) && (-$4 > 65535)) || ($4 > 65535))
				PrintError("immediate exceeds maximum value of 65535\n");
			else
				node->imm = lower($4);
			add_inst(node);
			node = NULL;
		}
	| ITYPE3 error REG error COMA error INT error LPAR error REG error RPAR
	;
iinst
	: ITYPE4 REG COMA INT
		{
			node = (inst_t*)malloc(sizeof(inst_t));
			memset(node, 0x00, sizeof(inst_t));
			node->line = yylineno;
			node->type = I;
			node->name = $1;
			node->rt = $2;
			if((($4 & 0x80000000) && (-$4 > 65535)) || ($4 > 65535))
				PrintError("immediate exceeds maximum value of 65535\n");
			else
				node->imm = lower($4);
			add_inst(node);
			node = NULL;
		}
	| ITYPE4 error REG error COMA error INT
	;

jinst
	: JTYPE ID
		{
			node = (inst_t*)malloc(sizeof(inst_t));
			memset(node, 0x00, sizeof(inst_t));
			node->line = yylineno;
			node->type = J;
			node->name = $1;
			node->label = strdup($2);
			add_inst(node);
			node = NULL;
		}
	| JTYPE INT
		{
			node = (inst_t*)malloc(sizeof(inst_t));
			memset(node, 0x00, sizeof(inst_t));
			node->line = yylineno;
			node->type = J;
			node->name = $1;
			node->name = $1;
			if($2 > 0x3FFFFFF)
				PrintError("immediate exceeds maximum value of 31\n");
			else
				node->imm = $2;
			add_inst(node);
			node = NULL;
		}	
	| JTYPE error ID
	| JTYPE error INT
	;

label
	: ID COLON
		{
			if(add_label(yylineno, $1) == 0)
				PrintError("redefinition of label '%s'\n", $1);
		}
	| error ID error COLON
	;

%%

extern
void yyerror(const char *s){

	PrintError(s);

}
