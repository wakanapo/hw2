#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define HASHSIZE 8
#define CASHSIZE 3
  
typedef struct cash {
  char *key;
  char *page;
  struct cash *older;
  struct cash *newer;
} cash;

cash *first = NULL;
cash *end = NULL;

void hash_init(cash **table) {
  int i;
  for (i = 0; i < HASHSIZE; i++) {
    table[i] = NULL;
  }
}

static void cash_free(cash *cash) {
  if (cash->key != NULL)
    free(cash->key);
  if (cash->page != NULL)
    free(cash->page);
  free(cash);
}

static int get_hash_value(char *key) {
  int hashval = 0;
  while (*key) {
    hashval += *key++;
  }
  return hashval % HASHSIZE;
}

char* hash_search(cash **table, char *key) {
  int hashval = get_hash_value(key);
  cash *hp = table[hashval];
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

  if(strncpy(*dest, src, length) == NULL) {
    fprintf(stderr, "ERROR: %s(%d line)\n", strerror(errno), __LINE__);
    return -1;
  }
  return 0;
}

int get_size(cash **table) {
  int i, size = 0;
  for (i = 0; i < HASHSIZE; i++) {
     if (table[i] != NULL) {
       size++;
     }
  }
  return size;
}

int hash_delete(cash **table, char *key) {
  cash *target = NULL;
  int hashval = get_hash_value(key);

  target = table[hashval];
  if (target == NULL) {
    fprintf(stderr, "target[%s] is not exist in hash table.\n", key);
    return -1;
  }

  if (strcmp(key, target->key) == 0) {
    cash_free(target);
    table[hashval] = NULL;
    return 0;
  }
  return -1;
}

int hash_insert(cash **table, char *key, char *page) {
  cash *p = NULL;
  int hashval = get_hash_value(key);
  if (hash_search(table, key) != NULL) {
    p = table[hashval];
    if (end != p) {
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

  p = (cash *) malloc(sizeof(cash));
  if (p ==NULL) {
    fprintf(stderr, "ERROR: %s(%d line)\n", strerror(errno), __LINE__);
    return -1;
  }

  if (strcpy_alloc(&(p->key), key) != 0)
    return -1;
  if (strcpy_alloc(&(p->page), page) != 0)
      return -1;

  table[hashval] = p;
  if (first == NULL) {
    end = p;
    first = p;
  } else { 
    p->older = first;
    first->newer = p;
    first = p; 
  }
  if (get_size(table) > CASHSIZE) {
    end = end->newer;
    hash_delete(table, end->older->key);
  }
  return 0;
}
      

static void hash_print_table(cash **table) {
  int i;
  for (i = 0; i < HASHSIZE; i++) {
    if (table[i] != NULL) {
      printf("table[%d]: {%s %s}\n", i, table[i]->key, table[i]->page);
    }
  }
}

int main() {
  cash *table[HASHSIZE];

  hash_init(table);

  hash_insert(table, "google", "google");
  hash_insert(table, "yahoo", "yahoo");
  hash_insert(table, "MicroSoft", "MicroSoft");
  hash_print_table(table);
  printf("\n");
  hash_insert(table, "google", "google");
  hash_insert(table, "github", "github");
  hash_print_table(table);
    printf("\n");
  hash_insert(table, "apple", "apple");
  hash_print_table(table);  


  return 0;
}
  