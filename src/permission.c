#include "permission.h"

char db_path[] = "./databases/permission.db";

static int find_idx_by_key(char key_arr[][255], char *key) {
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
    printf("Array length: %d\n", length);
    return length;
}

static void print_array(char key_arr[][255], entry_t *data_list[]) {
    entry_t *temp, *entry;
    int i;

    for (i = 0; i < MAX_RECORD; i++) {
        printf("key_arr[%d]: %s - ", i, key_arr[i]);
        DL_FOREACH_SAFE(data_list[i], temp, entry) {
            printf(" %s |", temp->value);
        }
        printf("\n");
    }
}

void load_database(opool_t *opool, hash_table_t *ht, char *table_name, db_t *database) {
    sqlite3 *db;
    char sql[50], key[10], key_temp[10];
    sqlite3_stmt *stmt;
    int i, idx, n;

    opool_item_t *item = NULL;

    entry_t *temp, *entry;
 
    //==================== Open database ================================//   
    CALL_SQLITE (open (db_path, &db));

    n = sprintf(sql ,"SELECT *FROM %s", table_name);
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
        idx = find_idx_by_key(database->key_arr, key_temp);
        //printf("key: %s - idx: %d\n", key_temp, idx);      
        if (idx < 0) {
            ansi_copy_str(database->key_arr[i], key_temp);
            DL_APPEND(database->data_list[i], temp);
            ht_add_item(ht, (void *)database->key_arr[i], (void *)database->data_list[i]);
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

            idx = find_idx_by_key(database->key_arr, key_temp);
            if (idx < 0) {
                ansi_copy_str(database->key_arr[i], key_temp);
                DL_APPEND(database->data_list[i], temp);
                ht_add_item(ht, (void *)database->key_arr[i], (void *)database->data_list[i]);
                i++;
            }
            else {
                DL_APPEND(database->data_list[idx], temp);
            }
        }
    }
#endif
    print_array(database->key_arr, database->data_list);
    int length = get_array_length(database->key_arr);
}

//============================ update_database =======================================//
void update_database(opool_t *opool, hash_table_t *ht, char *table_name, db_t *database) {
    sqlite3 *db;
    char sql[255], key[10], key_temp[10];
    char column1[10], column2[10];
    sqlite3_stmt *stmt;
    int i, n;
    entry_t *temp, *temp2, *entry;
    entry_t *data_list;

    //=========== DELETE ALL RACORD OF TABLE ===================//
    CALL_SQLITE (open (db_path, &db));
    n = sprintf(sql, "DELETE FROM %s ", table_name);
    printf("%s\n", sql);
    CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));

    CALL_SQLITE_EXPECT (step (stmt), DONE);
#if 1
    int length = get_array_length(database->key_arr);
    for (i = 0 ; i < length; i++) {
        data_list = get_data_list(ht, database->key_arr[i]);
        DL_FOREACH_SAFE(data_list, temp, entry) {
            n = sprintf(sql, "INSERT INTO %s VALUES ('%s', '%s')", table_name, database->key_arr[i], temp->value);              
            printf("%s\n", sql);
            CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));
        }
    CALL_SQLITE_EXPECT (step (stmt), DONE);
    }
#endif
}

void update_permission_database(opool_t *opool, hash_table_t *ht, db_t *database) {
    sqlite3 *db;
    char sql[255], key[10], key_temp[10];
    char column1[10], column2[10];
    sqlite3_stmt *stmt;
    int i, n;
    entry_t *temp, *temp2, *entry;
    entry_t *data_list;

    print_array(database->key_arr, database->data_list);

    //=========== DELETE ALL RACORD OF TABLE ===================//
    CALL_SQLITE (open (db_path, &db));
    n = sprintf(sql, "DELETE FROM permission");
    printf("%s\n", sql);
    CALL_SQLITE (prepare_v2 (db, sql, strlen (sql) + 1, &stmt, NULL));

    CALL_SQLITE_EXPECT (step (stmt), DONE);
#if 1
    int length = get_array_length(database->key_arr);
    
    print_array(database->key_arr, database->data_list);

    for (i = 0 ; i < length; i++) {
        if ( (strstr(database->key_arr[i], "OIUC") == NULL) && (strstr(database->key_arr[i], "RIUC") == NULL)) {
            //printf("key_arr[%d]: %s = ", i, database->key_arr[i]);
            //data_list = get_data_list(ht, database->key_arr[i]);
            DL_FOREACH_SAFE(database->data_list[i], temp, entry) {
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
//======================== update table ==================================//
static int value_cmp(entry_t *n1, entry_t *n2) {
    return strncmp(n1->value, n2->value, sizeof(n2->value));
}

#if 1
void update_table(opool_t *opool, hash_table_t *ht, char *table_name, db_t *db, char *field_1, char *field_2) {
    int length, idx;
    opool_item_t *item = NULL;
    opool_item_t *item2 = NULL;

    entry_t *temp, *to_return, *entry, *temp2;

    item = opool_get(opool);
    EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
    temp = (entry_t *)item->data;

    item2 = opool_get(opool);
    EXIT_IF_TRUE(item2 == NULL, "Cannot get from object pool\n");
    temp2 = (entry_t *)item2->data;

    printf("UPDATE TABLE %s VALUES('%s', '%s')\n", table_name, field_1, field_2);

    ansi_copy_str(temp->value, field_2);

    idx = find_idx_by_key(db->key_arr, field_1);
    if (idx < 0) {
        printf("field_1: %s\n", field_1);
        length = get_array_length(db->key_arr);
        DL_APPEND(db->data_list[length], temp);
        ansi_copy_str(db->key_arr[length], field_1);
        ht_add_item(ht, (void *)db->key_arr[length], (void *)db->data_list[length]);    
    }
    else {
        DL_SEARCH(db->data_list[idx], to_return, temp, value_cmp);
        if (to_return != NULL) {
            SHOW_LOG(3, "Record: %s - %s has already in the table %s!\n", field_1, field_2, table_name);
        }
        else {
                DL_REPLACE_ELEM(db->data_list[idx], db->data_list[idx], temp);
                ht_add_item(ht, (void *)db->key_arr[idx], (void *)db->data_list[idx]);    
            }
    }
}

void update_permission_table(opool_t *opool, hash_table_t *ht, char *table_name, db_t *db, char *field_1, char *field_2) {
    int length, idx;
    opool_item_t *item = NULL;
    opool_item_t *item2 = NULL;

    entry_t *temp, *to_return, *entry, *temp2;

    item = opool_get(opool);
    EXIT_IF_TRUE(item == NULL, "Cannot get from object pool\n");
    temp = (entry_t *)item->data;

    item2 = opool_get(opool);
    EXIT_IF_TRUE(item2 == NULL, "Cannot get from object pool\n");
    temp2 = (entry_t *)item2->data;

    printf("UPDATE TABLE %s VALUES('%s', '%s')\n", table_name, field_1, field_2);

    ansi_copy_str(temp->value, field_2);

    idx = find_idx_by_key(db->key_arr, field_1);
    if (idx < 0) {
        length = get_array_length(db->key_arr);
        DL_APPEND(db->data_list[length], temp);
        ansi_copy_str(db->key_arr[length], field_1);
        ht_add_item(ht, (void *)db->key_arr[length], (void *)db->data_list[length]);    
    }
    else {
        DL_SEARCH(db->data_list[idx], to_return, temp, value_cmp);
        if (to_return != NULL) {
            SHOW_LOG(3, "Record: %s - %s has already in the table %s!\n", field_1, field_2, table_name);
        }
        else {
                DL_APPEND(db->data_list[idx], temp);
            }
    }
    //Specific for permission talbe
#if 1
    ansi_copy_str(temp2->value, field_1);
    idx = find_idx_by_key(db->key_arr, field_2);
    if (idx < 0) {
        length = get_array_length(db->key_arr);
        DL_APPEND(db->data_list[length], temp2);
        ansi_copy_str(db->key_arr[length], field_2);
        ht_add_item(ht, (void *)db->key_arr[length], (void *)db->data_list[length]);    
    }
    else {
        DL_SEARCH(db->data_list[idx], to_return, temp2, value_cmp);
        if (to_return != NULL) {
            SHOW_LOG(3, "Record: %s - %s has already in the table %s!\n", field_2, field_1, table_name);
        }
        else {
            DL_APPEND(db->data_list[idx], temp2);
        }
    }
#endif
}
#endif
void show_record(hash_table_t *ht, char *key) {
    entry_t *data_list;
    entry_t *temp, *entry;

    data_list = (entry_t *)ht_get_item(ht, (void *)key);

    printf("Key: %s - Value:", key);

    DL_FOREACH_SAFE(data_list, temp, entry) {
        printf(" %s |", temp->value);
    }
    printf("\n");
}

entry_t *get_data_list(hash_table_t *ht, char *key) {
    return ht_get_item(ht, (void *)key);
}
