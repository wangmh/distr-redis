ARCH := $(shell uname -m)
X64 = x86_64
<<<<<<< HEAD
PROGS = test_ketama proftest test_feed_api test_link_api test_list test_string test_hash ketama_value

LIBS = -L../ -lredisclient -lm -lhiredis `pkg-config --libs glib-2.0 gthread-2.0`  

CFLAGS = -Wall -g `pkg-config --cflags glib-2.0`  -DPROF_TEST
=======
PROGS = test_ketama proftest

LIBS = -L../ -lredisclient -lm -lhiredis `pkg-config --libs glib-2.0 gthread-2.0`  

CFLAGS = -Wall -g `pkg-config --cflags glib-2.0` 
>>>>>>> af63872fa2d847e7929e85914300cf829b56a6c7

all: $(PROGS)

STPROG = test_ketama.o 
PROF = proftest.o
<<<<<<< HEAD
FEED_API = test_feed_api.o
LINK_API = test_link_api.o
TEST_LIST = test_list.o
TEST_STR = test_string.o
TEST_HASH = test_hash.o
KETAMA_VALUE = ketama_value.o
=======
>>>>>>> af63872fa2d847e7929e85914300cf829b56a6c7

proftest:$(PROF)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

<<<<<<< HEAD
test_hash: $(TEST_HASH)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

test_string: $(TEST_STR)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	
test_ketama: $(STPROG) 
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	
test_feed_api: $(FEED_API)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

test_link_api: $(LINK_API)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	
test_list: $(TEST_LIST)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
ketama_value: $(KETAMA_VALUE)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	
ketama_value.o: ketama_value.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(LIBS)
proftest.o: proftest.c
	$(CC) $(CFLAGS) -c -o $@ $^
test_hash.o:test_hash.c
	$(CC) $(CFLAGS) -c -o $@ $^
test_ketama.o: test_ketama.c 
	$(CC) $(CFLAGS) -c -o $@ $^

test_feed_api.o: test_feed_api.c
	$(CC) $(CFLAGS) -c -o $@ $^
test_link_api.o: test_link_api.c
	 $(CC) $(CFLAGS) -c -o $@ $^

=======
test_ketama: $(STPROG) 
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	
proftest.o: proftest.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
test_ketama.o: test_ketama.c 
	$(CC) $(CFLAGS) -c -o $@ $^

>>>>>>> af63872fa2d847e7929e85914300cf829b56a6c7

clean: 
	rm -f *.o *~ $(PROGS) core.*
