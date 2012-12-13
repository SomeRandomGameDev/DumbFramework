# Awful makefile to clean/port

application.o: src/application.cpp src/application.hpp
	g++ -Isrc -c src/application.cpp -o application.o

windowhint.o: src/windowhint.cpp src/windowhint.hpp
	g++ -Isrc -c src/windowhint.cpp -o windowhint.o

gears: application.o windowhint.o src/scene.hpp src/demo/gears.hpp src/demo/gears.cpp
	g++ -Isrc application.o windowhint.o src/demo/gears.cpp -o gears -lglfw -lm

clean:
	rm -f application.o windowhint.o gears

