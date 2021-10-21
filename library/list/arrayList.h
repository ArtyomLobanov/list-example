#ifndef SPBU_C_HOMEWORKS_ARRAYLIST_H
#define SPBU_C_HOMEWORKS_ARRAYLIST_H

#include "values/values.h"

typedef struct ArrayList ArrayList;

ArrayList* createArrayList();
void destroyArrayList(ArrayList* list);

Value get(ArrayList* list, int index);
bool set(ArrayList* list, int index, Value value);
bool add(ArrayList* list, Value value);
int find(ArrayList* list, Value key, Comparator comparator);
int getSize(ArrayList* list);

typedef struct ArrayListIterator ArrayListIterator;

ArrayListIterator* getIterator(ArrayList* list);
Value getValue(ArrayListIterator* iterator);
ArrayListIterator* next(ArrayListIterator* iterator);

#endif // SPBU_C_HOMEWORKS_ARRAYLIST_H
