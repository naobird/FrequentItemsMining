/********************************************************************
Imprementation  Hash and List structure to use Find Frequent Items
Implementaion by Naoya Toriyabe 2018.12
********************************************************************/

#ifndef HASHLIST_H
#define HASHLIST_H

typedef long long int val;

typedef struct _ItemNode {
    val item;
    int delta;
    struct _Bucket* parent;
    struct _ItemNode* prev_scl; // pointer to prev element in the same count list
    struct _ItemNode* next_scl; // pointer to next element in the same count list
    struct _ItemNode* prev_shvl; // pointer to prev element in the same hash value list
    struct _ItemNode* next_shvl; // pointer to next element in the same hash value list
} ItemNode;

typedef struct _Bucket {
    int count;
    struct _ItemNode* scl; // same_count_list
    struct _Bucket* prev_bucket;
    struct _Bucket* next_bucket;
} Bucket;

typedef struct _HashTableAndLinkedList {
    ItemNode** hash_table;
    Bucket* bucket;
    Bucket* dummy_bucket; // not_use_bucket_list
    ItemNode* dummy_itemnode; // not_use_item_node_list
} HashList; 

extern void calc_memory(int size, int dummy_bucket_size, int dummy_itemnode_size); 
extern void Output(HashList* hash_list, int threshold);
extern void PrintHashList(HashList* hash_list, int size);

extern HashList* initHashList(int size, int dummy_size); 
extern Bucket* initDummyBucket(int size);
extern ItemNode* initDummyItemNode(int size);
extern int hashValue(int size, val item);
extern Bucket* fetchBucket(HashList* hash_list); 
extern ItemNode* fetchItemNode(HashList* hash_list); 
extern void setBucket(Bucket* target, int count, ItemNode* scl, Bucket* prev_bucket, Bucket* next_bucket); 
extern void setItemNode(ItemNode* target, val item, int delta, Bucket* parent, ItemNode* prev_scl, ItemNode* next_scl, ItemNode* prev_shvl, ItemNode* next_shvl); 
extern void insertSclTop(Bucket* bucket, ItemNode* itemnode); 
extern void insertShvlTop(ItemNode** hash_table, int hash_value, ItemNode* itemnode); 
extern ItemNode* hashSearch(HashList* hash_list, val item, int hash_value);
extern void detachBucket(HashList* hash_list, Bucket* target); 
extern void detachScl(HashList* hash_list, ItemNode* target); 
extern void detachShvl(HashList* hash_list, ItemNode* target, int hash_value);
extern void addItemNode(HashList* hash_list, val item, int hash_value, int add_count);
extern void CountUpItem (HashList* hash_list, ItemNode* target);


#endif
