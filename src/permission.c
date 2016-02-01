#include "permission.h"

char db_path[] = "./databases/permission.db";

void perm_init(perm_db_t *database, pj_pool_t *pool) {
    opool_init(&database->opool, 100, sizeof(perm_node_t), pool);
}

static void load_db(perm_db_t *table, char *table_name) {
    sqlite3 *db;
    char sql[50], key_temp[10];
    sqlite3_stmt *stmt;
    int n;

    opool_item_t *item, *item2;

    perm_node_t *temp, *data_list;
    perm_node_t *temp2;

    data_list = NULL;
    item = NULL;
    item2 = NULL;

    //==================== Open database ================================//   
    CALL_SQLITE (open (db_path, &db));

    n = sprintf(sql ,"SELECT * FROM %s", table_name);
    sql[n] = '\0';
    CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));
    //===================== LOAD DATABASE (LEFT TO RIGHT) =====================================//
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        item = opool_get(&table->opool);
        EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
        temp = (perm_node_t *)item->data;

        ansi_copy_str(key_temp, (char *)sqlite3_column_text(stmt, 0)); 
        ansi_copy_str(temp->value, (char *)sqlite3_column_text(stmt, 1));
        data_list = (perm_node_t *)ht_get_item(&table->ht, (void *)key_temp);
        if (data_list == NULL) {
            item2 = opool_get(&table->opool);
            EXIT_IF_TRUE(item2 == NULL, "Cannot get from object pool\n");
            temp2 = (perm_node_t *)item2->data;

            ansi_copy_str(temp2->value, (char *)sqlite3_column_text(stmt, 0));

            DL_APPEND(table->key_list, temp2);

            DL_APPEND(data_list, temp);
            ht_add_item(&table->ht, (void *)sqlite3_column_text(stmt, 0), data_list);
        }
        else {
            DL_APPEND(data_list, temp);
        }
    }
}
void load_login_db(l_table_t *l_table) {
    load_db(l_table, "login");
}

void load_passphrase_db(pph_table_t *pph_table) {
    load_db(pph_table, "passphrase");
}

void load_permission_db(p_table_t *p_table) {
    sqlite3 *db;
    char sql[50], key_temp[10];
    sqlite3_stmt *stmt;
    int n;

    opool_item_t *item, *item2;

    perm_node_t *temp, *data_list;
    perm_node_t *temp2;

    data_list = NULL;
    item = NULL;
    item2 = NULL;

    //==================== Open database ================================//   
    CALL_SQLITE (open (db_path, &db));

    n = sprintf(sql ,"SELECT *FROM permission");
    sql[n] = '\0';
    CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));

    //===================== LOAD DATABASE (LEFT TO RIGHT) =====================================//
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        item = opool_get(&p_table->opool);
        EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
        temp = (perm_node_t *)item->data;

        ansi_copy_str(key_temp, (char *)sqlite3_column_text(stmt, 0)); 
        ansi_copy_str(temp->value, (char *)sqlite3_column_text(stmt, 1));
        data_list = (perm_node_t *)ht_get_item(&p_table->ht, (void *)key_temp);
        if (data_list == NULL) {
            item2 = opool_get(&p_table->opool);
            EXIT_IF_TRUE(item2 == NULL, "Cannot get from object pool\n");
            temp2 = (perm_node_t *)item2->data;

            ansi_copy_str(temp2->value, (char *)sqlite3_column_text(stmt, 0));

            DL_APPEND(p_table->key_list, temp2);

            DL_APPEND(data_list, temp);
            ht_add_item(&p_table->ht, (void *)sqlite3_column_text(stmt, 0), data_list);
        }
        else {
            DL_APPEND(data_list, temp);
        }
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        item = opool_get(&p_table->opool);
        EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
        temp = (perm_node_t *)item->data;

        ansi_copy_str(key_temp, (char *)sqlite3_column_text(stmt, 1)); 
        ansi_copy_str(temp->value, (char *)sqlite3_column_text(stmt, 0));
        data_list = (perm_node_t *)ht_get_item(&p_table->ht, (void *)key_temp);
        if (data_list == NULL) {
            item2 = opool_get(&p_table->opool);
            EXIT_IF_TRUE(item2 == NULL, "Cannot get from object pool\n");
            temp2 = (perm_node_t *)item2->data;

            ansi_copy_str(temp2->value, (char *)sqlite3_column_text(stmt, 1));

            DL_APPEND(p_table->key_list, temp2);

            DL_APPEND(data_list, temp);
            ht_add_item(&p_table->ht, (void *)sqlite3_column_text(stmt, 1), data_list);
        }
        else {
            DL_APPEND(data_list, temp);
        }
    }
}

static int value_cmp(perm_node_t *n1, perm_node_t *n2) {
    return strncmp(n1->value, n2->value, sizeof(n2->value));
}
//======================== update table ==================================//
static void add_element(perm_db_t *table, char *table_name, char *key, char *value) {
    opool_item_t *item = NULL, *item2 = NULL;

    perm_node_t *temp, *temp2, *to_return;

    perm_node_t *data_list = NULL;

    item = opool_get(&table->opool);
    EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
    temp = (perm_node_t *)item->data;

    printf("UPDATE TABLE %s VALUES('%s', '%s')\n", table_name, key, value);

    ansi_copy_str(temp->value, value);

    data_list = (perm_node_t *)ht_get_item(&table->ht, (void *)key);

    if (data_list == NULL) {
        item2 = opool_get(&table->opool);
        EXIT_IF_TRUE(item2 == NULL, "Cannot get from object pool\n");
        temp2 = (perm_node_t *)item2->data;

        ansi_copy_str(temp2->value, key);

        DL_APPEND(table->key_list, temp2);

        DL_APPEND(data_list, temp);
        ht_add_item(&table->ht, (void *)key, (void *)data_list);    
    }
    else {
        DL_SEARCH(data_list, to_return, temp, value_cmp);
        if (to_return != NULL) {
            SHOW_LOG(3, "Record: %s - %s has already in the table %s!\n", key, value, table_name);
        }
        else {
                DL_REPLACE_ELEM(data_list, data_list, temp);
                ht_add_item(&table->ht, (void *)key, (void *)data_list);    
            }
    }

}
void add_passphrase(pph_table_t *pph_table, char *node_id, char *passphrase) {
    add_element(pph_table, "passphrase", node_id, passphrase);
}

void add_active_acc(l_table_t *l_table, char *user_id, char *node_id) {
    add_element(l_table, "login", user_id, node_id);
}

static void _grant_access(p_table_t *p_table, char *user_id, char *node_id) {
    opool_item_t *item = NULL, *item2 = NULL;

    perm_node_t *temp, *temp2, *to_return;

    perm_node_t *data_list = NULL;

    item = opool_get(&p_table->opool);
    EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
    temp = (perm_node_t *)item->data;

    printf("UPDATE TABLE permission VALUES('%s', '%s')\n", user_id, node_id);

    ansi_copy_str(temp->value, node_id);

    data_list = (perm_node_t *)ht_get_item(&p_table->ht, (void *)user_id);

    if (data_list == NULL) {
        item2 = opool_get(&p_table->opool);
        EXIT_IF_TRUE(item2 == NULL, "Cannot get from object pool\n");
        temp2 = (perm_node_t *)item2->data;

        ansi_copy_str(temp2->value, user_id);

        DL_APPEND(p_table->key_list, temp2);

        DL_APPEND(data_list, temp);
        ht_add_item(&p_table->ht, (void *)user_id, (void *)data_list);    
    }
    else {
        DL_SEARCH(data_list, to_return, temp, value_cmp);
        if (to_return != NULL) {
            SHOW_LOG(3, "Record: %s - %s has already in the table permission!\n", user_id, node_id);
        }
        else {
                DL_APPEND(data_list, temp);
                ht_add_item(&p_table->ht, (void *)user_id, (void *)data_list);    
            }
    }
}
void grant_access(p_table_t *p_table, char *user_id, char *node_id) {
    _grant_access(p_table, user_id, node_id);
    _grant_access(p_table, node_id, user_id);
}

static void save_db(perm_db_t *table, char *table_name) {
    sqlite3 *db;
    char sql[255];
    sqlite3_stmt *stmt;
    int n;
    perm_node_t *temp, *entry;
    perm_node_t *temp2, *entry2;
    perm_node_t *value_list;

    //=========== DELETE ALL RECORD OF TABLE ===================//
    CALL_SQLITE (open (db_path, &db));
    n = sprintf(sql, "DELETE FROM %s", table_name);
    sql[n] = '\0';
    printf("%s\n", sql);
    CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));
    CALL_SQLITE_EXPECT (step (stmt), DONE);

    DL_FOREACH_SAFE(table->key_list, temp, entry) {
        value_list = (perm_node_t *)ht_get_item(&table->ht, (void *)temp->value);
        DL_FOREACH_SAFE(value_list, temp2, entry2) {
            n = sprintf(sql, "INSERT INTO %s VALUES ('%s', '%s')", table_name, temp->value, temp2->value);              
            printf("%s\n", sql);
            CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));
        }
        CALL_SQLITE_EXPECT (step (stmt), DONE);
    }

}

void save_login_db(l_table_t *l_table) {
    save_db(l_table, "login");
}

void save_passphrase_db(pph_table_t *pph_table) {
    save_db(pph_table, "passphrase");
}

void save_permission_db(p_table_t *p_table) {
    sqlite3 *db;
    char sql[255];
    sqlite3_stmt *stmt;
    int n;
    perm_node_t *temp, *entry;
    perm_node_t *temp2, *entry2;
    perm_node_t *value_list;

    //=========== DELETE ALL RECORD OF TABLE ===================//
    CALL_SQLITE (open (db_path, &db));
    n = sprintf(sql, "DELETE FROM permission");
    sql[n] = '\0';
    printf("%s\n", sql);
    CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));
    CALL_SQLITE_EXPECT (step (stmt), DONE);

    DL_FOREACH_SAFE(p_table->key_list, temp, entry) {
        if ( (strstr(temp->value, "OIUC") == NULL) && (strstr(temp->value, "RIUC") == NULL)) {
            value_list = (perm_node_t *)ht_get_item(&p_table->ht, (void *)temp->value);
            DL_FOREACH_SAFE(value_list, temp2, entry2) {
                n = sprintf(sql, "INSERT INTO permission VALUES ('%s', '%s')", temp->value, temp2->value);              
                printf("%s\n", sql);
                CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));
                CALL_SQLITE_EXPECT (step (stmt), DONE);
            }
        }
    }
}

static void remove_element(perm_db_t *table, char *table_name, char *key) {
    perm_node_t *data_list;
    perm_node_t *temp;
    perm_node_t *to_return;
    opool_item_t *item;

    SHOW_LOG(3, "REMOVE key %s of table %s\n", key, table_name);

    item = opool_get(&table->opool);
    EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
    temp = (perm_node_t *)item->data;

#if 1
    ansi_copy_str(temp->value, key);
    DL_SEARCH(table->key_list, to_return, temp, value_cmp);   
    if (to_return != NULL) {
        DL_DELETE(table->key_list, to_return); 
    }
    else {
        SHOW_LOG(3, "No record for key: %s\n", key);
    }
#endif
    data_list = (perm_node_t *)ht_get_item(&table->ht, (void *)key);   
    DL_DELETE(data_list, data_list);
    
    ht_add_item(&table->ht, (void *)key, NULL);
}

void remove_passphrase(pph_table_t *pph_table, char *node_id) {
    remove_element(pph_table, "passphrase", node_id);
}

void remove_active_acc(l_table_t *l_table, char *user_id) {
    remove_element(l_table, "login", user_id);
}

static void _revoke_access(p_table_t *p_table, char *key, char *value) {
    perm_node_t *data_list;
    perm_node_t *temp;
    perm_node_t *to_return;
    opool_item_t *item;

    SHOW_LOG(3, "REMOVE ACCESS: %s -> %s\n", key, value);

    item = opool_get(&p_table->opool);
    EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
    temp = (perm_node_t *)item->data;

    ansi_copy_str(temp->value, value);
    data_list = (perm_node_t *)ht_get_item(&p_table->ht, (void *)key);   
    DL_SEARCH(data_list, to_return, temp, value_cmp);   
    if (to_return != NULL) {
        DL_DELETE(data_list, to_return); 
    }
    else {
        SHOW_LOG(3, "No record for node id: %s\n", value);
    }
    
    ht_add_item(&p_table->ht, (void *)key, data_list);
}

void revoke_access(p_table_t *p_table, char *user_id, char *node_id) {
    _revoke_access(p_table, user_id, node_id);
    _revoke_access(p_table, node_id, user_id);
}

void show_record(perm_db_t *database, char *key) {
    perm_node_t *data_list;
    perm_node_t *temp, *entry;

    data_list = (perm_node_t *)ht_get_item(&database->ht, (void *)key);

    printf("Key: %s - Value:", key);

    DL_FOREACH_SAFE(data_list, temp, entry) {
        printf(" %s |", temp->value);
    }
    printf("\n");
}

void show_table(perm_db_t *db) {
    perm_node_t *temp, *entry;
    perm_node_t *temp2, *entry2;
    perm_node_t *data_list;

    DL_FOREACH_SAFE(db->key_list, temp, entry) {
        data_list = (perm_node_t *)ht_get_item(&db->ht, (void *)temp->value);
        printf("Key: %s :", temp->value);
        DL_FOREACH_SAFE(data_list, temp2, entry2) {
            printf(" %s |", temp2->value);
        }
        printf("\n");
    }
}
