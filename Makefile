#
# Makefile
#

CXX = g++


all: build test


test: build/main.o build/Cut.o
	${CXX} ${CXX_FLAGS} $^ -o $@

build/%.o: src/%.cpp src/%.hpp
	${CXX} ${CXX_FLAGS} -c $< -o $@

build:
	mkdir build

clean:
	rm -rf build/*

