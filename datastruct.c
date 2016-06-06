#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define HASHSIZE 10000
#define CACHESIZE 3
  
typedef struct cache {
  char *key;
  char *page;
  struct cache *older;
  struct cache *newer;
} cache;

cache *first = NULL;
cache *end = NULL;
int size = 0;

void hash_init(cache **table) {
  int i;
  for (i = 0; i < HASHSIZE; i++) {
    table[i] = NULL;
  }
}

static void cache_free(cache *cache) {
  if (cache->key != NULL)
    free(cache->key);
  if (cache->page != NULL)
    free(cache->page);
  free(cache);
}

static int get_hash_value(char *key) {
  int hashval = 0;
  while (*key) {
    hashval += *key++;
  }
  return hashval % HASHSIZE;
}

char* hash_search(cache **table, char *key) {
  int hashval = get_hash_value(key);
  cache *hp = table[hashval];
  if (hp != NULL)
    return hp->page;
  return NULL;
}

static int strcpy_alloc(char **dest, char *src) {
  int length = strlen(src);
  if (length <= 0) {
    fprintf(stderr, "ERROR: Invalid parameter(%d line)\n", __LINE__);
    return -1;
  }

  *dest = (char *)malloc(length);
  if (*dest == NULL) {
    fprintf(stderr, "ERROR: %s(%d line)\n", strerror(errno), __LINE__);
    return -1;
  }

  if (strncpy(*dest, src, length) == NULL) {
    fprintf(stderr, "ERROR: %s(%d line)\n", strerror(errno), __LINE__);
    return -1;
  }
  return 0;
}

int hash_delete(cache **table, char *key) {
  cache *target = NULL;
  int hashval = get_hash_value(key);

  target = table[hashval];
  if (target == NULL) {
    fprintf(stderr, "target[%s] is not exist in hash table.\n", key);
    return -1;
  }

  if (strcmp(key, target->key) == 0) {
    cache_free(target);
    table[hashval] = NULL;
    size--;
    return 0;
  }
  return -1;
}

int hash_insert(cache **table, char *key, char *page) {
  cache *p = NULL;
  int hashval = get_hash_value(key);
  if (hash_search(table, key) != NULL) {
    p = table[hashval];
    if (end != p) {
      if (first == p)
        return 0;
      p->newer->older = p->older;
      p->older->newer = p->newer;
    } else {
      p->newer->older = NULL;
      end = p->newer;
    }
    p->older = first;
    p->newer = NULL;
    first->newer = p;
    first = p;
    return 0;
  }

  p = (cache *) malloc(sizeof(cache));
  if (p ==NULL) {
    fprintf(stderr, "ERROR: %s(%d line)\n", strerror(errno), __LINE__);
    return -1;
  }

  if (strcpy_alloc(&(p->key), key) != 0)
    return -1;
  if (strcpy_alloc(&(p->page), page) != 0)
    return -1;

  table[hashval] = p;
  size++;
  if (first == NULL) {
    end = p;
    first = p;
  } else { 
    p->older = first;
    first->newer = p;
    first = p; 
  }
  if (size > CACHESIZE) {
    end = end->newer;
    hash_delete(table, end->older->key);
  }
  return 0;
}
      

static void hash_print_table(cache **table) {
  int i;
  for (i = 0; i < HASHSIZE; i++) {
    if (table[i] != NULL) {
      printf("table[%d]: {%s %s}\n", i, table[i]->key, table[i]->page);
    }
  }
}

int main() {
  cache *table[HASHSIZE];

  hash_init(table);

  hash_insert(table, "http://google", "google_page");
  hash_insert(table, "http://yahoo", "yahoo_page");
  hash_insert(table, "http://twitter", "titter_page");
  hash_print_table(table);
  printf("\n");
  hash_insert(table, "http://google", "google_page");
  hash_insert(table, "http://github", "github_page");
  hash_print_table(table);
  printf("\n");
  hash_insert(table, "http://tumblr", "tumblr_page");
  hash_print_table(table);
  
  return 0;
}
  
