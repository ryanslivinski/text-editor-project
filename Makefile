CPPFILES = ECEditorTest.cpp ECTextViewImp.cpp ECCommand.cpp ECTextDocument.cpp

HFILES = $(CPPFILES:.cpp=.h)

OFILES= $(CPPFILES:.cpp=.o)

CC = g++
LIBPATH =

CFLAGS = -O3 -std=c++11 -Wall -I.

all: editor

%.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

editor:$(OFILES)
	$(CC) -o editor $(OFILES)

clean:
	rm *.o
	rm editor
	rm *.txt
