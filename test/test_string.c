/*
 * test_string.c
 *
 *  Created on: 2011-4-29
 *      Author: saint
 */

/*
 * test_list.c
 *
 *  Created on: 2011-4-27
 *      Author: saint
 */

#include "../redis_client.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <assert.h>

#define PROGRAME "test_string"
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

	char *key = "{wangminghua@gozap.com}_test_str";

	redis_client = redis_client_init(config_path);
	if (NULL == redis_client)
		printf("error\n");

	redis_return_st rc = redis_set(redis_client, key, "hello");
	assert(rc == REDIS_SUCCESS);
	char *rt = redis_get(redis_client, key, &rc);
	assert(rc == REDIS_SUCCESS);
	assert(strcmp(rt, "hello") == 0);
	redis_client_free(redis_client);
	return 1;

}

