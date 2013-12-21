#
# Makefile
#

CXX = g++
CXX_FLAGS = -Wall -g -std=c++11 

all: build test


test: build/main.o build/Cut.o build/CutList.o
	${CXX} ${CXX_FLAGS} $^ -o $@

build/%.o: src/%.cpp src/%.hpp build
	${CXX} ${CXX_FLAGS} -c $< -o $@

build:
	mkdir build

clean:
	rm -rf build/*

