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

#include "redis_log.h"
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
	char *redis_err_file;
	unsigned int maxconns;
	unsigned int minconns;
	gsize servers_count;
	int retry_time;
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

 char * get_hash_key(const char *key);
 void initialize_redis_ketama(redis_client_st * redis_client);
redis_conn_pool_st* initialize_redis_server_conn_pool(const char *host,
		int port, uint32_t initial, uint32_t max);

void free_redis_server_conn_pool(redis_conn_pool_st *pool);

int redis_server_conn_pool_push(connection_pool_entry *item);

connection_pool_entry *redis_server_conn_pool_pop(redis_conn_pool_st *pool,
		int block);

redis_return_st redis_return_status(redis_client_st *redis_client, int argc, const char **argv);

int redis_return_integer(redis_client_st *redis_client, int argc, const char **argv, redis_return_st *rc);

char *redis_return_str(redis_client_st *redis_client,  int argc, const char **argv,
		redis_return_st *rc);
int redis_return_muti(redis_client_st *redis_client, int argc, const char **argv, char ***elements,  redis_return_st *rc);



redis_reply_st *redis_mget(redis_client_st *redis_client, char **keys, int len);

redis_reply_st *_redis_set(redis_client_st *redis_client, const char *key,
		const char *value);


#endif /* COMMON_H_ */
