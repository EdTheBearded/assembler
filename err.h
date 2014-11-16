#ifndef __ERR_H__
#define __ERR_H__

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include <stdarg.h>
#include "bis.tab.h"

extern int debug;

extern int yylex(void);
extern int yyparse(void);
extern void yyerror(const char*);

extern int GetNextChar(char *b, int maxBuffer);
int getNextLine(void);
extern void BeginToken(char*);
extern void PrintError(const char *s, ...);

#endif
