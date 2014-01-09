#
# Makefile
#

CXX = g++

CXX_FLAGS = -Wall -g -std=c++11 -fPIC
CXX_LIBS = -L/opt/local/lib  -lgsl -lgslcblas -lm

AR_PROG ?= ar 
AR_FLAGS ?= crf

ROOT_FLAGS = `root-config --cflags`
ROOT_LIBS = `root-config --libs`

CXX_FLAGS := ${CXX_FLAGS} ${ROOT_FLAGS}
CXX_LIBS := ${CXX_LIBS} ${ROOT_LIBS}

INSTALL_PREFIX ?= /usr/local

INSTALL_INCLUDE_DIR = ${INSTALL_PREFIX}/include/cutclass/
INSTALL_LIB_DIR = ${INSTALL_PREFIX}/lib/

all: build test

test: build/main.o
	${CXX} ${CXX_FLAGS} ${CXX_LIBS} $^ -o $@

build/%.o: src/%.cpp src/%.hpp
	${CXX} ${CXX_FLAGS} -c $< -o $@

build/libCutClass.a: build/Cut.o build/CutList.o
	${AR_PROG} ${AR_FLAGS} $@ $^

install: src/*.tcc src/Cut.hpp src/CutList.hpp 
	mkdir -p ${INSTALL_INCLUDE_DIR} && \
	cp src/*.tcc ${INSTALL_INCLUDE_DIR} && \
	cp src/Cut.hpp ${INSTALL_INCLUDE_DIR} && \
	cp src/CutList.hpp ${INSTALL_INCLUDE_DIR}

build:
	mkdir build

clean:
	rm -rf build/*

