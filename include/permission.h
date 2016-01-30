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
#define MAX_RECORD HASH_COUNT

extern char db_path[];

typedef struct entry_s entry_t;

struct entry_s {
    char value[100];

    entry_t *next;
    entry_t *prev;
};

typedef struct perm_db_s permission_table_t;
typedef struct perm_db_s login_table_t;
typedef struct perm_db_s passphrase_table_t;

typedef struct perm_db_s db_t;

struct perm_db_s {
    char *key_arr[MAX_RECORD];
    entry_t *key_list;
    opool_t o_key;
    opool_t o_value;

    hash_table_t ht;
};
void perm_init(db_t *database, pj_pool_t *pool);

void load_passphrase_db(passphrase_table_t *database);
void load_permission_db(permission_table_t *database);
void load_login_db(login_table_t *database);

void save_passphrase_database(passphrase_table_t *database);
void save_permission_database(permission_table_t *database);
void save_login_database(login_table_t *database);

void add_new_passphrase(passphrase_table_t *database, char *field_1, char *field_2);
void remove_passphrase(passphrase_table_t *database, char *field_1, char *field_2);

void grant_access(permission_table_t *database, char *field_1, char *field_2);
void revoke_access(permission_table_t *database, char *field_1, char *field_2);

void add_new_login_account(login_table_t *database, char *field_1, char *field_2);
void remove_login_account(login_table_t *database, char *field_1, char *field_2);

void show_record(db_t *database, char *key);
void show_table(db_t *db);
#endif
