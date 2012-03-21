/*
 * redis_hash.c
 *
 *  Created on: 2011-4-28
 *      Author: saint
 */

#include "common.h"

int redis_hdel(redis_client_st *redis_client, const char *key, const char *field, redis_return_st *rc)
{
	if(NULL == redis_client || NULL == key || NULL == field)
	{
		if(rc )
			*rc = REDIS_FAILURE;
		return -1;		
	}
	int argc = 3;
	const char *argv[] = {"HDEL", key, field};
	int ret = redis_return_integer(redis_client, argc, argv, rc);
	return ret;	
}

int redis_hexists(redis_client_st *redis_client, const char *key, const char *field, redis_return_st *rc)
{
	if(NULL == redis_client || NULL == key || NULL == field)
	{
		if(rc )
			*rc = REDIS_FAILURE;
		return -1;		
	}
	int argc = 3;
	const char *argv[] = {"HEXISTS", key, field};
	int ret = redis_return_integer(redis_client, argc, argv, rc);
	return ret;
}


char* redis_hget(redis_client_st *redis_client, const char *key, const char *field, redis_return_st *rc)
{
	if(NULL == redis_client || NULL == key || NULL == field)
	{
		if(rc )
			*rc = REDIS_FAILURE;
		return NULL;		
	}
	int argc = 3;
	const char *argv[] = {"HGET", key, field};
	char *ret = redis_return_str(redis_client, argc, argv, rc);
	return ret;
}

int redis_hgetall(redis_client_st *redis_client, const char *key,  char ***elements, redis_return_st *rc)
{
	if(NULL == redis_client || NULL == key  || NULL == elements)
	{
		if(rc )
			*rc = REDIS_FAILURE;
		return -1;		
	}
	int argc = 2;
	const char *argv[] = {"HGETALL", key};
	int ret =redis_return_muti(redis_client, argc, argv, elements, rc);
	return ret;
}

int redis_hincrby(redis_client_st *redis_client, const char *key, const char *field,  int num,  redis_return_st *rc)
{
	if(NULL == redis_client || NULL == key || NULL == field)
	{
		if(rc )
			*rc = REDIS_FAILURE;
		return -1;		
	}
	int argc = 4;
	char num_str[64] ;
	snprintf(num_str, sizeof(num_str), "%d", num);
	const char *argv[4] = {"HINCRBY", key, field, num_str};
	int ret = redis_return_integer(redis_client, argc, argv, rc);
	return ret;
}

int redis_hkeys(redis_client_st *redis_client, const char *key,  char ***elements, redis_return_st *rc)
{
	if(NULL == redis_client || NULL == key  || NULL == elements)
	{
		if(rc )
			*rc = REDIS_FAILURE;
		return -1;		
	}
	int argc = 2;
	const char *argv[2] = {"HKEYS", key};
	int ret =redis_return_muti(redis_client, argc, argv, elements, rc);
	return ret;
}

int redis_hvals(redis_client_st *redis_client, const char *key,  char ***elements, redis_return_st *rc)
{
	if(NULL == redis_client || NULL == key  || NULL == elements)
	{
		if(rc )
			*rc = REDIS_FAILURE;
		return -1;		
	}
	int argc = 2;
	const char *argv[2] = {"HVALS", key};
	int ret =redis_return_muti(redis_client, argc, argv, elements, rc);
	return ret;
}

int redis_hlen(redis_client_st *redis_client, const char *key, const char *field, redis_return_st *rc)
{
	if(NULL == redis_client || NULL == key || NULL == field)
	{
		if(rc )
			*rc = REDIS_FAILURE;
		return -1;		
	}
	int argc = 3;
	const char *argv[3] = {"HLEN", key, field};
	int ret = redis_return_integer(redis_client, argc, argv, rc);
	return ret;
}

int redis_hset(redis_client_st *redis_client, const char *key, const char *field, const char *val, redis_return_st *rc)
{
	if(NULL == redis_client || NULL == key || NULL == field || NULL == val)
	{	
		if(rc )
			*rc = REDIS_FAILURE;
		return -1;
	}
	int argc = 4;
	const char *argv[4] = {"HSET", key, field, val};
	int ret = redis_return_integer(redis_client, argc, argv, rc);
	return ret;
}

int redis_hsetnx(redis_client_st *redis_client, const char *key, const char *field, const char *val, redis_return_st *rc)
{
	if(NULL == redis_client || NULL == key || NULL == field || NULL == val)
	{	
		if(rc )
			*rc = REDIS_FAILURE;
		return -1;
	}
	int argc = 4;
	const char *argv[4] = {"HSETNX", key, field, val};
	int ret = redis_return_integer(redis_client, argc, argv, rc);
	return ret;
}






