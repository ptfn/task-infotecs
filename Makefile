# info project
PROG1EXE = prog1
PROG2EXE = prog2
LIBEXE = lib.so

# compile
CC = gcc
CFLAGS = -std=c++11 -Wall
#-Werror
LIBFLAGS = -fPIC -shared
LIBS = -lstdc++

# directory
PROG1SRCDIR = prog1/src/
PROG2SRCDIR = prog2/src/
LIBSRCDIR = lib/src/

PROG1OBJDIR = prog1/build/
PROG2OBJDIR = prog2/build/
LIBOBJDIR = ~/lib/build/

# source files
PROG1SRC = prog1.cpp
PROG2SRC = prog2.cpp
LIBSRC = lib.cpp

PROG1OBJ = ${PROG1SRC:.cpp=.o}
PROG2OBJ = ${PROG2SRC:.cpp=.o}
LIBOBJ = ${LIBSRC:.cpp=.o}

PROG1OBJD = ${patsubst %, $(PROG1OBJDIR)%, $(PROG1OBJ)}
PROG2OBJD = ${patsubst %, $(PROG2OBJDIR)%, $(PROG2OBJ)}
LIBOBJD = ${patsubst %, $(LIBOBJDIR)%, $(LIBOBJ)}

all: dirs build

$(LIBOBJDIR)%.o: $(LIBSRCDIR)%.cpp
	$(CC) -c -o $@ $< $(LIBFLAGS)
	@echo "compile $@ success"

$(PROG1OBJDIR)%.o: $(PROG1SRCDIR)%.cpp
	$(CC) -c -o $@ $< $(CFLAGS)
	@echo "compile $@ success"

$(PROG2OBJDIR)%.o: $(PROG2SRCDIR)%.cpp
	$(CC) -c -o $@ $< $(CFLAGS)
	@echo "compile $@ success"

dirs:
	@mkdir -p $(PROG1OBJDIR)
	@echo "create directory $(PROG1OBJDIR)"

	@mkdir -p $(PROG2OBJDIR)
	@echo "create directory $(PROG2OBJDIR)"

	@mkdir -p $(LIBOBJDIR)
	@echo "create directory $(LIBOBJDIR)"

build: $(PROG1OBJD) $(PROG2OBJD) $(LIBOBJD)
	$(CC) -o $(LIBOBJDIR)$(LIBEXE) $(LIBOBJD) $(LIBFLAGS)
	$(CC) -o $(PROG1OBJDIR)$(PROG1EXE) $(PROG1OBJD) -I . -L. $(LIBOBJDIR)$(LIBEXE) $(LIBS)
	$(CC) -o $(PROG2OBJDIR)$(PROG2EXE) $(PROG2OBJD) -I . -L. $(LIBOBJDIR)$(LIBEXE) $(LIBS)
	@echo "build success"

clean:
	@rm -rf $(PROG1OBJDIR)
	@rm -rf $(PROG2OBJDIR)
	@rm -rf $(LIBOBJDIR)
	@echo "clean success"

