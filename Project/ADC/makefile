.RECIPEPREFIX +=
CC = g++
CFLAGS = -Wall -lm -lrt -lwiringPi -lpthread

PROG = bin/*
OBJS = obj/*

default:
    mkdir -p bin obj
    $(CC) $(CFLAGS) -c adc.cpp -o obj/ADC
    $(CC) $(CFLAGS) obj/ADC -o bin/ADC

run:
    sudo ./bin/ADC

clean:
    rm $(PROG) $(OBJS)
