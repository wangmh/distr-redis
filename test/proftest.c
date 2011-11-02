/*
 * proftest.c
 *
 *  Created on: 2011-4-1
 *      Author: saint
 */

#include "../redis_client.h"
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <assert.h>

long timer(int reset)
{
	static long start = 0;
	struct timeval tv;

	gettimeofday(&tv, NULL);

	/* return timediff */
	if (!reset)
	{
		long stop = ((long) tv.tv_sec) * 1000 + tv.tv_usec / 1000;
		return (stop - start);
	}

	/* reset timer */
	start = ((long) tv.tv_sec) * 1000 + tv.tv_usec / 1000;

	return 0;
}

int main(int argc, char **argv)
{
	char c;
	redis_client_st *redis_client;
	char config_path[1024];
	int count = 10000;
	if (argc <= 2)
	{
		printf("usage: ./%s -f xxx.conf", "proftest");
		return 1;
	}
	memset(config_path, 0, sizeof(config_path));
	while (-1 != (c = getopt(argc, argv, "f:m:h")))
	{
		switch (c)
		{
		case 'f':
			snprintf(config_path, sizeof(config_path), "%s", optarg);
			break;
		case 'm':
			count = atoi(optarg);
			break;
		case '?':
		case 'h':
			printf("usage: ./%s -f xxx.conf -m 10000", "proftest");
			break;
		}

	}
<<<<<<< HEAD
=======
	redis_reply_st *reply = NULL;
>>>>>>> af63872fa2d847e7929e85914300cf829b56a6c7
	redis_client = redis_client_init(config_path);

	int i;
	long t;
	int num = count;
	printf("Sending %d 'set' commands ...\n", num);
	timer(1);
	for (i = 0; i < num; i++)
	{
		if (redis_query(redis_client, "set wangminghua@gozap.com hello") == NULL)
			printf("get returned error\n");
	}
	t = timer(0);
	printf("done! Took %.3f seconds, that is %ld commands/second\n", ((float) t) / 1000, (num
			* 1000) / t);
	printf("Sending %d 'get' commands ...\n", num);
	timer(1);
	for (i = 0; i < num; i++)
	{
		if (redis_query(redis_client, "get wangminghua@gozap.com") == NULL)
			printf("get returned error\n");
	}
	t = timer(0);
	printf("done! Took %.3f seconds, that is %ld commands/second\n", ((float) t) / 1000, (num
			* 1000) / t);
	return 0;

}
