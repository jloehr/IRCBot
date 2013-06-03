INCDIR = ./include/
LIBDIR = ./lib/
BUILDDIR = ./bin/
SOURCEDIR = ./src/

CC=g++
CPPFLAGS=-c -Wall -std=c++11 -I $(INCDIR) -ggdb
LDFLAGS= -ldl -lev -L $(LIBDIR) -ltinyirc

SOURCES=$(wildcard $(SOURCEDIR)*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=$(BUILDDIR)tinybot

all: $(EXECUTABLE)
clean: cleanup Makefile.dep $(EXECUTABLE) 

# dependencies collected with cc -MM
-include Makefile.dep
Makefile.dep	: $(SOURCES)
	$(CC) -MM $(CPPFLAGS) $^ > $@

# Compile
%.o	: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

# Link
$(EXECUTABLE): getlibraries $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

#clean
cleanup:
	rm -f $(EXECUTABLE) $(SOURCEDIR)*.o Makefile.dep


#Get Libraries
getlibraries: tinyirc

TINYIRCDIR = ./../TinyIRC/
TINYIRCBINDIR = $(TINYIRCDIR)bin/
TINYIRCBIN = $(TINYIRCBINDIR)libtinyirc.a
TINYIRCHEADERDIR = $(TINYIRCBINDIR)include/

tinyirc:
	cd $(TINYIRCDIR); make
	-@mkdir $(LIBDIR)
	-@mkdir $(INCDIR)
	-@cp -u $(TINYIRCBIN) $(LIBDIR)
	-@cp -u $(TINYIRCHEADERDIR)* $(INCDIR)

