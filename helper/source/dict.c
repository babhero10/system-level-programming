#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/dict.h"

// --- Constants ---
#define INITIAL_DEFAULT_CAPACITY 16
#define MAX_LOAD_FACTOR 0.75
#define GROWTH_FACTOR 2

// --- Internal Structures (Hidden from users) ---

typedef struct DictEntry {
  char *key;
  char *value;
  struct DictEntry *next;
} DictEntry;

struct Dictionary {
  DictEntry **buckets;
  size_t capacity;
  size_t size;
};

// --- Internal Hash Function ---
static unsigned long hash_function(const char *str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + c; // hash * 33 + c
  }
  return hash;
}

// --- Forward Declaration for Internal Resize Function ---
static int dict_resize(Dictionary *dict, size_t new_capacity);

// --- Public Function Implementations ---

Dictionary *dict_create(size_t initial_capacity) {
  size_t capacity =
      (initial_capacity > 0) ? initial_capacity : INITIAL_DEFAULT_CAPACITY;

  Dictionary *dict = malloc(sizeof(Dictionary));
  if (!dict) {
    return NULL;
  }

  dict->buckets = calloc(capacity, sizeof(DictEntry *));
  if (!dict->buckets) {
    free(dict);
    return NULL;
  }

  dict->capacity = capacity;
  dict->size = 0;
  return dict;
}

void dict_destroy(Dictionary **dict_ptr) {
  if (!dict_ptr || !(*dict_ptr)) {
    return;
  }

  Dictionary *dict = *dict_ptr;
  for (size_t i = 0; i < dict->capacity; ++i) {
    DictEntry *entry = dict->buckets[i];
    while (entry != NULL) {
      DictEntry *next_entry = entry->next;
      free(entry->key);
      free(entry->value);
      free(entry);
      entry = next_entry;
    }
  }
  free(dict->buckets);
  free(dict);
  *dict_ptr = NULL;
}

// Internal resize function
static int dict_resize(Dictionary *dict, size_t new_capacity) {
  if (!dict || new_capacity < dict->size || new_capacity == 0) {
    return -1; // Indicate failure or invalid request
  }
  if (new_capacity == dict->capacity) {
    return 0; // Nothing to do
  }

  // 1. Allocate new bucket array
  DictEntry **new_buckets = calloc(new_capacity, sizeof(DictEntry *));
  if (!new_buckets) {
    return -1;
  }

  // 2. Rehash existing entries into the new table
  for (size_t i = 0; i < dict->capacity; ++i) {
    DictEntry *entry = dict->buckets[i];
    while (entry != NULL) {
      DictEntry *next_entry =
          entry->next; // Store next before modifying entry->next

      // Calculate index in the *new* table
      unsigned long hash = hash_function(entry->key);
      size_t new_index = hash % new_capacity;

      // Insert entry at the beginning of the new bucket's list
      // (No need to copy key/value, just move the node)
      entry->next = new_buckets[new_index];
      new_buckets[new_index] = entry;

      // Move to the next entry in the old bucket's list
      entry = next_entry;
    }
  }

  // 3. Free the old bucket array
  free(dict->buckets);

  // 4. Update dictionary structure
  dict->buckets = new_buckets;
  dict->capacity = new_capacity;

  return 0; // Success
}

int dict_put(Dictionary *dict, const char *key, const char *value) {
  if (!dict || !key || !value) {
    return -1;
  }
  if (dict->capacity == 0) { // Should not happen with default capacity
    return -1;
  }

  // --- Check if resizing is needed *before* calculating index ---
  // We only resize when adding a *new* element increases the load factor.
  // So, first check if the key exists. If it does, we just update and don't
  // resize yet. If it doesn't exist, *then* we check the load factor *before*
  // inserting.

  unsigned long hash = hash_function(key);
  size_t index = hash % dict->capacity;
  DictEntry *entry = dict->buckets[index];

  // 1. Check if key already exists (and update)
  while (entry != NULL) {
    if (strcmp(entry->key, key) == 0) {
      // Key found, update value
      char *new_value = strdup(value);
      if (!new_value) {
        return -1;
      }
      free(entry->value);
      entry->value = new_value;
      return 0; // Update successful, no size change, no resize needed now
    }
    entry = entry->next;
  }

  // 2. Key doesn't exist. Check load factor *before* inserting.
  // We check based on adding ONE more element (dict->size + 1)
  if (((double)(dict->size + 1) / dict->capacity) > MAX_LOAD_FACTOR) {
    size_t new_capacity = dict->capacity * GROWTH_FACTOR;
    // Handle potential overflow if capacity gets huge (unlikely for size_t
    // usually)
    if (new_capacity <= dict->capacity) {
      new_capacity = dict->capacity + 1; // Minimal growth if overflow/no growth
    }

    dict_resize(dict, new_capacity);
    
    // Recalculate index after potential resize
    index = hash % dict->capacity;
  }

  // 3. Create and insert the new entry
  DictEntry *new_entry = malloc(sizeof(DictEntry));
  if (!new_entry) {
    return -1;
  }

  new_entry->key = strdup(key);
  if (!new_entry->key) {
    free(new_entry);
    return -1;
  }

  new_entry->value = strdup(value);
  if (!new_entry->value) {
    free(new_entry->key);
    free(new_entry);
    return -1;
  }

  // Add to the beginning of the list for the (potentially new) bucket index
  new_entry->next = dict->buckets[index];
  dict->buckets[index] = new_entry;
  dict->size++; // Increment size *after* successful insertion

  return 0; // Insertion successful
}

char *dict_get(Dictionary *dict, const char *key) {
  if (!dict || !key || dict->capacity == 0) {
    return NULL;
  }

  unsigned long hash = hash_function(key);
  size_t index = hash % dict->capacity;
  DictEntry *entry = dict->buckets[index];

  while (entry != NULL) {
    if (strcmp(entry->key, key) == 0) {
      return entry->value; // Found
    }
    entry = entry->next;
  }
  return NULL; // Not found
}

int dict_remove(Dictionary *dict, const char *key) {
  if (!dict || !key || dict->capacity == 0) {
    return -1;
  }

  unsigned long hash = hash_function(key);
  size_t index = hash % dict->capacity;
  DictEntry *entry = dict->buckets[index];
  DictEntry *prev = NULL;

  while (entry != NULL) {
    if (strcmp(entry->key, key) == 0) {
      // Found, remove it
      if (prev == NULL) { // Head of list
        dict->buckets[index] = entry->next;
      } else { // Middle or end
        prev->next = entry->next;
      }
      free(entry->key);
      free(entry->value);
      free(entry);
      dict->size--;

      return 0; // Removal successful
    }
    prev = entry;
    entry = entry->next;
  }
  return -1; // Key not found
}

size_t dict_size(const Dictionary *dict) { return dict ? dict->size : 0; }

size_t dict_capacity(const Dictionary *dict) {
  return dict ? dict->capacity : 0;
}
