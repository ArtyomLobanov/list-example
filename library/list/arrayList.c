#include "arrayList.h"
#include <stdlib.h>

#define INITIAL_SIZE 16

struct ArrayList {
    Value* data;
    int capacity;
    int size;
};

ArrayList* createArrayList()
{
    ArrayList* list = malloc(sizeof(ArrayList));
    if (!list)
        return NULL;
    list->capacity = INITIAL_SIZE;
    list->size = 0;
    list->data = malloc(INITIAL_SIZE * sizeof(Value));
    if (!list->data) {
        free(list);
        return NULL;
    }
    return list;
}
void destroyArrayList(ArrayList* list)
{
    free(list->data);
    free(list);
}

bool increaseCapacity(ArrayList* list)
{
    Value* buffer = realloc(list->data, 2 * list->capacity * sizeof(Value));
    if (buffer) {
        list->capacity *= 2;
        list->data = buffer;
    }
    return buffer;
}

Value get(ArrayList* list, int index)
{
    if (index < 0 || index >= list->size)
        return wrapNone();
    return list->data[index];
}

bool set(ArrayList* list, int index, Value value)
{
    if (index < 0 || index > list->size)
        return false;
    if (index == list->size)
        return add(list, value);
    list->data[index] = value;
    return true;
}

bool add(ArrayList* list, Value value)
{
    if (list->size == list->capacity && !increaseCapacity(list))
        return false;
    list->data[list->size++] = value;
    return true;
}

int find(ArrayList* list, Value key, Comparator comparator)
{
    for (int i = 0; i < list->size; i++) {
        if (comparator(list->data[i], key) == 0)
            return i;
    }
    return -1;
}

int getSize(ArrayList* list)
{
    return list->size;
}

struct ArrayListIterator {
    ArrayList* list;
    int index;
};

ArrayListIterator* getIterator(ArrayList* list)
{
    ArrayListIterator* iterator = malloc(sizeof(ArrayListIterator));
    iterator->list = list;
    iterator->index = 0;
    return iterator;
}

Value getValue(ArrayListIterator* iterator)
{
    return get(iterator->list, iterator->index);
}
ArrayListIterator* next(ArrayListIterator* iterator)
{
    iterator->index++;
    if (iterator->index < getSize(iterator->list))
        return iterator;
    free(iterator);
    return NULL;
}