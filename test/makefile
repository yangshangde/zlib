SRC = $(wildcard ./*.cpp)
INC = -I../
CC = g++
CFLAGS = -g
LIBS = ../libz.a
TARGET = mytest

all : $(TARGET)
.PYTHON: clean

$(TARGET) : $(SRC)
	$(CC) $(CFLAGS) $(INC) -o $@ $^ $(LIBS)

clean:
	rm -rf $(TARGET) *.dSYM
