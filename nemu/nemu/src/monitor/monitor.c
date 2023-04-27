#include "nemu.h"
// #include "hashmap.h"
#define ENTRY_START 0x100000

extern uint8_t entry [];
extern uint32_t entry_len;
extern char *exec_file;




/***************************/
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

Map *kernel_sym_map;

/****************************/

void load_elf_tables(int, char *[]);
void init_regex();
void init_wp_pool();
void init_ddr3();
void init_device();
void init_sdl();

void init_kernel_sym(){
	kernel_sym_map = create_map(MAP_CAPACITY);
	unsigned int count = 0;
	Data* data_array = read_file("kernel.sym", &count);

	int i = 0;
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

FILE *log_fp = NULL;

static void init_log() {
	log_fp = fopen("log.txt", "w");
	Assert(log_fp, "Can not open 'log.txt'");
}

static void welcome() {
	printf("Welcome to NEMU!\nThe executable is %s.\nFor help, type \"help\"\n",
			exec_file);
}

FILE *fp_func_log;

void init_monitor(int argc, char *argv[]) {
	/* Perform some global initialization */

	/* Open the log file. */
	init_log();

	// fp_func_log = fopen("func_log", "w");

	/* Load the string table and symbol table from the ELF file for future use. */
	// load_elf_tables(argc, argv);
	// init_kernel_sym();

	exec_file = argv[1];

	/* Compile the regular expressions. */
	init_regex();

	/* Initialize the watchpoint pool. */
	init_wp_pool();
	init_device();
	init_sdl();

	/* Display welcome message. */
	welcome();
}

#ifdef USE_RAMDISK
static void init_ramdisk() {
	int ret;
	const int ramdisk_max_size = 0xa0000;
	FILE *fp = fopen(exec_file, "rb");
	Assert(fp, "Can not open '%s'", exec_file);

	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);
	Assert(file_size < ramdisk_max_size, "file size(%zd) too large", file_size);

	fseek(fp, 0, SEEK_SET);
	ret = fread(hwa_to_va(0), file_size, 1, fp);
	assert(ret == 1);
	fclose(fp);
}
#endif

static void load_entry() {
	int ret;
	FILE *fp = fopen("entry", "rb");
	Assert(fp, "Can not open 'entry'");

	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);

	fseek(fp, 0, SEEK_SET);
	ret = fread(hwa_to_va(ENTRY_START), file_size, 1, fp);
	assert(ret == 1);
	fclose(fp);
}

static void init_CS(){
	cpu.cs.base = 0;
	cpu.cs.limit = 0xffffffff;
}

static void init_cr0(){
	cpu.cr0.protect_enable = 0;//real mode
	cpu.cr0.paging = 0;// paging mode
}

static void init_eflags(){
	cpu.EFLAGS = 0x00000002;
}
void restart() {
	/* Perform some initialization to restart a program */
	init_eflags();

#ifdef USE_RAMDISK
	/* Read the file with name `argv[1]' into ramdisk. */
	init_ramdisk();
#endif
	
	/* Read the entry code into memory. */
	load_entry();

	/* Set the initial instruction pointer. */
	cpu.eip = ENTRY_START;

	/* Initialize DRAM. */
	init_ddr3();

	/* Initialize Cache. */
	init_cache();
	
	/* Initialize CR0. */
	init_cr0();

	/* Initialize CS. */
	init_CS();
	
	/* Initialize TLB. */
	init_tlb();
}
