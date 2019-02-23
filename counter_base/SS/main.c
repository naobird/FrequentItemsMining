/********************************************************************
Implementaion by Naoya Toriyabe 2018.12-2019.3
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../hashList.h"
#include "ss.h"

int main(int argc, char* argv[]) {
    clock_t start = clock();
    int n = 0; // 受け取ったデータの数
    double phi = atof(argv[1]); // user specified parameter
    double eps = atof(argv[2]); // user specified parameter
    int size = 1 / eps; // 論文中のパラメータ k にあたる
    int hash_size = pow(2, (int)(ceil(log2f((double)(size))))) * 8;
    HashList* hash_list = initHashList(hash_size, size); // ハッシュリストの初期化

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
        ItemNode* search_pointer = hashSearch(hash_list, item, hash_value);
        if (search_pointer != NULL) {
            CountUpItem (hash_list, search_pointer);
        } else if (hash_list->dummy_itemnode != NULL) {
            addItemNode(hash_list, item, hash_value, 1); 
        } else {
            exchangeItems(hash_list, item, hash_size);
        }
    }
    clock_t end = clock();
    printf("TIME = %.4f sec\n",(double)(end-start)/CLOCKS_PER_SEC);
    calc_memory(hash_size, size, size);
    return 0;
}
