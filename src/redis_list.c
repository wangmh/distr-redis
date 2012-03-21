/*
 * redis_list.c
 *
 *  Created on: 2011-4-28
 *      Author: saint
 */

#include "common.h"

char *redis_lindex(redis_client_st *redis_client, const char*key,
		long int index, redis_return_st *rc) {
	if (redis_client == NULL || key == NULL) {
		if (rc != NULL)
			*rc = REDIS_FAILURE;
		return NULL;
	}

	char *val = NULL;

	int argc = 3;
	char index_str[64];

	snprintf(index_str, 64, "%ld", index);
	const char *argv[] = { "LINDEX", key, index_str };

	val = redis_return_str(redis_client, argc, argv, rc);

	return val;
}

int redis_linsert(redis_client_st *redis_client, const char *key, int is_before,
		const char* prov, const char *value, redis_return_st *rc) {
	char *before = "BEFORE";
	char *after = "AFTER";
	char *where = NULL;
	where = ((1 == is_before) ? before : after);
	if (redis_client == NULL || key == NULL || value == NULL) {
		if (rc != NULL)
			*rc = REDIS_FAILURE;
		return -1;
	}

	int rt = -1;
	int argc = 4;
	const char *argv[] = { "LINSERT", key, where, prov, value };

	rt = redis_return_integer(redis_client, argc, argv, rc);

	return rt;
}

int redis_llen(redis_client_st *redis_client, const char *key,
		redis_return_st *rc) {
	int rt = -1;
	if (redis_client == NULL || key == NULL) {
		if (rc) {
			*rc = REDIS_FAILURE;
		}
		return rt;
	}
	int argc = 2;
	const char *argv[] = { "LLEN", key };

	rt = redis_return_integer(redis_client, argc, argv, rc);

	return rt;
}

char *redis_lpop(redis_client_st *redis_client, const char *key,
		redis_return_st *rc) {
	if (redis_client == NULL || NULL == key) {
		if (rc) {
			*rc = REDIS_FAILURE;
		}
		return NULL;
	}

	char *rt = NULL;
	int argc = 2;
	const char *argv[] = { "LPOP", key };
	rt = redis_return_str(redis_client, argc, argv, rc);
	return rt;
}

int redis_lpush(redis_client_st *redis_client, const char *key,
		const char *value, redis_return_st *rc) {
	if (redis_client == NULL || NULL == key || NULL == value) {
		if (rc) {
			*rc = REDIS_FAILURE;
		}
		return -1;
	}
	int rt = -1;
	int argc = 3;
	const char *argv[] = { "LPUSH", key, value };
	rt = redis_return_integer(redis_client, argc, argv, rc);
	return rt;
}

int redis_lpushx(redis_client_st *redis_client, const char *key,
		const char *value, redis_return_st *rc) {

	if (redis_client == NULL || NULL == key || NULL == value) {
		if (rc) {
			*rc = REDIS_FAILURE;
		}
		return -1;
	}
	int rt = -1;
	int argc = 3;
	const char *argv[] = { "LPUSHX", key, value };
	rt = redis_return_integer(redis_client, argc, argv, rc);
	return rt;
}

int redis_lrange(redis_client_st *redis_client, const char *key, int begin,
		int end, char ***elements, redis_return_st *rc) {
	if (redis_client == NULL || key == NULL || elements == NULL) {
		if (rc)
			*rc = REDIS_FAILURE;
		*elements = NULL;
		return -1;
	}
	int ret = -1;

	int argc = 4;
	char begin_str[64];
	char end_str[64];
	snprintf(begin_str, sizeof(begin_str), "%d", begin);
	snprintf(end_str, sizeof(end_str), "%d", end);
	const char *argv[] = { "LRANGE", key, (const char* )begin_str, (const char* )end_str };

	ret = redis_return_muti(redis_client, argc, argv, elements, rc);

	return ret;
}

int redis_lrem(redis_client_st *redis_client, const char *key, int count,
		const char *value, redis_return_st *rc) {
	if (redis_client == NULL || key == NULL || value == NULL) {
		if (rc)
			*rc = REDIS_FAILURE;
		return 0;
	}
	int ret = 0;
	char *cmd = NULL;
	int argc = 4;
	char count_str[64];

	snprintf(count_str, sizeof(count_str), "%d", count);

	const char *argv[] = { "LRANGE", key, (const char *)count_str, value };

	ret = redis_return_integer(redis_client, argc, argv, rc);

	return ret;
}

redis_return_st redis_lset(redis_client_st *redis_client, const char *key,
		int index, const char *value) {
	redis_return_st rc = REDIS_FAILURE;

	if (redis_client == NULL || key == NULL || value == NULL) {
		return rc;
	}

	int argc = 4;
	char index_str[64];

	snprintf(index_str, sizeof(index_str), "%d", index);

	const char *argv[] = { "LSET", key, (const char *)index_str, value };

	rc = redis_return_status(redis_client, argc, argv);

	return rc;
}

redis_return_st redis_ltrim(redis_client_st *redis_client, const char *key,
		int start, int end) {
	redis_return_st rc = REDIS_FAILURE;
	if (redis_client == NULL || key == NULL) {
		return rc;
	}
	int argc = 4;
	char begin_str[64];
	char end_str[64];
	snprintf(begin_str, sizeof(begin_str), "%d", start);
	snprintf(end_str, sizeof(end_str), "%d", end);
	const char *argv[] = { "LTRIM", key, (const char *)begin_str, (const char *)end_str };
	rc = redis_return_status(redis_client, argc, argv);
	return rc;
}

char *redis_rpop(redis_client_st *redis_client, const char *key,
		redis_return_st *rc) {
	if (redis_client == NULL || key == NULL) {
		if (rc)
			*rc = REDIS_FAILURE;
		return NULL;
	}
	char *rt = NULL;
	int argc = 2;

	const char *argv[] = { "RPOP", key};
	rt = redis_return_str(redis_client, argc, argv, rc);
	return rt;
}

int redis_rpush(redis_client_st *redis_client, const char *key,
		const char *value, redis_return_st *rc) {
	if (redis_client == NULL || key == NULL) {
		if (rc)
			*rc = REDIS_FAILURE;
		return -1;
	}
	int rt = -1;
	int argc = 3;
	const char *argv[] = { "RPUSH", key, value};
	rt = redis_return_integer(redis_client, argc, argv, rc);
	return rt;
}

int redis_rpushx(redis_client_st *redis_client, const char *key,
		const char *value, redis_return_st *rc) {
	if (redis_client == NULL || key == NULL) {
		if (rc)
			*rc = REDIS_FAILURE;
		return -1;
	}
	int rt = -1;
	int argc = 3;
	const char *argv[] = { "RPUSHX", key, value};
	rt = redis_return_integer(redis_client, argc, argv, rc);
	return rt;
}

