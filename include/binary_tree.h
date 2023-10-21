#ifndef BINARY_TREE_H
#define BINARY_TREE_H
#include <stdlib.h>
#include <stdbool.h>
#include "comparison_functions.h" 

/**
 * @brief Opaque type for the binary tree data structure.
*/
typedef struct BTree BTree;

/**
 * @brief Creator function for the binary tree structure.
 * @param max_height Determines the maximum height possible for the new tree.
 * @param as_copy Determines whether the tree should store copy of the data.
 * @return A pointer for the new tree if allocation of memory was successeful, or
 * a NULL pointer otherwise.
*/
BTree* btCreate(const bool as_copy, const TComparisonFun comp_fun);

/**
 * @brief Deleting function for the binary tree structure.
 * @param bt A pointer to the tree to be destroyed.
*/
void btDelete(BTree* bt);
/**
 * @brief Inserting function for the binary tree data structure.
 *
 * If the binary tree was created as a copy, this function will
 * copy the data at the moment of the call and store this copy.
 * If memory allocation for the copy was not successeful, this function
 * will return false. The function returns true if the data is already
 * present in the tree.
 *
 * @param bt A pointer to the binary tree.
 * @param data A pointer to the data.
 * @param data_size The size of the data.
 * @return True if the insersion was successeful, or false otherwise.
*/
bool btInsert(BTree* const bt, const void* data, const size_t data_size);

/**
 * @brief Performs a search operation in the beinary tree.
 * @param bt A pointer to the binary tree.
 * @param data A pointer to the data.
 * @return true if the data is in the binary tree, or false otherwise.
*/
bool btSearch(const BTree* const bt, const void* const data);

/**
 * @brief Retrieve the height of the binary tree
 * @param bt A pointer to the binary tree
 * @return The height of the tree
*/
size_t btGetHeight(const BTree* const bt);

#endif // BINARY_TREE_H
