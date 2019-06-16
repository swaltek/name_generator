CC = g++
SRC = src/
TEST = test/
BIN = bin/
BUILD = build/

test : $(BIN)categorical_test $(BIN)model_test $(BIN)generate_test 
$(BIN)categorical_test : $(TEST)/categorical_test.cpp $(SRC)name_generator.cpp $(SRC)name_generator.h
	$(CC) $(TEST)categorical_test.cpp $(SRC)name_generator.cpp -o $(BIN)categorical_test
$(BIN)model_test: $(TEST)/model_test.cpp $(SRC)name_generator.cpp $(SRC)name_generator.h
	$(CC) $(TEST)model_test.cpp $(SRC)name_generator.cpp -o $(BIN)model_test
$(BIN)generate_test: $(TEST)/generate_test.cpp $(SRC)name_generator.cpp $(SRC)name_generator.h
	$(CC) $(TEST)generate_test.cpp $(SRC)name_generator.cpp -o $(BIN)generate_test

clean :
	rm $(BUILD)*
