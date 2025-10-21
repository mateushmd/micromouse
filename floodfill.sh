#!/usr/bin/env bash
set -e

if [ ! -d "bin" ]; then
    mkdir bin
fi

if [ ! -d ".obj" ]; then
    mkdir .obj
fi

g++ -c src/API.cpp -o .obj/API.o
g++ -c src/floodfill.cpp -o .obj/floodfill.o
g++ -c src/main-ff.cpp -o .obj/main.o
g++ .obj/*.o -o bin/micromouse
