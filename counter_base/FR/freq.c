/********************************************************************
Implementaion by Naoya Toriyabe 2018.12-2019.3
********************************************************************/

#include <stdlib.h>
#include "../hashListWithDelta.h"
#include "freq.h"

// decrement all items 
void allDec (HashList* hash_list, int delete_times, int size) {
    if (hash_list->bucket->count == delete_times) {
        ItemNode* target_itemnode = hash_list->bucket->scl;
        ItemNode* temp_dummy_itemnode = hash_list->dummy_itemnode;
        hash_list->dummy_itemnode = target_itemnode;
        while (1) {
            detachShvl(hash_list, target_itemnode, hashValue(size, target_itemnode->item));
            if (target_itemnode->next_scl == NULL) {
                break;
            } else {
                target_itemnode = target_itemnode->next_scl;
            }
        }
        if (temp_dummy_itemnode == NULL) {
            target_itemnode->next_scl = NULL;
        } else {
            target_itemnode->next_scl = temp_dummy_itemnode;
            temp_dummy_itemnode->prev_scl = target_itemnode;
        }
        Bucket* target_bucket = hash_list->bucket;
        detachBucket(hash_list, target_bucket);
    }
    return;
}
