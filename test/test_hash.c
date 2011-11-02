#include "redis_client.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <assert.h>

#define PROGRAME "task_hash"
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
	redis_return_st rc ;
	char **elements;
	if (NULL == redis_client)
		printf("error\n");
	char *key = "{wangminghua@gozap.com}_hash_test";
	char *field1 = "field1";
	char *field2 = "field2";
	redis_hdel(redis_client, key, field1,&rc);
	int ret =	redis_hdel(redis_client, key, field1,&rc);
	assert(ret == 0);
	ret = redis_hset(redis_client, key, field1, "121", &rc);
	assert(ret == 1);
	redis_hdel(redis_client, key, field2,&rc);
	ret = redis_hincrby(redis_client, key, field2, 1, &rc);
	assert(ret = 1);
	ret = redis_hkeys(redis_client, key, &elements, &rc);
	assert(ret == 2);
	
	
	
	return 1;

}

