CC 		= gcc
BISON 		= bison
FLEX 		= flex

LIBS 		= -lfl
FLAGS		= -Wall -O3

SRCFOLDER 	= src
TARGET		= asm
BISON_SRC 	= bis
FLEX_SRC 	= mips
SOURCES		= asm.c err.c $(BISON_SRC).tab.c lex.yy.c
OBJECTS		= $(addprefix $(SRCFOLDER)/, $(SOURCES))

all:
	$(BISON) -d $(addprefix $(SRCFOLDER)/, $(BISON_SRC).y) -o $(SRCFOLDER)/$(BISON_SRC).tab.c
	$(FLEX) $(addprefix $(SRCFOLDER)/, $(FLEX_SRC).l)
	mv lex.yy.c $(SRCFOLDER)/
	$(CC) $(FLAGS) $(OBJECTS) $(LIBS) -o $(TARGET)

#debug flex
flex:
	$(BISON) -d $(addprefix $(SRCFOLDER)/, $(BISON_SRC).y)
	$(FLEX) $(FLEX_SRC).l
	$(CC) $(FLAGS) lex.yy.c $(LIBS)  -o l

clean:
	rm -rf $(addprefix $(SRCFOLDER)/, $(BISON_SRC).tab.*) $(addprefix $(SRCFOLDER)/, lex.yy.c) $(TARGET)
