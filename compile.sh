#!/bin/sh

g++ -Wall -g simple_youtube.cpp -lcurl -o simple_youtube `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0`