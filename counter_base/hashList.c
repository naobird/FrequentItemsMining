/********************************************************************
Imprementation  Hash and List structure to use Find Frequent Items
Implementaion by Naoya Toriyabe 2018.12
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "hashList.h"

int itr;

// calculate mamory of data structure
void calc_memory(int size, int dummy_bucket_size, int dummy_itemnode_size) {
    int memory = sizeof(HashList*) + sizeof(HashList) + sizeof(ItemNode*) * size + sizeof(Bucket) * dummy_bucket_size + sizeof(ItemNode) * dummy_itemnode_size;
    printf("MEMORY = %d Byte\n", memory);
    return;
}

// output answers
void Output(HashList* hash_list, int threshold) {
    int number = 0;
    Bucket* bucket = hash_list->bucket;
    while (1) {
        if (bucket == NULL) {
            printf("NUMBER = %d\n", number);
            return;
        } else if (bucket->count <= threshold) {
            bucket = bucket->next_bucket;
        } else {
            break;
        }
    }
    while (1) {
        if (bucket == NULL) {
            break;
        }
        ItemNode* target = bucket->scl;
        while (1) {
            if (target == NULL) {
                break;
            }
            printf("%d %d\n", target->item, bucket->count);
            number++;
            target = target->next_scl;
        }
        bucket = bucket->next_bucket;
    }
    printf("NUMBER = %d\n", number);
    return;
}


// output summary data structure
void PrintHashList(HashList* hash_list, int size) {
    ItemNode** hash_table = hash_list->hash_table;
    printf("------------------------------------------------------------------\n");
    printf("HASH TABLE\n");
    int i;
    for (i = 0; i < size; i++) {
        printf("i = %d\n", i);
        ItemNode* target = hash_table[i];
        while (1) {
            if (target == NULL) {
                break;
            }
            printf("%d ", target->item);
            target = target->next_shvl;
        }
        printf("\n");
    }
    printf("------------------------------------------------------------------\n");
    printf("LINKED LIST\n");
    Bucket* bucket = hash_list->bucket;
    while (1) {
        if (bucket == NULL) {
            break;
        }
        printf("COUNT = %d\n", bucket->count);
        ItemNode* target = bucket->scl;
        while (1) {
            if (target == NULL) {
                break;
            }
            printf("%d\n", target->item);
            target = target->next_scl;
        }
        printf("\n");
        bucket = bucket->next_bucket;
    }
    printf("------------------------------------------------------------------\n");
    return;
}

// initialize summary data structure
ItemNode** initHashTable (int size) {
   ItemNode** hash_table = (ItemNode**) malloc(sizeof(ItemNode*) * size); 
   for (itr = 0; itr < size; itr++) {
       hash_table[itr] = NULL;
   }
   return hash_table;
}

Bucket* initBucket() {
    Bucket* bucket = (Bucket*) malloc(sizeof(Bucket));
    bucket = NULL;
    return bucket;
}

Bucket* initDummyBucket(int size) {
    Bucket* dummy_bucket_ptr = (Bucket*) malloc(sizeof(Bucket));
    Bucket* dummy_bucket = (Bucket*) malloc(sizeof(Bucket) * size);
    dummy_bucket_ptr = &dummy_bucket[0];
    for (itr = 0; itr < size; itr++) {
        dummy_bucket[itr].scl = NULL;
        if (itr == 0) {
            dummy_bucket[itr].prev_bucket = NULL;
        } else {
            dummy_bucket[itr].prev_bucket = &dummy_bucket[itr - 1];
        }
        if (itr == size - 1) {
            dummy_bucket[itr].next_bucket = NULL;
        } else {
            dummy_bucket[itr].next_bucket = &dummy_bucket[itr + 1];
        }
    }
    return dummy_bucket_ptr;
}

ItemNode* initDummyItemNode(int size) {
    ItemNode* dummy_itemnode_ptr = (ItemNode*) malloc(sizeof(ItemNode));
    ItemNode* dummy_itemnode = (ItemNode*) malloc(sizeof(ItemNode) * size);
    dummy_itemnode_ptr = &dummy_itemnode[0];
    for (itr = 0; itr < size; itr++) {
        dummy_itemnode[itr].parent = NULL;
        if (itr == 0) {
            dummy_itemnode[itr].prev_scl = NULL;
        } else {
            dummy_itemnode[itr].prev_scl = &dummy_itemnode[itr - 1]; 
        }
        if (itr == size - 1) {
            dummy_itemnode[itr].next_scl = NULL;
        } else {
            dummy_itemnode[itr].next_scl = &dummy_itemnode[itr + 1]; 
        }
        dummy_itemnode[itr].prev_shvl = NULL;
        dummy_itemnode[itr].next_shvl = NULL;
    }
    return dummy_itemnode_ptr;
}

HashList* initHashList(int size, int dummy_size) {
    HashList* hash_list = (HashList*) malloc(sizeof(HashList));
    hash_list->hash_table = initHashTable(size);
    hash_list->bucket = initBucket();
    hash_list->dummy_bucket = initDummyBucket(dummy_size);
    hash_list->dummy_itemnode = initDummyItemNode(dummy_size);
    return hash_list;
}

// calculate hash value
int hashValue(int size, val item) {
    return item % size;
}

// fetch bucket from dummy bucket (not using bucket)
Bucket* fetchBucket(HashList* hash_list) {
    Bucket* fetch_bucket = hash_list->dummy_bucket;
    if (hash_list->dummy_bucket->next_bucket != NULL) {
        hash_list->dummy_bucket->next_bucket->prev_bucket = NULL;
    }
    hash_list->dummy_bucket = hash_list->dummy_bucket->next_bucket;
    return fetch_bucket;
}

// fetch item_node from dummy item_node (not using item_node)
ItemNode* fetchItemNode(HashList* hash_list) {
    ItemNode* fetch_itemnode = hash_list->dummy_itemnode;
    if (hash_list->dummy_itemnode->next_scl != NULL) {
        hash_list->dummy_itemnode->next_scl->prev_scl = NULL;
    }
    hash_list->dummy_itemnode = hash_list->dummy_itemnode->next_scl;
    return fetch_itemnode;
}

// set bucket again
void setBucket(Bucket* target, int count, ItemNode* scl, Bucket* prev_bucket, Bucket* next_bucket) {
    target->count = count;
    target->scl = scl;
    target->prev_bucket = prev_bucket;
    target->next_bucket = next_bucket;
    return;
}

// set item_node again
void setItemNode(ItemNode* target, val item, Bucket* parent, ItemNode* prev_scl, ItemNode* next_scl, ItemNode* prev_shvl, ItemNode* next_shvl) {
    target->item = item;
    target->parent = parent;
    target->prev_scl = prev_scl; // pointer to prev element in the same count list
    target->next_scl = next_scl; // pointer to next element in the same count list
    target->prev_shvl = prev_shvl; // pointer to prev element in the same hash value list
    target->next_shvl = next_shvl; // pointer to next element in the same hash value list
    return;
}

// insert item in the top of scl list
void insertSclTop(Bucket* bucket, ItemNode* itemnode) {
    if (bucket->scl == NULL) {
        bucket->scl = itemnode;
        itemnode->prev_scl = NULL;
        itemnode->next_scl = NULL;
    } else {
        itemnode->prev_scl = NULL;
        itemnode->next_scl = bucket->scl;
        bucket->scl->prev_scl = itemnode;
        bucket->scl = itemnode;
    }
    itemnode->parent = bucket;
    return;
}

// insert item in the hash_table
void insertShvlTop(ItemNode** hash_table, int hash_value, ItemNode* itemnode) {
    if (hash_table[hash_value] == NULL) {
        itemnode->prev_shvl = NULL;
        itemnode->next_shvl = NULL;
        hash_table[hash_value] = itemnode;
    } else {
        itemnode->prev_shvl = NULL;
        itemnode->next_shvl = hash_table[hash_value];
        hash_table[hash_value]->prev_shvl = itemnode;
        hash_table[hash_value] = itemnode;
    }
    return;
}

// search item in hash_table
ItemNode* hashSearch(HashList* hash_list, val item, int hash_value) {
    ItemNode* target = hash_list->hash_table[hash_value];
    while (target != NULL) {
        if (target->item == item) {
            return target;
        } else {
            target = target->next_shvl;
        }
    }
    return NULL;
}

// delete the bucket in buckets and link it to the not using buckets list
void detachBucket(HashList* hash_list, Bucket* target) {
    if (target->prev_bucket == NULL && target->next_bucket == NULL) {
        hash_list->bucket = NULL;
    } else if (target->prev_bucket == NULL) {
        hash_list->bucket = target->next_bucket;
        target->next_bucket->prev_bucket = NULL;
    } else if (target->next_bucket == NULL) {
        target->prev_bucket->next_bucket = NULL;
    } else {
        target->prev_bucket->next_bucket = target->next_bucket;
        target->next_bucket->prev_bucket = target->prev_bucket;
    }
    if (hash_list->dummy_bucket == NULL) {
        setBucket(target, 0, NULL, NULL, NULL);
        hash_list->dummy_bucket = target;
    } else {
        setBucket(target, 0, NULL, NULL, hash_list->dummy_bucket);
        hash_list->dummy_bucket->prev_bucket = target;
        hash_list->dummy_bucket = target;
    }
    return;
}

// delete the item_node from scl list
void detachScl(HashList* hash_list, ItemNode* target) {
    if (target->prev_scl == NULL && target->next_scl == NULL) { 
        target->parent->scl = NULL;
        detachBucket(hash_list, target->parent);
    } else if (target->prev_scl == NULL) {
        target->parent->scl = target->next_scl;
        target->next_scl->prev_scl = NULL;
    } else if (target->next_scl == NULL) {
        target->prev_scl->next_scl = NULL;
    } else {
        target->prev_scl->next_scl = target->next_scl;
        target->next_scl->prev_scl = target->prev_scl;
    }
    return;
}

// delete the item from hash_table
void detachShvl(HashList* hash_list, ItemNode* target, int hash_value) {
    if (target->prev_shvl == NULL && target->next_shvl == NULL) { // 必要ないかも
        hash_list->hash_table[hash_value] = NULL;
    } else if (target->prev_shvl == NULL) {
        hash_list->hash_table[hash_value] = target->next_shvl;
        target->next_shvl->prev_shvl = NULL;
    } else if (target->next_shvl == NULL) {
        target->prev_shvl->next_shvl = NULL;
    } else {
        target->prev_shvl->next_shvl = target->next_shvl;
        target->next_shvl->prev_shvl = target->prev_shvl;
    }
    return;
}

// insert the item in summary data structure
void addItemNode(HashList* hash_list, val item, int hash_value, int add_count) {
    if (hash_list->bucket == NULL) {
        Bucket* fetch_bucket = fetchBucket(hash_list);
        setBucket(fetch_bucket, add_count, NULL, NULL, NULL);
        hash_list->bucket = fetch_bucket;
    } else if (hash_list->bucket->count != add_count) {
        Bucket* fetch_bucket = fetchBucket(hash_list);
        setBucket(fetch_bucket, add_count, NULL, NULL, hash_list->bucket);
        hash_list->bucket->prev_bucket = fetch_bucket;
        hash_list->bucket = fetch_bucket;
    }
    ItemNode* fetch_itemnode = fetchItemNode(hash_list);
    setItemNode(fetch_itemnode, item, NULL, NULL, NULL, NULL, NULL);
    insertShvlTop(hash_list->hash_table, hash_value, fetch_itemnode); 
    insertSclTop(hash_list->bucket, fetch_itemnode); 
    return;
}

// increment the count of item in summary data structure
void CountUpItem (HashList* hash_list, ItemNode* target) {
    Bucket* origin = target->parent;
    Bucket* next = origin->next_bucket;
    if (target->prev_scl == NULL && target->next_scl == NULL) { // 一個のみ
        if (next == NULL || next->count != origin->count + 1) {
            origin->count++;
        } else {
            insertSclTop(next, target); 
            detachBucket(hash_list, origin);
        }
    } else { 
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
        detachScl(hash_list, target);
        insertSclTop(fetch_bucket, target); 
    }
    return;
}
