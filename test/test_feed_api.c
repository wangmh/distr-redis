/*
 * test_feed_api.c
 *
 *  Created on: 2011-4-1
 *      Author: saint
 */

#include "redis_feed_api.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <assert.h>

#define PROGRAME "test_feed_api"
int main(int argc, char *argv[])
{
	char c;
	redis_client_st *redis_client;
	char config_path[1024];

	if (argc <= 2)
	{
		printf("usage: ./%s -f xxx.conf", PROGRAME);
		return 1;
	}
	memset(config_path, 0, sizeof(config_path));
	while (-1 != (c = getopt(argc, argv, "f:h")))
	{
		switch (c)
		{
		case 'f':
			snprintf(config_path, sizeof(config_path), "%s", optarg);
			break;
		case '?':
		case 'h':
			printf("usage: ./%s -f xxx.conf", PROGRAME);
			break;
		}

	}
	redis_reply_st *reply = NULL;
	redis_client = redis_client_init(config_path);
	redis_return_st rc;
	printf("test send_forbidden \n");
	rc = redis_add_send_forFeedtype(redis_client, "wangminghua@gozap.com", "sync.gozap.com");
	reply
			= redis_query(redis_client,
					"sismember {wangminghua@gozap.com}_send_forbidden_feeds sync.gozap.com");
	assert(reply->integer == 1);
	freeReply(reply);
	printf("* ");

	rc = redis_rem_send_forFeedtype(redis_client, "wangminghua@gozap.com", "sync.gozap.com");
	reply
			= redis_query(redis_client,
					"sismember {wangminghua@gozap.com}_send_forbidden_feeds sync.gozap.com");
	assert(reply->integer == 0);
	freeReply(reply);

	printf("* ");

	rc = redis_add_send_forJid(redis_client, "wangminghua@gozap.com", "yejianying@gozap.com");
	reply = redis_query(redis_client,
			"sismember {wangminghua@gozap.com}_send_forbidden_jids yejianying@gozap.com");
	assert(reply->integer == 1);
	assert(rc == REDIS_SUCCESS);
	freeReply(reply);
	printf("* ");

	rc = redis_rem_send_forJid(redis_client, "wangminghua@gozap.com", "yejianying@gozap.com");
	reply = redis_query(redis_client,
			"sismember {wangminghua@gozap.com}_send_forbidden_jids yejianying@gozap.com");
	assert(reply->integer == 0);
	assert(rc == REDIS_SUCCESS);
	freeReply(reply);
	printf("* ");

	rc = redis_add_send_forFeedtypeJid(redis_client, "wangminghua@gozap.com", "sync.gozap.com",
			"yexiaowei@gozap.com");
	reply = redis_query(redis_client,
			"sismember {wangminghua@gozap.com}_send_forbidden_sync.gozap.com yexiaowei@gozap.com");
	assert(reply->integer == 1);
	assert(rc == REDIS_SUCCESS);
	freeReply(reply);
	printf("* ");

	rc = redis_rem_send_forFeedtypeJid(redis_client, "wangminghua@gozap.com", "sync.gozap.com",
			"yexiaowei@gozap.com");
	reply = redis_query(redis_client,
			"sismember {wangminghua@gozap.com}_send_forbidden_sync.gozap.com yexiaowei@gozap.com");
	assert(reply->integer == 0);
	assert(rc == REDIS_SUCCESS);
	freeReply(reply);
	printf("* ");

	printf("\n end \n");

	printf("test receive_forbidden \n");
	rc = redis_add_receive_forFeedtype(redis_client, "wangminghua@gozap.com", "sync");
	reply = redis_query(redis_client,
			"sismember {wangminghua@gozap.com}_receive_forbidden_feeds sync");
	assert(reply->integer == 1);
	assert(rc == REDIS_SUCCESS);
	freeReply(reply);
	printf("* ");

	rc = redis_rem_receive_forFeedtype(redis_client, "wangminghua@gozap.com", "sync");
	reply = redis_query(redis_client,
			"sismember {wangminghua@gozap.com}_receive_forbidden_feeds sync");
	assert(reply->integer == 0);
	assert(rc == REDIS_SUCCESS);
	freeReply(reply);
	printf("* ");

	rc = redis_add_receive_forJid(redis_client, "wangminghua@gozap.com", "yejianying@gozap.com");
	reply = redis_query(redis_client,
			"sismember {wangminghua@gozap.com}_receive_forbidden_jids yejianying@gozap.com");
	assert(reply->integer == 1);
	assert(rc == REDIS_SUCCESS);
	freeReply(reply);
	printf("* ");
	rc
			= redis_rem_receive_forJid(redis_client, "wangminghua@gozap.com", "yejianying@gozap.com");
	reply = redis_query(redis_client,
			"sismember {wangminghua@gozap.com}_receive_forbidden_jids yejianying@gozap.com");
	assert(reply->integer == 0);
	assert(rc == REDIS_SUCCESS);
	freeReply(reply);
	printf("* ");

	rc = redis_add_receive_forFeedtypeJid(redis_client, "wangminghua@gozap.com", "sync",
			"yexiaowei@gozap.com");
	reply = redis_query(redis_client,
			"sismember {wangminghua@gozap.com}_receive_forbidden_sync yexiaowei@gozap.com");
	assert(reply->integer == 1);
	assert(rc == REDIS_SUCCESS);
	freeReply(reply);
	printf("* ");

	rc = redis_rem_receive_forFeedtypeJid(redis_client, "wangminghua@gozap.com", "sync",
			"yexiaowei@gozap.com");
	reply = redis_query(redis_client,
			"sismember {wangminghua@gozap.com}_receive_forbidden_sync yexiaowei@gozap.com");
	assert(reply->integer == 0);
	assert(rc == REDIS_SUCCESS);
	freeReply(reply);
	printf("* ");
	printf("\n end \n");

	printf("set auto\n");
	rc = redis_set_feed_auto(redis_client, "wangminghua@gozap.com", "1212",1);
	reply = redis_query(redis_client,
			"get {wangminghua@gozap.com}_1212_auto");
	assert(strcmp(reply->str,"1")==0 );
	assert(rc == REDIS_SUCCESS);
	freeReply(reply);
	printf("* ");

	
    char *feed = "10001";
	rc = redis_create_gozap_feed(redis_client, feed);
	reply = redis_query(redis_client, "sismember gozap_feeds %s", feed);
	assert(reply->integer == 1);
	freeReply(reply);
	printf("* ");
    char *comps[] = {"sync", "link"};
    rc  = redis_set_gozap_feed_comp(redis_client, feed, comps ,2);
  
	reply = redis_query(redis_client, "sismember {%s}_comps sync", feed);
	assert(reply->integer == 1);
	freeReply(reply);

	reply = redis_query(redis_client, "sismember {%s}_comps link", feed);
	assert(reply->integer == 1);
	freeReply(reply);
	printf("* ");

	rc  = redis_add_gozap_feed_process(redis_client, feed, "process2" );
	reply = redis_query(redis_client, "sismember {%s}_processes process2", feed);
	assert(reply->integer == 1);
	freeReply(reply);
	
	assert(rc == REDIS_SUCCESS);
	printf("* ");

	rc = redis_del_gozap_feed_type(redis_client, feed);
	reply = redis_query(redis_client, "sismember gozap_feeds %s", feed);
	assert(reply->integer == 0);
	freeReply(reply);
	printf("* ");

	reply = redis_query(redis_client, "EXISTS {%s}_comps", feed);
	assert(reply->integer == 0);
	freeReply(reply);
	printf("* ");

	reply = redis_query(redis_client, "EXISTS {%s}_processes", feed);
	assert(reply->integer == 0);
	freeReply(reply);
	printf("* ");

	assert(rc == REDIS_SUCCESS);
	printf("* ");


	printf("\nend\n");

	return 1;

}

