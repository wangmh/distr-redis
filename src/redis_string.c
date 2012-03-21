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
	int argc = 3;
	const char *argv[3] = {"SET", key, value};
	rc = redis_return_status(redis_client, argc, argv);
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
	int argc = 2;
	const char *argv[2] = {"GET", key};
	rt = redis_return_str(redis_client, argc, argv, rc);
	return rt;
}
