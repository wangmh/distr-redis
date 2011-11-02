#include "../redis_client.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <assert.h>

#define PROGRAME "test_ketama"
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
	if (NULL == redis_client)
		printf("error\n");
	char *str = "hello i'm wangmh";

	char*keys[2] =
	{ "{wangminghua@gozap.com}_follows", "wangminghua@gozap.com" };


	reply = redis_query(redis_client, "set %s %s ", "wangminghua@gozap.com", "12");
	freeReply(reply);
	sleep(5);

	reply = redis_query(redis_client, "get %s  ", "wangminghua@gozap.com");
	if (reply)
	{
		printf("reply->str is %s\n", reply->str);
		freeReply(reply);
	}

	int iargc = 2;
	const char *iargv[] =
	{ "get", "wangminghua@gozap.com" };

	reply = redis_query_argv(redis_client, iargc, iargv);
	if (reply)
	{
		printf("reply->str is %s\n", reply->str);
		freeReply(reply);
	}

	redis_client_free(redis_client);
	return 1;

}

