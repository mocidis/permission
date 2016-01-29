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

void load_database(opool_t *opool, db_t *database, char *table_name) {
    sqlite3 *db;
    char sql[50], key_temp[10];
    sqlite3_stmt *stmt;
    int i, idx, n;

    opool_item_t *item = NULL;

    entry_t *temp, *entry;
 
    //==================== Open database ================================//   
    CALL_SQLITE (open (db_path, &db));

    n = sprintf(sql ,"SELECT *FROM %s", table_name);
    sql[n] = '\0';
    CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));

    //==================== Delele previous data of table ========================//   
    for (i = 0; i < MAX_RECORD; i++) {
        DL_FOREACH_SAFE(database->data_list[i], temp, entry) {
            DL_DELETE(database->data_list[i], temp);
        }
        strcpy(database->key_arr[i], "");
    }
    //===================== LOAD DATABASE (LEFT TO RIGHT) =====================================//
    i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        item = opool_get(opool);
        EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
        temp = (entry_t *)item->data;

        ansi_copy_str(key_temp, (char *)sqlite3_column_text(stmt, 0)); 
        ansi_copy_str(temp->value, (char *)sqlite3_column_text(stmt, 1));
        idx = get_idx(database->key_arr, key_temp);
        if (idx < 0) {
            ansi_copy_str(database->key_arr[i], key_temp);
            DL_APPEND(database->data_list[i], temp);
            ht_add_item(&database->ht, (void *)database->key_arr[i], (void *)database->data_list[i]);
            i++;
        }
        else {
            DL_APPEND(database->data_list[idx], temp);
        }
    }

    //============= FOR PERMISSION TABLE ONLY (LOAD AGAIN BUT NOW IS RIGHT TO LEFT) ====================//
#if 1
    if (0 == strcmp(table_name, "permission")) {
        i = get_array_length(database->key_arr);
        //====================================================//   
        CALL_SQLITE (open (db_path, &db));
        n = sprintf(sql ,"SELECT *FROM %s", table_name);
        CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));
        //====================================================//   
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            item = opool_get(opool);
            EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
            temp = (entry_t *)item->data;

            ansi_copy_str(key_temp, (char *)sqlite3_column_text(stmt, 1));       
            ansi_copy_str(temp->value, (char *)sqlite3_column_text(stmt, 0));

            idx = get_idx(database->key_arr, key_temp);
            if (idx < 0) {
                ansi_copy_str(database->key_arr[i], key_temp);
                DL_APPEND(database->data_list[i], temp);
                ht_add_item(&database->ht, (void *)database->key_arr[i], (void *)database->data_list[i]);
                i++;
            }
            else {
                DL_APPEND(database->data_list[idx], temp);
            }
        }
    }
#endif
}

//============================ update_database =======================================//
void update_database(opool_t *opool, db_t *database, char *table_name) {
    sqlite3 *db;
    char sql[255];
    sqlite3_stmt *stmt;
    int i, n;
    entry_t *temp, *entry;
    entry_t *data_list;

    //=========== DELETE ALL RECORD OF TABLE ===================//
    CALL_SQLITE (open (db_path, &db));
    n = sprintf(sql, "DELETE FROM %s ", table_name);
    sql[n] = '\0';
    printf("%s\n", sql);
    CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));
    CALL_SQLITE_EXPECT (step (stmt), DONE);

#if 1
    int length = get_array_length(database->key_arr);
    for (i = 0 ; i < length; i++) {
        data_list = (entry_t *)ht_get_item(&database->ht, (void *)database->key_arr[i]);
        DL_FOREACH_SAFE(data_list, temp, entry) {
            n = sprintf(sql, "INSERT INTO %s VALUES ('%s', '%s')", table_name, database->key_arr[i], temp->value);              
            printf("%s\n", sql);
            CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));
        }
    CALL_SQLITE_EXPECT (step (stmt), DONE);
    }
#endif
}

void update_permission_database(opool_t *opool, db_t *database) {
    sqlite3 *db;
    char sql[255];
    sqlite3_stmt *stmt;
    int i, n;
    entry_t *temp, *entry;
    entry_t *data_list;

    //=========== DELETE ALL RACORD OF TABLE ===================//
    CALL_SQLITE (open (db_path, &db));
    n = sprintf(sql, "DELETE FROM permission");
    sql[n]='\0';
    printf("%s\n", sql);
    CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));

    CALL_SQLITE_EXPECT (step (stmt), DONE);
#if 1
    int length = get_array_length(database->key_arr);

    for (i = 0 ; i < length; i++) {
        if ( (strstr(database->key_arr[i], "OIUC") == NULL) && (strstr(database->key_arr[i], "RIUC") == NULL)) {
            //printf("key_arr[%d]: %s = ", i, database->key_arr[i]);
            data_list = (entry_t *)ht_get_item(&database->ht, (void *)database->key_arr[i]);
            DL_FOREACH_SAFE(data_list, temp, entry) {
                //printf("%s |", temp->value);
                n = sprintf(sql, "INSERT INTO permission VALUES ('%s', '%s')", database->key_arr[i], temp->value);              
                printf("%s\n", sql);
                CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));
                CALL_SQLITE_EXPECT (step (stmt), DONE);
            }
            printf("\n");
        }
    }
#endif
}
static int value_cmp(entry_t *n1, entry_t *n2) {
    return strncmp(n1->value, n2->value, sizeof(n2->value));
}

//======================== update table ==================================//
#if 1
void update_table(opool_t *opool, db_t *database, char *table_name, char *field_1, char *field_2) {
    int length, idx;
    opool_item_t *item = NULL;

    entry_t *temp, *to_return;

    item = opool_get(opool);
    EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
    temp = (entry_t *)item->data;

    printf("UPDATE TABLE %s VALUES('%s', '%s')\n", table_name, field_1, field_2);

    ansi_copy_str(temp->value, field_2);

    idx = get_idx(database->key_arr, field_1);
    if (idx < 0) {
        printf("field_1: %s\n", field_1);
        length = get_array_length(database->key_arr);
        DL_APPEND(database->data_list[length], temp);
        ansi_copy_str(database->key_arr[length], field_1);
        ht_add_item(&database->ht, (void *)database->key_arr[length], (void *)database->data_list[length]);    
    }
    else {
        DL_SEARCH(database->data_list[idx], to_return, temp, value_cmp);
        if (to_return != NULL) {
            SHOW_LOG(3, "Record: %s - %s has already in the table %s!\n", field_1, field_2, table_name);
        }
        else {
                DL_REPLACE_ELEM(database->data_list[idx], database->data_list[idx], temp);
                ht_add_item(&database->ht, (void *)database->key_arr[idx], (void *)database->data_list[idx]);    
            }
    }
}

void update_permission_table(opool_t *opool, db_t *database, char *table_name, char *field_1, char *field_2) {
    int length, idx;
    opool_item_t *item = NULL;
    opool_item_t *item2 = NULL;

    entry_t *temp, *to_return, *temp2;

    item = opool_get(opool);
    EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
    temp = (entry_t *)item->data;

    item2 = opool_get(opool);
    EXIT_IF_TRUE(item2 == NULL, "Cannot get from object pool\n");
    temp2 = (entry_t *)item2->data;

    printf("UPDATE TABLE %s VALUES('%s', '%s')\n", table_name, field_1, field_2);

    ansi_copy_str(temp->value, field_2);

    idx = get_idx(database->key_arr, field_1);
    if (idx < 0) {
        length = get_array_length(database->key_arr);
        DL_APPEND(database->data_list[length], temp);
        ansi_copy_str(database->key_arr[length], field_1);
        ht_add_item(&database->ht, (void *)database->key_arr[length], (void *)database->data_list[length]);    
    }
    else {
        DL_SEARCH(database->data_list[idx], to_return, temp, value_cmp);
        if (to_return != NULL) {
            SHOW_LOG(3, "Record: %s - %s has already in the table %s!\n", field_1, field_2, table_name);
        }
        else {
                DL_APPEND(database->data_list[idx], temp);
            }
    }
    //For permission talbe only
#if 1
    ansi_copy_str(temp2->value, field_1);
    idx = get_idx(database->key_arr, field_2);
    if (idx < 0) {
        length = get_array_length(database->key_arr);
        DL_APPEND(database->data_list[length], temp2);
        ansi_copy_str(database->key_arr[length], field_2);
        ht_add_item(&database->ht, (void *)database->key_arr[length], (void *)database->data_list[length]);    
    }
    else {
        DL_SEARCH(database->data_list[idx], to_return, temp2, value_cmp);
        if (to_return != NULL) {
            SHOW_LOG(3, "Record: %s - %s has already in the table %s!\n", field_2, field_1, table_name);
        }
        else {
            DL_APPEND(database->data_list[idx], temp2);
        }
    }
#endif
}
#endif
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
