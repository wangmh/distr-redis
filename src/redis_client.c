/*
 * redis_client.c
 *
 *  Created on: 2011-3-30
 *      Author: saint
 */

#include "common.h"

#define GOZAP "gozap@gozap.com"

extern struct timeval timeout;

FILE *redis_err_fp = NULL;

static pthread_mutex_t logerrMutex =
PTHREAD_MUTEX_INITIALIZER;

int retry_times = -1;
/************************************************
 *reconnection  to redis-server, if connection error , return LIGHT_REDIS_ERR_CONNECT;
 *so the client should use this to decide whether the return value can use;
 ************************************************/
#define RECONNECTE(FUN)   \
if (connection_entry->rh->err ){ \
	int retry = 0; \
	int err = connection_entry->rh->err ;\
	redis_log(L_ERR, "errstr is %d %s:%d n", connection_entry->rh->err,\
   		connection_entry->pool->host,connection_entry-> pool->port);\
   	redisFree(connection_entry->rh); \
	while (retry_times == -1 || retry < retry_times) \
	{ \
		  retry++; \
		connection_entry->rh = redisConnectWithTimeout(connection_entry->pool->host, connection_entry-> pool->port,  timeout);\
			err = connection_entry->rh->err;\
		if (err) \
		{ \
			redisFree(connection_entry->rh);\
			sleep(1); \
			redis_log( \
					L_ERR, \
					"%s connection wrong in pthread 0x%lx, %s:%d\n,retry %d times once in 1 seconds", \
					__FUNCTION__,pthread_self(), connection_entry->pool->host, \
					connection_entry->pool->port, retry); \
		} \
		else{\
			 redis_log(L_INFO, "%s:%d conned by retry %d %s:%d\n",\
		    		connection_entry->pool->host,connection_entry-> pool->port, retry, __FUNCTION__,\
		                        __LINE__);\
		     break;\
		}\
	} \
	if(retry == retry_times){	\
		redis_log(L_ERR, "connection  err after try %d times\n", retry);\
		free(connection_entry);\
		connection_entry = NULL;\
		return NULL;\
	}\
	reply = FUN;\
}

void free_and_nil(void *key)
{
	if (key)
	{
		free(key);
		key = NULL;
	}
}

//when the value remove from the hashtable，free the mem
static void g_queue_free_all(gpointer q)
{
	redis_conn_pool_st *pool = (redis_conn_pool_st *) q;

	free_redis_server_conn_pool(pool);
}

static void free_key(gpointer data)
{
	free(data);
}
static void free_server_config_st(server_config_st *node)
{
	if (node)
	{
		if (node->key)
		{
			free(node->key);
		}
		if (node->value)
		{
			free(node->value);
		}
		free(node);
	}
}
static void free_redis_conf(redis_client_config_st *conf)
{
	if (conf)
	{
		if (conf->redis_log_path)
			free(conf->redis_log_path);
		int i;
		for (i = 0; i < conf->servers_count; i++)
		{
			free_server_config_st(conf->server_nodes[i]);
		}

		free(conf);
	}
}

static void free_redis_servers(GHashTable *redis_servers)
{
	g_hash_table_destroy(redis_servers);
}
//解析hostname
static void parse_redis_server(char *value, char *hostname, int len, int *port)
{
	char *p = NULL;
	if (NULL == (p = strchr(value, ':')))
	{
		snprintf(hostname, len, "%s", value);
		*port = 6379;
	}
	else
	{
		*p = '\0';
		snprintf(hostname, len, "%s", value);
		*port = atoi(p + 1);
		if (*port <= 0)
			*port = 6379;
		*p = ':';
	}
}

//初始化到后端的servers的链接
static void initialize_redis_servers(redis_client_st *redis_client)
{

	if (NULL == redis_client)
		return;
	redis_client_config_st *conf = redis_client->conf;
	char hostname[200];
	int port = 0;
	int server_count, i;

	redis_client->redis_servers = g_hash_table_new_full(g_str_hash, g_str_equal, free_key, g_queue_free_all);
	redis_conn_pool_st *pool = NULL;
	server_config_st *server = NULL;
	for (i = 0, server_count = conf->servers_count; i < server_count; i++)
	{
		server = conf->server_nodes[i];
		parse_redis_server(server->value, hostname, sizeof(hostname), &port);
		pool = initialize_redis_server_conn_pool(hostname, port, conf->minconns, conf->maxconns);
		if (pool != NULL)
		{
			g_hash_table_insert(redis_client->redis_servers, server->key, pool);
		}
	}
	if (i != conf->servers_count)
	{
		redis_log(L_ERR, "conns hash create wrong\n");
		exit(1);
	}

}

//初始化一致性hash算法
 void initialize_redis_ketama(redis_client_st * redis_client)
{
	int i;
	if (NULL == redis_client)
		return;
	redis_client_config_st *conf = redis_client->conf;
	if (conf->servers_count <= 0)
	{
		redis_log(L_ERR, "please config redis servers hostname and port\n");
		exit(1);
	}

	struct ketama * ketama = (struct ketama *) calloc(sizeof(struct ketama), 1);
	if (NULL == ketama)
	{
		LOGMEMORYOUT;
	}
	ketama->count = conf->servers_count;
	server_config_st *server = NULL;
	ketama->name = (char **) calloc(sizeof(char *), ketama->count);
	ketama->weight = (int *) calloc(sizeof(int), ketama->count);
	for (i = 0; i < ketama->count; i++)
	{
		server = conf->server_nodes[i];
		if (server == NULL)
			break;
		redis_log(L_DEBUG, "%s:%s", server->key, server->value);
		ketama->name[i] = strdup(server->key);
		ketama->weight[i] = 100;
		ketama->totalweight += ketama->weight[i];
	}
	if (i != ketama->count)
	{
		redis_log(L_ERR, "ketama create wrong\n");
		exit(1);
	}
	redis_client->ketama = ketama;
	create_ketama(redis_client->ketama, 500);
}

//if key is {wangminghua@gozap.com}_follows_jid
//means hash by new_key wangminghua@gozap.com,not the original key
 char * get_hash_key(const char *key)
{
	if (key == NULL)
		return NULL;
	char *new_key = strdup(key);
	char *ret = NULL;
	char *p = NULL;
	char *q = NULL;
	if (NULL != (p = strchr(new_key, '{')))
	{
		if (NULL != (q = strchr(new_key, '}')))
		{
			if (q > p)
			{
				*q = '\0';
				ret = strdup(p + 1);
				*q = '}';
				free(new_key);
				return ret;
			}
		}

	}
	return new_key;
}
//"*3\r\n$3\r\nSET\r\n$5\r\nmykey\r\n$7\r\nmyvalue\r\n"
static char *get_key(const char *cmd)
{
	if (cmd == NULL)
		return NULL;
	const char *p = cmd;
	int line_number = 0;
	const char *begin = NULL;
	const char *end = NULL;
	while (*p != '\0')
	{
		if (*p == '\r' && *++p == '\n')
		{
			line_number++;
			if (line_number == 4)
			{
				begin = p + 1;
			}
			if (line_number == 5)
			{
				end = p;
			}
		}
		p++;
	}
	if (line_number < 5)
		return NULL;
	char *key = (char*) calloc(sizeof(char), end - begin);
	snprintf(key, end - begin, "%s", begin);
	redis_log(L_DEBUG, "get key is %s", key);

	return key;
}

static redis_conn_pool_st *get_redis_conn_pool(redis_client_st *redis_client, const char *hash_key)
{
	redis_conn_pool_st *pool = NULL;
	int index = get_server(redis_client->ketama, hash_key);
	char *server_name = redis_client->conf->server_nodes[index]->key;
	pool = (redis_conn_pool_st *) g_hash_table_lookup(redis_client->redis_servers, server_name);
	return pool;
}

static connection_pool_entry *get_redis_server_conn(redis_client_st *redis_client, const char *key)
{
	if (NULL == redis_client || NULL == key)
	{
		return NULL;
	}
	redis_conn_pool_st *pool = NULL;
	connection_pool_entry * entry;
	char *hash_key = get_hash_key(key);
	if (hash_key)
	{
		pool = get_redis_conn_pool(redis_client, hash_key);

		entry = redis_server_conn_pool_pop(pool, 1);
		if (entry)
			redis_log(L_DEBUG, "key %s %s, %d fd:%d", hash_key, pool->host, pool->port, entry->rh->fd);
		else
			redis_log(L_ERR, "key %s can't find conn", hash_key);
		free_and_nil(hash_key);
		return entry;
	}
	return NULL;
}

static redis_client_st* initialize_redis_client(redis_client_config_st *conf)
{

	redis_client_st *redis_client = (redis_client_st *) calloc(sizeof(redis_client), 1);

	if (NULL == redis_client)
	{
		LOGMEMORYOUT;
	}
	redis_client->conf = conf;
	initialize_redis_ketama(redis_client);
	initialize_redis_servers(redis_client);
	redis_err_fp = fopen(conf->redis_err_file, "a+");
	if (redis_err_fp == NULL)
	{
		redis_log(L_ERR, "open redis_err_file %s error", conf->redis_err_file);
		exit(-1);
	}
	return redis_client;
}

static void generate_m_argv(char **keys, int len, char ***argv, size_t **argvlen, char *command)
{
	char **argv_tmp = NULL;
	size_t *argvlen_tmp = NULL;
	argvlen_tmp = (size_t *) calloc(sizeof(size_t), len + 1);
	argv_tmp = (char **) calloc(sizeof(char *), len + 1);
	int i = 0;
	for (i = 0; i < len + 1; i++)
	{
		if (i == 0)
		{
			argvlen_tmp[i] = strlen(command);
			argv_tmp[i] = command;
		}
		else
		{
			if (keys[i - 1])
			{
				argvlen_tmp[i] = strlen(keys[i - 1]);
				argv_tmp[i] = keys[i - 1];
			}
			else
			{
				break;
			}
		}
	}
	*argv = argv_tmp;
	*argvlen = argvlen_tmp;
}

static void do_redis_err_from_mysql(redis_err_st *data)
{

	char message[124];
	time_t timeval;
	timeval = time(NULL);
	memset(message, 0, sizeof(message));
	strcpy(message, ctime(&timeval));

	pthread_mutex_lock(&logerrMutex);
	fprintf(redis_err_fp, "%s", message);
	fprintf(redis_err_fp, "%d, %s, %s, %s\n", data->type, data->hash_key, data->tblname, data->guid);
	pthread_mutex_unlock(&logerrMutex);
}
/***********************init&free***************************/

redis_client_st* redis_client_init(const char *config_path)
{
	redis_client_st *redis_client = NULL;
	redis_client_config_st *redis_config = redis_client_conf_init(config_path);
	redis_log_init(redis_config->redis_log_path, redis_config->log_level, "redis_client");
	retry_times = redis_config->retry_time;
	redis_client = initialize_redis_client(redis_config);
	return redis_client;
}

void redis_client_free(redis_client_st* redis_client)
{
	if (redis_client)
	{
		if (redis_client->ketama)
			free_ketama(redis_client->ketama);
		if (redis_client->redis_servers)
			free_redis_servers(redis_client->redis_servers);
		if (redis_client->conf)
			free_redis_conf(redis_client->conf);
	}

}

void freeReply(void *reply)
{
	if (reply)
		freeReplyObject(reply);
}



redis_reply_st *redis_query(redis_client_st *redis_client, const char *format, ...)
{
	va_list ap;
	char *cmd;
	redis_reply_st *reply = NULL;

	va_start(ap,format);
	redisvFormatCommand(&cmd, format, ap);
	char *key = get_key(cmd);
	if (NULL == key)
	{
		redis_log(L_ERR, "please input a key for distribution");
		return NULL;
	}

	connection_pool_entry *connection_entry = get_redis_server_conn(redis_client, key);
	free_and_nil(key);
	if (NULL == connection_entry)
	{
		return NULL;
	}
#ifdef PROF_TEST
	long start = 0;
	long end = 0;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	start = tv.tv_sec * 1000000 + tv.tv_usec;
#endif
	va_start(ap,format);
	reply = redisvCommand(connection_entry->rh, format, ap);
#ifdef PROF_TEST
	gettimeofday(&tv, NULL);
	end = tv.tv_sec * 1000000 + tv.tv_usec - start;
	if(end > 10000)
	redis_log(L_INFO, "command %s:%d %s cmd total cost time %ldus",connection_entry->pool->host,
			connection_entry->pool->port, cmd, end);
#endif
	va_start(ap,format);
	RECONNECTE(redisvCommand(connection_entry->rh, format, ap));
	va_end(ap);
	redis_server_conn_pool_push(connection_entry);

	free_and_nil(cmd);
	return reply;

}

redis_reply_st *redis_query_argv(redis_client_st *redis_client, int argc, const char **argv)
{

	if (NULL == redis_client || argc < 2 || NULL == argv)
		return NULL;
	redis_reply_st *reply = NULL;
	connection_pool_entry *connection_entry = get_redis_server_conn(redis_client, argv[1]);
	if (NULL == connection_entry)
	{
		return NULL;
	}
	size_t *argvlen = (size_t *) calloc(sizeof(size_t), argc);
	if (argvlen == NULL)
		return NULL;
	int i;
	for (i = 0; i < argc; i++)
	{
		if (argv[i])
			argvlen[i] = strlen(argv[i]);
		else
			argvlen[i] = 0;
	}
	reply = redisCommandArgv(connection_entry->rh, argc, argv, argvlen);
	RECONNECTE(redisCommandArgv(connection_entry->rh, argc, argv, argvlen));
	redis_server_conn_pool_push(connection_entry);
	free(argvlen);

	return reply;
}

void redis_err(redis_err_st *data)
{

	if (data == NULL)
		return;
	switch (data->type)
	{
	case ERR_FROM_MYSQL:
		do_redis_err_from_mysql(data);
		break;
	default:
		break;
	}
}



/****************************string*****************************/



redis_reply_st *redis_mget(redis_client_st *redis_client, char **keys, int len)
{
        if (NULL == redis_client || NULL == keys || len <= 0)
        {
                return NULL;
        }
        connection_pool_entry *connection_entry = get_redis_server_conn(
                        redis_client, keys[0]);
        if (NULL == connection_entry)
        {
                return NULL;
        }
        redis_reply_st *reply = NULL;
        char command[] = "mget";
        char **argv = NULL;
        size_t *argvlen = NULL;
        generate_m_argv(keys, len, &argv, &argvlen, command);
        reply = redisCommandArgv(connection_entry->rh, len + 1,
                        (const char **) argv, argvlen);
        RECONNECTE(redisCommandArgv(connection_entry->rh, len + 1,(const char **) argv, argvlen));
        free(argv);
        free(argvlen);
        redis_server_conn_pool_push(connection_entry);
        return reply;
}

