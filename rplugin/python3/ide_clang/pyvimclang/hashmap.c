#include "hashmap.h"

#define SET_INITIAL_SIZE 4
#define SET_INCREASE_FACTOR 2
#define SET_DECREASE_FACTOR 2

typedef struct bucket
{
    const void* key;
    void* data;
    struct bucket* next;

} bucket_t;

struct hashmap
{
    bucket_t** buckets;
    size_t length;
    size_t size;
    hash_t hash;
    equals_t equals;
};

hashmap_t* hashmap_alloc(hash_t hash, equals_t equals)
{
    hashmap_t* map = (hashmap_t*)malloc(sizeof(hashmap_t));
    map->length = SET_INITIAL_SIZE;
    map->size = 0;
    map->hash = hash;
    map->equals = equals;
    map->buckets = (bucket_t**)calloc(map->length, sizeof(bucket_t*));
    return map;
}

void hashmap_free(hashmap_t* map)
{
    for (size_t i = 0; i < map->length; ++i)
    {
        bucket_t* bucket = map->buckets[i];

        while (bucket != NULL)
        {
            bucket_t* removing = bucket;
            bucket = bucket->next;
            free(removing);
        }
    }

    free(map->buckets);
    free(map);
}

static void hashmap_resize(hashmap_t* map, size_t new_length)
{
    size_t old_length = map->length;
    bucket_t** old_buckets = map->buckets;

    map->size = 0;
    map->length = new_length;
    map->buckets = (bucket_t**)calloc(map->length, sizeof(bucket_t*));

    for (size_t i = 0; i < old_length; ++i)
    {
        bucket_t* bucket = old_buckets[i];

        while (bucket != NULL)
        {
            hashmap_set(map, bucket->key, bucket->data);
            bucket = bucket->next;
        }
    }

    free(old_buckets);
}

static bucket_t** hashmap_find(hashmap_t* map, const void* key)
{
    bucket_t** bucket = &(map->buckets[map->hash(key) % map->length]);

    while (*bucket != NULL && !map->equals((*bucket)->key, key))
    {
        bucket = &((*bucket)->next);
    }

    return bucket;
}

void hashmap_set(hashmap_t* map, const void* key, void* data)
{
    if (map->size == map->length / SET_INCREASE_FACTOR)
    {
        hashmap_resize(map, map->length * SET_INCREASE_FACTOR);
    }

    bucket_t** bucket = hashmap_find(map, key);

    if (*bucket == NULL)
    {
        *bucket = (bucket_t*)malloc(sizeof(bucket_t));
        (*bucket)->key = key;
        (*bucket)->data = data;
        (*bucket)->next = NULL;
        ++map->size;
    }
    else
    {
        (*bucket)->data = data;
    }
}

bool hashmap_get(hashmap_t* map, const void* key, void** data)
{
    bool result = false;
    bucket_t** bucket = hashmap_find(map, key);

    if (*bucket != NULL)
    {
        *data = (*bucket)->data;
        result = true;
    }

    return result;
}

bool hashmap_remove(hashmap_t* map, const void* key)
{
    if (map->size == map->length / SET_DECREASE_FACTOR)
    {
        hashmap_resize(map, map->length / SET_DECREASE_FACTOR);
    }

    bucket_t** bucket = hashmap_find(map, key);

    bool result = false;

    if (*bucket != NULL)
    {
        bucket_t* removing = *bucket;
        *bucket = (*bucket)->next;
        free(removing);
        --map->size;
        result = true;
    }

    return result;
}

size_t hashmap_size(hashmap_t* map)
{
    return map->size;
}

void hashmap_each(hashmap_t* map, void* ctx,
                  void (*action)(void*, const void*, void*))
{
    for (size_t i = 0; i < map->length; ++i)
    {
        bucket_t* bucket = map->buckets[i];
        while (bucket)
        {
            (*action)(ctx, bucket->key, bucket->data);
            bucket = bucket->next;
        }
    }
}
