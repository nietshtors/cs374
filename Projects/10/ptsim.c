#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define PAGE_SHIFT 8  // Shift page number this much
#define PAGE_COUNT 64
#define PAGE_SIZE 256  // MUST equal 2^PAGE_SHIFT
#define MEM_SIZE (PAGE_SIZE * PAGE_COUNT)  // MUST equal PAGE_SIZE * PAGE_COUNT

#define PTP_OFFSET 64 // How far offset in page 0 is the page table pointer table

// Simulated RAM
unsigned char mem[MEM_SIZE];

//
// Convert a page,offset into an address
//
int get_address(int page, int offset)
{
    return (page << PAGE_SHIFT) | offset;
}

//
// Initialize RAM
//
void initialize_mem(void)
{
    memset(mem, 0, MEM_SIZE);

    int zpfree_addr = get_address(0, 0);
    mem[zpfree_addr] = 1;  // Mark zero page as allocated
}

//
// Get the page table page for a given process
//
unsigned char get_page_table(int proc_num)
{
    int ptp_addr = get_address(0, PTP_OFFSET + proc_num);
    return mem[ptp_addr];
}

//
// Allocate pages for a new process
//
// This includes the new process page table and page_count data pages.
//
void new_process(int proc_num, int page_count)
{
    int page_num = 0;
    int page_table_num;

    for (int i = 0; i < page_count + 1; i++) {
        while (mem[++page_num] && page_num <= PAGE_COUNT) {}
        if (page_num > PAGE_COUNT) {
            if (i == 0) {printf("OOM: proc %d: page table\n", proc_num);}
            else {printf("OOM: proc %d: data page\n", proc_num);}
            return;
        }
        mem[page_num] = 1;
        // Write process's page table
        if (i == 0) {
            mem[proc_num + PAGE_COUNT] = page_num;
            page_table_num = page_num;
        }
        else {
            mem[get_address(page_table_num, i - 1)] = page_num;
        }
    }
}

// 
// Killing a process frees all associated memory
// 
void kill_process(int proc_num)
{
    int page_table_num = mem[proc_num + PAGE_COUNT];

    for (int i = 0; i < PAGE_SIZE; i++) {
        int page_addr = get_address(page_table_num, i);
        if (mem[page_addr]) {
            for (int j = 0; j < PAGE_SIZE; j++) {
                mem[get_address(mem[page_addr], j)] = 0;
            }
            mem[mem[page_addr]] = 0;
        }
    }

    mem[page_table_num] = 0;
    mem[proc_num + PAGE_COUNT] = 0;
}

// 
// Converts a virtual address to a physical address
// 
int get_physical_address(int proc_num, int vaddr)
{
    int page_table = get_page_table(proc_num);
    int phys_page = mem[get_address(page_table, vaddr >> PAGE_SHIFT)];
    if (phys_page == 0) {
        printf("PAGE FAULT: proc %d, vaddr %d\n", proc_num, vaddr);
    }
    return (phys_page << PAGE_SHIFT) | (vaddr & 255);
}

// 
// Stores a value at a virtual address
// 
void store_value(int proc_num, int vaddr, int val)
{
    int addr = get_physical_address(proc_num, vaddr);
    mem[addr] = val;
    printf("Store proc %d: %d => %d, value=%d\n", proc_num, vaddr, addr, val);
}

// 
// Loads a value from a virtual address
// 
int load_value(int proc_num, int vaddr) 
{
    int addr = get_physical_address(proc_num, vaddr);
    int val = mem[addr];
    printf("Load proc %d: %d => %d, value=%d\n", proc_num, vaddr, addr, val);
    return val;
}

// 
// Adds pages to a process's memory
// 
void add_pages(int proc_num, int page_count)
{
    int page_num = 0;
    int page_table_num = get_page_table(proc_num);
    int page_table_start = 0;
    int i = 0;
    while (mem[get_address(page_table_num, i++)]) {page_table_start++;}

    for (int i = 0; i < page_count; i++) {
        while (mem[++page_num] && page_num <= PAGE_COUNT) {}
        if (page_num > PAGE_COUNT) {
            if (i == 0) {printf("OOM: proc %d: page table\n", proc_num);}
            else {printf("OOM: proc %d: data page\n", proc_num);}
            return;
        }
        mem[page_num] = 1;
        mem[get_address(page_table_num, page_table_start + i)] = page_num;
    }
}

//
// Print the free page map
//
// Don't modify this
//
void print_page_free_map(void)
{
    printf("--- PAGE FREE MAP ---\n");

    for (int i = 0; i < 64; i++) {
        int addr = get_address(0, i);

        printf("%c", mem[addr] == 0? '.': '#');

        if ((i + 1) % 16 == 0)
            putchar('\n');
    }
}

//
// Print the address map from virtual pages to physical
//
// Don't modify this
//
void print_page_table(int proc_num)
{
    printf("--- PROCESS %d PAGE TABLE ---\n", proc_num);

    // Get the page table for this process
    int page_table = get_page_table(proc_num);

    // Loop through, printing out used pointers
    for (int i = 0; i < PAGE_COUNT; i++) {
        int addr = get_address(page_table, i);

        int page = mem[addr];

        if (page != 0) {
            printf("%02x -> %02x\n", i, page);
        }
    }
}

//
// Main -- process command line
//
int main(int argc, char *argv[])
{
    assert(PAGE_COUNT * PAGE_SIZE == MEM_SIZE);

    if (argc == 1) {
        fprintf(stderr, "usage: ptsim commands\n");
        return 1;
    }
    
    initialize_mem();

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "pfm") == 0) {
            print_page_free_map();
        }
        else if (strcmp(argv[i], "ppt") == 0) {
            int proc_num = atoi(argv[++i]);
            print_page_table(proc_num);
        }
        else if (!strcmp(argv[i], "np")) {
            int proc_num = atoi(argv[++i]);
            int page_count = atoi(argv[++i]);
            new_process(proc_num, page_count);
        }
        else if (!strcmp(argv[i], "kp")) {
            int proc_num = atoi(argv[++i]);
            kill_process(proc_num);
        }
        else if (!strcmp(argv[i], "sb")) {
            int proc_num = atoi(argv[++i]);
            int virtual_address = atoi(argv[++i]);
            int value = atoi(argv[++i]);
            store_value(proc_num, virtual_address, value);
        }
        else if (!strcmp(argv[i], "lb")) {
            int proc_num = atoi(argv[++i]);
            int virtual_address = atoi(argv[++i]);
            load_value(proc_num, virtual_address);
        }
        else if (!strcmp(argv[i], "ap")) {
            int proc_num = atoi(argv[++i]);
            int page_count = atoi(argv[++i]);
            add_pages(proc_num, page_count);
        }

    }
}
