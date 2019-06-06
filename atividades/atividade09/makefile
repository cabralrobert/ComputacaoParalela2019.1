all:
	+@echo "Digite make serial ou make paralelo"

serial: multiplicacaoSerial

paralelo: multiplicacaoParalelo

OMP = -fopenmp
GCC = gcc
BIN = bin
OBJ = obj
INC = -Iinc/ -w

multiplicacaoSerial: folder arquivo
	+@echo "Compile: main.c"
	@$(GCC) src/main.c $(INC) -c -o $(OBJ)/main.o -lm

	@echo "Linking..."
	+@$(GCC) -o $(BIN)/appSerial $(OBJ)/main.o $(OBJ)/arquivo.o -lm
	@echo "Binary serial are ready in $(BIN)/app"


multiplicacaoParalelo: folder arquivo
	+@echo "Compile: main.c"
	@$(GCC) $(OMP)  src/main.c $(INC) -c -o $(OBJ)/main.o -lm

	@echo "Linking..."
	+@$(GCC) $(OMP) -o $(BIN)/appParallel $(OBJ)/main.o $(OBJ)/arquivo.o -lm
	@echo "Binary parallel are ready in $(BIN)/app"

arquivo:
	+@echo "Compile: $@"
	@$(GCC) src/$@.c $(INC) -c -o $(OBJ)/$@.o -lm

folder:
	@mkdir -p $(OBJ) $(BIN)

clean:
	@rm -rf $(OBJ)/* $(BIN)/* $(OBJ) $(BIN)

###############################################################################
