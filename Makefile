# .cpp files
#SOURCES := $(shell find . -name '*.cpp')
SOURCES := $(wildcard *.cpp) $(wildcard **/*.cpp)
HEADERS := $(wildcard *.h) $(wildcard **/*.h)

# Object files
OBJ=$(addprefix objects/,$(subst .cpp,.o,$(notdir $(SOURCES))))

# Compiler and linker
CC=g++

# Flags for compiler
CC_FLAGS = -g -c -Wall -I ./util -I ./lib -std=c++20

ifeq ($(OS),Windows_NT)
  LDFLAGS = -lfreeglut -lGLEW32 -lopengl32 -lopenal
  CC_FLAGS += -D WIN32
else
  LDFLAGS = -lglut -lGLEW -lGL -lopenal
endif


all: objFolder projeto
	@ echo 'Done!'

projeto: $(OBJ) 
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o $@ $(LDFLAGS)
	@ echo 'Finished building binary: $@'
	@ echo ' '

objects/%.o: ./%.cpp
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo ' '
objects/%.o: ./lib/%.cpp
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo ' '

objFolder:
	@ mkdir -p objects

clean:
	@ rm -rf ./objects/*.o projeto
	@ rmdir objects

.PHONY: all clean