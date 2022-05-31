CC      = g++
SRC     = $(sort $(wildcard *.cpp))
OBJ	    = $(patsubst %.cpp,%.o,$(SRC))
CFLAG   = -O2 -Wall -std=c++11
LD      = ld

all:$(OBJ)
	${CC} ${OBJ} -o main.exe

%.o:%.cpp
	${CC} ${CFLAG} -c $<

clean:
	del .\*.o .\*.exe