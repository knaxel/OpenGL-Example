# Example 8
EXE=test

# Main target
all: $(EXE)

LIB_DIR := -L./lib/ 
INC_DIR := -I./include/glm -I./include/ 

#  MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall  -DUSEGLEW
LIBS= -lsoil2 -lfreeglut  -lglew32 -lglu32 -lopengl32 -lglfw3
CLEAN=rm *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
CFLG=-O3 -Wall -Wno-deprecated-declarations 
LIBS=-framework -framework GLUT OpenGL -lglfw3
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall 
LIBS=-lsoil2 -lglut32  -lGLU -lGL -lm -lglfw 
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif

# Compile rules
%.o: %.cpp
	g++ $(INC_DIR) -lstdc++ -c $(CFLG)    $<

#  Link
test:main.o
	g++  $(LIB_DIR) -O3 -o $@ $^   $(LIBS)

#  Clean
clean:
	$(CLEAN)
