INCDIR = -I ./include/
LIBDIR = ./lib/
BUILDDIR = ./bin/
SOURCEDIR = ./src/

CC=g++
CPPFLAGS=-c -Wall -fPIC -std=c++11 $(INCDIR) -ggdb
LDFLAGS= -shared -L$(LIBDIR) -lsqlite3

SOURCES=$(wildcard $(SOURCEDIR)*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=$(BUILDDIR)sqllogplugin.so

all: $(EXECUTABLE) copyplugin
clean: cleanup Makefile.dep $(EXECUTABLE) copyplugin

# dependencies collected with cc -MM
-include Makefile.dep
Makefile.dep	: $(SOURCES)
	$(CC) -MM $(CPPFLAGS) $^ > $@

# Compile
%.o	: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

# Link
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

#clean
cleanup:
	rm -f $(EXECUTABLE) $(SOURCEDIR)*.o Makefile.dep

#copy

BOTDIR = ./../TinyBot/bin/

copyplugin:
	-@cp -u $(EXECUTABLE) $(BOTDIR)
