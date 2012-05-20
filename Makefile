
# Just hard-code the compiler, since it won't change...
CC=arm-none-linux-gnueabi-gcc

all: v4l2reg 

clean:
	rm -f v4l2reg

v4l2reg: v4l2reg.c
	$(CC) -o $@ $<

