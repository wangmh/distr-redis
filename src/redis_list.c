/*
 * redis_list.c
 *
 *  Created on: 2011-4-28
 *      Author: saint
 */

#include "common.h"

char *redis_lindex(redis_client_st *redis_client, const char*key,
		long int index, redis_return_st *rc)
{
	if (redis_client == NULL || key == NULL)
	{
		if (rc != NULL)
			*rc = REDIS_FAILURE;
		return NULL;
	}

	char *cmd = NULL;
	char *val = NULL;
	cmd = generate_key("LINDEX %s %ld", key, index);
	val = redis_return_str(redis_client, cmd, rc);
	free_and_nil(cmd);
	return val;
}

int redis_linsert(redis_client_st *redis_client, const char *key,
		int is_before, const char* prov, const char *value, redis_return_st *rc)
{
	char *before = "BEFORE";
	char *after = "AFTER";
	char *where = NULL;
	where = ((1 == is_before) ? before : after);
	if (redis_client == NULL || key == NULL || value == NULL)
	{
		if (rc != NULL)
			*rc = REDIS_FAILURE;
		return -1;
	}

	int rt = -1;
	char *cmd = generate_key("LINSERT %s %s %s %s", key, where, prov, value);
	rt = redis_return_integer(redis_client, cmd, rc);
	free_and_nil(cmd);
	return rt;
}

int redis_llen(redis_client_st *redis_client, const char *key,
		redis_return_st *rc)
{
	int rt = -1;
	if (redis_client == NULL || key == NULL)
	{
		if (rc)
		{
			*rc = REDIS_FAILURE;
		}
		return rt;
	}
	char *cmd = generate_key("LLEN %s", key);
	rt = redis_return_integer(redis_client, cmd, rc);
	free_and_nil(cmd);
	return rt;
}

char *redis_lpop(redis_client_st *redis_client, const char *key,
		redis_return_st *rc)
{
	if (redis_client == NULL || NULL == key)
	{
		if (rc)
		{
			*rc = REDIS_FAILURE;
		}
		return NULL;
	}
	char *cmd = NULL;
	char *rt = NULL;
	cmd = generate_key("LPOP %s", key);
	rt = redis_return_str(redis_client, cmd, rc);
	free_and_nil(cmd);
	return rt;
}

int redis_lpush(redis_client_st *redis_client, const char *key,
		const char *value, redis_return_st *rc)
{
	if (redis_client == NULL || NULL == key || NULL == value)
	{
		if (rc)
		{
			*rc = REDIS_FAILURE;
		}
		return -1;
	}
	char *cmd = NULL;
	int rt = -1;
	cmd = generate_key("LPUSH %s %s", key, value);
	rt = redis_return_integer(redis_client, cmd, rc);
	free_and_nil(cmd);
	return rt;
}

int redis_lpushx(redis_client_st *redis_client, const char *key,
		const char *value, redis_return_st *rc)
{

	if (redis_client == NULL || NULL == key || NULL == value)
	{
		if (rc)
		{
			*rc = REDIS_FAILURE;
		}
		return -1;
	}
	char *cmd = NULL;
	int rt = -1;
	cmd = generate_key("LPUSHX %s %s", key, value);
	rt = redis_return_integer(redis_client, cmd, rc);
	free_and_nil(cmd);
	return rt;
}

int redis_lrange(redis_client_st *redis_client, const char *key, int begin,
		int end, char ***elements, redis_return_st *rc)
{
	if (redis_client == NULL || key == NULL || elements == NULL)
	{
		if(rc)
			*rc = REDIS_FAILURE;
		*elements = NULL;
		return -1;
	}
	int ret = -1;
	char *cmd = NULL;
	cmd = generate_key("LRANGE %s %d %d", key, begin, end);
	ret = redis_return_muti(redis_client, cmd, elements, rc);
	free_and_nil(cmd);
	return ret;
}

int redis_lrem(redis_client_st *redis_client, const char *key, int count, const char *value, redis_return_st *rc)
{
	if(redis_client == NULL || key == NULL || value == NULL)
	{
		if(rc)
			*rc = REDIS_FAILURE;
		return 0;
	}
	int ret = 0;
	char *cmd = NULL;
	cmd = generate_key("LREM %s %d %s", key, count, value);
	ret = redis_return_integer(redis_client, cmd, rc);
	free_and_nil(cmd);
	return ret;
}

redis_return_st redis_lset(redis_client_st *redis_client, const char *key, int index, const char *value)
{
	redis_return_st rc = REDIS_FAILURE;

	if(redis_client == NULL || key == NULL || value == NULL)
	{
		return rc;
	}
	char *cmd = NULL;
	cmd = generate_key("LSET %s %d %s", key, index, value);
	rc = redis_return_status(redis_client, cmd);
	free_and_nil(cmd);
	return rc;
}

redis_return_st redis_ltrim(redis_client_st *redis_client, const char *key, int start, int end)
{
	redis_return_st rc = REDIS_FAILURE;
	if(redis_client == NULL || key == NULL)
	{
		return rc;
	}
	char *cmd = generate_key("LTRIM %s %d %d", key, start, end);
	rc = redis_return_status(redis_client, cmd);
	free_and_nil(cmd);
	return rc;
}

char *redis_rpop(redis_client_st *redis_client, const char *key, redis_return_st *rc)
{
	if(redis_client == NULL || key == NULL)
	{
		if(rc)
			*rc = REDIS_FAILURE;
		return NULL;
	}
	char *rt = NULL;
	char *cmd = NULL;
	cmd = generate_key("RPOP %s", key);
	rt = redis_return_str(redis_client, cmd, rc);
	free_and_nil(cmd);
	return rt;
}

int redis_rpush(redis_client_st *redis_client, const char *key, const char *value, redis_return_st *rc)
{
	if(redis_client == NULL || key == NULL)
	{
		if(rc)
			*rc = REDIS_FAILURE;
		return -1;
	}
	int rt = -1;
	char *cmd = NULL;
	cmd = generate_key("RPUSH %s %s",key, value);
	rt = redis_return_integer(redis_client, cmd, rc);
	free_and_nil(cmd);
	return rt;
}


int redis_rpushx(redis_client_st *redis_client, const char *key, const char *value, redis_return_st *rc)
{
	if(redis_client == NULL || key == NULL)
	{
		if(rc)
			*rc = REDIS_FAILURE;
		return -1;
	}
	int rt = -1;
	char *cmd = NULL;
	cmd = generate_key("RPUSHX %s %s",key, value);
	rt = redis_return_integer(redis_client, cmd, rc);
	free_and_nil(cmd);
	return rt;
}




