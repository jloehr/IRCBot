INCDIR = -I ./include/
LIBDIR = ./lib/
BUILDDIR = ./bin/
HEADERDIR = $(BUILDDIR)include/ 
SOURCEDIR = ./src/

CC=g++
CL=ar rcs
CPPFLAGS=-c -Wall -std=c++11 $(INCDIR) -ggdb
LDFLAGS= -lev -L$(LIBDIR)

SOURCES=$(wildcard $(SOURCEDIR)*.cpp)
HEADERS=$(wildcard $(SOURCEDIR)*.h)
OBJECTS=$(SOURCES:.cpp=.o)
LIBRARY=$(BUILDDIR)libtinyirc.a

all: library
clean: cleanup Makefile.dep library
objects: $(OBJECTS)
library: $(LIBRARY) copyheaders

# dependencies collected with cc -MM
-include Makefile.dep
Makefile.dep	: $(SOURCES)
	$(CC) -MM $(CPPFLAGS) $^ > $@

# Compile
%.o	: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

# Link
$(LIBRARY): $(OBJECTS) 
	$(CL) $@ $(OBJECTS)
	#$(CC) $(OBJECTS) $(LDFLAGS) -o $@

#Copy Headers
copyheaders:
	-@mkdir $(HEADERDIR)
	-@cp -u $(SOURCEDIR)*.h $(HEADERDIR)


#clean
cleanup:
	-rm -f $(LIBRARY) $(SOURCEDIR)*.o Makefile.dep
