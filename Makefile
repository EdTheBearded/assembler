CC 			= gcc
BISON 		= bison
FLEX 		= flex

LIBS 		= -lfl

TARGET		= asm
BISON_OBJ 	= bis
FLEX_OBJ 	= mips


all:
	$(BISON) -d $(BISON_OBJ).y
	$(FLEX) $(FLEX_OBJ).l
	$(CC) $(BISON_OBJ).tab.c lex.yy.c $(LIBS) -o $(TARGET)

#debug flex
flex:
	$(BISON) -d $(BISON_OBJ).y
	$(FLEX) $(FLEX_OBJ).l
	$(CC) lex.yy.c $(LIBS)  -o l

clean:
	rm -rf $(BISON_OBJ).tab.* lex.yy.c $(TARGET)
