CXX = g++
DBG = gdb
CFLAGS = -g -Wall -std=c++11 -Iinc -I../inc
LFLAGS =
SRC = $(wildcard src/*.cpp)
OBJ = $(addprefix obj/,$(subst .cpp,.o,$(notdir $(SRC))))
BIN = machine

default: build
.PHONY: clean run debug build

obj/%.o: src/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

$(BIN) : $(OBJ)
	$(CXX) $(LFLAGS) $^ -o $@

build: $(BIN)

debug: $(BIN)
	$(DBG) ./$(BIN)

clean:
	rm obj/* $(BIN)
