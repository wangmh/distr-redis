/*
 * common.h
 *
 *  Created on: 2011-3-30
 *      Author: saint
 */

#ifndef COMMON_H_
#define COMMON_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <hiredis/hiredis.h>
#include <limits.h>
#include <errno.h>
#include "redis_client.h"

#include "log.h"
#include "ketama.h"

#ifndef MEMORYOUT

#define MEMORYOUT \
	do {\
			fprintf(stderr, "%s:%d memory is not enough\n",__FILE__,__LINE__);\
			exit(1);\
		}while(0)

#endif

#ifndef LOGMEMORYOUT

#define LOGMEMORYOUT \
	do {\
			redis_log(L_ERR, "%s:%d memory is not enough\n",__FILE__,__LINE__);\
			exit(1);\
		}while(0)

#endif

#define FILE_PATH_MAX_LENGTH 1024

struct _server_config_st
{
	char *key;
	char *value;
};

struct _redis_client_config_st
{
	char config_path[FILE_PATH_MAX_LENGTH];
	server_config_st **server_nodes;
	char *redis_log_path;
	unsigned int maxconns;
	unsigned int minconns;
	gsize servers_count;
	int log_level;
};

typedef struct _connection_pool_entry
{
	redisContext *rh;
	redis_conn_pool_st *pool;//回指到pool;
} connection_pool_entry;

struct _redis_conn_pool_st
{
	char host[128];
	int port;
	connection_pool_entry **conns;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int firstfree;
	uint32_t current_size;
	uint32_t size;
};

redis_conn_pool_st* initialize_redis_server_conn_pool(const char *host,
		int port, uint32_t initial, uint32_t max);

void free_redis_server_conn_pool(redis_conn_pool_st *pool);

int redis_server_conn_pool_push(connection_pool_entry *item);

connection_pool_entry *redis_server_conn_pool_pop(redis_conn_pool_st *pool,
		int block);

redis_client_config_st* redis_client_conf_init(const char *config_path);

void free_and_nil(void *key);

#endif /* COMMON_H_ */
