#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAP_CAPACITY 1000

typedef struct node {
    unsigned int key;
    char* value;
    struct node* next;
} Node;

typedef struct map {
    Node** buckets;
    unsigned int capacity;
} Map;

#define MAX_LINE_LENGTH 256

typedef struct data {
    unsigned int num;
    char str[MAX_LINE_LENGTH];
} Data;


unsigned int hash_func(unsigned int key, unsigned int capacity) {
    return key % capacity;
}

Node* create_node(unsigned int key, char* value) {
    Node* node = (Node*) malloc(sizeof(Node));
    node->key = key;
    node->value = (char*) malloc(strlen(value) + 1);
    strcpy(node->value, value);
    node->next = NULL;
    return node;
}

Map* create_map(unsigned int capacity) {
    Map* map = (Map*) malloc(sizeof(Map));
    map->buckets = (Node**) malloc(sizeof(Node*) * capacity);
    map->capacity = capacity;
    memset(map->buckets, 0, sizeof(Node*) * capacity);
    return map;
}

void put(Map* map, unsigned int key, char* value) {
    unsigned int index = hash_func(key, map->capacity);
    Node* node = create_node(key, value);
    node->next = map->buckets[index];
    map->buckets[index] = node;
}

char* get(Map* map, unsigned int key) {
    unsigned int index = hash_func(key, map->capacity);
    Node* node = map->buckets[index];
    while (node) {
        if (node->key == key) {
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}

unsigned int contains(Map* map, unsigned int key) {
    unsigned int index = hash_func(key, map->capacity);
    Node* node = map->buckets[index];
    while (node) {
        if (node->key == key) {
            return 1;
        }
        node = node->next;
    }
    return 0;
}
Data* read_file(char* filename, unsigned int* count) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Failed to open file: %s\n", filename);
        return NULL;
    }

    Data* data_array = NULL;
    unsigned int capacity = 0;
    unsigned int size = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, fp)) {
        unsigned int num;
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

unsigned int main() {
    Map *kernel_sym_map = create_map(MAP_CAPACITY);
	unsigned int count = 0;
	Data* data_array = read_file("kernel.sym", &count);

	unsigned int i = 0;
    for (i = 0; i < count; i++) {
        put(kernel_sym_map, data_array[i].num, data_array[i].str);
    }

	for (i = 0; i < count; i++) {
        char *str = get(kernel_sym_map, data_array[i].num);
		if(str){
			printf("%s\n", str);
		}
    }
}