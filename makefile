# Check for OS Type
UNAME_S := $(shell uname -s)

# Linux
ifeq ($(UNAME_S), Linux)
	CC=g++
	LIBS=-lSDL2 -lGLEW -lGL -lassimp -lMagick++ -lOpenGL -lEGL
# Mac
else
	CC=clang++
	LIBS=-lSDL2 -framework OpenGL -framework Cocoa -lGLEW -stdlib=libc++ -lassimp -lMagick++
endif

# Paths, use this to set Include directories, or library directories; uncomment in all: if used
#PATHI=-I/usr/local/include/
#PATHL=-L/usr/local/lib/

#Compiler Flags
CXXFLAGS=-g -Wall -std=c++0x

# .o Compilation
O_FILES=main.o Camera.o engine.o graphics.o object.o shader.o window.o loader.o event.o planet.o framebuffer.o generator.o

# Point to includes of local directories
INDLUDES=-I../include


all: $(O_FILES)
	$(CC) $(CXXFLAGS) -o Tutorial $(O_FILES) $(LIBS) $(PATHI) $(PATHL)

main.o: ../src/main.cpp
	$(CC) $(CXXFLAGS) -c ../src/main.cpp -o main.o $(INDLUDES)

Camera.o: ../src/camera.cpp
	$(CC) $(CXXFLAGS) -c ../src/Camera.cpp -o Camera.o $(INDLUDES)

engine.o: ../src/engine.cpp
	$(CC) $(CXXFLAGS) -c ../src/engine.cpp -o engine.o $(INDLUDES)

graphics.o: ../src/graphics.cpp
	$(CC) $(CXXFLAGS) -c ../src/graphics.cpp -o graphics.o $(INDLUDES)

object.o: ../src/object.cpp
	$(CC) $(CXXFLAGS) -c ../src/object.cpp -o object.o $(INDLUDES)

shader.o: ../src/shader.cpp
	$(CC) $(CXXFLAGS) -c ../src/shader.cpp -o shader.o $(INDLUDES)

window.o: ../src/window.cpp
	$(CC) $(CXXFLAGS) -c ../src/window.cpp -o window.o $(INDLUDES)
	
loader.o: ../src/loader.cpp
	$(CC) $(CXXFLAGS) -c ../src/loader.cpp -o loader.o $(INDLUDES)

event.o: ../src/event.cpp
	$(CC) $(CXXFLAGS) -c ../src/event.cpp -o event.o $(INDLUDES)

planet.o: ../src/planet.cpp
	$(CC) $(CXXFLAGS) -c ../src/planet.cpp -o planet.o $(INDLUDES)
	
framebuffer.o: ../src/framebuffer.cpp
	$(CC) $(CXXFLAGS) -c ../src/framebuffer.cpp -o framebuffer.o $(INDLUDES)
	
generator.o: ../src/generator.cpp
	$(CC) $(CXXFLAGS) -c ../src/generator.cpp -o generator.o $(INDLUDES)


clean:
	-@if rm *.o Tutorial 2>/dev/null || true; then echo "Main Removed"; else echo "No Main"; fi
