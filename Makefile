main: vec3.o vistascii.o main.cpp
	g++ vec3.o vistascii.o main.cpp -lncurses -o main

vistascii.o: vec3.o src/vistascii.cpp
	g++ -c src/vistascii.cpp

vec3.o: src/vec3.cpp
	g++ -c src/vec3.cpp

clean:
	rm main *.o