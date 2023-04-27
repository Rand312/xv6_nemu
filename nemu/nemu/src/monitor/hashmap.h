#ifndef __HASH_MAP__
#define __HASH_MAP__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAP_CAPACITY 1000

typedef struct node {
    int key;
    char* value;
    struct node* next;
} Node;

typedef struct map {
    Node** buckets;
    int capacity;
} Map;

#define MAX_LINE_LENGTH 256

typedef struct data {
    int num;
    char str[MAX_LINE_LENGTH];
} Data;


int hash_func(int key, int capacity) {
    return key % capacity;
}

Node* create_node(int key, char* value) {
    Node* node = (Node*) malloc(sizeof(Node));
    node->key = key;
    node->value = (char*) malloc(strlen(value) + 1);
    strcpy(node->value, value);
    node->next = NULL;
    return node;
}

Map* create_map(int capacity) {
    Map* map = (Map*) malloc(sizeof(Map));
    map->buckets = (Node**) malloc(sizeof(Node*) * capacity);
    map->capacity = capacity;
    memset(map->buckets, 0, sizeof(Node*) * capacity);
    return map;
}

void put(Map* map, int key, char* value) {
    int index = hash_func(key, map->capacity);
    Node* node = create_node(key, value);
    node->next = map->buckets[index];
    map->buckets[index] = node;
}

char* get(Map* map, int key) {
    int index = hash_func(key, map->capacity);
    Node* node = map->buckets[index];
    while (node) {
        if (node->key == key) {
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}

int contains(Map* map, int key) {
    int index = hash_func(key, map->capacity);
    Node* node = map->buckets[index];
    while (node) {
        if (node->key == key) {
            return 1;
        }
        node = node->next;
    }
    return 0;
}



/************************************************************************/


Data* read_file(char* filename, int* count) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Failed to open file: %s\n", filename);
        return NULL;
    }

    Data* data_array = NULL;
    int capacity = 0;
    int size = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, fp)) {
        int num;
        char str[MAX_LINE_LENGTH];
        if (sscanf(line, "%x %s", &num, str) != 2) {
            printf("Failed to parse line: %s", line);
            continue;
        }

        if (size >= capacity) {
            capacity = (capacity == 0) ? 1 : capacity * 2;
            data_array = (Data*) realloc(data_array, sizeof(Data) * capacity);
            if (!data_array) {
                printf("Failed to allocate memory\n");
                break;
            }
        }

        data_array[size].num = num;
        strcpy(data_array[size].str, str);
        size++;
    }

    fclose(fp);
    *count = size;
    return data_array;
}

#endif