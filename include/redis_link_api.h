/*
 * redis_link_api.h
 *
 *  Created on: 2011-4-1
 *      Author: saint
 */

#ifndef REDIS_LINK_API_H_
#define REDIS_LINK_API_H_
#include "redis_client.h"

/**
 * 是否在阻止列表里
 * params:
 *   @redis_client: redis客户端的数据结构
 *   @jid:
 *   @conn_jid: 待检测是否在jid的blocks的list中。
 *   @rc return REDIS_SUCCESS为正确，其他的为错误。
  * return 0非阻止，1阻止
 **/
int redis_is_in_blocks(redis_client_st *redis_client,  char *jid,  char *conn_jid, redis_return_st *rc);

/**
 * 添加双向链接
 * params:
 *   @redis_client: redis客户端的数据结构
 *   @jid:
 *   @conn_jid:
 *   return REDIS_SUCCESS为正确，其他的为错误。
 **/
redis_return_st redis_add_conns(redis_client_st *redis_client, const char *jid, const char *conn_jid);

/**
 * 添加正向链接
 * params:
 *   @redis_client: redis客户端的数据结构
 *   @jid:
 *   @conn_jid:
 *   return REDIS_SUCCESS为正确，其他的为错误。
 **/
redis_return_st redis_add_following(redis_client_st *redis_client, const char *jid, const char*conn_jid);

/**
 * 删除双向链接
 * params:
 *   @redis_client: redis客户端的数据结构
 *   @jid:
 *   @conn_jid:
 *   return REDIS_SUCCESS为正确，其他的为错误。
 **/
redis_return_st redis_del_conns(redis_client_st *redis_client, const char *jid, const char *conn_jid);



/**
 * 删除单项正向链接
 * params:
 *   @redis_client: redis客户端的数据结构
 *   @jid:
 *   @conn_jid:
 *   return REDIS_SUCCESS为正确，其他的为错误。
 **/
redis_return_st redis_del_followings(redis_client_st *redis_client, const char *jid, const char *conn_jid);


/**
 * 删除单项反向链接
 * params:
 *   @redis_client: redis客户端的数据结构
 *   @jid:
 *   @conn_jid:
 *   return REDIS_SUCCESS为正确，其他的为错误。
 **/
redis_return_st redis_del_followers(redis_client_st *redis_client, const char *jid, const char *conn_jid);
/**
 * 添加阻止列表
 * params:
 *   @redis_client: redis客户端的数据结构
 *   @jid:
 *   @conn_jid:
 *   return REDIS_SUCCESS为正确，其他的为错误。
 **/
redis_return_st redis_add_block(redis_client_st *redis_client, const char *jid, const char *conn_jid);

/**
 * 删除阻止列表
 * params:
 *   @redis_client: redis客户端的数据结构
 *   @jid:
 *   @conn_jid:
 *   return REDIS_SUCCESS为正确，其他的为错误。
 **/
redis_return_st redis_del_block(redis_client_st *redis_client, const char *jid, const char *conn_jid);

/**
 * 删除阻止列表
 * params:
 *   @redis_client: redis客户端的数据结构
 *   @jid:
 *   @conn_jid:
 *   @guids return;
 *   @len return
 *   return REDIS_SUCCESS为正确，其他的为错误。
 **/
redis_return_st redis_get_guids(redis_client_st *redis_client, const char *jid, const char *conn_jid,
		char ***guids, int *len);

/**
 * 向jid的分组group中添加connJid
 * params:
 *   @redis_client: redis客户端的数据结构
 *   @jid:
 *   @conn_jid:
 *   @group ;
 *   return REDIS_SUCCESS为正确，其他的为错误。
 **/
redis_return_st redis_add_jid_to_group(redis_client_st *redis_client, const char *jid,
		const char*conn_jid, const char *group);

/**
 * 删除jid的某个分组
 * params:
 *   @redis_client: redis客户端的数据结构
 *   @jid:
 *   @group ;
 *   return REDIS_SUCCESS为正确，其他的为错误。
 **/
redis_return_st redis_del_group(redis_client_st *redis_client, const char *jid, const char *group);

/**
 * 从jid的分组group中删除connJid
 * params:
 *   @redis_client: redis客户端的数据结构
 *   @jid:
 *   @conn_jid:
 *   @group ;
 *   return REDIS_SUCCESS为正确，其他的为错误。
 **/
redis_return_st redis_del_jid_from_group(redis_client_st *redis_client, const char *jid,
		const char*conn_jid, const char *group);



char *jid_blocks_key(char *jid);

#endif /* REDIS_LINK_API_H_ */
