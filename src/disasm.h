#ifndef __DISASM_H__
#define __DISASM_H__

#define R 0
#define I 1
#define J 2

#define opc(op) (((op) & (0xFC000000)) >> (26))

#define rs(op) (((op) & (0x03E00000)) >> (21))

#define rt(op) (((op) & (0x001F0000)) >> (16))

#define rd(op) (((op) & (0x0000F800)) >> (11))

#define shmt(op) (((op) & (0x000007C0)) >> (6))

#define fnct(op) ((op) & (0x0000003F))

#define imm(op) ((op) & (0x0000FFFF))

#define addr(op) ((op) & (0x03FFFFFF))

const char regs[] = {"zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
					 "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
					 "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
					 "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"};

void disas(FILE *in, FILE *out){

	int opcode;
	int type;

	while(!feof(in)){
		fscanf(in, "%x", opcode);
	}



}

#endif