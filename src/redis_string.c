/*
 * redis_string.c
 *
 *  Created on: 2011-4-29
 *      Author: saint
 */
#include "common.h"

redis_return_st redis_set(redis_client_st *redis_client, const char *key, const char *value)
{

	if(redis_client == NULL || key == NULL)
	{
		return REDIS_FAILURE;
	}
	redis_return_st rc;
	char *cmd = generate_key("set %s %s", key, value);
	rc = redis_return_status(redis_client, cmd);
	free_and_nil(cmd);
	return rc;
}

char *redis_get(redis_client_st *redis_client, const char *key, redis_return_st *rc)
{
	if(redis_client == NULL || key == NULL)
	{
		if(rc)
			*rc = REDIS_FAILURE;
		return NULL;
	}
	char *rt = NULL;
	char *cmd = generate_key("get %s", key);
	rt = redis_return_str(redis_client, cmd, rc);
	free_and_nil(cmd);
	return rt;
}
