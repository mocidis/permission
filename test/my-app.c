#include <stdio.h>
#include <stdlib.h>
#include <pjlib.h>
#include "my-pjlib-utils.h"
#include "ansi-utils.h"
#include "mysqlite.h"
#include <sqlite3.h>
#include "object-pool.h"
#include "utlist.h"
#include "proto-constants.h"

#include "permission.h"

db_t permission_db;
db_t login_db;
db_t passphrase_db;

int main() {
    pj_caching_pool cp;
    pj_pool_t *pool;
    opool_t opool;

    
    //INIT POOL
    CHECK(__FILE__, pj_init());
    SET_LOG_LEVEL(4);
    pj_log_set_level(3);    

    pj_caching_pool_init(&cp, NULL, 1024);
    pool = pj_pool_create(&cp.factory, "pool", 1024, 1024, NULL);
    opool_init(&opool, 100, sizeof(entry_t), pool);

    //INIT HASH TABLE
    ht_init(&permission_db.ht, pool);
    ht_init(&login_db.ht, pool);
    ht_init(&passphrase_db.ht, pool);

    //CREATE HASH TABLE
    ht_create(&permission_db.ht, HASH_COUNT);
    ht_create(&login_db.ht, HASH_COUNT);
    ht_create(&passphrase_db.ht, HASH_COUNT);

    //=========== LOAD DATA FROM DB THEN INSERT INTO HASH TABLE ==========//
#if 1
    printf("=============== Login =============\n");
    load_login_db(&opool, &login_db);
    SHOW_LOG(3, "SHOW TABLE PERMISSION\n");
    show_record(&login_db, "Acc_1");
    show_record(&login_db, "Acc_2");
    show_record(&login_db, "Acc_3");
    show_record(&login_db, "Acc_4");

    SHOW_LOG(3, "UPDATE TABLE LOGIN\n");
    update_passphrase_table(&opool, &login_db, "Acc_1", "OIUC2");
    update_passphrase_table(&opool, &login_db, "Acc_2", "OIUC3");
    update_passphrase_table(&opool, &login_db, "Acc_3", "OIUC1");

    //SHOW_LOG(3, "UPDATE FILE DATABASE PASSPHRASE\n");
    //update_database(&opool, &passphrase_db, "passphrase");
    SHOW_LOG(3, "SHOW TABLE PERMISSION\n");
    show_record(&login_db, "Acc_1");
    show_record(&login_db, "Acc_2");
    show_record(&login_db, "Acc_3");
    show_record(&login_db, "Acc_4");

#endif


#if 0
    printf("=============== Passphrase =============\n");
    load_passphrase_db(&opool, &passphrase_db);
    SHOW_LOG(3, "SHOW TABLE PASSPHRASE\n");
    show_record(&passphrase_db, "OIUC1");
    show_record(&passphrase_db, "OIUC2");
    show_record(&passphrase_db, "RIUC11");
    show_record(&passphrase_db, "RIUC12");
    show_record(&passphrase_db, "RIUC14");

    SHOW_LOG(3, "UPDATE TABLE PASSPHRASE\n");
    update_passphrase_table(&opool, &passphrase_db, "OIUC2", "qqqqqqq");
    update_passphrase_table(&opool, &passphrase_db, "RIUC14", "abcxyz");
    update_passphrase_table(&opool, &passphrase_db, "RIUC14", "12312312312");

    //SHOW_LOG(3, "UPDATE FILE DATABASE PASSPHRASE\n");
    //update_database(&opool, &passphrase_db, "passphrase");
    SHOW_LOG(3, "SHOW TABLE PASSPHRASE\n");
    show_record(&passphrase_db, "OIUC1");
    show_record(&passphrase_db, "OIUC2");
    show_record(&passphrase_db, "RIUC11");
    show_record(&passphrase_db, "RIUC12");
    show_record(&passphrase_db, "RIUC14");
#endif
#if 0
    printf("=============== Permisison =============\n");
    load_permission_db(&opool, &permission_db);
    SHOW_LOG(3, "SHOW TABLE PERMISSION\n");
    show_record(&permission_db, "Acc_1");
    show_record(&permission_db, "Acc_2");
    show_record(&permission_db, "Acc_3");
    show_record(&permission_db, "Acc_4");
    show_record(&permission_db, "RIUC11");
    show_record(&permission_db, "RIUC12");
    show_record(&permission_db, "RIUC13");
    show_record(&permission_db, "RIUC14");
    show_record(&permission_db, "OIUC1");
#if 1
    SHOW_LOG(3, "UPDATE TABLE PERMISSION\n");
    update_permission_table(&opool, &permission_db, "Acc_3", "RIUC11");
    update_permission_table(&opool, &permission_db, "Acc_3", "RIUC14");
    update_permission_table(&opool, &permission_db, "Acc_1", "RIUC11");
    update_permission_table(&opool, &permission_db, "Acc_1", "RIUC13");
    update_permission_table(&opool, &permission_db, "Acc_4", "OIUC1");

    SHOW_LOG(3, "UPDATE FILE DATABASE PERMISSION\n");
    //update_permission_database(&opool, &permission_db);
    SHOW_LOG(3, "SHOW TABLE PERMISSION\n");
    show_record(&permission_db, "Acc_1");
    show_record(&permission_db, "Acc_2");
    show_record(&permission_db, "Acc_3");
    show_record(&permission_db, "Acc_4");
    show_record(&permission_db, "RIUC11");
    show_record(&permission_db, "RIUC12");
    show_record(&permission_db, "RIUC13");
    show_record(&permission_db, "RIUC14");
    show_record(&permission_db, "OIUC1");
#endif
#endif
    //==================== check_permission_table && send passphrase ========================//
#if 0
    //Already has sender_id (user_id in permission_table)
    char user_id[] = "RIUC12";

    entry_t *temp, *entry;
    entry_t *passphrase_list;
    entry_t *device_id_list;
    //Get list of device id belongs to user_id
    entry_t *role_id_list = (entry_t *)ht_get_item(&permission_db.ht, user_id);
    int idx;

    printf("Received GM_REG with user_id: %s\n", user_id);
    //For each element of list above
    DL_FOREACH_SAFE(role_id_list, temp, entry) {
        //Get the list of passphrase (in fact, these list has only 1 element) belongs to device id a.k.a temp->value
        if (strstr(user_id, "RIUC") != NULL || strstr(user_id, "OIUC")) { 
            idx = get_idx(login_db.key_arr, temp->value);
            if (idx == -1) {
                printf("This user_id hasn't login yet! (lol) : %s\n", temp->value);
            }
            else {
                device_id_list = (entry_t *)ht_get_item(&login_db.ht, temp->value);

                idx = get_idx(passphrase_db.key_arr, device_id_list->value);
                if (idx == -1) {
                    printf("No passphrase for device: %s\n", device_id_list->value);
                }
                else {
                    passphrase_list = (entry_t *)ht_get_item(&passphrase_db.ht, device_id_list->value);
                    printf("Send passphrase: %s of %s to %s\n", passphrase_list->value, temp->value, user_id );
                }
            }
        }
        else {
            idx = get_idx(passphrase_db.key_arr, temp->value);
            if (idx == -1) {
                printf("No passphrase for device: %s\n", temp->value);
            }
            else {
                passphrase_list = (entry_t *)ht_get_item(&passphrase_db.ht, temp->value);
                printf("Send passphrase: %s of %s to %s\n", passphrase_list->value, temp->value, user_id );
            }
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
    return 0;
}
