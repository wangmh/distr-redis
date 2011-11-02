/*
 * redis_feed_api.c
 *
 *  Created on: 2011-4-1
 *      Author: saint
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "redis_feed_api.h"

#define FORBIDDEN_FEEDS "_forbidden_feeds"
#define FORBIDDEN_JIDS "_forbidden_jids"
#define FORBIDDEN "_forbidden_"
#define GOZAP_FEEDS "gozap_feeds"
#define SEND "_send"
#define receive "_receive"
#define COMPS "comps"
#define PROCESSES "processes"

typedef enum{
    ADD, REMOVE
}fobidden_op_st;

static redis_return_st do_key_value_by_op(redis_client_st *redis_client,
		const char *key, const char *val, fobidden_op_st op) {

	redis_return_st rc = REDIS_FAILURE;
	switch (op) {
	case ADD:
		rc = redis_sadd(redis_client, key, val);
		break;
	case REMOVE:
	default:
		rc = redis_srem(redis_client, key, val);
		break;
	}
	return rc;
}


char *jid_send_forFeedtype_key(const char *jid)
{
    return generate_key("{%s}%s%s", jid, SEND, FORBIDDEN_FEEDS);
}

char *jid_send_forJid_key(const char *jid)
{
    return generate_key("{%s}%s%s", jid, SEND, FORBIDDEN_JIDS);
}

char *jid_send_forFeedtypeJid_key(const char *jid, const char *feed)
{
    return generate_key("{%s}%s%s%s",jid, SEND, FORBIDDEN, feed);
}
char *jid_feed_auto_key(const char *jid, const char *feed)
{
    return generate_key("{%s}_%s_%s", jid, feed, "auto");
}

char *gozap_feed_auto_key( const char *feed)
{
	return generate_key("%s_%s_%s", "gozap", feed, "auto");
}

char *jid_receive_forFeedtype_key(const char *jid)
{
    return generate_key("{%s}%s%s", jid, receive, FORBIDDEN_FEEDS);
}


char *jid_receive_forJid_key(const char *jid)
{
    return generate_key("{%s}%s%s", jid, receive, FORBIDDEN_JIDS);
}

char *jid_receive_forFeedtypeJid_key(const char *jid, const char *feed)
{
    return generate_key("{%s}%s%s%s",jid, receive, FORBIDDEN, feed);
}


char *gozap_feed_comps_key(const char *feed)
{
    return generate_key("{%s}_%s", feed, COMPS );
}

char *gozap_feed_processes_key(const char *feed)
{
    return generate_key("{%s}_%s", feed, PROCESSES);
}

/******************************************send********************************/
redis_return_st redis_set_send_forFeedtype(redis_client_st *redis_client, const char *srcJid,
    char *feeds[], int argc)
{
   if(redis_client == NULL || srcJid == NULL || feeds == NULL || argc < 0)
   {
        return REDIS_FAILURE;
   }
   redis_return_st rc;
   char *key = jid_send_forFeedtype_key(srcJid);
   if (REDIS_SUCCESS != (rc = redis_del(redis_client,key)))
   {
        free_and_nil(key);
        return rc;
   }
   int i = 0;
   for(i = 0; i < argc; i ++)
   {
        if(REDIS_SUCCESS != (rc = redis_sadd(redis_client, key, feeds[i])))
        {
            break;
        }
   }
   free_and_nil(key);
   return rc;
}


redis_return_st redis_set_send_forJid(redis_client_st *redis_client, const char *srcJid,
    char *jids[], int argc)
{
    if(redis_client == NULL || srcJid == NULL || jids == NULL || argc < 0)
    {
        return REDIS_FAILURE;
    }
    redis_return_st rc;
    char *key = jid_send_forJid_key(srcJid);
    if(REDIS_SUCCESS != (rc = redis_del(redis_client, key)))
    {
        free_and_nil(key);
        return rc;
    }
    int i = 0;
    for(i = 0; i < argc; i ++)
    {
        if(REDIS_SUCCESS != (rc = redis_sadd(redis_client, key, jids[i])))
        {
            break;
        }
    }
    free_and_nil(key);
    return rc;
}

redis_return_st redis_set_send_forFeedtypeJid(redis_client_st *redis_client,
    const char *srcJid, char *feeds[], int feed_argc, char *jids[], int jid_argc)
{
    if(redis_client == NULL || srcJid == NULL || feeds == NULL || feed_argc < 0 || jids == NULL || jid_argc < 0)
    {
        return REDIS_FAILURE;
    }
    redis_return_st rc = REDIS_FAILURE;
    int i, j;
    char *key = NULL;
    for(i = 0; i < feed_argc; i ++)
    {
        key = jid_send_forFeedtypeJid_key(srcJid, feeds[i]);
        if(REDIS_SUCCESS != (rc  = redis_del(redis_client, key)))
        {
            free_and_nil(key);
            return rc;
        }
        for(j = 0; j < jid_argc; j ++)
        {
            if(REDIS_SUCCESS != (rc = redis_sadd(redis_client, key, jids[j])))
            {
                free_and_nil(key);
                return rc;
            }
        }
        free_and_nil(key);
    }
    return rc;
}

redis_return_st redis_set_feed_auto(redis_client_st *redis_client, const char *jid,
    const char *feed, int is_auto)
{
    char *auto_yes = "1";
    char *auto_no = "0";
    char *auto_str = NULL;
    if(redis_client == NULL || jid == NULL || feed == NULL)
    {
        return REDIS_FAILURE;
    }
    redis_return_st rc = REDIS_FAILURE;
    char *key = jid_feed_auto_key(jid,feed);
    auto_str = (is_auto == 1 ? auto_yes : auto_no);
    rc = redis_set(redis_client, key, auto_str);
    free_and_nil(key);
    return rc;
}



redis_return_st redis_add_send_forFeedtype(redis_client_st *redis_client, const char *srcJid,
    const char *feed)
{
    if(redis_client == NULL || srcJid == NULL || feed == NULL)
    {
        return REDIS_FAILURE;
    }
    redis_return_st rc = REDIS_FAILURE;
    char *key = jid_send_forFeedtype_key(srcJid);
    rc = do_key_value_by_op(redis_client, key, feed, ADD);
    free_and_nil(key);
    return rc;
}

redis_return_st redis_add_send_forJid(redis_client_st * redis_client,const char * srcJid,
    const char * jid)
{
    if(redis_client == NULL || srcJid == NULL || jid == NULL)
    {
        return REDIS_FAILURE;
    }
    redis_return_st rc = REDIS_FAILURE;
    char *key = jid_send_forJid_key(srcJid);
    rc = do_key_value_by_op(redis_client,key, jid, ADD);
    free_and_nil(key);
    return rc;
}


redis_return_st redis_add_send_forFeedtypeJid(redis_client_st * redis_client,
    const char * srcJid,const char * feed,const char * jid)
{
    if(redis_client == NULL || srcJid == NULL || feed == NULL || jid == NULL)
    {
        return REDIS_FAILURE;
    }
    redis_return_st rc = REDIS_FAILURE;
    char *key = jid_send_forFeedtypeJid_key(srcJid, feed);
    rc = do_key_value_by_op(redis_client,key, jid,ADD);
    free_and_nil(key);
    return rc;
}

redis_return_st redis_rem_send_forFeedtype(redis_client_st *redis_client,
    const char *srcJid,  const char *feed)
{
    if(redis_client == NULL || srcJid == NULL || feed == NULL)
    {
        return REDIS_FAILURE;
    }
    redis_return_st rc = REDIS_FAILURE;
    char *key = jid_send_forFeedtype_key(srcJid);
    rc = do_key_value_by_op(redis_client, key, feed, REMOVE);
    free_and_nil(key);
    return rc;
}


redis_return_st redis_rem_send_forJid(redis_client_st *redis_client, const char *srcJid,
    const char *jid)
{
    if(redis_client == NULL || srcJid == NULL || jid == NULL)
    {
        return REDIS_FAILURE;
    }
    redis_return_st rc = REDIS_FAILURE;
    char *key = jid_send_forJid_key(srcJid);
    rc = do_key_value_by_op(redis_client,key, jid, REMOVE);
    free_and_nil(key);
    return rc;
}


redis_return_st redis_rem_send_forFeedtypeJid(redis_client_st *redis_client,
    const char *srcJid, const char *feed, const char *jid)
{
    if(redis_client == NULL || srcJid == NULL || feed == NULL || jid == NULL)
    {
        return REDIS_FAILURE;
    }
    redis_return_st rc = REDIS_FAILURE;
    char *key = jid_send_forFeedtypeJid_key(srcJid, feed);
    rc = do_key_value_by_op(redis_client,key, jid, REMOVE);
    free_and_nil(key);
    return rc;
}

/******************************************receive********************************/

redis_return_st redis_set_receive_forFeedtype(redis_client_st * redis_client,
    const char * srcJid,char * feeds [ ],int argc)
{
    if(redis_client == NULL || srcJid == NULL || feeds == NULL || argc < 0)
    {
        return REDIS_FAILURE;
    }
    redis_return_st rc;
    char *key = jid_receive_forFeedtype_key(srcJid);
    if (REDIS_SUCCESS != (rc = redis_del(redis_client,key)))
    {
       free_and_nil(key);
       return rc;
    }
    int i = 0;
    for(i = 0; i < argc; i ++)
    {
        if(REDIS_SUCCESS != (rc = redis_sadd(redis_client, key, feeds[i])))
        {
            break;
        }
    }
    free_and_nil(key);
    return rc;
}


redis_return_st redis_set_receive_forJid(redis_client_st *redis_client,
    const char * srcJid,char * jids [ ],int argc)
{
    if(redis_client == NULL || srcJid == NULL || jids == NULL || argc < 0)
    {
        return REDIS_FAILURE;
    }
    redis_return_st rc;
    char *key = jid_receive_forJid_key(srcJid);
    if(REDIS_SUCCESS != (rc = redis_del(redis_client, key)))
    {
        free_and_nil(key);
        return rc;
    }
    int i = 0;
    for(i = 0; i < argc; i ++)
    {
        if(REDIS_SUCCESS != (rc = redis_sadd(redis_client, key, jids[i])))
        {
            break;
        }
    }
    free_and_nil(key);
    return rc;
}

redis_return_st redis_set_receive_forFeedtypeJid(redis_client_st *redis_client,
    const char *srcJid, char *feeds[], int feed_argc, char *jids[], int jid_argc)
{
    if(redis_client == NULL || srcJid == NULL || feeds == NULL || feed_argc < 0 || jids == NULL || jid_argc < 0)
    {
        return REDIS_FAILURE;
    }
    redis_return_st rc;
    int i, j;
    char *key = NULL;
    for(i = 0; i < feed_argc; i ++)
    {
        key = jid_receive_forFeedtypeJid_key(srcJid, feeds[i]);
        if(REDIS_SUCCESS != (rc  = redis_del(redis_client, key)))
        {
            free_and_nil(key);
            return rc;
        }
        for(j = 0; j < jid_argc; j ++)
        {
            if(REDIS_SUCCESS != (rc = redis_sadd(redis_client, key, jids[j])))
            {
                free_and_nil(key);
                return rc;
            }
        }
        free_and_nil(key);
    }
    return rc;
}

redis_return_st redis_add_receive_forFeedtype(redis_client_st *redis_client, const char *srcJid,
    const char *feed)
{
    if(redis_client == NULL || srcJid == NULL || feed == NULL)
    {
        return REDIS_FAILURE;
    }
    redis_return_st rc = REDIS_FAILURE;
    char *key = jid_receive_forFeedtype_key(srcJid);
    rc = do_key_value_by_op(redis_client, key, feed, ADD);
    free_and_nil(key);
    return rc;
}

redis_return_st redis_add_receive_forJid(redis_client_st * redis_client,const char * srcJid,
    const char * jid)
{
    if(redis_client == NULL || srcJid == NULL || jid == NULL)
    {
        return REDIS_FAILURE;
    }
    redis_return_st rc = REDIS_FAILURE;
    char *key = jid_receive_forJid_key(srcJid);
    rc = do_key_value_by_op(redis_client,key, jid, ADD);
    free_and_nil(key);
    return rc;
}


redis_return_st redis_add_receive_forFeedtypeJid(redis_client_st * redis_client,
    const char * srcJid,const char * feed,const char * jid)
{
    if(redis_client == NULL || srcJid == NULL || feed == NULL || jid == NULL)
    {
        return REDIS_FAILURE;
    }
    redis_return_st rc = REDIS_FAILURE;
    char *key = jid_receive_forFeedtypeJid_key(srcJid, feed);
    rc = do_key_value_by_op(redis_client,key, jid,ADD);
    free_and_nil(key);
    return rc;
}

redis_return_st redis_rem_receive_forFeedtype(redis_client_st *redis_client,
    const char *srcJid,  const char *feed)
{
    if(redis_client == NULL || srcJid == NULL || feed == NULL)
    {
        return REDIS_FAILURE;
    }
    redis_return_st rc = REDIS_FAILURE;
    char *key = jid_receive_forFeedtype_key(srcJid);
    rc = do_key_value_by_op(redis_client, key, feed, REMOVE);
    free_and_nil(key);
    return rc;
}


redis_return_st redis_rem_receive_forJid(redis_client_st *redis_client, const char *srcJid,
    const char *jid)
{
    if(redis_client == NULL || srcJid == NULL || jid == NULL)
    {
        return REDIS_FAILURE;
    }
    redis_return_st rc = REDIS_FAILURE;
    char *key = jid_receive_forJid_key(srcJid);
    rc = do_key_value_by_op(redis_client,key, jid, REMOVE);
    free_and_nil(key);
    return rc;
}


redis_return_st redis_rem_receive_forFeedtypeJid(redis_client_st *redis_client,
    const char *srcJid, const char *feed, const char *jid)
{
    if(redis_client == NULL || srcJid == NULL || feed == NULL || jid == NULL)
    {
        return REDIS_FAILURE;
    }
    redis_return_st rc = REDIS_FAILURE;
    char *key = jid_receive_forFeedtypeJid_key(srcJid, feed);
    rc = do_key_value_by_op(redis_client,key, jid, REMOVE);
    free_and_nil(key);
    return rc;
}



/***************为平台添加feed处理的事件。添加feed的类型，feed的描述，feed的自动处理组件****/



redis_return_st redis_create_gozap_feed(redis_client_st *redis_client,
		const char *feed_type) {
	if (redis_client == NULL || feed_type == NULL)
		return REDIS_FAILURE;
	return redis_sadd(redis_client, GOZAP_FEEDS, feed_type);
}


redis_return_st redis_set_gozap_feed_auto(redis_client_st *redis_client, const char *feed_type,
        int is_auto)
{

    char *auto_yes = "1";
    char *auto_no = "0";
    char *auto_str = NULL;
    if(redis_client == NULL  || feed_type == NULL)
    {
        return REDIS_FAILURE;
    }
    redis_return_st rc = REDIS_FAILURE;
    char *key = gozap_feed_auto_key(feed_type);
    auto_str = (is_auto == 1 ? auto_yes : auto_no);
    rc = redis_set(redis_client, key, auto_str);
    free_and_nil(key);
    return rc;

}

redis_return_st redis_set_gozap_feed_comp(redis_client_st *redis_client,
    const char *feed_type,		char *new_comp[],	 int argc)
{
    if(redis_client == NULL || feed_type == NULL || new_comp == NULL || argc < 0)
    {
        return REDIS_FAILURE;
    }
    redis_return_st rc;
    char * key = NULL;
    key = gozap_feed_comps_key(feed_type);
   if(REDIS_SUCCESS != (rc = redis_del(redis_client, key)))
    {
        free_and_nil(key);
        return rc;
    }
    int i = 0;
    for(i = 0; i < argc; i ++)
    {
        if(REDIS_SUCCESS != (rc = redis_sadd(redis_client, key, new_comp[i])))
        {
            break;
        }
    }
    free_and_nil(key);
    return rc;
}



redis_return_st redis_set_gozap_feed_process(redis_client_st *redis_client,
    const char *feed_type,     char *new_processor[],   int argc)
{
    if(redis_client == NULL || new_processor == NULL || feed_type == NULL || argc < 0)
    {
        return REDIS_FAILURE;
    }
    redis_return_st  rc;
    char * key = NULL;
    key = gozap_feed_processes_key(feed_type);
    if(REDIS_SUCCESS != (rc = redis_del(redis_client, key)))
    {
        free_and_nil(key);
        return rc;
    }
    int i = 0;
    for(i = 0; i < argc; i ++)
    {
        if(REDIS_SUCCESS != (rc = redis_sadd(redis_client, key, new_processor[i])))
        {
            break;
        }
    }
    free_and_nil(key);
    return rc;
}


redis_return_st redis_del_gozap_feed_type(redis_client_st *redis_client,
		const char *feed_type) {
	redis_return_st rc = REDIS_FAILURE;
	if (redis_client == NULL || feed_type == NULL)
		return rc;
    char *key = gozap_feed_comps_key(feed_type);
	if (REDIS_SUCCESS != (rc = redis_del(redis_client, key))) {
        free_and_nil(key);
		return rc;
	}
	free_and_nil(key);
    key = gozap_feed_processes_key(feed_type);
	if (REDIS_SUCCESS != (rc = redis_del(redis_client, key))) {
		free_and_nil(key);
		return rc;
	}
	free_and_nil(key);
	if(REDIS_SUCCESS
			!= (rc = redis_srem(redis_client, GOZAP_FEEDS, feed_type))) {
		return rc;
	}
	return rc;
}


redis_return_st redis_add_gozap_feed_process(redis_client_st *redis_client,
    const char *feed_type,     const char *processor)
{
    if(redis_client == NULL || feed_type == NULL || processor == NULL)
    {
        return REDIS_FAILURE;
    }
    char *key = gozap_feed_processes_key(feed_type);
    redis_return_st rc;
    rc = redis_sadd(redis_client, key, processor);
    free_and_nil(key);
    return rc;
}



redis_return_st redis_rem_gozap_feed_process(redis_client_st *redis_client,
    const char *feed_type,     const char *processor)
{
    if(redis_client == NULL || feed_type == NULL || processor == NULL)
    {
        return REDIS_FAILURE;
    }
    char *key = gozap_feed_processes_key(feed_type);
    redis_return_st rc;
    rc = redis_srem(redis_client, key, processor);
    free_and_nil(key);
    return rc;
}

redis_return_st redis_add_gozap_feed_comp(redis_client_st *redis_client,
    const char *feed_type,		const char *comp)
{
    if(redis_client == NULL || feed_type == NULL || comp == NULL)
    {
        return REDIS_FAILURE;
    }
    char *key = gozap_feed_comps_key(feed_type);
    redis_return_st rc;
    rc = redis_sadd(redis_client, key, comp);
    free_and_nil(key);
    return rc;
}

redis_return_st redis_rem_gozap_feed_comp(redis_client_st *redis_client,
    const char *feed_type,		const char *comp)
{
    if(redis_client == NULL || feed_type == NULL || comp == NULL)
    {
        return REDIS_FAILURE;
    }
    char *key = gozap_feed_comps_key(feed_type);
    redis_return_st rc;
    rc = redis_srem(redis_client, key, comp);
    free_and_nil(key);
    return rc;
}


int redis_gozap_feed_exist(redis_client_st *redis_client,
    char *feed, redis_return_st *rc)
{

    return redis_sismember(redis_client, GOZAP_FEEDS,feed, rc);
}



int redis_is_jid_forFeedtype(redis_client_st *redis_client,char *srcJid,
    char *feed, redis_return_st *rc)
{
    int rt = 0;
    char *key = jid_send_forFeedtype_key(srcJid);
    rt = redis_sismember(redis_client, key, feed, rc);
    free_and_nil(key);
    return rt;
}

char *redis_gozap_feed_isauto(redis_client_st *redis_client,	char *feed,
		redis_return_st *rc)
{
	char *is_auto = NULL;
	char *key = gozap_feed_auto_key(feed);
	is_auto = redis_get(redis_client, key, rc);
	free_and_nil(key);
	return is_auto;
}


int redis_is_in_jid_receive_forFeedtype(redis_client_st *redis_client,char *srcJid,
	    char *feed, redis_return_st *rc)
{
	int ret = 0;
	char *key = jid_receive_forFeedtype_key(srcJid);
	ret = redis_sismember(redis_client, key, feed, rc);
	free_and_nil(key);
	return ret;
}

int redis_is_in_jid_recive_forJId(redis_client_st *redis_client, char *srcJid,
		char *jid, redis_return_st *rc)
{
	int ret = 0;
	char *key = jid_receive_forJid_key(srcJid);
	ret = redis_sismember(redis_client, key, jid, rc);
	free_and_nil(key);
	return ret;
}

int redis_is_in_jid_recive_forFeedtypeJid(redis_client_st *redis_client,char *srcJid,
	    char *feed, char *jid, redis_return_st *rc)
{
	int ret = 0;
	char *key = jid_receive_forFeedtypeJid_key(srcJid, feed);
	ret = redis_sismember(redis_client, key, jid, rc);
	return ret;

}

char *redis_get_jid_feed_auto(redis_client_st *redis_client, char *srcJid,char *feed_type,
		redis_return_st *rc)
{
	char *ret = NULL;
	char *key  = jid_feed_auto_key(srcJid, feed_type);
	ret = redis_get(redis_client, key, rc);
	free_and_nil(key);
	return ret;
}


