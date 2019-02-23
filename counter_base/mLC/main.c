/********************************************************************
Implementaion by Naoya Toriyabe 2018.12-2019.3
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../hashList.h"
#include "lc.h"

int main(int argc, char* argv[]) {
    clock_t start = clock();
    int n = 0; // 受け取ったデータの数
    int delta = 0; // 削除操作を行った回数
    double phi = atof(argv[1]); // user specified parameter
    double eps = atof(argv[2]); // user specified parameter
    int size = 1 / eps; // 論文中のパラメータ k にあたる
    int dummy_bucket_size = 1;
    int dummy_itemnode_size = 1;
    int hash_size = pow(2, (int)(ceil(log2f((double)(size))))) * 8;
    HashList* hash_list = initHashList(hash_size, dummy_bucket_size);

    val item; 
    int hash_value;
    while (1) {
        scanf("%ld", &item);
        if (item == -1) {
            int threshold = (int)((double)(phi) * (double)(n) + 0.1); // SS
            Output(hash_list, threshold);
            break;
        }
        n++;
        hash_value = hashValue(hash_size, item);
        if (hash_list->dummy_bucket == NULL) {
            hash_list->dummy_bucket = initDummyBucket(dummy_bucket_size);
            dummy_bucket_size *= 2;
        }
        if (hash_list->dummy_itemnode == NULL) {
            hash_list->dummy_itemnode = initDummyItemNode(dummy_itemnode_size);
            dummy_itemnode_size *= 2;
        }
        ItemNode* search_pointer = hashSearch(hash_list, item, hash_value);
        if (search_pointer != NULL) {
            CountUpItem (hash_list, search_pointer);
        } else {
            addItemNode(hash_list, item, hash_value, delta + 1); 
        }
        if ((int)(eps * n) != delta) {
            delta++;
            DeleteItems(hash_list, delta, hash_size);
        }
    }
    clock_t end = clock();
    printf("TIME = %.4f sec\n",(double)(end-start)/CLOCKS_PER_SEC);
    calc_memory(hash_size, dummy_bucket_size, dummy_itemnode_size);
    return 0;
}
