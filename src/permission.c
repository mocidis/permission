#include "permission.h"

char db_path[] = "./databases/permission.db";

void show_table(db_t *db) {
#if 0
    entry_t *temp, *entry;
    int i;
    entry_t *data_list;

    for (i = 0; i < MAX_RECORD; i++) {
        if (db->key_arr[i][0] != '\0') {
            printf("key_arr[%d]: %s - ", i, db->key_arr[i]);
            data_list = (entry_t *)ht_get_item(&db->ht, (void *)db->key_arr[i]);
            DL_FOREACH_SAFE(data_list, temp, entry) {
                printf(" %s |", temp->value);
            }
            printf("\n");
        }
    }
#endif
}

void perm_init(db_t *database, pj_pool_t *pool) {
    opool_init(&database->opool, 100, sizeof(entry_t), pool);
}

void load_login_db(login_table_t *l_table) {
    sqlite3 *db;
    char sql[50], key_temp[10];
    sqlite3_stmt *stmt;
    int n;

    opool_item_t *item, *item2;

    entry_t *temp, *data_list;
    entry_t *temp2;

    data_list = NULL;
    item = NULL;
    item2 = NULL;

    //==================== Open database ================================//   
    CALL_SQLITE (open (db_path, &db));

    n = sprintf(sql ,"SELECT * FROM login");
    sql[n] = '\0';
    CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));

    //===================== LOAD DATABASE (LEFT TO RIGHT) =====================================//
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        item = opool_get(&l_table->opool);
        EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
        temp = (entry_t *)item->data;

        ansi_copy_str(key_temp, (char *)sqlite3_column_text(stmt, 0)); 
        ansi_copy_str(temp->value, (char *)sqlite3_column_text(stmt, 1));
        data_list = (entry_t *)ht_get_item(&l_table->ht, (void *)key_temp);
        if (data_list == NULL) {
            item2 = opool_get(&l_table->opool);
            EXIT_IF_TRUE(item2 == NULL, "Cannot get from object pool\n");
            temp2 = (entry_t *)item2->data;

            ansi_copy_str(temp2->value, (char *)sqlite3_column_text(stmt, 0));

            DL_APPEND(l_table->key_list, temp2);

            DL_APPEND(data_list, temp);
            ht_add_item(&l_table->ht, (void *)sqlite3_column_text(stmt, 0), data_list);
        }
        else {
            DL_APPEND(data_list, temp);
        }
    }
}

void load_passphrase_db(passphrase_table_t *pph_table) {
    sqlite3 *db;
    char sql[50], key_temp[10];
    sqlite3_stmt *stmt;
    int n;

    opool_item_t *item, *item2;

    entry_t *temp, *data_list;
    entry_t *temp2;

    data_list = NULL;
    item = NULL;
    item2 = NULL;

    //==================== Open database ================================//   
    CALL_SQLITE (open (db_path, &db));

    n = sprintf(sql ,"SELECT * FROM passphrase");
    sql[n] = '\0';
    CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));
    //===================== LOAD DATABASE (LEFT TO RIGHT) =====================================//
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        item = opool_get(&pph_table->opool);
        EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
        temp = (entry_t *)item->data;

        ansi_copy_str(key_temp, (char *)sqlite3_column_text(stmt, 0)); 
        ansi_copy_str(temp->value, (char *)sqlite3_column_text(stmt, 1));
        data_list = (entry_t *)ht_get_item(&pph_table->ht, (void *)key_temp);
        if (data_list == NULL) {
            item2 = opool_get(&pph_table->opool);
            EXIT_IF_TRUE(item2 == NULL, "Cannot get from object pool\n");
            temp2 = (entry_t *)item2->data;

            ansi_copy_str(temp2->value, (char *)sqlite3_column_text(stmt, 0));

            DL_APPEND(pph_table->key_list, temp2);

            DL_APPEND(data_list, temp);
            ht_add_item(&pph_table->ht, (void *)sqlite3_column_text(stmt, 0), data_list);
        }
        else {
            DL_APPEND(data_list, temp);
        }
    }
}

void load_permission_db(permission_table_t *p_table) {
    sqlite3 *db;
    char sql[50], key_temp[10];
    sqlite3_stmt *stmt;
    int n;

    opool_item_t *item, *item2;

    entry_t *temp, *data_list;
    entry_t *temp2;

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
        temp = (entry_t *)item->data;

        ansi_copy_str(key_temp, (char *)sqlite3_column_text(stmt, 0)); 
        ansi_copy_str(temp->value, (char *)sqlite3_column_text(stmt, 1));
        data_list = (entry_t *)ht_get_item(&p_table->ht, (void *)key_temp);
        if (data_list == NULL) {
            item2 = opool_get(&p_table->opool);
            EXIT_IF_TRUE(item2 == NULL, "Cannot get from object pool\n");
            temp2 = (entry_t *)item2->data;

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
        temp = (entry_t *)item->data;

        ansi_copy_str(key_temp, (char *)sqlite3_column_text(stmt, 1)); 
        ansi_copy_str(temp->value, (char *)sqlite3_column_text(stmt, 0));
        data_list = (entry_t *)ht_get_item(&p_table->ht, (void *)key_temp);
        if (data_list == NULL) {
            DL_APPEND(data_list, temp);
            ht_add_item(&p_table->ht, (void *)sqlite3_column_text(stmt, 1), data_list);
        }
        else {
            DL_APPEND(data_list, temp);
        }
    }
}

static int value_cmp(entry_t *n1, entry_t *n2) {
    return strncmp(n1->value, n2->value, sizeof(n2->value));
}

//======================== update table ==================================//
void add_passphrase(passphrase_table_t *pph_table, char *node_id, char *passphrase) {
    opool_item_t *item = NULL, *item2 = NULL;

    entry_t *temp, *temp2, *to_return;

    entry_t *data_list = NULL;

    item = opool_get(&pph_table->opool);
    EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
    temp = (entry_t *)item->data;

    printf("UPDATE TABLE passphrase VALUES('%s', '%s')\n", node_id, passphrase);

    ansi_copy_str(temp->value, passphrase);

    data_list = (entry_t *)ht_get_item(&pph_table->ht, (void *)node_id);

    if (data_list == NULL) {
        item2 = opool_get(&pph_table->opool);
        EXIT_IF_TRUE(item2 == NULL, "Cannot get from object pool\n");
        temp2 = (entry_t *)item2->data;

        ansi_copy_str(temp2->value, node_id);

        DL_APPEND(pph_table->key_list, temp2);

        DL_APPEND(data_list, temp);
        ht_add_item(&pph_table->ht, (void *)node_id, (void *)data_list);    
    }
    else {
        DL_SEARCH(data_list, to_return, temp, value_cmp);
        if (to_return != NULL) {
            SHOW_LOG(3, "Record: %s - %s has already in the table passphrase!\n", node_id, passphrase);
        }
        else {
                DL_REPLACE_ELEM(data_list, data_list, temp);
                ht_add_item(&database->ht, (void *)node_id, (void *)data_list);    
            }
    }
}

void add_active_acc(login_table_t *l_table, char *user_id, char *node_id) {
    opool_item_t *item = NULL, *item2 = NULL;

    entry_t *temp, *temp2, *to_return;

    entry_t *data_list = NULL;

    item = opool_get(&l_table->opool);
    EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
    temp = (entry_t *)item->data;

    printf("UPDATE TABLE login VALUES('%s', '%s')\n", user_id, node_id);

    ansi_copy_str(temp->value, node_id);

    data_list = (entry_t *)ht_get_item(&l_table->ht, (void *)user_id);

    if (data_list == NULL) {
        item2 = opool_get(&l_table->opool);
        EXIT_IF_TRUE(item2 == NULL, "Cannot get from object pool\n");
        temp2 = (entry_t *)item2->data;

        ansi_copy_str(temp2->value, user_id);

        DL_APPEND(l_table->key_list, temp2);

        DL_APPEND(data_list, temp);
        ht_add_item(&l_table->ht, (void *)user_id, (void *)data_list);    
    }
    else {
        DL_SEARCH(data_list, to_return, temp, value_cmp);
        if (to_return != NULL) {
            SHOW_LOG(3, "Record: %s - %s has already in the table login!\n", user_id, node_id);
        }
        else {
                DL_REPLACE_ELEM(data_list, data_list, temp);
                ht_add_item(&l_table->ht, (void *)user_id, (void *)data_list);    
            }
    }
}
void grant_access(permission_table_t *p_table, char *user_id, char *node_id) {
    opool_item_t *item = NULL, *item2 = NULL;

    entry_t *temp, *temp2, *to_return;

    entry_t *data_list = NULL;

    item = opool_get(&p_table->opool);
    EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
    temp = (entry_t *)item->data;

    printf("UPDATE TABLE permission VALUES('%s', '%s')\n", user_id, node_id);

    ansi_copy_str(temp->value, node_id);

    data_list = (entry_t *)ht_get_item(&p_table->ht, (void *)user_id);

    if (data_list == NULL) {
        item2 = opool_get(&p_table->opool);
        EXIT_IF_TRUE(item2 == NULL, "Cannot get from object pool\n");
        temp2 = (entry_t *)item2->data;

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

    //REFLEX !!!
#if 1
    printf("UPDATE TABLE permission VALUES('%s', '%s')\n", node_id, user_id);
    data_list = NULL;
    to_return = NULL;

    item = opool_get(&database->opool);
    EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
    temp = (entry_t *)item->data;

    ansi_copy_str(temp->value, user_id);

    data_list = (entry_t *)ht_get_item(&p_table->ht, (void *)node_id);

    if (data_list == NULL) {
        item2 = opool_get(&p_table->opool);
        EXIT_IF_TRUE(item2 == NULL, "Cannot get from object pool\n");
        temp2 = (entry_t *)item2->data;

        ansi_copy_str(temp2->value, node_id);

        DL_APPEND(p_table->key_list, temp2);

        DL_APPEND(data_list, temp);
        ht_add_item(&p_table->ht, (void *)node_id, (void *)data_list);    
    }
    else {
        DL_SEARCH(data_list, to_return, temp, value_cmp);
        if (to_return != NULL) {
            SHOW_LOG(3, "Record: %s - %s has already in the table passphrase!\n", node_id, user_id);
        }
        else {
                DL_APPEND(data_list, temp);
                ht_add_item(&p_table->ht, (void *)node_id, (void *)data_list);    
            }
    }
#endif
}
#if 1
void save_login_database(login_table_t *l_table) {
    sqlite3 *db;
    char sql[255];
    sqlite3_stmt *stmt;
    int n;
    entry_t *temp, *entry;
    entry_t *temp2, *entry2;
    entry_t *value_list;

    //=========== DELETE ALL RECORD OF TABLE ===================//
    CALL_SQLITE (open (db_path, &db));
    n = sprintf(sql, "DELETE FROM login ");
    sql[n] = '\0';
    printf("%s\n", sql);
    CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));
    CALL_SQLITE_EXPECT (step (stmt), DONE);

    DL_FOREACH_SAFE(database->key_list, temp, entry) {
        value_list = (entry_t *)ht_get_item(&l_table->ht, (void *)temp->value);
        DL_FOREACH_SAFE(value_list, temp2, entry2) {
            n = sprintf(sql, "INSERT INTO login VALUES ('%s', '%s')", temp->value, temp2->value);              
            printf("%s\n", sql);
            CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));
        }
        CALL_SQLITE_EXPECT (step (stmt), DONE);
    }
}
#endif

#if 1
void save_passphrase_database(passphrase_table_t *pph_table) {
    sqlite3 *db;
    char sql[255];
    sqlite3_stmt *stmt;
    int n;
    entry_t *temp, *entry;
    entry_t *temp2, *entry2;
    entry_t *value_list;

    //=========== DELETE ALL RECORD OF TABLE ===================//
    CALL_SQLITE (open (db_path, &db));
    n = sprintf(sql, "DELETE FROM passphrase");
    sql[n] = '\0';
    printf("%s\n", sql);
    CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));
    CALL_SQLITE_EXPECT (step (stmt), DONE);

    DL_FOREACH_SAFE(database->key_list, temp, entry) {
        value_list = (entry_t *)ht_get_item(&pph_table->ht, (void *)temp->value);
        DL_FOREACH_SAFE(value_list, temp2, entry2) {
            n = sprintf(sql, "INSERT INTO passphrase VALUES ('%s', '%s')", temp->value, temp2->value);              
            printf("%s\n", sql);
            CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));
        }
        CALL_SQLITE_EXPECT (step (stmt), DONE);
    }
}
#endif

#if 1
void save_permission_database(permission_table_t *p_table) {
    sqlite3 *db;
    char sql[255];
    sqlite3_stmt *stmt;
    int n;
    entry_t *temp, *entry;
    entry_t *temp2, *entry2;
    entry_t *value_list;

    //=========== DELETE ALL RECORD OF TABLE ===================//
    CALL_SQLITE (open (db_path, &db));
    n = sprintf(sql, "DELETE FROM permission");
    sql[n] = '\0';
    printf("%s\n", sql);
    CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));
    CALL_SQLITE_EXPECT (step (stmt), DONE);

    DL_FOREACH_SAFE(database->key_list, temp, entry) {
        if ( (strstr(temp->value, "OIUC") == NULL) && (strstr(temp->value, "RIUC") == NULL)) {
            value_list = (entry_t *)ht_get_item(&p_table->ht, (void *)temp->value);
            DL_FOREACH_SAFE(value_list, temp2, entry2) {
                n = sprintf(sql, "INSERT INTO permission VALUES ('%s', '%s')", temp->value, temp2->value);              
                printf("%s\n", sql);
                CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));
                CALL_SQLITE_EXPECT (step (stmt), DONE);
            }
        }
    }
}
#endif

void remove_passphrase(passphrase_table_t *pph_table, char *node_id) {
    entry *temp;
    entry *data_list;
    
    ansi_copy_str(temp->value, node_id);
    data_list = (entry_t *)ht_get_item(&pph_table->ht, (void *)node_id);
    
    //List has only one element
    DL_DELETE(data_list, data_list);
    
    DL_DELETE(pph_table->key_list, temp);
}
void revoke_access(permission_table_t *p_table, char *user_id, char *node_id) {
    entry *temp, *temp2;
    entry *data_list;
    
    ansi_copy_str(temp->value, user_id);
    ansi_copy_str(temp2->value, node_id);
    
    data_list = (entry_t *)ht_get_item(&p_table->ht, (void *)user_id);
    
    DL_DELETE(data_list, temp2);
    DL_DELETE(pph_table->key_list, temp);
}
void remove_active_acc(login_table_t *l_table, char *user_id) {
    entry *temp;
    entry *data_list;
    
    ansi_copy_str(temp->value, user_id);
    data_list = (entry_t *)ht_get_item(&l_table->ht, (void *)user_id);
   
   //List has only one element 
    DL_DELETE(data_list, data_list);
    
    DL_DELETE(l_table->key_list, temp);
}

void show_record(db_t *database, char *key) {
    entry_t *data_list;
    entry_t *temp, *entry;

    data_list = (entry_t *)ht_get_item(&database->ht, (void *)key);

    printf("Key: %s - Value:", key);

    DL_FOREACH_SAFE(data_list, temp, entry) {
        printf(" %s |", temp->value);
    }
    printf("\n");
}
