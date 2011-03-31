CC = gcc


LIBS = -lm $(shell pkg-config --libs glib-2.0)  -lpthread -lhiredis  -lgthread-2.0

CFLAGS = -Wall -g `pkg-config --cflags glib-2.0`


OS = $(shell uname -s)
ifeq ($(OS),Darwin)
	SHAREDLIB_LINK_OPTIONS=-dynamiclib -Wl,-install_name -Wl,
else
	SHAREDLIB_LINK_OPTIONS=-shared -Wl,-soname,
endif



TARGETS = libredisclient.a 

OBJ = redis_client.o ketama.o config.o  log.o conn_pool.o

all: $(TARGETS)

libredisclient.a: $(OBJ) 
	$(AR) -cvq $@ $^
libredisclient.so: $(OBJ)
	$(CC) $(SHAREDLIB_LINK_OPTIONS)$@ -o $@ $^
redis_client.o: redis_client.c redis_client.h ketama.h
	$(CC) -c -fPIC $(CFLAGS) -o $@ redis_client.c
ketama.o: ketama.c ketama.h
	$(CC) -c -fPIC $(CFLAGS) -o $@ ketama.c
conn_pool.o: conn_pool.c common.h
	$(CC) -c -fPIC $(CFLAGS) -o $@ conn_pool.c
config.o: config.c common.h 
	$(CC) -c -fPIC $(CFLAGS) -o $@ config.c
log.o: log.c log.h
	$(CC) -c -fPIC $(CFLAGS) -o $@ log.c
install: 
	@echo "installing library(to be done)"
clean: 
	rm -f *o *~ $(TARGETS)
