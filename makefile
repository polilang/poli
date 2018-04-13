SOURCE := src
LIB := $(SOURCE)/lib
MAIN := $(SOURCE)/main.pi


OUT := build
EXE := $(OUT)/main

DEBUG := valgrind -q --leak-check=full --track-origins=yes

SOURCES := $(wildcard $(SOURCE)/*/*.pi) $(MAIN)
TARGETS := $(addprefix $(OUT)/,$(subst $(SOURCE)/,,$(SOURCES:.pi=.c))) $(wildcard $(LIB)/*/*.c)
DIRS := $(OUT) $(dir $(TARGETS))


CC := cc
CFLAGS := -Werror=discarded-qualifiers -g -I$(OUT) -I$(LIB)

SC := ./parse
SFLAGS := -s '"' '"' -c /\* \*/ -c // \\n
TEMPLATE := poli.t


all : $(EXE)


$(OUT)/%.c : $(SOURCE)/%.pi $(TEMPLATE)
	$(SC) $(SFLAGS) $(TEMPLATE) $< | tee $@

$(EXE) : $(DIRS) $(TARGETS)
	$(CC) $(CFLAGS) -o $(EXE) $(TARGETS)

$(DIRS) :
	mkdir $@

.PHONY : clean test

clean : $(OUT)
	rm -r $(OUT)

test : $(EXE)
	$(DEBUG) ./$(OUT)/main
