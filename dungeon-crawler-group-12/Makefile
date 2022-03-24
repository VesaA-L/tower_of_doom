all: compile link

compile:
	g++ -I src -c src/main.cpp
link:
	g++ -o main.exe main.o -L libs/lib -l sfml-graphics -l sfml-window -l sfml-system