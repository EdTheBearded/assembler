#include "asm.h"

char *buffer = NULL;
extern int lMaxBuffer;
FILE *infile = NULL, *outfile = NULL;

extern
int main(int argc, char *argv[]) {
	
	int i;
	char *inname = NULL;
	char *outname = NULL;

	debug = 0;

	if(argc == 1){
		printf("%s: fatal error: no input file\n", argv[0]);
		exit(1);
	}

	for (i=1; i<argc; i++) {
		if (argv[i][0] == '-') {
			switch(argv[i][1]){
				case 'd':
					printf("debugging activated\n");
					debug = 1;
					break;
				case 'o':
					outname = argv[i+1];
					i++;
					break;
				default:
					printf("%s: error: unrecognized command line option '%s'\n", argv[0], argv[i]);
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

  	/* parse it */
	if (getNextLine() == 0)
		yyparse();

	free(buffer);
	fclose(infile);

	return 0;
}

