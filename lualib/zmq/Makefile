OUTLIB=../../build/zmq.so

all : zmq

zmq : luazmq.c
	gcc -g -Wall -fPIC --shared -o $(OUTLIB) luazmq.c -I/usr/include/lua5.2/ -lzmq

clean :
	rm $(OUTLIB)

.PHONY: all clean