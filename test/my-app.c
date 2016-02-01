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

p_table_t permission_db;
l_table_t login_db;
pph_table_t passphrase_db;

int main() {
    pj_caching_pool cp;
    pj_pool_t *pool;

    char db_path[] = "./databases/permission.db";

    //INIT POOL
    CHECK(__FILE__, pj_init());
    SET_LOG_LEVEL(4);
    pj_log_set_level(3);    

    pj_caching_pool_init(&cp, NULL, 1024);
    pool = pj_pool_create(&cp.factory, "pool", 1024, 1024, NULL);

    perm_init(&permission_db, pool, db_path);
    perm_init(&login_db, pool, db_path);
    perm_init(&passphrase_db, pool, db_path);

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
    SHOW_LOG(3, "=== SHOW TABLE PERMISSION\n");
    show_table(&login_db);

    SHOW_LOG(3, "UPDATE TABLE LOGIN\n");
    add_active_acc(&login_db, "Acc_1", "OIUC2");
    add_active_acc(&login_db, "Acc_2", "OIUC3");
    add_active_acc(&login_db, "Acc_3", "OIUC1");
    SHOW_LOG(3, "=== SHOW TABLE LOGIN\n");
    show_table(&login_db);

    remove_active_acc(&login_db, "Acc_3");
    SHOW_LOG(3, "=== SHOW TABLE LOGIN\n");
    show_table(&login_db);

    SHOW_LOG(3, "UPDATE FILE DATABASE PASSPHRASE\n");
    save_login_db(&login_db);
    SHOW_LOG(3, "=== SHOW TABLE LOGIN\n");
    show_table(&login_db);
#endif

#if 1
    printf("=============== Passphrase =============\n");
    load_passphrase_db(&passphrase_db);
    SHOW_LOG(3, "=== SHOW TABLE PASSPHRASE\n");
    show_table(&passphrase_db);

    SHOW_LOG(3, "UPDATE TABLE PASSPHRASE\n");
    add_passphrase(&passphrase_db, "OIUC2", "qqqqqqq");
    add_passphrase(&passphrase_db, "RIUC14", "abcxyz");
    add_passphrase(&passphrase_db, "RIUC14", "12312312312");
    SHOW_LOG(3, "=== SHOW TABLE PASSPHRASE\n");
    show_table(&passphrase_db);

    remove_passphrase(&passphrase_db, "RIUC14");
    SHOW_LOG(3, "=== SHOW TABLE PASSPHRASE\n");
    show_table(&passphrase_db);

    SHOW_LOG(3, "UPDATE FILE DATABASE PASSPHRASE\n");
    save_passphrase_db(&passphrase_db);
    SHOW_LOG(3, "=== SHOW TABLE PASSPHRASE\n");
    show_table(&passphrase_db);
#endif

#if 1
    printf("=============== Permisison =============\n");
    load_permission_db(&permission_db);
    SHOW_LOG(3, "=== SHOW TABLE PERMISSION\n");
    show_table(&permission_db);
    SHOW_LOG(3, "UPDATE TABLE PERMISSION\n");
    grant_access(&permission_db, "Acc_3", "RIUC11");
    grant_access(&permission_db, "Acc_3", "RIUC14");
    grant_access(&permission_db, "Acc_1", "RIUC11");
    grant_access(&permission_db, "Acc_1", "RIUC13");
    grant_access(&permission_db, "Acc_4", "OIUC1");

    SHOW_LOG(3, "=== SHOW TABLE PERMISSION\n");
    show_table(&permission_db);

    revoke_access(&permission_db, "Acc_3", "RIUC11");
    SHOW_LOG(3, "=== SHOW TABLE PERMISSION\n");
    show_table(&permission_db);

    SHOW_LOG(3, "UPDATE FILE DATABASE PERMISSION\n");
    save_permission_db(&permission_db);
    SHOW_LOG(3, "=== SHOW TABLE PERMISSION\n");
    show_table(&permission_db);
#endif
    //==================== check_permission_table && send passphrase ========================//
#if 1
    //Already has sender_id (user_id in permission_table)
    SHOW_LOG(3, "=== SHOW TABLE PERMISSION\n");
    show_table(&permission_db);

    SHOW_LOG(3, "=== SHOW TABLE LOGIN\n");
    show_table(&login_db);

    SHOW_LOG(3, "=== SHOW TABLE PASSPHRASE\n");
    show_table(&passphrase_db);
    
    char user_id[] = "Acc_1";

    perm_node_t *temp, *entry;
    perm_node_t *passphrase_list;
    perm_node_t *login_list;
    //Get list of device id belongs to user_id
    perm_node_t *node_id_list = (perm_node_t *)ht_get_item(&permission_db.ht, user_id);

    printf("Received GM_REG with user_id: %s\n", user_id);
    //For each element of list above
    DL_FOREACH_SAFE(node_id_list, temp, entry) {
        if (strstr(user_id, "RIUC") != NULL || strstr(user_id, "OIUC")) {
#if 1
            login_list = (perm_node_t *)ht_get_item(&login_db.ht, temp->value);
            if (login_list == NULL) {
                printf("This user_id hasn't login yet! (lol) : %s\n", temp->value);
            }
            else {
                passphrase_list = (perm_node_t *)ht_get_item(&passphrase_db.ht, login_list->value);
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
            passphrase_list = (perm_node_t *)ht_get_item(&passphrase_db.ht, temp->value);
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
