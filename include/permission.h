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
#define DEVICE_NAME_LENGTH 100

extern char db_path[];
//chinh func pinter

typedef struct perm_node_s perm_node_t;

struct perm_node_s {
    char value[DEVICE_NAME_LENGTH];

    perm_node_t *next;
    perm_node_t *prev;
};

typedef struct perm_db_s p_table_t;
typedef struct perm_db_s l_table_t;
typedef struct perm_db_s pph_table_t;
typedef struct perm_db_s perm_db_t;

struct perm_db_s {
    char db_path[100];
    perm_node_t *key_list;
    opool_t opool;
    hash_table_t ht;
};

void perm_init(perm_db_t *database, pj_pool_t *pool, char *db_path);

void load_passphrase_db(pph_table_t *pph_table);
void load_login_db(l_table_t *l_table);
void load_permission_db(p_table_t *p_table);

void save_passphrase_db(pph_table_t *pph_table);
void save_login_db(l_table_t *l_table);
void save_permission_db(p_table_t *p_table);

void add_passphrase(pph_table_t *pph_table, char *node_id, char *passphrase);
void add_active_acc(l_table_t *l_table, char *user_id, char *node_id);
void grant_access(p_table_t *p_table, char *user_id, char *node_id);

void remove_passphrase(pph_table_t *pph_table, char *node_id);
void remove_active_acc(l_table_t *l_table, char *user_id);
void revoke_access(p_table_t *p_table, char *user_id, char *node_id);

void show_record(perm_db_t *database, char *key);
void show_table(perm_db_t *db);
#endif
