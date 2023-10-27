#include "../include/binary_tree.h"

typedef struct BTNode{
    cds_bool        is_tombstone;
    cds_size        depth;
    cds_size        height;
    void*           data;
    struct BTNode*  parent;
    struct BTNode*  left;
    struct BTNode*  right;
}BTNode;

struct BTree{
    cds_size    data_size;
    BTNode*     root;
    TComparisonFun compare;
};

BTree* btCreate(const TComparisonFun comp_fun, const cds_size data_size){
    BTree* new_bt = (BTree*) malloc(sizeof(BTree));
    if (!new_bt){
        return (BTree*) NULL;
    }
    new_bt->data_size = data_size;
    new_bt->root = (BTNode*) NULL;
    new_bt->compare = comp_fun;
    return new_bt;
}
