CC 		= gcc
BISON 		= bison
FLEX 		= flex

LIBS 		= -lfl
FLAGS		= -Wall -O3

TARGET		= asm
BISON_OBJ 	= bis
FLEX_OBJ 	= mips
FILES		= $(BISON_OBJ).tab.c lex.yy.c asm.c err.c

all:
	$(BISON) -d $(BISON_OBJ).y
	$(FLEX) $(FLEX_OBJ).l
	$(CC) $(FLAGS) $(FILES) $(LIBS) -o $(TARGET)

#debug flex
flex:
	$(BISON) -d $(BISON_OBJ).y
	$(FLEX) $(FLEX_OBJ).l
	$(CC) $(FLAGS) lex.yy.c $(LIBS)  -o l

clean:
	rm -rf $(BISON_OBJ).tab.* lex.yy.c $(TARGET)
