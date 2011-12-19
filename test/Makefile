ARCH := $(shell uname -m)
X64 = x86_64
PROGS = test_ketama proftest test_feed_api test_link_api test_list test_string test_hash ketama_value

INCLUDE = -I ../include
LIBS = -L../libs -lredisclient -lm -lhiredis `pkg-config --libs glib-2.0 gthread-2.0`  

CFLAGS = -Wall -g `pkg-config --cflags glib-2.0`  -DPROF_TEST $(INCLUDE)

all: $(PROGS)

STPROG = test_ketama.o 
PROF = proftest.o
FEED_API = test_feed_api.o
LINK_API = test_link_api.o
TEST_LIST = test_list.o
TEST_STR = test_string.o
TEST_HASH = test_hash.o
KETAMA_VALUE = ketama_value.o

proftest:$(PROF)
        $(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(INCLUDE)

test_hash: $(TEST_HASH)
        $(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(INCLUDE)

test_string: $(TEST_STR)
        $(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(INCLUDE)

test_ketama: $(STPROG) 
        $(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(INCLUDE)

test_feed_api: $(FEED_API)
        $(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(INCLUDE)

test_link_api: $(LINK_API)
        $(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(INCLUDE)

test_list: $(TEST_LIST)
        $(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(INCLUDE)
ketama_value: $(KETAMA_VALUE)
        $(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(INCLUDE)

ketama_value.o: ketama_value.c
        $(CC) $(CFLAGS) -c -o $@ $^ $(INCLUDE)
proftest.o: proftest.c
        $(CC) $(CFLAGS) -c -o $@ $^ $(INCLUDE)
test_hash.o:test_hash.c
        $(CC) $(CFLAGS) -c -o $@ $^ $(INCLUDE)
test_ketama.o: test_ketama.c 
        $(CC) $(CFLAGS) -c -o $@ $^ $(INCLUDE)

test_feed_api.o: test_feed_api.c
        $(CC) $(CFLAGS) -c -o $@ $^ $(INCLUDE)
test_link_api.o: test_link_api.c
         $(CC) $(CFLAGS) -c -o $@ $^ $(INCLUDE)


clean: 
        rm -f *.o *~ $(PROGS) core.*