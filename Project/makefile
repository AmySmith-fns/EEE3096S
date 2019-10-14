.RECIPEPREFIX +=
CC = g++
CFLAGS = -Wall -lm -lrt -lwiringPi -lpthread

PROG = bin/*
OBJS = obj/*

default:
    mkdir -p bin obj
    $(CC) $(CFLAGS) -c fk.cpp -o obj/Project
    $(CC) $(CFLAGS) obj/Project -o bin/Project

run:
    sudo ./bin/Project

clean:
    rm $(PROG) $(OBJS)
