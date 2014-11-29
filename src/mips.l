%{
#include <stdio.h>

#include "err.h"
#include "asm.h"

#define YY_NO_INPUT

#define YY_INPUT(buf,result,max_size)  {\
    result = GetNextChar(buf, max_size); \
    if (  result <= 0  ) \
      result = YY_NULL; \
    }

%}

%option nounput

%x IN_COMMENT
%x IN_ERROR

%%

<INITIAL>{
	[ \t\r]+							BeginToken(yytext); 
	[\n]								BeginToken(yytext); yylineno++;
	"#"[^\n]*							BeginToken(yytext); 
	"("								BeginToken(yytext); return LPAR;
	")"								BeginToken(yytext); return RPAR;
	"/*"								BeginToken(yytext); BEGIN(IN_COMMENT);
	","								BeginToken(yytext); return COMA;
	("add"|"addu"|"and"|"nor"|"or"|"slt"|"sltu"|"sub"|"subu")	BeginToken(yytext); yylval.s = strdup(yytext); return RTYPE1;
	("sll"|"srl"|"sra")						BeginToken(yytext); yylval.s = strdup(yytext); return RTYPE2;
	("div"|"divu"|"mult"|"multu")					BeginToken(yytext); yylval.s = strdup(yytext); return RTYPE3;
	("mfhi"|"mflo"|"mfco"|"jr")					BeginToken(yytext); yylval.s = strdup(yytext); return RTYPE4;

	("addi"|"addiu"|"andi"|"ori"|"slti"|"sltiu")			BeginToken(yytext); yylval.s = strdup(yytext); return ITYPE1;
	("beq"|"bne")							BeginToken(yytext); yylval.s = strdup(yytext); return ITYPE2;
	("lbu"|"lhu"|"ll"|"lw"|"sb"|"sc"|"sh"|"sw")			BeginToken(yytext); yylval.s = strdup(yytext); return ITYPE3;
	"lui"								BeginToken(yytext); yylval.s = strdup(yytext); return ITYPE4;
	
	("j"|"jal")							BeginToken(yytext); yylval.s = strdup(yytext); return JTYPE;
	"$r"[1-2]?[0-9]|"$r30"|"$r31"					BeginToken(yytext); yylval.s = strdup(yytext); return REG;
	"$zero" 							BeginToken(yytext); yylval.s = strdup(yytext); return REG;
	"$at" 								BeginToken(yytext); yylval.s = strdup(yytext); return REG;
	("$v0"|"$v1")							BeginToken(yytext); yylval.s = strdup(yytext); return REG;
	"$a"[0-3]							BeginToken(yytext); yylval.s = strdup(yytext); return REG;
	"$t"[0-9]							BeginToken(yytext); yylval.s = strdup(yytext); return REG;
	"$s"[0-7]							BeginToken(yytext); yylval.s = strdup(yytext); return REG;
	("$k0"|"$k1")							BeginToken(yytext); yylval.s = strdup(yytext); return REG;
	"$gp" 								BeginToken(yytext); yylval.s = strdup(yytext); return REG;
	"$sp" 								BeginToken(yytext); yylval.s = strdup(yytext); return REG;
	"$fp" 								BeginToken(yytext); yylval.s = strdup(yytext); return REG;
	"$ra" 								BeginToken(yytext); yylval.s = strdup(yytext); return REG;
	":"								BeginToken(yytext); return COLON;
	([a-zA-z]|"_")([a-zA-Z0-9]|"_")*				BeginToken(yytext); yylval.s = strdup(yytext); return ID;
	[0-9][0-9]*|"-"[0-9][0-9]*					BeginToken(yytext); yylval.ival = atoi(yytext); return INT;
	"0b"[0-1][0-1]*							BeginToken(yytext); yylval.ival = btoi(yytext); return INT;

	"0x"[0-9a-fA-F][0-9a-fA-F]*					BeginToken(yytext); yylval.ival = htoi(yytext); return INT;
	.								BeginToken(yytext); return yytext[0];
}

<IN_COMMENT>{
	"*/"								BeginToken(yytext); BEGIN(INITIAL);
	[^*\n]+																				
	"*"												
	\n								BeginToken(yytext); yylineno++;
}

<IN_ERROR>{
	[ \t",""("")"]							BeginToken(yytext); fprintf(stderr, "on line %d\n", yylineno); BEGIN(INITIAL);
	[^\n]								BeginToken(yytext); fprintf(stderr, "%s", yytext);
	[\n]								BeginToken(yytext); fprintf(stderr, "on line %d\n", yylineno); yylineno++; BEGIN(INITIAL);
}

%%