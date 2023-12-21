SRCDIR = src
BUILDDIR = build

########################################################
# main build target filename and source:

TARGET = $(BUILDDIR)/main.out
SRC = $(SRCDIR)/main.cpp $(BUILDDIR)/decoder.o $(BUILDDIR)/printer.o $(BUILDDIR)/tetrominos.o $(BUILDDIR)/input.o $(BUILDDIR)/game.o

########################################################
# main test filename:

#TESTSRC = $(TESTDIR)/test.cpp $(BUILDDIR)/decoder.o


# compiler flags:
CFLAGS = -std=c++17 -I $(SRCDIR) -O3 -funroll-loops

# linker flags
#    add -lm to use mathematical funtions

########################################################
# do not change

#include ../tools/Makefile

########################################################
# project specific targets
#   adapt / expand as needed
$(TARGET): $(BUILDDIR) $(SRC)
	$(CXX) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

$(BUILDDIR)/decoder.o: $(BUILDDIR) $(SRCDIR)/decoder.cpp
	$(CXX) $(CFLAGS) -o $(BUILDDIR)/decoder.o -c $(SRCDIR)/decoder.cpp $(LDFLAGS)

$(BUILDDIR)/printer.o: $(BUILDDIR) $(SRCDIR)/printer.cpp
	$(CXX) $(CFLAGS) -o $(BUILDDIR)/printer.o -c $(SRCDIR)/printer.cpp $(LDFLAGS)

$(BUILDDIR)/tetrominos.o: $(BUILDDIR) $(SRCDIR)/tetrominos.cpp
	$(CXX) $(CFLAGS) -o $(BUILDDIR)/tetrominos.o -c $(SRCDIR)/tetrominos.cpp $(LDFLAGS)
	
$(BUILDDIR)/input.o: $(BUILDDIR) $(SRCDIR)/input.cpp
	$(CXX) $(CFLAGS) -o $(BUILDDIR)/input.o -c $(SRCDIR)/input.cpp $(LDFLAGS)

$(BUILDDIR)/game.o: $(BUILDDIR) $(SRCDIR)/game.cpp
	$(CXX) $(CFLAGS) -o $(BUILDDIR)/game.o -c $(SRCDIR)/game.cpp $(LDFLAGS)
