all: multiplicacaoMatrizVetor

GCC = gcc
BIN = bin
OBJ = obj
INC = -Iinc/ -w

multiplicacaoMatrizVetor: folder arquivo
	+@echo "Compile: $@"
	@$(GCC) src/$@.c $(INC) -c -o $(OBJ)/$@.o -lm

	@echo "Linking..."
	+@$(GCC) -o $(BIN)/app $(OBJ)/$@.o $(OBJ)/arquivo.o -lm
	@echo "Binary are ready in $(BIN)/app"

arquivo:
	+@echo "Compile: $@"
	@$(GCC) src/$@.c $(INC) -c -o $(OBJ)/$@.o -lm

folder:
	@mkdir -p $(OBJ) $(BIN)

clean:
	@rm -rf $(OBJ)/* $(BIN)/* $(OBJ) $(BIN)

###############################################################################
