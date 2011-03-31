ARCH := $(shell uname -m)
X64 = x86_64
PROGS = test_ketama proftest

LIBS = -L../ -lredisclient -lm -lhiredis `pkg-config --libs glib-2.0 gthread-2.0`  

CFLAGS = -Wall -g `pkg-config --cflags glib-2.0` 

all: $(PROGS)

STPROG = test_ketama.o 
PROF = proftest.o

proftest:$(PROF)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

test_ketama: $(STPROG) 
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	
proftest.o: proftest.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
test_ketama.o: test_ketama.c 
	$(CC) $(CFLAGS) -c -o $@ $^


clean: 
	rm -f *.o *~ $(PROGS) core.*
