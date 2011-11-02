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

typedef enum {
	REDIS_SUCCESS, REDIS_FAILURE, REDIS_OP_ERR_KEY_TYPE, REDIS_CONNECTION_FAILURE
} redis_return_st;

typedef struct _server_config_st server_config_st;
typedef struct _redis_conn_pool_st redis_conn_pool_st;
typedef struct _redis_client_config_st redis_client_config_st;

typedef struct _redis_client_st {
	GHashTable *redis_servers;//<redis_name, redis_conn_pool_st>
	struct ketama *ketama;
	redis_client_config_st *conf;
} redis_client_st;

typedef struct redisReply redis_reply_st;

typedef enum{
	ERR_FROM_MYSQL, ERR_END
}redis_err_type;

typedef struct _redis_err_msg
{
	char *hash_key;
	char *tblname;
	char *guid;
	redis_err_type type;
}redis_err_st;


void redis_err(redis_err_st *data);

char *generate_key(const char *format, ...);

redis_reply_st *redis_query(redis_client_st *redis_client, const char *format, ...);
redis_reply_st *redis_query_argv(redis_client_st *redis_client, int argc, const char **argv);




redis_return_st redis_del(redis_client_st *redis_client, const char *key);


/***************************************string op**********************************/

redis_return_st redis_set(redis_client_st *redis_client, const char *key, const char *value);
char * redis_get(redis_client_st *redis_client, const char *key, redis_return_st *rc);
/***************************************string end**********************************/


/*********************************set op******************************/
redis_return_st redis_sadd(redis_client_st *redis_client, const char *key, const char *value);
redis_return_st redis_srem(redis_client_st *redis_client, const char *key, const char *value);
int redis_sismember(redis_client_st * redis_client,const char * key,const char * value,redis_return_st * rc);
int redis_smembers(redis_client_st *redis_client, const char *key, char ***elements, redis_return_st *rc);

/*********************************set end******************************/

/*********************************hash op*****************************/
int redis_hdel(redis_client_st *redis_client, const char *key, const char *field, redis_return_st *rc);
int redis_hsetnx(redis_client_st *redis_client, const char *key, const char *field, const char *val, redis_return_st *rc);
int redis_hset(redis_client_st *redis_client, const char *key, const char *field, const char *val, redis_return_st *rc);
int redis_hlen(redis_client_st *redis_client, const char *key, const char *field, redis_return_st *rc);

int redis_hvals(redis_client_st *redis_client, const char *key,  char ***elements, redis_return_st *rc);
int redis_hkeys(redis_client_st *redis_client, const char *key,  char ***elements, redis_return_st *rc);
int redis_hincrby(redis_client_st *redis_client, const char *key, const char *field,  int num,  redis_return_st *rc);

int redis_hgetall(redis_client_st *redis_client, const char *key,  char ***elements, redis_return_st *rc);
char* redis_hget(redis_client_st *redis_client, const char *key, const char *field, redis_return_st *rc);
int redis_hexists(redis_client_st *redis_client, const char *key, const char *field, redis_return_st *rc);

/*********************************hash end*****************************/



/*********************************list op******************************/
char *redis_lindex(redis_client_st *redis_client, const char*key, long int index, redis_return_st *rc);
int redis_linsert(redis_client_st *redis_client, const char *key, int is_before, const char* prov, const char *value,
		redis_return_st *rc);
int redis_llen(redis_client_st *redis_client, const char *key, redis_return_st *rc);
int redis_lpush(redis_client_st *redis_client, const char *key, const char *value, redis_return_st *rc);
char *redis_lpop(redis_client_st *redis_client, const char *key, redis_return_st *rc);
int redis_lpushx(redis_client_st *redis_client, const char *key, const char *value, redis_return_st *rc);
int redis_lrange(redis_client_st *redis_client, const char *key, int begin, int end, char ***elements,redis_return_st *rc);
int redis_lrem(redis_client_st *redis_client, const char *key, int count, const char *value, redis_return_st *rc);
redis_return_st redis_lset(redis_client_st *redis_client, const char *key, int index, const char *value);
redis_return_st redis_ltrim(redis_client_st *redis_client, const char *key, int start, int end);
char *redis_rpop(redis_client_st *redis_client, const char *key, redis_return_st *rc);
int redis_rpush(redis_client_st *redis_client, const char *key, const char *value, redis_return_st *rc);
/*********************************list end******************************/

void free_and_nil(void *key);
void freeReply(void *reply);

void redis_client_free(redis_client_st* redis_client);
redis_client_st * redis_client_init(const char *config_path);
#endif /* REDIS_CLIENT_H_ */
