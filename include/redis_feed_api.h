/*
 * redis_feed_api.h
 *
 *  Created on: 2011-4-1
 *      Author: saint
 */

#ifndef REDIS_FEED_API_H_
#define REDIS_FEED_API_H_
#include "redis_client.h"



/**
 * srcJid���ý�ֹ����feed������
 * params:
 *   @redis_client:
 *   @srcJid: ���ͷ�
 *   @feeds: ��ֹ����feed���б�
 *   @argc: feeds������
 * return REDIS_SUCCESS or other
 **/
redis_return_st redis_set_send_forFeedtype(redis_client_st *redis_client, const char *srcJid, char *feeds[], int argc);

/**
 * srcJid���feed�����͵ķ��ͽ�ֹ
 * params:
 *   @redis_client:
 *   @srcJid: ���ͷ�
 *   @feed: ��ӵĽ�ֹ����feed������
 * return REDIS_SUCCESS or other
 **/
redis_return_st redis_add_send_forFeedtype(redis_client_st *redis_client,  const char *srcJid,  const char *feed);


/**
 * srcJidȡ���˶�feed���͵ķ��ͽ�ֹ
 * params:
 *   @redis_client:
 *   @srcJid: ���ͷ�
 *   @feed: ɾ��Ľ�ֹ����feed������
 * return REDIS_SUCCESS or other
 **/
 redis_return_st redis_rem_send_forFeedtype(redis_client_st *redis_client, const char *srcJid,  const char *feed);



/**
 * ���ý�ֹ���͸�ĳЩjid
 * params:
 *   redis_client:
 *   jid: ���ͷ�
 *   jids: ��ֹ���͵�jids
 *  argc: jids������
 * return REDIS_SUCCESS or other
 **/
redis_return_st redis_set_send_forJid(redis_client_st *redis_client, const char *srcJid, char *jids[], int argc);


/**
 * ��ӽ�ֹ���͵�jid������
 * params:
 *   @redis_client:
 *   @srcJid: ���ͷ�
 *   @jid: ��ֹ���ʹ�jid
 * return REDIS_SUCCESS or other
 **/
redis_return_st redis_add_send_forJid(redis_client_st *redis_client, const char *srcJid, const char *jid);


/**
 * ȡ���ֹ���͵�jid������
 * params:
 *   @redis_client:
 *   @srcJid: ���ͷ�
 *   @jid: ��ֹ���͵�jid
 * return REDIS_SUCCESS or other
 **/
redis_return_st redis_rem_send_forJid(redis_client_st *redis_client, const char *srcJid, const char *jid);


/**
 * ��ֹĳЩfeed���͵�ĳЩjid
 * params:
 *   @redis_client:
 *   @srcJid: ���ͷ�
 *   @feeds: feeds���б�
 *   @feed_argc����ֹ���͵�feed������
 *   @jids����ֹ���͵�jid���б�
 *	 @jid_argc: ��ֹ���͵�jid������
 * return REDIS_SUCCESS or other
 **/
redis_return_st redis_set_send_forFeedtypeJid(redis_client_st *redis_client, const char *srcJid, char *feeds[]
		, int feed_argc, char *jids[], int jid_argc);


/**
 * ��ӽ�ֹsrcJid��feed���͵�jid
 * params:
 *   @redis_client:
 *   @srcJid: ���ͷ�
 *   @feed: feed������
 *   @jid: ��feed��ֹ���͵�jid
 * return REDIS_SUCCESS or other
 **/
redis_return_st redis_add_send_forFeedtypeJid(redis_client_st *redis_client, const char *srcJid, const char *feed, const char *jid);


/**
 * ɾ���ֹsrcJid��feed���͵�jid
 * params:
 *   @redis_client:
 *   @srcJid: ���ͷ�
 *   @feed: feed������
 *   @jid: ��feed��ֹ���͵�jid
 * return REDIS_SUCCESS or other
 **/
redis_return_st redis_rem_send_forFeedtypeJid(redis_client_st *redis_client, const char *srcJid, const char *feed, const char *jid);


/**
 * ���ý�ֹ����feeds
 * params:
 *   @redis_client:
 *   @srcJid: ���ͷ�
 *   @feeds ��ֹ���յ�feeds�����͵��б?
 *   @argc: feeds������
 * return REDIS_SUCCESS or other
 **/
redis_return_st redis_set_receive_forFeedtype(redis_client_st *redis_client, const char *srcJid, char *feeds[], int argc);



/**
 * ��ӽ�ֹ����feed������
 * params:
 *   @redis_client:
 *   @srcJid: ���ͷ�
 *   @feed ��ֹ���յ�feeds������
 * return REDIS_SUCCESS or other
 **/
redis_return_st redis_add_receive_forFeedtype(redis_client_st *redis_client, const char *srcJid,  const char *feed);

/**
 * ɾ���ֹ����feed������
 * params:
 *   @redis_client:
 *   @srcJid: ���ͷ�
 *   @feed ��ֹ���յ�feeds������
 * return REDIS_SUCCESS or other
 **/
redis_return_st redis_rem_receive_forFeedtype(redis_client_st *redis_client, const char *srcJid,  const char *feed);


/**
 * ���ý�ֹ����ĳЩjid��feed
 * params:
 *   @redis_client:
 *   @srcJid: ���ͷ�
 *   @jids: ��ֹ����jids
 *  @argc: jid������
 * return REDIS_SUCCESS or other
 **/
redis_return_st redis_set_receive_forJid(redis_client_st *redis_client, const char *srcJid, char *jids[], int argc);


/**
 * ��ӽ�ֹ����jid������feed
 * params:
 *   @redis_client:
 *   @srcJid: ���ͷ�
 *   @jid ��ֹ���յ�jid
 * return REDIS_SUCCESS or other
 **/
redis_return_st redis_add_receive_forJid(redis_client_st *redis_client, const char *srcJid, const char *jid);

/**
 * ɾ���jid�Ľ��ս�ֹ
 * params:
 *   @redis_client:
 *   @srcJid: ���ͷ�
 *   @jid ��ֹ���յ�jid
 * return REDIS_SUCCESS or other
 **/
redis_return_st redis_rem_receive_forJid(redis_client_st *redis_client, const char *srcJid, const char *jid);


/**
 * ��ֹ��ĳЩjid�н���ĳЩfeed
 * params:
 *   @redis_client:
 *   @srcJid: ���ͷ�
 *   @feeds: feeds������
 *   @feed_argc��feed������
  *  @jids: ��ֹ����jids
 *   @jid_argc: jid������
 * return REDIS_SUCCESS or other
 **/
redis_return_st redis_set_receive_forFeetypeJid(redis_client_st *redis_client, const char *srcJid, char *feeds[], int feed_argc,
		char *jids[], int jid_argc);


/**
 * ��ӽ�ֹsrcJid��jid����ĳfeed����Ϣ
 * params:
 *   @redis_client:
 *   @srcJid: ���ͷ�
 *   @feed: feed������
 *   @jid: ��feed��ֹ���յ�jid
 * return REDIS_SUCCESS or other
 **/
redis_return_st redis_add_receive_forFeedtypeJid(redis_client_st *redis_client, const char *srcJid, const char *feed, const char *jid);

/**
 * ɾ��srcJid��jid����ĳfeed��Ϣ������
 * params:
 *   @redis_client:
 *   @srcJid: ���ͷ�
 *   @feed: feed������
 *   @jid: ��feed��ֹ���͵�jid
 * return REDIS_SUCCESS or other
 **/
redis_return_st redis_rem_receive_forFeedtypeJid(redis_client_st *redis_client, const char *srcJid, const char *feed, const char *jid);




/**
 * ����feed�Ƿ��Զ�����
 * params:
 *   @redis_client:
 *   @jid: ���õ�jid
 *   @feed_type: feed������
 *   @auto:0 ���Զ��� 1 �Զ�
 * return REDIS_SUCCESS or other
 **/
redis_return_st redis_set_feed_auto(redis_client_st *redis_client, const char *jid,
        const char *feed_type, int is_auto);


/***************Ϊƽ̨���feed������¼������feed�����ͣ�feed��������feed���Զ��������****/
/**
 * ����ƽ̨�����feed��ֻ�Ǵ���һ��feed�����ͣ�feed�����ԣ������Զ�����������Ƿ��Զ���
 *	���͵Ĵ����������������ƣ�����Ҫ�����Ӧ�ĺ�������
 * params:
 *   @redis_client:
 *   @feed:feed_type������
 * return REDIS_SUCCESS or other
 **/
redis_return_st redis_create_gozap_feed(redis_client_st *redis_client, const char *feed_type);


/**
 * ɾ��ƽ̨����feed��ɾ��ƽ̨�����feed������Ҫ��ִ����ص�ɾ��feed���Զ����������
 *	�����Ͷ��е����ã������ڲ�ʵ����
 * params:
 *   @redis_client:
 *   @feed_type:��Ҫɾ���feed_type
 * return REDIS_SUCCESS or other
 **/
redis_return_st redis_del_gozap_feed_type(redis_client_st *redis_client, const char *feed_type);


/**
 * ����feed���Ƿ��Զ�����
 * params:
 *   @redis_client:
 *   @feed_type: feed������
 *   @is_auto: 0 �ֶ� 1�Զ�
 * return REDIS_SUCCESS or other
 **/
redis_return_st redis_set_gozap_feed_auto(redis_client_st *redis_client, const char *feed_type,
        int is_auto);


/**
 * ����feed���Զ��������
 * params:
 *   @redis_client:
 *   @feed_type: feed������
 *   @new_comp: ��feed���Զ�����������б�
 *   @argc:��feed���Զ�����������б�ĸ���
 * return REDIS_SUCCESS or other
 **/
redis_return_st redis_set_gozap_feed_comp(redis_client_st *redis_client, const char *feed_type,
        char *new_comp[],    int argc);


  /**
 * ���gozap ƽ̨feed���Զ��������
 * params:
 *   @redis_client:
 *   @feed_type: feed������
 *   @comp:  �Զ��������
 * return REDIS_SUCCESS or other
 **/
redis_return_st redis_add_gozap_feed_comp(redis_client_st *redis_client, const char *feed_type,
        const char *comp);
  /**
 * ɾ��gozap ƽ̨feed���Զ��������
 * params:
 *   @redis_client:
 *   @feed_type: feed������
 *   @comp:  �Զ��������
 * return REDIS_SUCCESS or other
 **/
redis_return_st redis_rem_gozap_feed_comp(redis_client_st *redis_client, const char *feed_type,
        const char *comp);


/**
 * ����feed�Ľ��մ��������е����
 * params:
 *   @redis_client:
 *   @feed_type: feed������
 *   @new_processor: ���ո�feed�Ķ�������б�
 *   @argc: ���ո�feed�Ķ��е���Ŀ
 * return REDIS_SUCCESS or other
 **/
 redis_return_st redis_set_gozap_feed_process(redis_client_st *redis_client, const char *feed_type,
        char *new_processor[],  int argc);

  /**
 * ���feed�Ľ��մ��������е����
 * params:
 *   @redis_client:
 *   @feed_type: feed������
 *   @new_processor: ���ո�feed�Ķ�������б�
 *   @argc: ���ո�feed�Ķ��е���Ŀ
 * return REDIS_SUCCESS or other
 **/
 redis_return_st redis_add_gozap_feed_process(redis_client_st *redis_client, const char *feed_type,
        const char *processor);


 /**
 * ɾ��gozap ƽ̨feed��ĳ�����մ�����
 * params:
 *   @redis_client:
 *   @feed_type: feed������
 *   @processor:  ���մ������Ķ��е����
 * return REDIS_SUCCESS or other
 **/
 redis_return_st redis_rem_gozap_feed_process(redis_client_st *redis_client, const char *feed_type,
        const char *processor);


/**
 * feed�Ƿ����
 * params:
 *   @redis_client
 *   @feed:
 *   @rc: redis��״̬
 * return REDIS_SUCCESS or other
 **/

int redis_gozap_feed_exist(redis_client_st *redis_client, char *feed, redis_return_st *rc);

/**
 * feed�Ƿ��ڽ�ֹ��feed��
 * params:
     @redis_client
 *   @feed:
 *   @srcJid
 *   @rc: redis��״̬
 * return REDIS_SUCCESS or other
 **/

int redis_is_jid_forFeedtype(redis_client_st *redis_client,char *srcJid,
    char *feed, redis_return_st *rc);

char *redis_gozap_feed_isauto(redis_client_st *redis_client,	char *feed,
		redis_return_st *rc);

char *redis_get_jid_feed_auto(redis_client_st *redis_client, char *srcJid,char *feed_type,
		redis_return_st *rc);

char *jid_send_forFeedtype_key(const char *jid);
char *jid_send_forJid_key(const char *jid);
char *jid_send_forFeedtypeJid_key(const char *jid, const char *feed);
char *jid_feed_auto_key(const char *jid, const char *feed);
char *gozap_feed_auto_key( const char *feed);

char *jid_receive_forFeedtype_key(const char *jid);
char *jid_receive_forJid_key(const char *jid);
char *jid_receive_forFeedtypeJid_key(const char *jid, const char *feed);
char *gozap_feed_processes_key(const char *feed);
char *gozap_feed_comps_key(const char *feed);

int redis_is_in_jid_receive_forFeedtype(redis_client_st *redis_client,char *srcJid,
	    char *feed, redis_return_st *rc);
int redis_is_in_jid_recive_forJId(redis_client_st *redis_client, char *srcJid,
		char *jid, redis_return_st *rc);
int redis_is_in_jid_recive_forFeedtypeJid(redis_client_st *redis_client,char *srcJid,
	    char *feed, char *jid, redis_return_st *rc);

#endif /* REDIS_FEED_API_H_ */

