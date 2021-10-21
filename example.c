#include "list/arrayList.h"
#include "values/values.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compareLengths(Value first, Value second)
{
    if (first.type != second.type)
        return first.type - second.type;
    if (first.type != STRING_TYPE)
        return 0;
    return strlen(getString(first)) - strlen(getString(second));
}

void printValueType(Value value)
{
    switch (value.type) {
    case INT_TYPE:
        printf("Type of value %d if int\n", getInt(value));
        break;
    case DOUBLE_TYPE:
        printf("Type of value %f if int\n", getDouble(value));
        break;
    case STRING_TYPE:
        printf("Type of value \"%s\" is string\n", getString(value));
        break;
    case POINTER_TYPE:
        printf("Type of value %d is pointer\n", (int)getPointer(value));
        break;
    case NONE_TYPE:
        printf("Type of value is None\n");
        break;
    }
}

char* makeCopy(char* string)
{
    char* buffer = malloc(sizeof(char) * (strlen(string) + 1));
    strcpy(buffer, string);
    return buffer;
}

int main()
{
    // Тип Value позволяет хранить данные разных типов, используя конструкцию union
    // Давайте создадим обёртку для значения типа int
    Value value = wrapDouble(10.0);
    // Обратите внимание, что Value мы храним по значению,а не по указателю
    // В частности, это значит что нам не нужно очищать память из-под его хранения
    printValueType(value);
    // Так можно обратиться к значению конкретного типа, если вы уверены
    // какой конкретно тип хранится в union
    printf("5 + 5 = %f\n", value.doubleValue);
    // Но если вы обратитесь не к тому типу, то получите какой-то мусор вместо значения
    printf("5 + 5 = %d\n", value.intValue);
    // Так можно присвоить переменной новое значение. Не используйте
    // value.intValue = 20, чтобы не забыть одновременно поменять тип значения.
    value = wrapInt(20);
    // Чуть безопаснее можно получить значение интересующего вас типа через функции
    // getInt, getDouble и т.д. Внутри они делают assert, что в Value действительно
    // хранится интересующий вас тип. Если у вас они не отключены в настройках, то
    // программа должна падать при несовпадении типов
    printf("15 + 5 = %d\n", getInt(value));
    // Например, если вы раскомментируете строку ниже, то программа должна упасть.
    //    printf("15 - 5 = %s\n", getString(value));

    // В Value можно хранить и указатели.
    void* data = calloc(10, sizeof(int));
    Value pointer = wrapPointer(data);
    // Внутри они хранятся как void*, но вы можете самостоятельно привести
    // их к интересующему вас типу
    int* array = getPointer(pointer);
    printf("The first element of array is %d\n", array[0]);
    // Память из под value очищать не нужно, но вот из-под памяти, указатель на
    // которую хранит value, всё-таки придётся.
    free(getPointer(pointer));

    ArrayList* numbers = createArrayList();
    add(numbers, wrapInt(5));
    add(numbers, wrapInt(6));
    add(numbers, wrapInt(6));
    for (int i = 0; i < getSize(numbers); i++)
        printf("%d ", get(numbers, i).intValue);
    printf("\n");
    // Вот так можно использовать итератор для прохода по элементам структуры
    for (ArrayListIterator* iterator = getIterator(numbers); iterator; iterator = next(iterator))
        printf("%d ", getInt(getValue(iterator)));
    printf("\n");
    destroyArrayList(numbers);

    ArrayList* names = createArrayList();
    add(names, wrapString("Alyona"));
    add(names, wrapString("Artyom"));
    add(names, wrapString("Alice"));
    add(names, wrapString("Anton"));
    // В values.c определена функция compare, которая сравнивает примитивные типы по значениям,
    // строки с помощью strcmp, а остальные указатели по адресу, на который они указывают.
    // Вы можете использовать её для сравнения элементов
    printf("Index of \"%s\" is %d\n", "Artyom", find(names, wrapString("Artyom"), &compare));
    printf("Index of \"%s\" is %d\n", "Ivan", find(names, wrapString("Ivan"), &compare));
    // А вот так можно передать указатель на свою собственную функцию сравнения
    int sameLengthIndex = find(names, wrapString("Kolya"), &compareLengths);
    printf("Name \"%s\" at index %d has the same length as \"%s\"\n",
        getString(get(names, sameLengthIndex)), sameLengthIndex, "Kolya");
    // В этом списке мы хранили указатели на строковые константы, которые лежат
    // в статической области памяти, поэтому освобождать память из под них не нужно
    destroyArrayList(names);

    ArrayList* animals = createArrayList();
    // А в этот раз мы будем создавать копии строк
    add(animals, wrapString(makeCopy("Cat")));
    add(animals, wrapString(makeCopy("Dog")));
    add(animals, wrapString(makeCopy("Cow")));
    int goodBoyIndex = find(animals, wrapString("Dog"), &compare);
    printf("The animal \"%s\" at index %d is a good boy\n",
        getString(get(animals, goodBoyIndex)), goodBoyIndex);
    for (ArrayListIterator* iterator = getIterator(animals); iterator; iterator = next(iterator)) {
        // Не забываем очищать память из-под элементов, т.к. теперь они хранятся на куче.
        Value element = getValue(iterator);
        free(getString(element));
        // Можно и в одну строчку
        //        free(getPointer(getValue(iterator)));
    }
    destroyArrayList(animals);
    return 0;
}