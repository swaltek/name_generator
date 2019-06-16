CC = g++
SRC = src/
TEST = test/
BIN = bin/
BUILD = build/

test : $(BIN)categorical_test
$(BIN)categorical_test : $(TEST)/categorical_test.cpp $(SRC)name_generator.cpp $(SRC)name_generator.h
	$(CC) $(TEST)categorical_test.cpp $(SRC)name_generator.cpp -o $(BIN)categorical_test
clean :
	rm $(BUILD)*
