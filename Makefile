CC := gcc
CFLAGS := -Wall -Wextra -g -Iinclude

SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

COMMON_SRCS := \
	$(SRC_DIR)/analyse_lexicale.c \
	$(SRC_DIR)/lecture_caracteres.c \
	$(SRC_DIR)/analyse_syntaxique.c \
	$(SRC_DIR)/ast_construction.c \
	$(SRC_DIR)/ast_parcours.c \
	$(SRC_DIR)/table_symbole.c

COMMON_OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(COMMON_SRCS))

BINARIES := \
	$(BIN_DIR)/interpreteur \
	$(BIN_DIR)/calculette \
	$(BIN_DIR)/test_lexeme \
	$(BIN_DIR)/essai_ast

.PHONY: all clean test

all: $(BINARIES)

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

$(BIN_DIR)/interpreteur: $(COMMON_OBJS) $(OBJ_DIR)/main.o | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/calculette: $(COMMON_OBJS) $(OBJ_DIR)/calculette.o | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/test_lexeme: $(OBJ_DIR)/analyse_lexicale.o $(OBJ_DIR)/lecture_caracteres.o $(OBJ_DIR)/test_lexeme.o | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/essai_ast: $(OBJ_DIR)/ast_construction.o $(OBJ_DIR)/ast_parcours.o $(OBJ_DIR)/table_symbole.o $(OBJ_DIR)/essai_ast.o | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

test: all
	$(BIN_DIR)/calculette tests/valides/calculette/expr_plus_mult.txt
	$(BIN_DIR)/interpreteur examples/ex1_arithmetique.txt
	$(BIN_DIR)/interpreteur examples/ex2_condition.txt
	$(BIN_DIR)/interpreteur examples/ex3_boucle.txt

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
