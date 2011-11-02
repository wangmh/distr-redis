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
	char *cmd = generate_key("hdel %s %s", key, field);
	int ret = redis_return_integer(redis_client, cmd, rc);
	free_and_nil(cmd);
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
	char *cmd = generate_key("HEXISTS %s %s", key, field);
	int ret = redis_return_integer(redis_client, cmd, rc);
	free_and_nil(cmd);
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
	char *cmd = generate_key("HGET %s %s", key, field);
	char *ret = redis_return_str(redis_client, cmd, rc);
	free_and_nil(cmd);
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
	char *cmd = generate_key("HGETALL %s ", key);
	int ret =redis_return_muti(redis_client, cmd, elements, rc);
	free_and_nil(cmd);
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
	char *cmd = generate_key("HINCRBY %s %s  %d", key, field, num);
	int ret = redis_return_integer(redis_client, cmd, rc);
	free_and_nil(cmd);
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
	char *cmd = generate_key("HKEYS %s ", key);
	int ret =redis_return_muti(redis_client, cmd, elements, rc);
	free_and_nil(cmd);
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
	char *cmd = generate_key("HVALS %s ", key);
	int ret =redis_return_muti(redis_client, cmd, elements, rc);
	free_and_nil(cmd);
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
	char *cmd = generate_key("HLEN %s %s", key, field);
	int ret = redis_return_integer(redis_client, cmd, rc);
	free_and_nil(cmd);
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
	char *cmd = generate_key("HSET %s %s %s", key, field, val);
	int ret = redis_return_integer(redis_client, cmd, rc);
	free_and_nil(cmd);
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
	char *cmd = generate_key("HSETNX %s %s %s", key, field, val);
	int ret = redis_return_integer(redis_client, cmd, rc);
	free_and_nil(cmd);
	return ret;
}






