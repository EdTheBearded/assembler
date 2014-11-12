/*! @file ccalc.h
 * @brief Include for all modules
 *********************************************************************
 * a simple calculator with variables
 * 
 * sample-files for a artikel in developerworks.ibm.com
 * Author: Christian Hagen, chagen@de.ibm.com
 * 
 * @par ccalc.h
 * includes, prototypes & types
 * 
 *********************************************************************
 */
#ifndef CCALC_H_
#define CCALC_H_

#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include <stdarg.h>
#include <float.h>
#include "bis.tab.h"

/*
 * global variable
 */
extern int debug;

/*
 * lex & parse
 */
extern int yylex(void);
extern int yyparse(void);
extern void yyerror(const char*);

/*
 * ccalc.c
 */
extern void DumpRow(void); 
extern int GetNextChar(char *b, int maxBuffer);
extern void BeginToken(char*);
extern void PrintError(const char *s, ...);

#endif /*CCALC_H_*/
