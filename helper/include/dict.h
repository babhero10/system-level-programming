#ifndef DICT_H
#define DICT_H

#include <stddef.h> // For size_t

struct Dictionary;
typedef struct Dictionary Dictionary;

// --- Public Function Declarations ---

/**
 * @brief Creates a new dictionary (hash map).
 *
 * @param capacity The initial number of buckets for the hash table.
 *                 A larger capacity reduces collisions but uses more memory.
 *                 Must be greater than 0.
 * @return A pointer to the newly created Dictionary, or NULL on failure
 *         (e.g., memory allocation error or invalid capacity).
 */
Dictionary *dict_create(size_t capacity);

/**
 * @brief Destroys a dictionary and frees all associated memory.
 *
 * Frees the memory for all keys, values, internal nodes, the bucket array,
 * and the dictionary structure itself.
 * Sets the passed pointer *dict_ptr to NULL after destruction.
 *
 * @param dict_ptr A pointer to the Dictionary pointer. This allows the function
 *                 to set the original pointer to NULL after freeing.
 */
void dict_destroy(Dictionary **dict_ptr);

/**
 * @brief Inserts or updates a key-value pair in the dictionary.
 *
 * If the key already exists, its associated value is updated.
 * Otherwise, a new entry is created.
 * The dictionary creates its own copies of the key and value strings using strdup.
 *
 * @param dict A pointer to the Dictionary.
 * @param key The string key (null-terminated). Must not be NULL.
 * @param value The string value (null-terminated). Must not be NULL.
 * @return 0 on success, -1 on failure (e.g., memory allocation error, invalid args).
 */
int dict_put(Dictionary *dict, const char *key, const char *value);

/**
 * @brief Retrieves the value associated with a key.
 *
 * @param dict A pointer to the Dictionary.
 * @param key The string key to look up (null-terminated). Must not be NULL.
 * @return A pointer to the associated value string if the key is found,
 *         otherwise NULL.
 * @warning The returned string is owned by the dictionary. DO NOT free it.
 *          If you need a mutable copy or need it to persist after the
 *          dictionary might change or be destroyed, use strdup() on the result.
 */
char *dict_get(Dictionary *dict, const char *key);

/**
 * @brief Removes a key-value pair from the dictionary.
 *
 * If the key is found, the entry (including its copied key and value) is freed.
 *
 * @param dict A pointer to the Dictionary.
 * @param key The string key to remove (null-terminated). Must not be NULL.
 * @return 0 if the key was found and removed, -1 if the key was not found or
 *         an error occurred.
 */
int dict_remove(Dictionary *dict, const char *key);

/**
 * @brief Gets the current number of key-value pairs stored in the dictionary.
 *
 * @param dict A pointer to the Dictionary.
 * @return The number of items in the dictionary, or 0 if dict is NULL.
 */
size_t dict_size(const Dictionary *dict);

/**
 * @brief Gets the current capacity (number of buckets) of the dictionary's hash table.
 *
 * @param dict A pointer to the Dictionary.
 * @return The capacity of the dictionary, or 0 if dict is NULL.
 */
size_t dict_capacity(const Dictionary *dict);


#endif // DICT_H
