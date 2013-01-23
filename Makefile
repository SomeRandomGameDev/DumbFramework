# Awful makefile to clean/port
CFLAGS=-Wall -Isrc -g
LIBS=-lglfw -lGLEW -lSOIL -lexpat -lm

application.o: src/application.cpp src/application.hpp
	g++ ${CFLAGS} -c src/application.cpp -o application.o

windowhint.o: src/windowhint.cpp src/windowhint.hpp
	g++ ${CFLAGS} -c src/windowhint.cpp -o windowhint.o

sprite.o: src/sprite.hpp src/sprite.cpp
	g++ ${CFLAGS} -c src/sprite.cpp -o sprite.o

sprengine.o: src/sprengine.hpp src/sprengine.cpp src/sprite.hpp
	g++ ${CFLAGS} -c src/sprengine.cpp -o sprengine.o

shader.o: src/shader.hpp src/shader.cpp
	g++ ${CFLAGS} -c src/shader.cpp -o shader.o

gears: application.o windowhint.o src/scene.hpp src/demo/gears.hpp src/demo/gears.cpp
	g++ ${CFLAGS} application.o windowhint.o src/demo/gears.cpp -o gears ${LIBS}

vbo: application.o windowhint.o src/scene.hpp src/demo/vbo.cpp
	g++ ${CFLAGS} application.o windowhint.o src/demo/vbo.cpp -o vbo ${LIBS}

flatengine: application.o windowhint.o sprite.o sprengine.o src/demo/flatengine.cpp
	g++ ${CFLAGS} application.o windowhint.o sprite.o sprengine.o src/demo/flatengine.cpp -o flatengine ${LIBS}

clean:
	rm -f *.o gears vbo flatengine

