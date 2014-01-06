#
# Makefile
#

CXX = g++

CXX_FLAGS = -Wall -g
CXX_LIBS = -L/opt/local/lib  -lgsl -lgslcblas -lm

ROOT_FLAGS = `root-config --cflags`
ROOT_LIBS = `root-config --libs`

CXX_FLAGS := ${CXX_FLAGS} ${ROOT_FLAGS}
CXX_LIBS := ${CXX_LIBS} ${ROOT_LIBS}

all: build test

test: build/main.o build/Cut.o build/CutList.o
	${CXX} ${CXX_FLAGS} ${CXX_LIBS} $^ -o $@

build/%.o: src/%.cpp src/%.hpp
	${CXX} ${CXX_FLAGS} -c $< -o $@

build:
	mkdir build

clean:
	rm -rf build/*

