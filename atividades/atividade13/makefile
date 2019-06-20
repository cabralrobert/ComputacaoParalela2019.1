all: main

GCC = gcc
BIN = bin
OBJ = obj
INC = -Iinc/ -w

main: folder arquivo
	+@echo "Compile: $@"
	@$(GCC) -fopenmp src/$@.c $(INC) -c -o $(OBJ)/$@.o -lm

	@echo "Linking..."
	+@$(GCC) -fopenmp -o $(BIN)/app $(OBJ)/$@.o $(OBJ)/arquivo.o -lm
	@echo "Binary are ready in $(BIN)/app"

arquivo:
	+@echo "Compile: $@"
	@$(GCC) -fopenmp src/$@.c $(INC) -c -o $(OBJ)/$@.o -lm

folder:
	@mkdir -p $(OBJ) $(BIN)

clean:
	@rm -rf $(OBJ)/* $(BIN)/* $(OBJ) $(BIN)

###############################################################################
