/*
 * test_list.c
 *
 *  Created on: 2011-4-27
 *      Author: saint
 */

#include "redis_client.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <assert.h>

#define PROGRAME "test_list"
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

	char *key = "{wangminghua@gozap.com}_list";

	redis_client = redis_client_init(config_path);
	if (NULL == redis_client)
		printf("error\n");

	redis_return_st rc;
	int i = redis_lpush(redis_client, "{wangminghua@gozap.com}_list", "hi", &rc);
	if (rc == REDIS_SUCCESS) {
		printf("lpush return %d\n", i);
	} else {
		printf("error is %d\n", rc);
	}
	i = redis_lpush(redis_client, "{wangminghua@gozap.com}_list", "hi1", &rc);
	if (rc == REDIS_SUCCESS) {
		printf("lpush return %d\n", i);
	} else {
		printf("error is %d\n", rc);
	}

	char *item = redis_lindex(redis_client, "{wangminghua@gozap.com}_list", 0, &rc);
	if (rc == REDIS_SUCCESS) {
		printf("lpush return %s\n", item);
	} else {
		printf("error is %d\n", rc);
	}
	free_and_nil(item);

	int len = redis_llen(redis_client, "{wangminghua@gozap.com}_list", &rc);
	if (rc == REDIS_SUCCESS) {
		printf("llen return %d\n", len);
	} else {
		printf("error is %d\n", rc);
	}

	char **elements = NULL;
	len = redis_lrange(redis_client, key, 0, -1, &elements, &rc);
	if (rc == REDIS_SUCCESS) {
		for (i = 0; i < len; i++) {
			printf("(%d): %s\n", i, elements[i]);
		}
	} else {
		printf("error is %d\n", rc);
	}

	for (i = 0; i < len; i++) {
		free(elements[i]);
	}
	free(elements);

	len = redis_lrem(redis_client, key, -1, "hi", &rc);
	if (rc == REDIS_SUCCESS) {
		printf("lrem return %d\n", len);
	} else {
		printf("error is %d\n", rc);
	}

	len = redis_lpush(redis_client, key, "world", &rc);
	if (rc == REDIS_SUCCESS) {
		printf("redis_lpush return %d\n", len);
	} else {
		printf("error is %d\n", rc);
	}


	rc = redis_lset(redis_client, key, 0, "hello");
	if (rc == REDIS_SUCCESS) {
		printf("redis_lset success\n");
	} else {
		printf("error is %d\n", rc);
	}
	rc = redis_ltrim(redis_client, key, 0, -1);
	if (rc == REDIS_SUCCESS) {
		printf("redis_ltrim success\n");
	} else {
		printf("error is %d\n", rc);
	}
	item = redis_rpop(redis_client, key, &rc);
	if (rc == REDIS_SUCCESS) {
		printf("rpop   %s\n", item);
	} else {
		printf("error is %d\n", rc);
	}

	elements = NULL;
	len = redis_lrange(redis_client, key, 0, -1, &elements, &rc);
	if (rc == REDIS_SUCCESS) {
		for (i = 0; i < len; i++) {
			printf("(%d): %s\n", i, elements[i]);
		}
	} else {
		printf("error is %d\n", rc);
	}

	for (i = 0; i < len; i++) {
		free(elements[i]);
	}
	free(elements);

	redis_client_free(redis_client);
	return 1;

}

