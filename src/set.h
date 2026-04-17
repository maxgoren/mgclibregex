#ifndef set_h
#define set_h
#include "rb.h"
#include <stdbool.h>
typedef RedBlackBST OrderedSet;
typedef rbentry SetItem;

void init_set(OrderedSet* set, int (*cmp)(void*,void*));
void setAdd(OrderedSet* set, void* data);
void setErase(OrderedSet* set, void* data);
SetItem* setGet(OrderedSet* set, void* key);
bool setContains(OrderedSet* set, void* key);
int setSize(OrderedSet* set);
int setEmpty(OrderedSet* set);

OrderedSet* setUnion(OrderedSet* lhs, OrderedSet* rhs);
OrderedSet* setIntersection(OrderedSet* lhs, OrderedSet* rhs);
OrderedSet* setDifference(OrderedSet* lhs, OrderedSet* rhs);
OrderedSet* setSymmetricDifference(OrderedSet* lhs, OrderedSet* rhs);

#endif