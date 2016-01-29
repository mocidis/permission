#include "permission.h"

char db_path[] = "./databases/permission.db";

int get_idx(char key_arr[][255], char *key) {
    int i;
    for (i = 0; i < MAX_RECORD; i++) {
        if (0 == strcmp(key_arr[i], key)) {
            return i;
        }
    }
    return -1;
}

static int get_array_length(char key_arr[][255]) {
    int i, length;

    length = 0;
    for (i = 0; i < MAX_RECORD; i++) {
        if (key_arr[i][0] != '\0') {
            length++;
        }
    }
    return length;
}
void show_table(db_t *db) {
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
}

void load_login_db(opool_t *opool, db_t *database) {
    sqlite3 *db;
    char sql[50], key_temp[10];
    sqlite3_stmt *stmt;
    int i, n;

    opool_item_t *item, *item2;

    entry_t *temp, *entry, *data_list;
    entry_t *temp2, *entry2;

    data_list = NULL;
    item = NULL;
    item2 = NULL;

    //==================== Open database ================================//   
    CALL_SQLITE (open (db_path, &db));

    n = sprintf(sql ,"SELECT * FROM login");
    sql[n] = '\0';
    CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));

    //==================== Delele previous data of table ========================//   
#if 0
    for (i = 0; i < MAX_RECORD; i++) {
        DL_FOREACH_SAFE(database->data_list[i], temp, entry) {
            DL_DELETE(database->data_list[i], temp);
        }
    }
#endif
    //===================== LOAD DATABASE (LEFT TO RIGHT) =====================================//
    i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        item = opool_get(opool);
        EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
        temp = (entry_t *)item->data;

        ansi_copy_str(key_temp, (char *)sqlite3_column_text(stmt, 0)); 
        ansi_copy_str(temp->value, (char *)sqlite3_column_text(stmt, 1));
        data_list = (entry_t *)ht_get_item(&database->ht, (void *)key_temp);
        if (data_list == NULL) {
            DL_APPEND(data_list, temp);
            ht_add_item(&database->ht, (void *)sqlite3_column_text(stmt, 0), data_list);
        }
        else {
            DL_APPEND(data_list, temp);
        }
    }
}
void load_passphrase_db(opool_t *opool, db_t *database) {
    sqlite3 *db;
    char sql[50], key_temp[10];
    sqlite3_stmt *stmt;
    int i, n;

    opool_item_t *item, *item2;

    entry_t *temp, *entry, *data_list;
    entry_t *temp2, *entry2;

    data_list = NULL;
    item = NULL;
    item2 = NULL;

    //==================== Open database ================================//   
    CALL_SQLITE (open (db_path, &db));

    n = sprintf(sql ,"SELECT *FROM passphrase");
    sql[n] = '\0';
    CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));

    //==================== Delele previous data of table ========================//   
#if 0
    for (i = 0; i < MAX_RECORD; i++) {
        DL_FOREACH_SAFE(database->data_list[i], temp, entry) {
            DL_DELETE(database->data_list[i], temp);
        }
    }
#endif
    //===================== LOAD DATABASE (LEFT TO RIGHT) =====================================//
    i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        item = opool_get(opool);
        EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
        temp = (entry_t *)item->data;

        ansi_copy_str(key_temp, (char *)sqlite3_column_text(stmt, 0)); 
        ansi_copy_str(temp->value, (char *)sqlite3_column_text(stmt, 1));
        data_list = (entry_t *)ht_get_item(&database->ht, (void *)key_temp);
        if (data_list == NULL) {
            DL_APPEND(data_list, temp);
            ht_add_item(&database->ht, (void *)sqlite3_column_text(stmt, 0), data_list);
        }
        else {
            DL_APPEND(data_list, temp);
        }
    }
}

void load_permission_db(opool_t *opool, db_t *database) {
    sqlite3 *db;
    char sql[50], key_temp[10];
    sqlite3_stmt *stmt;
    int i, n;

    opool_item_t *item, *item2;

    entry_t *temp, *entry, *data_list;
    entry_t *temp2, *entry2;

    data_list = NULL;
    item = NULL;
    item2 = NULL;

    //==================== Open database ================================//   
    CALL_SQLITE (open (db_path, &db));

    n = sprintf(sql ,"SELECT *FROM permission");
    sql[n] = '\0';
    CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));

    //===================== LOAD DATABASE (LEFT TO RIGHT) =====================================//
    i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        item = opool_get(opool);
        EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
        temp = (entry_t *)item->data;

        ansi_copy_str(key_temp, (char *)sqlite3_column_text(stmt, 0)); 
        ansi_copy_str(temp->value, (char *)sqlite3_column_text(stmt, 1));
        data_list = (entry_t *)ht_get_item(&database->ht, (void *)key_temp);
        if (data_list == NULL) {
            DL_APPEND(data_list, temp);
            ht_add_item(&database->ht, (void *)sqlite3_column_text(stmt, 0), data_list);
        }
        else {
            DL_APPEND(data_list, temp);
        }
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        item = opool_get(opool);
        EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
        temp = (entry_t *)item->data;

        ansi_copy_str(key_temp, (char *)sqlite3_column_text(stmt, 1)); 
        ansi_copy_str(temp->value, (char *)sqlite3_column_text(stmt, 0));
        data_list = (entry_t *)ht_get_item(&database->ht, (void *)key_temp);
        if (data_list == NULL) {
            DL_APPEND(data_list, temp);
            ht_add_item(&database->ht, (void *)sqlite3_column_text(stmt, 1), data_list);
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
void update_passphrase_table(opool_t *opool, db_t *database, char *field_1, char *field_2) {
    int length, idx;
    opool_item_t *item = NULL;

    entry_t *temp, *to_return;

    entry_t *data_list = NULL;

    item = opool_get(opool);
    EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
    temp = (entry_t *)item->data;

    printf("UPDATE TABLE passphrase VALUES('%s', '%s')\n", field_1, field_2);

    ansi_copy_str(temp->value, field_2);

    data_list = (entry_t *)ht_get_item(&database->ht, (void *)field_1);

    if (data_list == NULL) {
        DL_APPEND(data_list, temp);
        ht_add_item(&database->ht, (void *)field_1, (void *)data_list);    
    }
    else {
        DL_SEARCH(data_list, to_return, temp, value_cmp);
        if (to_return != NULL) {
            SHOW_LOG(3, "Record: %s - %s has already in the table passphrase!\n", field_1, field_2);
        }
        else {
                DL_REPLACE_ELEM(data_list, data_list, temp);
                ht_add_item(&database->ht, (void *)field_1, (void *)data_list);    
            }
    }
}

void update_login_table(opool_t *opool, db_t *database, char *field_1, char *field_2) {
    int length, idx;
    opool_item_t *item = NULL;

    entry_t *temp, *to_return;

    entry_t *data_list = NULL;

    item = opool_get(opool);
    EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
    temp = (entry_t *)item->data;

    printf("UPDATE TABLE login VALUES('%s', '%s')\n", field_1, field_2);

    ansi_copy_str(temp->value, field_2);

    data_list = (entry_t *)ht_get_item(&database->ht, (void *)field_1);

    if (data_list == NULL) {
        DL_APPEND(data_list, temp);
        ht_add_item(&database->ht, (void *)field_1, (void *)data_list);    
    }
    else {
        DL_SEARCH(data_list, to_return, temp, value_cmp);
        if (to_return != NULL) {
            SHOW_LOG(3, "Record: %s - %s has already in the table login!\n", field_1, field_2);
        }
        else {
                DL_REPLACE_ELEM(data_list, data_list, temp);
                ht_add_item(&database->ht, (void *)field_1, (void *)data_list);    
            }
    }
}
void update_permission_table(opool_t *opool, db_t *database, char *field_1, char *field_2) {
    int length, idx;
    opool_item_t *item = NULL;

    entry_t *temp, *to_return;

    entry_t *data_list = NULL;

    item = opool_get(opool);
    EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
    temp = (entry_t *)item->data;

    printf("UPDATE TABLE permission VALUES('%s', '%s')\n", field_1, field_2);

    ansi_copy_str(temp->value, field_2);

    data_list = (entry_t *)ht_get_item(&database->ht, (void *)field_1);

    if (data_list == NULL) {
        printf("field_1: %s\n", field_1);
        DL_APPEND(data_list, temp);
        ht_add_item(&database->ht, (void *)field_1, (void *)data_list);    
    }
    else {
        DL_SEARCH(data_list, to_return, temp, value_cmp);
        if (to_return != NULL) {
            SHOW_LOG(3, "Record: %s - %s has already in the table permission!\n", field_1, field_2);
        }
        else {
                DL_APPEND(data_list, temp);
                ht_add_item(&database->ht, (void *)field_1, (void *)data_list);    
            }
    }

    //REFLEX !!!
#if 1
    printf("UPDATE TABLE permission VALUES('%s', '%s')\n", field_2, field_1);
    data_list = NULL;
    to_return = NULL;

    item = opool_get(opool);
    EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
    temp = (entry_t *)item->data;

    ansi_copy_str(temp->value, field_1);

    data_list = (entry_t *)ht_get_item(&database->ht, (void *)field_2);

    if (data_list == NULL) {
        DL_APPEND(data_list, temp);
        ht_add_item(&database->ht, (void *)field_2, (void *)data_list);    
    }
    else {
        DL_SEARCH(data_list, to_return, temp, value_cmp);
        if (to_return != NULL) {
            SHOW_LOG(3, "Record: %s - %s has already in the table passphrase!\n", field_2, field_1);
        }
        else {
                DL_APPEND(data_list, temp);
                ht_add_item(&database->ht, (void *)field_2, (void *)data_list);    
            }
    }
#endif
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
