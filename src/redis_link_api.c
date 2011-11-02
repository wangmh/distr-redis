/*
 * redis_link_api.c
 *
 *  Created on: 2011-4-1
 *      Author: saint
 */
#include <glib.h>
#include "redis_link_api.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#define CONNS "_conns"
#define FOLLOWINGS "_followings"
#define FOLLOWERS "_followers"
#define BLOCKS "_blocks"
#define GROUP "_group"



char *jid_blocks_key(char *jid)
{
	return generate_key("{%s}%s", jid, BLOCKS);
}

int redis_is_in_blocks(redis_client_st *redis_client,  char *jid,
		 char *conn_jid, redis_return_st *rc)
{
	int ret = -1;
	if (redis_client == NULL || jid == NULL || conn_jid == NULL)
	{
		if (rc != NULL)
		{
			*rc = REDIS_FAILURE;
		}
		return 0;
	}
	char *key = jid_blocks_key(jid);
	ret = redis_sismember(redis_client, key, conn_jid, rc);
	free_and_nil(key);
	return ret;
}

//添加双向链接，删除单项链接
redis_return_st redis_add_conns(redis_client_st *redis_client, const char *jid,
		const char *conn_jid)
{
	redis_return_st rc = REDIS_FAILURE;
	char *key = generate_key("{%s}%s", jid, CONNS);
	rc = redis_sadd(redis_client, key, conn_jid);
	free(key);
	key = generate_key("{%s}%s", jid, FOLLOWINGS);
	rc = redis_srem(redis_client, key, conn_jid);
	free(key);

	key = generate_key("{%s}%s", conn_jid, FOLLOWERS);
	rc = redis_srem(redis_client, key, jid);
	free(key);
	return rc;
}

//添加正向链接，同时添加反向链接
redis_return_st redis_add_following(redis_client_st *redis_client, const char *jid,
		const char*conn_jid)
{
	redis_return_st rc = REDIS_FAILURE;
	char *key = generate_key("{%s}%s", jid, FOLLOWINGS);
	rc = redis_sadd(redis_client, key, conn_jid);
	free(key);

	key = generate_key("{%s}%s", conn_jid, FOLLOWERS);
	rc = redis_sadd(redis_client, key, jid);
	free(key);
	return rc;
}

//删除链接
redis_return_st redis_del_conns(redis_client_st *redis_client, const char *jid,
		const char *conn_jid)
{
	redis_return_st rc = REDIS_FAILURE;
	char *key = generate_key("{%s}%s", jid, CONNS);
	rc = redis_srem(redis_client, key, conn_jid);
	free(key);

	return rc;
}


redis_return_st redis_del_followings(redis_client_st *redis_client, const char *jid, const char *conn_jid)
{
		redis_return_st rc = REDIS_FAILURE;
		char *key = generate_key("{%s}%s", jid, FOLLOWINGS);
		rc = redis_srem(redis_client, key, conn_jid);
		free(key);

		return rc;
}


redis_return_st redis_del_followers(redis_client_st *redis_client, const char *jid, const char *conn_jid)
{
		redis_return_st rc = REDIS_FAILURE;
		char *key = generate_key("{%s}%s", jid, FOLLOWERS);
		rc = redis_srem(redis_client, key, conn_jid);
		free(key);

		return rc;
}
redis_return_st redis_add_block(redis_client_st *redis_client, const char *jid,
		const char *conn_jid)
{
	redis_return_st rc = REDIS_FAILURE;
	char *key = generate_key("{%s}%s", jid, BLOCKS);
	rc = redis_sadd(redis_client, key, conn_jid);
	free(key);
	return rc;
}

redis_return_st redis_del_block(redis_client_st *redis_client, const char *jid,
		const char *conn_jid)
{
	redis_return_st rc = REDIS_FAILURE;
	char *key = generate_key("{%s}%s", jid, BLOCKS);
	rc = redis_srem(redis_client, key, conn_jid);
	free(key);
	return rc;
}

static char *get_guid_from_str(char *str)
{
	if(str == NULL)
		return NULL;
	char *p = str;
	while(*p != '\0')
	{
		if(*p == '/')
		{
			break;
		}
		p++;
	}
	*p = '\0';
	char *ret = strdup(str);
	*p = '/';
	return ret;

}

static int get_guids_from_arr(char **ret, int length, char ***guids)
{
	if (ret == NULL) return 0;
	int i = -1, j = -1, k = -1;
	char **ret_guids;
	ret_guids = (char **)calloc(sizeof(char *), length);
	for(i = 0, j = 0; i < length; i ++)
	{
		for(k = 0; k < j; k ++)
		{
			if(strncmp(ret_guids[k], ret[i], strlen(ret_guids[k])) == 0)
			{
				break;
			}
		}
		if(k < j)
			continue;
		ret_guids[j++] = get_guid_from_str(ret[i]);
	}
	*guids = ret_guids;
	return j;
}

//获取guid/phnum
redis_return_st redis_get_guids(redis_client_st *redis_client, const char *jid,
		const char *conn_jid, char ***guids, int *len)
{
	char **rets = NULL;
	redis_return_st rc = REDIS_FAILURE;
	redis_reply_st *reply = NULL;
	int index;
	char *key = generate_key("{%s}_%s_%s", jid, "following", conn_jid);
	reply = redis_query(redis_client, "smembers %s", key);
	free(key);
	if (reply == NULL)
		return REDIS_CONNECTION_FAILURE;
	else if (reply->type != REDIS_REPLY_ARRAY)
		rc = REDIS_OP_ERR_KEY_TYPE;
	else
	{
		rc = REDIS_SUCCESS;
		*len = reply->elements;
		if (*len == 0)
		{
			*guids = rets;
		}
		else
		{
			rets = (char **) calloc(sizeof(char *), *len);
			for (index = 0; index < *len; index++)
			{
				if (reply->element[index]->str)
					rets[index] = reply->element[index]->str;
			}
			*len = get_guids_from_arr(rets, reply->elements, guids);
			free(rets);
		}

	}
	freeReply(reply);
	return rc;
}

//分組管理
redis_return_st redis_add_jid_to_group(redis_client_st *redis_client,
		const char *jid, const char*conn_jid, const char *group)
{
	redis_return_st rc = REDIS_FAILURE;
	char *key = generate_key("{%s}%s_%s", jid, GROUP, group);
	rc = redis_sadd(redis_client, key, conn_jid);
	free(key);
	return rc;
}
redis_return_st redis_del_group(redis_client_st *redis_client, const char *jid,
		const char *group)
{
	redis_return_st rc = REDIS_FAILURE;
	char *key = generate_key("{%s}%s_%s", jid, GROUP, group);
	rc = redis_del(redis_client, key);
	free(key);
	return rc;
}

redis_return_st redis_del_jid_from_group(redis_client_st *redis_client,
		const char *jid, const char*conn_jid, const char *group)
{
	redis_return_st rc = REDIS_FAILURE;
	char *key = generate_key("{%s}%s_%s", jid, GROUP, group);
	rc = redis_srem(redis_client, key, conn_jid);
	free(key);
	return rc;
}

