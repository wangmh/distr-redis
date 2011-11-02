/*
 * conn_pool.c
 *
 *  Created on: 2011-3-30
 *      Author: saint
 */
#include"common.h"

struct timeval timeout =
{ 3, 500000 }; // 1.5 seconds


static void mutex_enter(pthread_mutex_t *mutex)
{
	int ret;
	do
		ret = pthread_mutex_lock(mutex);
	while (ret == -1 && errno == EINTR);
}

static void mutex_exit(pthread_mutex_t *mutex)
{
	int ret;
	do
		ret = pthread_mutex_unlock(mutex);
	while (ret == -1 && errno == EINTR);
}

static redis_conn_pool_st * create_redis_conn_poll(const char *host, int port,
		uint32_t initial, uint32_t max)
{

	redis_conn_pool_st *pool = NULL;
	pool = (redis_conn_pool_st *) calloc(sizeof(redis_conn_pool_st), 1);
	if (NULL == pool)
	{
		LOGMEMORYOUT;
	}
	snprintf(pool->host, sizeof(pool->host), "%s", host);
	pool->port = port;
	pool->conns = (connection_pool_entry **) calloc(
			sizeof(connection_pool_entry *), max);
	pthread_mutex_init(&pool->mutex, NULL);
	pthread_cond_init(&pool->cond, NULL);
	pool->size = max;
	pool->current_size = 0;
	pool->firstfree = -1;
	return pool;
}

static int grow_pool(redis_conn_pool_st *pool)
{
	redisContext* rh = redisConnectWithTimeout(pool->host, pool->port, timeout);

	if (rh->err)
	{
		redis_log(L_ERR, " %s: %d redis is can't connect\n", pool->host,
				pool->port);
		return -1;
	}

	redis_log(L_INFO, "host:port = %s:%d and fd is %d redis conn is runing\n",
			pool->host, pool->port, rh->fd);
	connection_pool_entry *item = calloc(1, sizeof(connection_pool_entry));
	if (NULL == item)
	{
		redis_log(L_ERR, "%s:%d calloc error\n", __FUNCTION__, __LINE__);
		redisFree(rh);
		return -1;
	}
	item->pool = pool;
	item->rh = rh;
	pool->conns[++pool->firstfree] = item;
	pool->current_size++;
	return 0;
}

redis_conn_pool_st* initialize_redis_server_conn_pool(const char *host,
		int port, uint32_t initial, uint32_t max)
{
	if (host == NULL || max < initial || max <= 0)
		return NULL;
	int ii;
	redis_conn_pool_st* pool = create_redis_conn_poll(host, port, initial, max);
	if (NULL == pool)
	{
		return NULL;
	}

	for (ii = 0; ii < initial; ++ii)
	{
		if (grow_pool(pool) == -1)
			break;
	}
	if (ii != initial)
	{
		redis_log(L_ERR, "there are some servers can't reach");
		exit(-1);
	}

	return pool;

}

void free_redis_server_conn_pool(redis_conn_pool_st *pool)
{
	int xx;
	for (xx = 0; xx <= pool->firstfree; ++xx)
	{
		redisFree(pool->conns[xx]->rh);
		free(pool->conns[xx]);
	}

	pthread_mutex_destroy(&pool->mutex);
	pthread_cond_destroy(&pool->cond);
	free(pool->conns);
	free(pool);
}

connection_pool_entry *redis_server_conn_pool_pop(redis_conn_pool_st *pool,
		int block)
{
	if (pool == NULL)
		return NULL;
	connection_pool_entry *node = NULL;
	mutex_enter(&pool->mutex);
	do
	{
		if (pool->firstfree > -1)
		{
			node = pool->conns[pool->firstfree--];
			redis_log(L_DEBUG,
					"current is %d and left_size is %d  in pthread : 0x%lx \n",
					pool->current_size, pool->firstfree + 1, pthread_self());
		}
		else if (pool->size == pool->current_size)
		{
			if (block != 1)
			{
				mutex_exit(&pool->mutex);
				return NULL;
			}
			redis_log(
					L_DEBUG,
					"wait for a ideal connection   for %s:%d in pthread : 0x%lx \n",
					pool->host, pool->port, pthread_self());
			pthread_cond_wait(&pool->cond, &pool->mutex);
		}
		else
		{
			grow_pool(pool);
		}
	} while (node == NULL);
	pthread_mutex_unlock(&pool->mutex);
	return node;
}

int redis_server_conn_pool_push(connection_pool_entry *item)
{
	if (item)
	{
		redis_conn_pool_st *pool = item->pool;
		redis_log(L_DEBUG,
								"befor push current is %d and left_size is %d  in pthread : 0x%lx \n",
								pool->current_size, pool->firstfree + 1, pthread_self());

		mutex_enter(&pool->mutex);
		pool->conns[++pool->firstfree] = item;
		if ((0 == pool->firstfree) && (pool->size == pool->current_size))
		{
			pthread_cond_broadcast(&pool->cond);
		}
		mutex_exit(&pool->mutex);
		redis_log(L_DEBUG,
							"after push current is %d and left_size is %d  in pthread : 0x%lx \n",
							pool->current_size, pool->firstfree + 1, pthread_self());
	}
	return 0;
}
