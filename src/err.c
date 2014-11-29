#include "err.h"

#define true 1
#define false 0

#define dumpChar(c) (isprint((int)c) ? c : '@')

int debug=0;

static int eof = 0;
static int nRow = 0;
static int nBuffer = 0;
static int lBuffer = 0;
static int nTokenStart = 0;
static int nTokenLength = 0;
static int nTokenNextStart = 0;
int lMaxBuffer = 1000;
extern char *buffer;
extern FILE *infile;


 static
 char *dumpString(char *s) {
 	static char buf[101];
 	int i;
 	int n = strlen(s);

 	if (  n > 100  )
 		n = 100;

 	for (i=0; i<n; i++)
 		buf[i] = dumpChar(s[i]);
 	buf[i] = 0;
 	return buf;
 }


 extern
 void PrintError(const char *errorstring, ...) {
 	
 	static char errmsg[10000];
 	va_list args;

 	int start=nTokenStart;
 	int end=start + nTokenLength - 1;
 	int i;

	/* print it using variable arguments */
    va_start(args, errorstring);
    vsprintf(errmsg, errorstring, args);
    va_end(args);

	printf("file:%d:%d: %s\n", nRow, start, errmsg);
  
  	fprintf(stdout, "%.*s", lBuffer, buffer);
  	
	if (  eof  ) {
    	for (i=0; i<lBuffer; i++)
    		fprintf(stdout, " ");
    		fprintf(stdout, "^-EOF\n");
    	}
    else {
    	for (i=1; i<start; i++)
    		fprintf(stdout, " ");
    	for (i=start; i<=end; i++)
    		fprintf(stdout, "^");
    }
    putchar('\n');

}


int getNextLine(void) {

 	char *p;

 	nBuffer = 0;
 	nTokenStart = -1;
 	nTokenNextStart = 1;
 	eof = false;


  	/* read a line */
 	p = fgets(buffer, lMaxBuffer, infile);
 	if (  p == NULL  ) {
 		if (ferror(infile))
 			return -1;
 		eof = true;
 		return 1;
 	}

 	nRow += 1;
 	lBuffer = strlen(buffer);

 	return 0;
 }


extern
int GetNextChar(char *b, int maxBuffer) {

 	int frc;

 	if (  eof  )
 		return 0;


  	/* read next line if at the end of the current */
 	while (  nBuffer >= lBuffer  ) {
 		frc = getNextLine();
 		if (  frc != 0  ) return 0;
 	}

  	/* ok, return character */
 	b[0] = buffer[nBuffer];
 	nBuffer += 1;

 	if (  debug  )
 		printf("GetNextChar() => '%c'0x%02x at %d\n",
 			dumpChar(b[0]), b[0], nBuffer);
 	
 	return b[0]==0?0:1;
 }


extern
void BeginToken(char *t) {
 
  	/* remember last read token */
 	nTokenStart = nTokenNextStart;
 	nTokenLength = strlen(t);
  	nTokenNextStart = nBuffer;

	/* location for bison */
	yylloc.first_line = nRow;
	yylloc.first_column = nTokenStart;
	yylloc.last_line = nRow;
	yylloc.last_column = nTokenStart + nTokenLength - 1;

	if ( debug ) {
		printf("Token '%s' at %d:%d next at %d\n", dumpString(t),
			yylloc.first_column,
			yylloc.last_column, nTokenNextStart);
	}
}

