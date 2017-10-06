SRC := src
LIB := $(SRC)/lib
MAIN := $(SRC)/parser.c

CC := cc
CFLAGS := -Wall -I$(LIB) -g

OUT := build
EXE := $(OUT)/main

SOURCES := $(wildcard $(LIB)/*/*.c) $(MAIN)
TARGETS := $(addprefix $(OUT)/,$(subst $(LIB)/,,$(SOURCES:.c=.o)))
DIRS := $(OUT) $(dir $(TARGETS))

all : $(EXE)

$(OUT)/%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OUT)/%.o : $(LIB)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXE) : $(DIRS) $(TARGETS)
	$(CC) $(CFLAGS) -o $(EXE) $(TARGETS)

$(DIRS) :
	mkdir $@

.PHONY : clean test

clean : $(OUT)
	rm -r $(OUT)

test : $(EXE)
	./$(OUT)/main
