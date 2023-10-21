#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../include/binary_tree.h" 

typedef struct BTNode{
    const void* data;
    size_t data_size;
    struct BTNode* left;
    struct BTNode* right;
}BTNode;

struct BTree{
    BTNode* root;
    TComparisonFun comp_fun;
    size_t height;
    bool as_copy;
};

static const void* _dataDup(const void* data, const size_t data_size){
    void* _data = (void*) malloc(data_size);
    if (!_data){
        return (void*) NULL;
    }
    (void) memcpy(_data, data, data_size);
    return _data;
}

static BTNode* _btCreateNode(const void* data, const size_t data_size, const bool as_copy){
    const void* _data;
    if (as_copy){
        _data = _dataDup(data, data_size);
        if (!_data){
            //handle errors
            return (BTNode*) NULL;
        }
    }else{
        _data = data;
    }
    BTNode* new_node = (BTNode*) malloc(sizeof(BTNode));
    if (!new_node){
        //handle errors
        return (BTNode*) NULL;
    }
    new_node->data = _data;
    new_node->data_size = data_size;
    new_node->left = (BTNode*) NULL;
    new_node->right = (BTNode*) NULL;
    return new_node;
}

BTree* btCreate(const bool as_copy, const TComparisonFun comp_fun){
    BTree* new_bt = (BTree*) malloc(sizeof(BTree));
    if (!new_bt){
        //handle errors
        return (BTree*) NULL;
    }
    new_bt->height = 0;
    new_bt->root = (BTNode*) NULL;
    new_bt->comp_fun = comp_fun;
    new_bt->as_copy = as_copy;
    return new_bt;
}

static void _btDeleteNodes(BTNode* root, const bool as_copy){
    if (!root){
        return;
    }
    _btDeleteNodes(root->left, as_copy);
    _btDeleteNodes(root->right, as_copy);
    if (as_copy){
        free((void*) root->data);
    }
    free(root);
}

void btDelete(BTree* bt){
    if (!bt){
        return;
    }
    _btDeleteNodes(bt->root, bt->as_copy);
    free(bt);
}
// if decided later that a BT has to have a maximum height, one should 
// add it to the loop below.
static void _btInsertNode(BTNode* root, BTNode* new_node, 
                    const TComparisonFun comp_fun, size_t* pheight){
    size_t height = 1;
    while (root){
        if (0 == comp_fun(root->data, new_node->data)){
            
        }else if (-1 == comp_fun(root->data, new_node->data)){
            height++;
            if (!root->left){
                root->left = new_node;
                root = root->left;
            }else{
                root = root->left;
            }
        }else{
            height++;
            if (!root->right){
                root->right = new_node;
                root = root->right;
            }else{
                root = root->right;
            }
        }
    }
    *pheight = height;
}

bool btInsert(BTree* const bt, const void* data, const size_t data_size){
    if (!data || data_size == 0){
        //handle erros
        return false;
    }
    BTNode* new_node = _btCreateNode(data, data_size, bt->as_copy);
    if (!new_node){
        //handle errors
        return false;
    }
    if (!bt->root){
        bt->root = new_node;
        bt->height++;
        return true;
    }
    _btInsertNode(bt->root, new_node, bt->comp_fun, &bt->height);
    return true;
}

static bool _btSearch(BTNode* root, const void* data, const TComparisonFun comp_fun){
    while (root){
        if (0 == comp_fun(root->data, data)){
            return true;
        }else if(-1 == comp_fun(root->data, data)){
            root = root->left;
        }else{
            root = root->right;
        }
    }
    return false;
}

bool btSearch(const BTree* const bt, const void* const data){
    if (!data){
        return false;
    }
    return _btSearch(bt->root, data, bt->comp_fun);
}

size_t btGetHeight(const BTree *const bt){
    return bt->height;
}
