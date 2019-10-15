#ifndef __PMKV_H
#define __PMKV_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {} pmkv;

pmkv* pmkv_open(const char *path, size_t pool_size, int force_create);
void pmkv_close(pmkv *kv);
void* pmkv_get(pmkv *kv, const char *key, size_t key_size);
void pmkv_put(pmkv *kv, const char *key, size_t key_size, const char *value, size_t value_size);
void pmkv_del(pmkv *kv, const char *key, size_t key_size);
void pmkv_count_all(pmkv *kv, size_t *cnt);
int pmkv_exists(pmkv *kv, const char *key, size_t key_size);

#ifdef __cplusplus
}
#endif
#endif
