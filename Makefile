#
# Makefile
#

CXX = g++
CXX_FLAGS = -Wall -g -std=c++11
CXX_LIBS = -L/opt/local/lib  -lgsl -lgslcblas -lm

all: build test


test: build/main.o build/Cut.o build/CutList.o
	${CXX} ${CXX_FLAGS} ${CXX_LIBS} $^ -o $@

build/%.o: src/%.cpp src/%.hpp
	${CXX} ${CXX_FLAGS} -c $< -o $@

build:
	mkdir build

clean:
	rm -rf build/*

