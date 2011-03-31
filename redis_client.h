/*
 * redis_client.h
 *
 *  Created on: 2011-3-30
 *      Author: saint
 */

#ifndef REDIS_CLIENT_H_
#define REDIS_CLIENT_H_
#include <glib.h>
#include <hiredis/hiredis.h>
#include "ketama.h"


typedef struct _server_config_st server_config_st;
typedef struct _redis_conn_pool_st redis_conn_pool_st;
typedef struct _redis_client_config_st redis_client_config_st;


typedef struct _redis_client_st
{
	GHashTable *redis_servers;//<redis_name, redis_conn_pool_st>
	struct ketama *ketama;
	redis_client_config_st *conf;
} redis_client_st;

typedef struct redisReply redis_reply_st;


redis_reply_st *redis_mget(redis_client_st *redis_client,  char **keys,
		int len);
redis_reply_st *redis_get(redis_client_st *redis_client, const char *key);
redis_reply_st *redis_set(redis_client_st *redis_client, const char *key, const char *value);
redis_reply_st *redis_hget(redis_client_st *redis_client, const char *key, const char *sub_key);
redis_reply_st *redis_hgetall(redis_client_st *redis_client, const char *key);
redis_reply_st *redis_hset(redis_client_st *redis_client, const char *key, const char *sub_key, const char *value);
redis_reply_st *redis_query(redis_client_st *redis_client, const char *format, ...);
redis_reply_st *redis_query_argv(redis_client_st *redis_client, int argc, const char **argv);
void freeReply(void *reply);

void redis_client_free(redis_client_st* redis_client);
redis_client_st * redis_client_init(const char *config_path);
#endif /* REDIS_CLIENT_H_ */
