/********************************************************************
Implementaion by Naoya Toriyabe 2018.12-2019.3
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "../hashList.h"
#include "ss.h"

void exchangeItems (HashList* hash_list, val new_item, int size) {
    Bucket* origin = hash_list->bucket;
    Bucket* next = origin->next_bucket;
    Bucket* fetch_bucket;
    if (next == NULL) {
        fetch_bucket = fetchBucket(hash_list);
        setBucket(fetch_bucket, origin->count + 1, NULL, origin, NULL);
        origin->next_bucket = fetch_bucket;
    } else if (next->count != origin->count + 1) {
        fetch_bucket = fetchBucket(hash_list);
        setBucket(fetch_bucket, origin->count + 1, NULL, origin, next);
        origin->next_bucket = fetch_bucket;
        next->prev_bucket = fetch_bucket;
    } else {
        fetch_bucket = next;
    }
    ItemNode* target = origin->scl;
    detachScl(hash_list, target); // sclを切り離す
    insertSclTop(fetch_bucket, target); 
    detachShvl(hash_list, target, hashValue(size, target->item));
    target->item = new_item;
    insertShvlTop(hash_list->hash_table, hashValue(size, target->item), target); 
    return;
}
