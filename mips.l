%{
#include <stdio.h>

#ifndef DEBUG
	#include "bis.tab.h"
#endif
%}

%x IN_COMMENT
%x IN_ERROR

%%

<INITIAL>{
	[ \t\r]+														;
	[\n]															yylineno++;
	"#"[^\n]*														;
	"("																return LPAR;
	")"																return RPAR;
	"/*"															BEGIN(IN_COMMENT);
	","																return COMA;
	("add"|"addu"|"and"|"nor"|"or"|"slt"|"sltu"|"sub"|"subu")		yylval.s = strdup(yytext); return RTYPE1;
	("sll"|"srl"|"sra")												yylval.s = strdup(yytext); return RTYPE2;
	("div"|"divu"|"mult"|"multu")									yylval.s = strdup(yytext); return RTYPE3;
	("mfhi"|"mflo"|"mfco"|"jr")										yylval.s = strdup(yytext); return RTYPE4;

	("addi"|"addiu"|"andi"|"ori"|"slti"|"sltiu")					yylval.s = strdup(yytext); return ITYPE1;
	("beq"|"bne")													yylval.s = strdup(yytext); return ITYPE2;
	("lbu"|"lhu"|"ll"|"lw"|"sb"|"sc"|"sh"|"sw")						yylval.s = strdup(yytext); return ITYPE3;
	"lui"															yylval.s = strdup(yytext); return ITYPE4;
	
	("j"|"jal")														yylval.s = strdup(yytext); return JTYPE;
	"$r"[1-2]?[0-9]|"$r30"|"$r31"									yylval.s = strdup(yytext); return REG;
	"$zero" 														yylval.s = strdup(yytext); return REG;
	"$at" 															yylval.s = strdup(yytext); return REG;
	("$v0"|"$v1")													yylval.s = strdup(yytext); return REG;
	"$a"[0-3]														yylval.s = strdup(yytext); return REG;
	"$t"[0-9]														yylval.s = strdup(yytext); return REG;
	"$s"[0-7]														yylval.s = strdup(yytext); return REG;
	("$k0"|"$k1")													yylval.s = strdup(yytext); return REG;
	"$gp" 															yylval.s = strdup(yytext); return REG;
	"$sp" 															yylval.s = strdup(yytext); return REG;
	"$fp" 															yylval.s = strdup(yytext); return REG;
	"$ra" 															yylval.s = strdup(yytext); return REG;
	":"																return COLON;
	([a-zA-z]|"_")([a-zA-Z0-9]|"_")*								yylval.s = strdup(yytext); return ID;
	[0-9][0-9]*|"-"[0-9][0-9]*										yylval.ival = atoi(yytext); return INT;
	"0b"[0-1][0-1]*													yylval.ival = atoi(yytext); return INT;
	
	"0x"[0-9a-fA-F][0-9a-fA-F]*										yylval.ival = atoi(yytext); return INT;
	.																fprintf(stderr, "ERROR: invalid token %s ", yytext);
}

<IN_COMMENT>{
	"*/"															BEGIN(INITIAL);
	[^*\n]+																				
	"*"												
	\n																yylineno++;
}

<IN_ERROR>{
	[ \t",""("")"]													fprintf(stderr, "on line %d\n", yylineno); BEGIN(INITIAL);
	[^\n]															fprintf(stderr, "%s", yytext);
	[\n]															fprintf(stderr, "on line %d\n", yylineno); yylineno++; BEGIN(INITIAL);
}

%%
/*
main(int argc, char* argv[]){

	FILE *myfile = NULL;

	if(argc == 2){
		fprintf(stderr, "Usage:\n$%s file\n", argv[0]);

		myfile = fopen(argv[1], "r");
		if(myfile == NULL){
			fprintf(stderr, "Can not open specified file %s\n", argv[1]);
			exit(1);
		}

		yyin = myfile;
	}
	yylex();

}*/