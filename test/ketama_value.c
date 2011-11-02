/*
 * katam_value.c
 *
 *  Created on: 2011-4-1
 *      Author: saint
 */

#include "../common.h"
#include "../redis_client.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <assert.h>

#define PROGRAME "ketama_value"

int main(int argc, char *argv[])
{
	redis_client_st *redis_client;
	char config_path[1024];

	if (argc != 3)
	{
		printf("usage: ./%s xxx.conf key\n", PROGRAME);
		return 1;
	}
	memset(config_path, 0, sizeof(config_path));
	snprintf(config_path, sizeof(config_path), "%s",  argv[1]);
	
	
	redis_client_config_st *redis_config = redis_client_conf_init(config_path);
	redis_log_init(redis_config->redis_log_path, redis_config->log_level, PROGRAME);
	redis_client = (redis_client_st *) calloc(sizeof(redis_client), 1);
	redis_client->conf = redis_config;
	initialize_redis_ketama(redis_client);
	char *hash_key = get_hash_key(argv[2]);
	int index = get_server(redis_client->ketama, hash_key);
	char *server_name = redis_client->conf->server_nodes[index]->key;
	char *server_value = redis_client->conf->server_nodes[index]->value;
	printf("%s \t%s\n", server_name, server_value);
	redis_client_free(redis_client);	
	return 1;

}
