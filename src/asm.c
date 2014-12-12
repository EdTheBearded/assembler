#include "asm.h"

label_t *labels = NULL, *ls = NULL;
inst_t *insts = NULL, *is_tail = NULL, *ind;

int addr_cnt = 0;
int error = 0;


char *buffer = NULL;
extern int lMaxBuffer;
FILE *infile = NULL, *outfile = NULL;
char *inname = NULL;

extern
int main(int argc, char *argv[]) {
	
	int i;
	char *outname = NULL;

	debug = 0;

	if(argc == 1){
		printf("%s: fatal error: no input file. Try using '-h' for help.\n", argv[0]);
		exit(1);
	}

	for (i=1; i<argc; i++) {
		if (argv[i][0] == '-') {
			switch(argv[i][1]){
				case 'b':
					printf("debugging activated\n");
					debug = 1;
					break;
				case 'o':
					outname = argv[i+1];
					i++;
					break;
				case 'h':
					usage(argv[0]);
					exit(0);
				default:
					printf("%s: error: unrecognized command line option '%s'. Try using '-h' for help.\n", argv[0], argv[i]);
					exit(1);
			}
		}
		else
			inname = argv[i];
	}

	if (inname == NULL){
		printf("%s: fatal error: no input file\n", argv[0]);
		exit(2);
	}

	if(outname == NULL){
		outname = (char*)malloc(2*sizeof(char));
		outname = "a";
	}

  	/* opening input */
	infile = fopen(inname, "r");
	if (infile == NULL) {
		printf("%s: fatal error: cannot open input %s\n", argv[0], inname);
		exit(3);
	}

	buffer = malloc(lMaxBuffer);
	if (buffer == NULL) {
		printf("%s: fatal error: cannot allocate %d bytes of memory\n", argv[0], lMaxBuffer);
		fclose(infile);
		exit(4);
	}

	if(debug == 1) printf("\n============ PARSER STAGE ============\n");
  	/* parse it */
	if (getNextLine() == 0)
		yyparse();

	/*check if control is last instruction, and adds a nop*/
	if(is_tail->name != NULL && (is_tail->name[0] == 'j' || is_tail->name[0] == 'b')){
		is_tail->next = (inst_t*)malloc(sizeof(inst_t));
		memset(is_tail->next, 0x00, sizeof(inst_t));
		is_tail->next->line = is_tail->line;
		is_tail->next->addr = is_tail->addr + 4;
		is_tail = is_tail->next;
		is_tail->type = R;
	}

	if(debug == 1) printf("\n========= NAME SOLVING STAGE =========\n");
	resolve_names();

	if(debug == 1)
		for(ind = insts; ind != NULL; ind = ind->next)
			printf("%08x: %s\n", ind->addr, ind->name);

	if(error == 0){
		outfile = fopen(outname, "w");
		if(outfile == NULL){
			printf("%s: fatal error: cannot open output %s\n", argv[0], outname);
			exit(4);
		}
		binary();
	}


	//clean labels list
	while(labels != NULL){
		ls = labels;
		labels = labels->next;
		free(ls->name);
		free(ls);
	}
	while(insts != NULL){
		is_tail = insts;
		insts = insts->next;
		free(is_tail->name);
		free(is_tail->rs);
		free(is_tail->rt);
		free(is_tail->rd);
		free(is_tail->label);
		free(is_tail);
	}

	free(buffer);
	fclose(infile);
	fclose(outfile);

	return 0;
}

void usage(char *name){

	printf("\tASM - Simple MIPS assembler\n\
Usage:\n %s INPUTNAME [OPTIONS]\n\
Options:\n\
  -o [FILENAME]\t\tdefine output file name to FILENAME (default is 'a')\n\
  -b           \t\tset internal debug flag (prints useful asm debug info)\n\
  -h           \t\tprint this help message\n", name);

}

int htoi(char *s){

	int i, num = 0, h;

	for(i = 2; s[i] != '\0'; i++){
		if(s[i] >= 97)
			h = s[i] - 'a' + 10;
		else if(s[i] >= 65)
			h = s[i] - 'A' + 10;
		else
			h = s[i] - '0';
		num = 16*num + h;
	}

	return num;
}

int btoi(char *s){

	int i, num = 0;

	for(i = 2; s[i] != '\0'; i++){
		if(s[i] == '1')
			num += 1;
		num <<= 1;
	}

	return num;
}

int otoi(char *s){

	int i, num = 0;

	for(i = 1; s[i] != '\0'; i++){
		num = 8*num + (s[i] - '0');
	}

	return num;
}

int add_label(int line, char *name){

	label_t *i;

	if(labels == NULL){
		labels = (label_t*)malloc(sizeof(label_t));
		labels->addr = addr_cnt;
		labels->line = line;
		labels->name = strdup(name);
		labels->next = NULL;
	}
	else{
		for(i = labels; i->next != NULL; i = i->next){
			if(!strcmp(i->name, name)){
				error = 1;
				return 0;
			}
		}
		if(!strcmp(i->name, name)){
			error = 1;
			return 0;
		}
		else{
			i->next = (label_t*)malloc(sizeof(label_t));
			i = i->next;
			i->addr = addr_cnt;
			i->line = line;
			i->name = strdup(name);
			i->next = NULL;
		}
	}

	return 1;
}

void add_inst(inst_t *node){

	/*bubble flag*/
	static int bubble = -1;

	node->addr = addr_cnt;
	if(insts == NULL){
		insts = node;
		is_tail = insts;
	}
	else{
		/*if we need a nop, and node isnt one, we create one*/
		if(bubble >= 0 && node->name != NULL){
			/*insert into instruction list*/
			is_tail->next = (inst_t*)malloc(sizeof(inst_t));
			memset(is_tail->next, 0x00, sizeof(inst_t));
			is_tail = is_tail->next;
			is_tail->line = bubble;
			is_tail->type = R;
			/*give it a new address*/
			is_tail->addr = addr_cnt;
			/*corrects the node address, adding 4 to it*/
			node->addr += 4;
			/*and corrects the global address counter*/
			addr_cnt += 4;
		}
		is_tail->next = node;
		is_tail = is_tail->next;
	}
	/*avoid double nop insertion*/
	bubble = -1;
	/*if control instruction, next call must treat nop insertion*/
	if(node->name != NULL && (node->name[0] == 'j' || node->name[0] == 'b')){
		bubble = node->line;
	}

	addr_cnt += 4;
}

void resolve_names(){

	inst_t *i;
	label_t *j;

	for(i = insts; i != NULL; i = i->next){
		if(i->type == J){
			for(j = labels; j != NULL; j = j->next){
				if(!strcmp(i->label, j->name)) break;
			}
			if(j == NULL){
				error = 1;
				printf("%s:%d:: undefined label %s\n", inname, i->line, i->label);
			}
			else if(j->addr > 0x3FFFFFF){
				error = 1;
				printf("%s:%d:: target too far away for jump instruction\n", inname, i->line);
			}else{
				i->imm = j->addr >> 2;
				if(debug == 1) printf("%s %s resolved into %s 0x%08x\n", i->name, i->label, i->name, i->imm);
			}
		}
		else if((i->type == I) && (i->label != NULL)){
			for(j = labels; j != NULL; j = j->next){
				if(!strcmp(i->label, j->name)) break;
			}
			if(j == NULL){
				error = 1;
				printf("%s:%d:: undefined label %s\n", inname, i->line, i->label);
			}
			else{
				i->imm = j->addr - i->addr - 4;
				if(i->imm > 32767 || i->imm < -32768){
					error = 1;
					printf("%s:%d:: target too far away for branch instruction\n", inname, i->line);
				}
				else{
					i->imm &= 0x0000FFFF;
				}
				i->imm >>= 2;
				if(debug == 1 && i->name[0] == 'b') printf("%s %s, %s, %s resolved into %s %s, %s, 0x%08x\n", i->name, i->rs, i->rt, i->label,
																											  i->name, i->rs, i->rt, i->imm);
				if(debug == 1 && i->name[0] == 'l') printf("%s %s, %s(%s) resolved into %s %s, 0x%08x(%s)\n", i->name, i->rt, i->label, i->rs,
																											  i->name, i->rs, i->imm, i->rs);
				free(i->label);
				i->label = NULL;
			}
		}
	}

}

uint32_t opt(char *s){

	if(s == NULL)
		return 0;
	else{
		switch(s[0]){
			case 'a':
				if(!strcmp(s, "addi")){
					return 0x8 << 26;
				}else if(!strcmp(s, "addiu")){
					return 0x9 << 26;
				}else if(!strcmp(s, "andi")){
					return 0xC << 26;
				}
				else{
					return 0x00000000;
				}
				break;
			case 'b':
				if(!strcmp(s, "beq")){
					return 0x4 << 26;
				}else if(!strcmp(s, "bne")){
					return 0x5 << 26;
				}
				break;
			case 'd':
				return 0x00000000;
				break;
			case 'j':
				if(!strcmp(s, "j")){
					return 0x2 << 26;
				}else if(!strcmp(s, "jal")){
					return 0x3 << 26;
				}
				else{
					return 0x00000000;
				}
				break;
			case 'l':
				if(!strcmp(s, "lbu")){
					return 0x24 << 26;
				}else if(!strcmp(s, "lhu")){
					return 0x25 << 26;
				}else if(!strcmp(s, "ll")){
					return 0x30 << 26;
				}else if(!strcmp(s, "lui")){
					return 0xf << 26;
				}else if(!strcmp(s, "lw")){
					return 0x23 << 26;
				}
				break;
			case 'm':
				if(!strcmp(s, "mfc0")){
					return 0x10 << 26;
				}
				else{
					return 0x00000000;
				}
				break;
			case 'n':
				return 0x00000000;
				break;
			case 'o':
				if(!strcmp(s, "or")){
					return 0x00000000;
				}else if(!strcmp(s, "ori")){
					return 0xD << 26;
				}
				break;
			case 's':
				if(!strcmp(s, "slti")){
					return 0xA << 26;
				}else if(!strcmp(s, "sltiu")){
					return 0xB << 26;
				}else if(!strcmp(s, "sb")){
					return 0x28 << 26;
				}else if(!strcmp(s, "sc")){
					return 0x38 << 26;
				}else if(!strcmp(s, "sh")){
					return 0x29 << 26;
				}else if(!strcmp(s, "sw")){
					return 0x2B << 26;
				}else{
					return 0x00000000;
				}
		}
	}

	return 0;
}

int reg(char *s){

	int reg = 0;

	if(s == NULL || s[1] == 'z')
		return 0;
	else if(s[1] == 'r' && s[2] == 'a'){
		return 31;
	}
	else if(s[1] == 'r'){
		reg = s[2] - '0';
		if(s[3] == '\0')
			return reg;
		else
			return (10*reg + (s[3] - '0'));
	}
	else if(s[1] == 'a' && s[2] == 't'){
		return 1;
	}
	else if(s[1] == 'v'){
		return 2 + (s[2] - '0');
	}
	else if(s[1] == 'a'){
		return 4 + (s[2] - '0');
	}
	else if(s[1] == 't' && (s[2] == '8' || s[2] == '9')){
		return 24 + (s[2] - '0');
	}
	else if(s[1] == 't'){
		return 8 + (s[2] - '0');
	}
	else if(s[1] == 's' && s[2] == 'p'){
		return 29;
	}
	else if(s[1] == 's'){
		return 16 + (s[2] - '0');
	}
	else if(s[1] == 'k'){
		return 26 + (s[2] - '0');
	}
	else if(s[1] == 'g'){
		return 28;
	}
	else{
		return 30;
	}

	return 0;
}

int funct(char *s){

	if(s == NULL)
		return 0;
	else{
		switch(s[0]){
			case 'a':
				if(!strcmp(s, "add")){
					return 0x20;
				}else if(!strcmp(s, "addu")){
					return 0x21;
				}else{
					return 0x24;
				}
				break;
			case 'd':
				if(!strcmp(s, "div")){
					return 0x1A;
				}
				else{
					return 0x1B;
				}
				break;
			case 'j':
				return 0x08;
				break;
			case 'm':
				if(!strcmp(s, "mfhi")){
					return 0x10;
				}
				else if(!strcmp(s, "mflo")){
					return 0x12;
				}
				else if(!strcmp(s, "mfc0")){
					return 0x00;
				}
				else if(!strcmp(s, "mult")){
					return 0x18;
				}
				else if(!strcmp(s, "multu")){
					return 0x19;
				}
				break;
			case 'n':
				return 0x27;
				break;
			case 'o':
				return 0x25;
				break;
			case 's':
				if(!strcmp(s, "slt")){
					return 0x2A;
				}else if(!strcmp(s, "sltu")){
					return 0x2B;
				}else if(!strcmp(s, "sll")){
					return 0x00;
				}else if(!strcmp(s, "srl")){
					return 0x02;
				}else if(!strcmp(s, "sub")){
					return 0x22;
				}else if(!strcmp(s, "subu")){
					return 0x23;
				}else{
					return 0x03;
				}
		}
	}

	return 0;
}

void binary(){

	inst_t *i;
	uint32_t bin;

	for(i = insts; i != NULL; i = i->next){
		bin = 0;
		switch(i->type){
			case R:			
				bin |= opt(i->name);
				bin = regs(bin, reg(i->rs));
				bin = regt(bin, reg(i->rt));
				bin = regd(bin, reg(i->rd));
				bin = shamt(bin, i->shamt);
				bin |= funct(i->name);
				//printf("%d %d %d %d %d %d\n", opt(i->name), reg(i->rs), reg(i->rt), reg(i->rd), i->shamt, funct(i->name));
				break;
			case I:
				bin |= opt(i->name);
				bin = regs(bin, reg(i->rs));
				bin = regt(bin, reg(i->rt));
				bin |= i->imm;
				break;
			case J:
				bin |= opt(i->name);
				bin |= i->imm;
				break;
		}
		fprintf(outfile, "%08x\n", bin);
	}
}