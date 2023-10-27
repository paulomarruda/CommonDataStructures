#include "common.h"
#include "comparison_functions.h"
#include "linear.h"

typedef struct BTree BTree;

BTree* btCreate(const TComparisonFun compare_fun, const cds_size data_size);

Vector* btTransversal(const BTree* const bt);

