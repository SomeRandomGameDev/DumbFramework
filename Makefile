# Awful makefile to clean/port

application.o: src/application.cpp src/application.hpp
	g++ -Isrc -c src/application.cpp -o application.o

windowhint.o: src/windowhint.cpp src/windowhint.hpp
	g++ -Isrc -c src/windowhint.cpp -o windowhint.o

sprite.o: src/sprite.hpp src/sprite.cpp
	g++ -Isrc -c src/sprite.cpp -o sprite.o

sprengine.o: src/sprengine.hpp src/sprengine.cpp src/sprite.hpp
	g++ -Isrc -c src/sprengine.cpp -o sprengine.o

gears: application.o windowhint.o src/scene.hpp src/demo/gears.hpp src/demo/gears.cpp
	g++ -Isrc application.o windowhint.o src/demo/gears.cpp -o gears -lglfw -lGLEW -lm

vbo: application.o windowhint.o src/scene.hpp src/demo/vbo.cpp
	g++ -Isrc application.o windowhint.o src/demo/vbo.cpp -o vbo -lglfw -lGLEW -lSOIL -lexpat -lm

flatengine: application.o windowhint.o sprite.o sprengine.o src/demo/flatengine.cpp
	g++ -Isrc application.o windowhint.o sprite.o sprengine.o src/demo/flatengine.cpp -o flatengine -lglfw -lGLEW -lSOIL -lexpat -lm

clean:
	rm -f *.o gears vbo flatengine

