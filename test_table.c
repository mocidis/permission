#include <stdio.h>
#include <stdlib.h>
#include <pjlib.h>
#include "hash-table.h"
#include "my-pjlib-utils.h"
#include "ansi-utils.h"
#include "mysqlite.h"
#include <sqlite3.h>
#include "object-pool.h"
#include "utlist.h"
#include "proto-constants.h"

#include "permission.h"

#define HASH_COUNT 10
#define MAX_RECORD 50

db_t permission_db;
db_t login_db;
db_t passphrase_db;

int main() {
    int key_arr_size;

    hash_table_t ht_permission;
    hash_table_t ht_login;
    hash_table_t ht_passphrase;

    pj_status_t status;
    pj_caching_pool cp;
    pj_pool_t *pool;
    opool_t opool;

    //INIT POOL
    CHECK(__FILE__, pj_init());
    SET_LOG_LEVEL(4);
    pj_log_set_level(3);    

    pj_caching_pool_init(&cp, NULL, 1024);
    pool = pj_pool_create(&cp.factory, "pool", 1024, 1024, NULL);
    opool_init(&opool, 40, sizeof(entry_t), pool);

    //INIT HASH TABLE
    ht_init(&ht_permission, pool);
    //ht_init(&ht_login, pool);
    ht_init(&ht_passphrase, pool);

    //CREATE HASH TABLE
    ht_create(&ht_permission, HASH_COUNT);
    //ht_create(&ht_login, HASH_COUNT);
    ht_create(&ht_passphrase, HASH_COUNT);

    //=========== LOAD DATA FROM DB THEN INSERT INTO HASH TABLE ==========//
    key_arr_size = load_database(&opool, &ht_permission, "permission", &permission_db);
    load_database_reflex(&opool, &ht_permission, "permission", &permission_db, key_arr_size);
    //load_database(&opool, &ht_login, "login", &login_db);
    load_database(&opool, &ht_passphrase, "passphrase", &passphrase_db);

    //======================= PROCESS DATA FROM HASH TABLE================//
    //HT_permission sample:
    // Acc_1 | RIUC11, RIUC12
    // RIUC11 | RIUC11, Acc_1
    //entry_t *temp2, *entry;

#if 1
    printf("=============== Permission =============\n");
    show_record(&ht_permission, "Acc_1");
    show_record(&ht_permission, "Acc_2");
    show_record(&ht_permission, "RIUC11");
    show_record(&ht_permission, "RIUC12");
#endif
#if 0
    printf("=============== Login =============\n");
    show_record(&ht_login, "Acc_1");
    show_record(&ht_login, "Acc_2");
#endif
    printf("=============== Passphrase =============\n");
    show_record(&ht_passphrase, "OIUC1");
    show_record(&ht_passphrase, "OIUC2");
    show_record(&ht_passphrase, "RIUC11");
    show_record(&ht_passphrase, "RIUC12");
    
    update_table(&opool, &ht_passphrase, "passphrase", &passphrase_db, "OIUC2", "qqqqqqq");
    update_table(&opool, &ht_permission, "permission", &permission_db, "Acc_3", "RIUC11");
    update_table(&opool, &ht_permission, "permission", &permission_db, "Acc_1", "RIUC11");
    update_table(&opool, &ht_permission, "permission", &permission_db, "Acc_1", "RIUC13");
    //update_table(&opool, &ht_passphrase, "passphrase", &passphrase_db, "COORD", COORD_PP);
    show_record(&ht_passphrase, "OIUC2");
    show_record(&ht_passphrase, "RIUC11");

    show_record(&ht_permission, "Acc_3");
    show_record(&ht_permission, "Acc_1");
    show_record(&ht_permission, "RIUC11");

    //==================== check_permission_table && send passphrase ========================//
#if 0
    //Already has sender_id (user_id in permission_table)
    char user_id[] = "Acc_1";

    entry_t *temp, *temp2, *entry, *entry2;
    entry_t *passphrase_list;
    //Get list of device id belongs to user_id
    entry_t *data_list = get_data_list(&ht_permission, user_id);

    printf("Received GM_REG with user_id: %s\n", user_id);
    //For each element of list above
    DL_FOREACH_SAFE(data_list, temp, entry) {
        //Get the list of passphrase (in fact, these list has only 1 element) belongs to device id a.k.a temp->value
        passphrase_list = get_data_list(&ht_passphrase, temp->value);
        DL_FOREACH_SAFE(passphrase_list, temp2, entry2) {
            printf("Send passphrase: %s of %s to %s\n", temp2->value, temp->value, user_id );
        }
    }
#endif
    //=========================================================================================//

#if 0
    GM_REG request comming
        if node is new
            update_passphrase_table
            check_permission_table
            send_passphrase

         if node is already in table
            check_passphrase_table
                if passphrase change
                    update_passphrase_table
                    check_permission_table
                    send_passphrase
                else
                    do nothing
#endif

#if 0
#endif
    return 0;
}
