#ifndef __ASM_H__
#define __ASM_H__

#define R 0
#define I 1
#define J 2

#define regs(bin, reg) ((bin) | ((reg) << (21)))

#define regt(bin, reg) ((bin) | ((reg) << (16)))

#define regd(bin, reg) ((bin) | ((reg) << (11)))

#define shamt(bin, s) ((bin) | ((s) << (6)))

#include "err.h"
#include <stdint.h>

typedef struct label_s{
	int addr;
	int line;
	char *name;
	struct label_s *next;
} label_t;

typedef struct inst_s{
	int addr;
	int line;
	int type;
	char *name;
	char *rs, *rt, *rd;
	int shamt;
	int imm;
	char *label;
	struct inst_s *next;	
} inst_t;

void usage();

int htoi(char *s);

int btoi(char *s);

int otoi(char *s);

int add_label(int line, char *name);

void add_inst(inst_t *node);

void resolve_names();

void bubble_check();

uint32_t opt(char *s);

int reg(char *s);

int funct(char *s);

void binary();

#endif
