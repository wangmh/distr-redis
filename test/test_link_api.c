/*
 * test_link_api.c
 *
 *  Created on: 2011-4-8
 *      Author: saint
 */

/*
 * test_feed_api.c
 *
 *  Created on: 2011-4-1
 *      Author: saint
 */

#include "redis_link_api.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <assert.h>

#define PROGRAME "test_link_api"
int main(int argc, char *argv[]) {
	char c;
	redis_client_st *redis_client;
	char config_path[1024];

	if (argc <= 2) {
		printf("usage: ./%s -f xxx.conf", PROGRAME);
		return 1;
	}
	memset(config_path, 0, sizeof(config_path));
	while (-1 != (c = getopt(argc, argv, "f:h"))) {
		switch (c) {
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
	printf("test add conn\n");

	redis_add_conns(redis_client, "wangminghua@gozap.com", "liule@gozap.com"
		);

	reply = redis_query(redis_client,
			"sismember {wangminghua@gozap.com}_conns liule@gozap.com");
	assert(reply->integer == 1);
	freeReply(reply);
	printf("* ");

	printf("test add following\n");

	redis_add_following(redis_client, "wangminghua@gozap.com", "yexiaowei@gozap.com");

	reply = redis_query(redis_client,
			"sismember {wangminghua@gozap.com}_followings yexiaowei@gozap.com");
	assert(reply->integer == 1);
	freeReply(reply);

	reply
			= redis_query(redis_client,
					"sismember wangminghua@gozap.com_following_yexiaowei@gozap.com  2233");
	assert(reply->integer == 0);
	freeReply(reply);
	printf("* ");




	printf("test del conns\n");

	redis_del_conns(redis_client, "wangminghua@gozap.com", "liule@gozap.com");

	reply = redis_query(redis_client,
			"sismember {wangminghua@gozap.com}_conns liule@gozap.com");
	assert(reply->integer == 0);
	freeReply(reply);

	reply = redis_query(redis_client,
			"sismember wangminghua@gozap.com_followings liule@gozap.com ");
	assert(reply->integer == 0);
	freeReply(reply);

	printf("* ");

	printf("test add_block\n");

	redis_add_block(redis_client, "wangminghua@gozap.com", "liule@gozap.com");

	reply = redis_query(redis_client,
			"sismember {wangminghua@gozap.com}_blocks liule@gozap.com");
	assert(reply->integer == 1);
	freeReply(reply);

	printf("* ");

	printf("test del_block\n");

	redis_del_block(redis_client, "wangminghua@gozap.com", "liule@gozap.com");

	reply = redis_query(redis_client,
			"sismember {wangminghua@gozap.com}_blocks liule@gozap.com");
	assert(reply->integer == 0);
	freeReply(reply);
	printf("* ");

	printf("test get_guids\n");

	char **guid;
	int len;
	redis_get_guids(redis_client, "wangminghua@gozap.com", "liule@gozap.com", &guid,
			&len);
	int i = 0;
	for (i = 0; i < len; i++) {
		printf("%s\n", guid[i]);
	}

	printf("* ");
	printf("test add_jid_to_group\n");

	redis_add_jid_to_group(redis_client, "wangminghua@gozap.com", "liule@gozap.com",
			"1221");

	redis_add_jid_to_group(redis_client, "wangminghua@gozap.com", "yexiaowei@gozap.com",
			"1221");
	reply = redis_query(redis_client,
			"sismember {wangminghua@gozap.com}_group_1221 liule@gozap.com");
	assert(reply->integer == 1);
	freeReply(reply);
	//分組管理
	printf("* ");
	printf("test del_jid_from_group\n");

	redis_del_jid_from_group(redis_client, "wangminghua@gozap.com", "liule@gozap.com",
			"1221");


	reply = redis_query(redis_client,
			"sismember {wangminghua@gozap.com}_group_1221 liule@gozap.com");
	assert(reply->integer == 0);
	freeReply(reply);
	//分組管理
	printf("* ");
	printf("test del group\n");

	redis_del_group(redis_client, "wangminghua@gozap.com",
			"1221");


	reply = redis_query(redis_client,
			"sismember {wangminghua@gozap.com}_group_1221 yexiaowei@gozap.com");
	assert(reply->integer == 0);
	freeReply(reply);
	//分組管理
	printf("* ");




	printf("\nend\n");

	return 1;

}

