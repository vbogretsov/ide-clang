/**
 * Simple buckets based hashmap implementation.
 *
 *
 * May 31 2017 Vladimir Bogretsov <bogrecov@gmail.com>
 */
#ifndef hashmap_H
#define hashmap_H

#include <stdbool.h>
#include <stdlib.h>

typedef int (*hash_t)(const void*);
typedef bool (*equals_t)(const void*, const void*);

typedef struct hashmap hashmap_t;

/**
 * Allocate a new hashmap.
 * @param  hash   hash function.
 * @param  equals equality function.
 * @return        the new hashmap allocated.
 */
hashmap_t* hashmap_alloc(hash_t hash, equals_t eq);

/**
 * Deallocate the hashmap provided.
 * @param map hashmap to be deallocated.
 */
void hashmap_free(hashmap_t* map);

/**
 * Insert data with the key prvided in in the map.
 * @param map  hashmapmap to be updated.
 * @param key  the key associated with the data.
 * @param data the data to be inserted.
 */
void hashmap_set(hashmap_t* map, const void* key, void* data);

/**
 * Get the data with the key provided.
 * @param  map  hashmap where the check should be performed.
 * @param  key  the key with which the data desired is associated.
 * @param  data the data desired.
 * @return      true if the key provided was found owhterwise false.
 */
bool hashmap_get(hashmap_t* map, const void* key, void** data);

/**
 * Remove the key provided and data associated with it from the hashmap.
 * @param map  hashmap to be updated.
 * @param key  key to be removed.
 * @return     true if the key was removed otherwise false.
 */
bool hashmap_remove(hashmap_t* map, const void* key);

/**
 * Get size of the hashmap provided.
 * @param  map the hashmap.
 * @return     size of the hashmap provided.
 */
size_t hashmap_size(hashmap_t* map);

/**
 * Apply the action provided to each item in the map.
 * @param set    the map to iterate.
 * @param ctx    closure context.
 * @param action the action to apply to items of the map.
 */
void hashmap_each(hashmap_t* map, void* ctx,
                  void (*action)(void*, const void*, void*));

#endif //! HASHMAP_H
