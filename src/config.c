/*
 * config.c
 *
 *  Created on: 2011-3-30
 *      Author: saint
 */

#include "common.h"

static void load_config(redis_client_config_st * redis_config)
{
	if (NULL == redis_config)
		return;
	redis_config->servers_count = 0;
	GKeyFile *key_file;
	GError *err = NULL;
	key_file = g_key_file_new();
	g_key_file_set_list_separator(key_file, ',');
	if (!g_key_file_load_from_file(key_file, redis_config->config_path, 0, &err))
	{
		fprintf(stderr, "Parsing %s failed: %s", redis_config->config_path,
				err->message);
		g_error_free(err);
		g_key_file_free(key_file);
		return;
	}
	redis_config->retry_time = g_key_file_get_integer(key_file, "General", "reconn_times", &err);
	if(err)
	{
		redis_config->retry_time = -1;
	}
	redis_config->maxconns = g_key_file_get_integer(key_file, "General",
			"maxconns", &err);
	if (err)
	{
		redis_config->maxconns = 10;
		g_clear_error(&err);
	}
	redis_config->minconns = g_key_file_get_integer(key_file, "General",
			"minconns", &err);
	if (err)
	{
		redis_config->minconns = 5;
		g_clear_error(&err);
	}
//	redis_config->redis_err_file = g_key_file_get_string(key_file, "General",
//			"redis_err_log", &err);
//	if(err)
//	{
//		redis_config->redis_err_file = strdup("/home/redis_err.log");
//		g_clear_error(&err);
//	}
	redis_config->redis_log_path = g_key_file_get_string(key_file, "General",
			"redis_log", &err);
	if (err)
	{
		redis_config->redis_log_path = strdup("/home");
		g_clear_error(&err);
	}
	char *log_level = g_key_file_get_string(key_file, "General", "log_level",
			&err);
	if (err)
	{
		redis_config->log_level = L_INFO;
		g_clear_error(&err);
	}
	else
	{
		if (strcasecmp(log_level, "debug") == 0)
		{
			redis_config->log_level = L_DEBUG;
		}
		else if (strcasecmp(log_level, "info") == 0)
		{
			redis_config->log_level = L_INFO;
		}
		else if (strcasecmp(log_level, "warn") == 0)
		{
			redis_config->log_level = L_WARN;
		}
		else if (strcasecmp(log_level, "err") == 0)
		{
			redis_config->log_level = L_ERR;
		}
	}
	gsize i, length;
	gchar** servers = g_key_file_get_keys(key_file, "RedisServers", &length,
			&err);
	server_config_st * temp = NULL;
	if (NULL == err)
	{
		redis_config->server_nodes = (server_config_st **)calloc(sizeof(server_config_st*), length);
		for (i = 0; i < length; i++)
		{
			temp = (server_config_st *) calloc(sizeof(server_config_st), 1);
			temp->key = strdup(servers[i]);
			temp->value = g_key_file_get_string(key_file, "RedisServers",
					temp->key, &err);
			if (err)
			{
				fprintf(stderr, "config group RedisServers node %s error\n",
						temp->key);
				exit(1);
			}
			redis_config->server_nodes[i]  = temp;
			redis_config->servers_count  = i + 1;
		}
	}
	g_strfreev(servers);
	g_key_file_free(key_file);
}

redis_client_config_st* redis_client_conf_init(const char *config_path)
{
	if (NULL == config_path)
	{
		return NULL;
	}
	redis_client_config_st *redis_config = (redis_client_config_st *) calloc(
			sizeof(redis_client_config_st), 1);
	if (NULL == redis_config)
	{
		MEMORYOUT;
	}

	snprintf(redis_config->config_path, sizeof(redis_config->config_path),
			"%s", config_path);
	load_config(redis_config);
	return redis_config;
}
