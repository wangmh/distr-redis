/*
 * common.c
 *
 *  Created on: 2011-4-1
 *      Author: saint
 */
#include "common.h"

char *
generate_key(const char *format, ...)
{
	va_list ap;
	char key[5120];
	memset(key, 0, sizeof(key));
	va_start(ap,format);
	vsnprintf(key, sizeof(key), format, ap);
	va_end(ap);
	char *ret = strdup(key);
	return ret;
}

int redis_return_integer(redis_client_st *redis_client, const char *cmd, redis_return_st *rc)
{
	redis_return_st ret = REDIS_FAILURE;
	int rt = -1;
	redis_reply_st *reply = NULL;
	if (redis_client != NULL && cmd != NULL)
	{
		reply = redis_query(redis_client, cmd);
		if (reply == NULL)
		{
			ret = REDIS_CONNECTION_FAILURE;
		}
		else if (reply->type != REDIS_REPLY_INTEGER)
		{
			ret = REDIS_OP_ERR_KEY_TYPE;
		}
		else
		{
			rt = reply->integer;
			ret = REDIS_SUCCESS;
		}
	}
	if (rc != NULL)
	{
		*rc = ret;
	}
	return rt;
}

char *
redis_return_str(redis_client_st *redis_client, const char *cmd, redis_return_st *rc)
{
	if (redis_client == NULL || cmd == NULL)
	{
		if (rc != NULL)
			*rc = REDIS_FAILURE;
		return NULL;
	}

	redis_return_st ret = REDIS_FAILURE;
	redis_reply_st *reply = NULL;
	char *val = NULL;
	reply = redis_query(redis_client, cmd);
	if (reply == NULL)
	{
		ret = REDIS_CONNECTION_FAILURE;
	}
	else if (reply->type != REDIS_REPLY_STRING && reply->type != REDIS_REPLY_NIL)
	{
		ret = REDIS_OP_ERR_KEY_TYPE;
	}
	else
	{
		ret = REDIS_SUCCESS;
		if (reply->type == REDIS_REPLY_STRING)
		{
			if (reply->str)
			{
				val = strdup(reply->str);
			}
		}

	}
	if (rc != NULL)
		*rc = ret;
	freeReply(reply);
	return val;
}

int redis_return_muti(redis_client_st *redis_client, const char *cmd, char ***elements, redis_return_st *rc)
{
	if (redis_client == NULL || cmd == NULL || elements == NULL)
	{
		if (rc)
		{
			*rc = REDIS_FAILURE;
		}
		return -1;
	}
	int len = 0;
	char **rt = NULL;
	;
	redis_return_st ret;
	redis_reply_st *reply = NULL;
	reply = redis_query(redis_client, cmd);
	if (reply == NULL)
	{
		ret = REDIS_CONNECTION_FAILURE;
	}
	else if (reply->type == REDIS_REPLY_ARRAY)
	{
		ret = REDIS_SUCCESS;
		len = reply->elements;
		int i;
		if (len > 0)
		{
			rt = calloc(len, sizeof(char *));

		}
		for (i = 0; i < len; i++)
		{
			rt[i] = strdup(reply->element[i]->str);
		}
	}
	else
	{
		ret = REDIS_FAILURE;
	}
	if (rc)
	{
		*rc = ret;
	}
	freeReply(reply);
	*elements = rt;
	return len;
}

redis_return_st redis_return_status(redis_client_st *redis_client, const char *cmd)
{
	if (redis_client == NULL || cmd == NULL)
	{
		return REDIS_FAILURE;
	}
	redis_return_st rc = REDIS_FAILURE;
	redis_reply_st *reply = NULL;
	reply = redis_query(redis_client, cmd);
	if (reply == NULL)
	{
		rc = REDIS_CONNECTION_FAILURE;
	}
	else if (reply->type != REDIS_REPLY_STATUS)
	{
		rc = REDIS_FAILURE;
	}
	else
	{
		if (reply->str)
		{
			if (strcasecmp(reply->str, "OK") == 0)
			{
				rc = REDIS_SUCCESS;
			}
		}
	}
	freeReply(reply);
	return rc;
}

static redis_return_st redis_key_action_integer(redis_client_st *redis_client, const char *cmd, const char *key)
{
	redis_return_st rc = REDIS_FAILURE;
	redis_reply_st *reply = NULL;
	reply = redis_query(redis_client, "%s %s", cmd, key);
	if (reply == NULL)
	{
		rc = REDIS_CONNECTION_FAILURE;
	}
	else
	{
		switch (reply->type)
		{
		case REDIS_REPLY_INTEGER:
			rc = REDIS_SUCCESS;
			break;
		default:
			rc = REDIS_OP_ERR_KEY_TYPE;
			break;
		}
		freeReply(reply);
	}

	return rc;
}

static redis_return_st redis_key_val_action_integer(redis_client_st *redis_client, const char*cmd, const char *key,
		const char *value)
{
	redis_return_st rc = REDIS_FAILURE;
	redis_reply_st *reply = NULL;
	reply = redis_query(redis_client, "%s %s %s", cmd, key, value);
	if (reply == NULL)
	{
		rc = REDIS_CONNECTION_FAILURE;
	}
	else
	{
		switch (reply->type)
		{
		case REDIS_REPLY_INTEGER:
			rc = REDIS_SUCCESS;
			break;
		default:
			rc = REDIS_OP_ERR_KEY_TYPE;
			break;
		}
		freeReply(reply);
	}

	return rc;
}

redis_return_st redis_sadd(redis_client_st *redis_client, const char *key, const char *value)
{
	redis_return_st rc = REDIS_FAILURE;
	rc = redis_key_val_action_integer(redis_client, "sadd", key, value);
	return rc;
}

redis_return_st redis_srem(redis_client_st *redis_client, const char *key, const char *value)
{
	redis_return_st rc = REDIS_FAILURE;
	rc = redis_key_val_action_integer(redis_client, "srem", key, value);
	return rc;
}

redis_return_st redis_del(redis_client_st *redis_client, const char *key)
{
	redis_return_st rc = REDIS_FAILURE;
	rc = redis_key_action_integer(redis_client, "del", key);
	return rc;
}


int redis_sismember(redis_client_st *redis_client, const char *key,
    const char *value, redis_return_st *rc)
{
    if(redis_client == NULL || key == NULL || value == NULL)
    {
        if(rc)
            *rc = REDIS_FAILURE;
        return 0;
    }
    int rt = 0;
    char *cmd = generate_key("sismember  %s %s", key, value);
    rt = redis_return_integer(redis_client, cmd,  rc);
    free_and_nil(cmd);
    return rt;
}

int redis_smembers(redis_client_st *redis_client, const char *key, char ***elements, redis_return_st *rc)
{
    if(redis_client == NULL || key == NULL || elements == NULL )
    {
        if(rc)
            *rc = REDIS_FAILURE;
        return -1;        
    }
    int rt = 0;
    char *cmd = generate_key("SMEMBERS %s", key);
    rt = redis_return_muti(redis_client, cmd, elements, rc);
    free_and_nil(cmd);
    return rt;
}


    
