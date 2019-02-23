/********************************************************************
Implementaion by Naoya Toriyabe 2018.12-2019.3
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "../hashListWithDelta.h"
#include "freq.h"

int main(int argc, char* argv[]) {
    clock_t start = clock();
    int n = 0; // 受け取ったデータの数
    int delete_times = 0; // 削除操作を行った回数
    double phi = atof(argv[1]); // user specified parameter
    double eps = atof(argv[2]); // user specified parameter
    int size = 1 / eps; // 論文中のパラメータ k にあたる
    int hash_size = pow(2, (int)(ceil(log2f((double)(size))))) * 8;
    HashList* hash_list = initHashList(hash_size, size - 1); // ハッシュリストの初期化

    val item; 
    int hash_value;
    while (1) {
        scanf("%ld", &item);
        if (item == -1) {
            int threshold = (int)(((double)(phi) - (double)(eps)) * (double)(n) + 0.1); // SS
            Output(hash_list, threshold);
            break;
        }
        n++;
        hash_value = hashValue(hash_size, item);
        ItemNode* search_pointer = hashSearch(hash_list, item, hash_value);
        if (search_pointer != NULL) {
            CountUpItem (hash_list, search_pointer);
        } else if (hash_list->dummy_itemnode != NULL) {
            addItemNode(hash_list, item, hash_value, delete_times + 1); 
        } else {
            delete_times++;
            allDec(hash_list, delete_times, hash_size);
        }
    }
    clock_t end = clock();
    printf("TIME = %.4f sec\n",(double)(end-start)/CLOCKS_PER_SEC);
    calc_memory(hash_size, size - 1, size - 1);
    return 0;
}
