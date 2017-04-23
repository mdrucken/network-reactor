
SRC=$(wildcard src/*.cpp)
OBJ=$(patsubst %.cpp,%.o,$(SRC))

TEST_SRC=$(wildcard test/*.cpp) $(filter-out src/main.cpp,$(SRC))
TEST_OBJ=$(patsubst %.cpp,%.o,$(TEST_SRC))

PROGRAM=main
TEST=test/testrunner

.PHONY: clean unittest

$PROGRAM): $(OBJ)
	@echo "Linking $(PROGRAM)"
	g++ -o $(PROGRAM) $(OBJ)

test: $(TEST)
	./$(TEST)

$(TEST): $(TEST_OBJ) 
	g++ -o $(TEST) $(TEST_OBJ) -lgtest -lpthread

test/%.o: test/%.cpp
	g++ -c -g -std=c++14 -Isrc $< -o $@

src/%.o: src/%.cpp
	g++ -c -g -std=c++14 $< -o $@

clean:
	rm -f $(OBJ) $(PROGRAM) $(TEST) $(TEST_OBJ)

list:
	@echo $(TEST_SRC)
