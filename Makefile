.PHONY: clean debug

main: vec3.o vistascii.o main.cpp
	g++ ${DEBUG} vec3.o vistascii.o main.cpp -lncurses -o main

debug: DEBUG = -g

debug: main

vistascii.o: vec3.o src/vistascii.cpp
	g++ ${DEBUG} -c src/vistascii.cpp

vec3.o: src/vec3.cpp
	g++ ${DEBUG} -c src/vec3.cpp

clean:
	rm main *.o