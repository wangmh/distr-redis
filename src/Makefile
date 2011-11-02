CC = gcc


LIBS = -lm $(shell pkg-config --libs glib-2.0)  -lpthread -lhiredis  -lgthread-2.0

CFLAGS = -Wall -g `pkg-config --cflags glib-2.0` -c  -I. -DPROF_TEST -I../include


OS = $(shell uname -s)
ifeq ($(OS),Darwin)
	SHAREDLIB_LINK_OPTIONS=-dynamiclib -Wl,-install_name -Wl,
else
	SHAREDLIB_LINK_OPTIONS=-shared -Wl,-soname,
endif



TARGETS = libredisclient.a 

OBJ = redis_client.o ketama.o config.o  redis_log.o conn_pool.o redis_feed_api.o common.o redis_link_api.o redis_list.o redis_string.o redis_hash.o

all: $(TARGETS)

%.o: %.c
	$(CC) $(CFLAGS)  $*.c

libredisclient.a: $(OBJ) 
	$(AR) crsv  $@ $^
	mkdir -p ../libs
	cp -f libredisclient.a ../libs
libredisclient.so: $(OBJ)
	$(CC) $(SHAREDLIB_LINK_OPTIONS)$@ -o $@ $^
install: 
	@echo "installing library(to be done)"
clean: 
	rm -f *o *~ $(TARGETS)
