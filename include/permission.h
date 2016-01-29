#ifndef __PERMISSION__
#define __PERMISSION__

#include <sqlite3.h>
#include "hash-table.h"
#include "my-pjlib-utils.h"
#include "ansi-utils.h"
#include "mysqlite.h"
#include "object-pool.h"
#include "utlist.h"

#include "proto-constants.h"

#define HASH_COUNT 10
#define MAX_RECORD 10

extern char db_path[];

typedef struct entry_s entry_t;

struct entry_s {
    char value[100];

    entry_t *next;
    entry_t *prev;
};

typedef struct db_s db_t;

struct db_s {
    char key_arr[MAX_RECORD][255];
    entry_t *data_list[MAX_RECORD];
    hash_table_t ht;
};

int get_idx(char key_arr[][255], char *key);

void load_database(opool_t *opool, db_t *database, char *table_name);

void update_database(opool_t *opool, db_t *database, char *table_name);
void update_permission_database(opool_t *opool, db_t *database);

void update_table(opool_t *opool, db_t *database, char *table_name, char *field_1, char *field_2);
void update_permission_table(opool_t *opool, db_t *database, char *table_name, char *field_1, char *field_2);

void show_record(db_t *database, char *key);
void show_table(db_t *db);

entry_t *get_data_list(hash_table_t *ht, char *key);

#endif
