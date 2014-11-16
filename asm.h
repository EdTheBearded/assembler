#ifndef __ASM_H__
#define __ASM_H__

#include "err.h"

label_t *label = NULL, *l_tail = NULL;
inst_t *inst_h = NULL, *i_tail = NULL;

int addr = 0;

typedef struct label_s{
	int addr;
	int line;
	char *name;
	struct label_s *next;
} label_t;

typedef struct inst_s{
	int line;
	char *name;
	char *rs, *rt, *rd;
	int shamt;
	int imm;	
} inst_t;

void add_label(label_t *tail, int addr, int line, char *name);

void add_inst(inst_t *tail, int line, char *name, char *rs, char *rt, char *rd, int shamt, int imm);

#endif
