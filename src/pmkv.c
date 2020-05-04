#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "libpmem.h"
#include "pmkv.h"

#define FILE_MODE (0666)

#define DEFAULT_TABLE_SIZE 100

pmkv* pmkv_open(const char *path, size_t pool_size, int force_create)
{
	u_int64_t maplen;
	pmkv *db = malloc(sizeof(pmkv));
	db->data = pmem_map_file(path, 1 << 32, PMEM_FILE_CREATE, FILE_MODE, &maplen, NULL);
	db->fsize = maplen;
	db->table.keymap  = malloc(DEFAULT_TABLE_SIZE * sizeof(HKey));
	db->table.valmap  = malloc(DEFAULT_TABLE_SIZE * sizeof(HVal));
	db->table.sizemap = malloc(DEFAULT_TABLE_SIZE * sizeof(ValSize));
	db->table.cap     = DEFAULT_TABLE_SIZE;
	return db;
}

void pmkv_close(pmkv *kv)
{
	pmem_unmap(kv->data, kv->fsize);
	free(kv->table.keymap);
	free(kv->table.valmap);
	free(kv->table.sizemap);
	free(kv);
}

int pmkv_get(pmkv *kv, const char *key, size_t key_size, char *val, size_t *out_val_size)
{
	const char *hashed_key = hash(key);

	u_int64_t index = kv->table.get(hashed_key);
	if(index >= kv->table.cap) return 1;

	val = kv->table.valmap[index];
	out_val_size = kv->table.sizemap[index];

	return 0;
}

int pmkv_put(pmkv *kv, const char *key, size_t key_size, const char *val, size_t val_size)
{
	const char *hashed_key = hash(key);

	u_int16_t index = kv->table.insert(hashed_key, val);
	if(index >= kv->table.cap) return 1;

	kv->table.keymap[index] = key;
	kv->table.valmap[index] = val;
	kv->table.sizemap[index] = val_size;
	kv->table.size++;

	return 0;
}

int pmkv_delete(pmkv *kv, const char *key, size_t key_size)
{
	const char *hashed_key = hash(key);

	u_int16_t index = kv->table.remove(hashed_key);
	if(index >= kv->table.cap) return 1;

	kv->table.size--;

	return 0;
}

int pmkv_count_all(pmkv *kv, size_t *out_cnt)
{
	return kv->table.size;
}

int pmkv_exists(pmkv *kv, const char *key, size_t key_size)
{
	const char *hashed_key = hash(key);

	return (kv->table.get(key) >= kv->table.cap)?1:0;
}
