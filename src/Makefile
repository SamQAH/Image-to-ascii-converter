
CC = clang -fcolor-diagnostics -fansi-escape-codes
#CC = g++ -fpermissive
OP = -g -O0 -MMD
FLAGS = -DOPENFILE
OT = -o analyse.exe
INC = -I"C:/Program Files/ImageMagick-7.1.2-Q16-HDRI/include"
LK = -L"C:\Program Files\ImageMagick-7.1.2-Q16-HDRI\lib" -lCORE_RL_Magick++_

SRCS = $(wildcard *.cpp)
#SRCS = main.cpp methods.cpp
OBJS = $(SRCS:.cpp=.o)

analyse.exe: $(OBJS)
	$(CC) $(OP) $(INC) $(FLAGS) $(OT) $(OBJS) $(LK)

%.o: %.cpp
	$(CC) $(OP) $(INC) -c $< $(FLAGS) -o $@ 

clean:
	rm -f *.o *.d main.exe

test:
	make FLAGS="-DNONE"
	analyse.exe image.png < test.in > debug.out

.PHONY: clean test
