# Project name
NAME=programaTrab1


# Directories
INCDIR=include
LIBDIR=lib
BLDDIR=build
SRCDIR=src
OBJDIR=$(SRCDIR)/obj

# Can use any debbuger (like gdb) or overlay (like valgrind) (though gdb is an overlay too)
DEBUGGER=
DBGFLAGS=-v --leak-check=full --show-leak-kinds=all --read-var-info=yes --track-origins=yes


# This will search for 'anything' terminating in .c inside SRCDIR and set an object rule for each of them (make *.o)
SRC=$(wildcard $(SRCDIR)/*.c)
OBJ=$(foreach file, $(SRC), $(file:$(SRCDIR)/%.c=$(OBJDIR)/%.o))

# Find all .h dependencies
DEPS=$(wildcard $(INCDIR)/*.h)

# Finally something easy to read!
CC=gcc
CFLAGS=-std=c99 -Wall -I./$(INCDIR)
# -O3 -Wall -Wextra -I./$(INCDIR)

# Library links (example: -lm for math lib)
LIBS=-lm

# if you run make [target] debug=____ where ____ is anything (even an empty string will do)
ifdef debug
	CFLAGS+= -g
	DEBUGGER=valgrind $(DBGFLAGS) 
endif

all: clean main

# Compile directives
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	@echo Building $*
	@$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	@echo Linking object files
	@$(CC) -o $(BLDDIR)/$(NAME) $^ $(CFLAGS) $(LIBS)


# Run directives
run:
	$(DEBUGGER) ./$(BLDDIR)/$(NAME)

go: main run


# Utility directives
clean:
	-rm -f $(OBJDIR)/*.o
	clear
	clear

list:
	clear
	ls -lhR

tar: clean
	@echo Compressing files...
	@tar -zcvf $(NAME).tar.gz *
	@echo Done.

zip: clean
	@echo Compressing files...
	@zip -r $(NAME).zip *
	@echo Done.

# Check if project has a name before creating it
create:
ifeq ($(strip $(NAME)),)
	$(error No project name provided (open this make and set NAME))
endif
	mkdir $(NAME) 
	mkdir $(NAME)/$(SRCDIR)
	mkdir $(NAME)/$(INCDIR)
	mkdir $(NAME)/$(LIBDIR)
	mkdir $(NAME)/$(BLDDIR)
	mkdir $(NAME)/$(OBJDIR)
	cp ./Makefile ./$(NAME)


teste:
	@clear
	@make run < casoteste/1.in > saidas/meu1.out
	@diff casoteste/1.out saidas/meu1.out
	@make run < casoteste/2.in > saidas/meu2.out
	@diff casoteste/2.out saidas/meu2.out
	@make run < casoteste/3.in > saidas/meu3.out
	@diff casoteste/3.out saidas/meu3.out
	@make run < casoteste/4.in > saidas/meu4.out
	@diff casoteste/4.out saidas/meu4.out
	@make run < casoteste/5.in > saidas/meu5.out
	@diff casoteste/5.out saidas/meu5.out
	@make run < casoteste/6.in > saidas/meu6.out
	@diff casoteste/6.out saidas/meu6.out
	@make run < casoteste/7.in > saidas/meu7.out
	@diff casoteste/7.out saidas/meu7.out
	@make run < casoteste/8.in > saidas/meu8.out
	@diff casoteste/8.out saidas/meu8.out
	@make run < casoteste/9.in > saidas/meu9.out
	@diff casoteste/9.out saidas/meu9.out
	@make run < casoteste/10.in > saidas/meu10.out
	@diff casoteste/10.out saidas/meu10.out
	@make run < casoteste/11.in > saidas/meu11.out
	@diff casoteste/11.out saidas/meu11.out
	@make run < casoteste/12.in > saidas/meu12.out
	@diff casoteste/12.out saidas/meu12.out
	@make run < casoteste/13.in > saidas/meu13.out
	@diff casoteste/13.out saidas/meu13.out
	@make run < casoteste/14.in > saidas/meu14.out
	@diff casoteste/14.out saidas/meu14.out
	@make run < casoteste/15.in > saidas/meu15.out
	@diff casoteste/15.out saidas/meu15.out
