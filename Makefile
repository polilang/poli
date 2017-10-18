SOURCE := src
LIBRARIES := $(SOURCE)/lib
COMPILER := $(SOURCE)/compiler
MAIN := $(SOURCE)/main.c

CC := cc
CFLAGS := -Wall -I$(SOURCE) -g

OUT := build
EXE := $(OUT)/main

SOURCES := $(wildcard $(LIBRARIES)/*/*.c) $(wildcard $(COMPILER)/*/*.c) $(MAIN)
TARGETS := $(addprefix $(OUT)/,$(subst $(LIBRARIES)/,,$(subst $(COMPILER)/,,$(SOURCES:.c=.o))))
DIRS := $(OUT) $(dir $(TARGETS))

all : $(EXE)

$(OUT)/%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OUT)/%.o : $(LIBRARIES)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OUT)/%.o : $(COMPILER)/%.c
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
