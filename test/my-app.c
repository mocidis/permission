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

permission_table_t permission_db;
login_table_t login_db;
passphrase_table_t passphrase_db;

int main() {
    pj_caching_pool cp;
    pj_pool_t *pool;

    //INIT POOL
    CHECK(__FILE__, pj_init());
    SET_LOG_LEVEL(4);
    pj_log_set_level(3);    

    pj_caching_pool_init(&cp, NULL, 1024);
    pool = pj_pool_create(&cp.factory, "pool", 1024, 1024, NULL);

    perm_init(&permission_db, pool);
    perm_init(&login_db, pool);
    perm_init(&passphrase_db, pool);

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
    load_login_db(&login_db);
    SHOW_LOG(3, "SHOW TABLE PERMISSION\n");
    show_record(&login_db, "Acc_1");
    show_record(&login_db, "Acc_2");
    show_record(&login_db, "Acc_3");
    show_record(&login_db, "Acc_4");

    SHOW_LOG(3, "UPDATE TABLE LOGIN\n");
    add_new_account(&login_db, "Acc_1", "OIUC2");
    add_new_account(&login_db, "Acc_2", "OIUC3");
    add_new_account(&login_db, "Acc_3", "OIUC1");

    //SHOW_LOG(3, "UPDATE FILE DATABASE PASSPHRASE\n");
    save_login_database(&login_db);
    SHOW_LOG(3, "SHOW TABLE LOGIN\n");
    show_record(&login_db, "Acc_1");
    show_record(&login_db, "Acc_2");
    show_record(&login_db, "Acc_3");
    show_record(&login_db, "Acc_4");

#endif

#if 1
    printf("=============== Passphrase =============\n");
    load_passphrase_db(&passphrase_db);
    SHOW_LOG(3, "SHOW TABLE PASSPHRASE\n");
    show_record(&passphrase_db, "OIUC1");
    show_record(&passphrase_db, "OIUC2");
    show_record(&passphrase_db, "RIUC11");
    show_record(&passphrase_db, "RIUC12");
    show_record(&passphrase_db, "RIUC14");

    SHOW_LOG(3, "UPDATE TABLE PASSPHRASE\n");
    add_new_passphrase(&passphrase_db, "OIUC2", "qqqqqqq");
    add_new_passphrase(&passphrase_db, "RIUC14", "abcxyz");
    add_new_passphrase(&passphrase_db, "RIUC14", "12312312312");

    //SHOW_LOG(3, "UPDATE FILE DATABASE PASSPHRASE\n");
    save_passphrase_database(&passphrase_db);
    SHOW_LOG(3, "SHOW TABLE PASSPHRASE\n");
    show_record(&passphrase_db, "OIUC1");
    show_record(&passphrase_db, "OIUC2");
    show_record(&passphrase_db, "RIUC11");
    show_record(&passphrase_db, "RIUC12");
    show_record(&passphrase_db, "RIUC14");
#endif
#if 1
    printf("=============== Permisison =============\n");
    load_permission_db(&permission_db);
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
    grant_access(&permission_db, "Acc_3", "RIUC11");
    grant_access(&permission_db, "Acc_3", "RIUC14");
    grant_access(&permission_db, "Acc_1", "RIUC11");
    grant_access(&permission_db, "Acc_1", "RIUC13");
    grant_access(&permission_db, "Acc_4", "OIUC1");

    SHOW_LOG(3, "UPDATE FILE DATABASE PERMISSION\n");
    save_permission_database(&permission_db);
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
#if 1
    //Already has sender_id (user_id in permission_table)
    char user_id[] = "RIUC11";

    entry_t *temp, *entry;
    entry_t *passphrase_list;
    entry_t *login_list;
    //Get list of device id belongs to user_id
    entry_t *role_id_list = (entry_t *)ht_get_item(&permission_db.ht, user_id);

    printf("Received GM_REG with user_id: %s\n", user_id);
    //For each element of list above
    DL_FOREACH_SAFE(role_id_list, temp, entry) {
        if (strstr(user_id, "RIUC") != NULL || strstr(user_id, "OIUC")) {
#if 1
            login_list = (entry_t *)ht_get_item(&login_db.ht, temp->value);
            if (login_list == NULL) {
                printf("This user_id hasn't login yet! (lol) : %s\n", temp->value);
            }
            else {
                passphrase_list = (entry_t *)ht_get_item(&passphrase_db.ht, login_list->value);
                if (passphrase_list == NULL) {
                    printf("No passphrase for device: %s\n", login_list->value);
                }
                else {
                    printf("Send passphrase: %s of %s to %s\n", passphrase_list->value, temp->value, user_id );
                }
            }
#endif
        }
        else {
            passphrase_list = (entry_t *)ht_get_item(&passphrase_db.ht, temp->value);
            if (passphrase_list == NULL) {
                printf("No passphrase for device: %s\n", temp->value);
            }
            else {
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
