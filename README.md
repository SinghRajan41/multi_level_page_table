# multi_level_page_table

# Two-Level Page Table Implementation

This repository contains a C++ implementation of a **two-level page table** for virtual memory management. The code provides a simple way to simulate a two-level page table structure and manage virtual to physical memory address translation.

## What is a Multilevel Page Table?

A **multilevel page table** is a memory management technique that divides the virtual address space into multiple levels of page tables. In such systems, the page table entries are split across multiple levels instead of using a single large table. This helps in reducing memory consumption by only allocating page table entries when required, instead of reserving a large amount of memory for the entire address space.

In a two-level page table, the virtual address is divided into:
1. **Level 1 index**: Points to a specific page table at the second level.
2. **Level 2 index**: Points to the actual page table entry for the physical address.
3. **Page offset**: Specifies the exact location within the page.

## Why is a Multilevel Page Table Needed?

In traditional, simple page tables, a **single level** page table is used to map virtual addresses to physical addresses. However, a single-level page table can become **inefficient** in terms of memory usage, especially when the virtual address space is very large but only a small portion of it is used. The entire page table is allocated for the entire address space, leading to wasted memory.

### Problems with a Simple Page Table:
1. **Memory Wastage**: A large address space requires a large page table, even if only a small portion of the address space is in use.
2. **Scalability**: As the size of the address space increases, the page table size grows linearly, which is not efficient for large systems.

### Benefits of Multilevel Page Tables:
- **Memory Efficiency**: Memory for page tables is only allocated as needed, reducing waste.
- **Scalability**: Large address spaces can be divided into smaller sections (levels), which makes the system more scalable and manageable.
- **Reduced Overhead**: By dividing the page table into levels, the system only needs to allocate memory for the levels that are in use, improving overall performance.

## Basic Working Idea of a Multilevel Page Table

A multilevel page table structure divides the virtual address into several parts. In the case of a two-level page table, the address is divided into:
1. **Level 1 Index**: A portion of the virtual address that maps to a higher-level page table.
2. **Level 2 Index**: A portion of the virtual address that maps to the actual page table entry (which holds the physical address).
3. **Page Offset**: The part of the virtual address that specifies the location of data within the page.

### Address Translation Process:
1. The virtual address is divided into **Level 1 index**, **Level 2 index**, and **Page offset**.
2. The Level 1 index points to a page table at the second level (a set of page table entries).
3. The Level 2 index identifies a specific page table entry within the second-level page table.
4. The page table entry holds the physical address, which is then combined with the page offset to form the final physical address.

## How My Code Implements This Multilevel Page Table

This code implements a **two-level page table** using the following components:

1. **Page Table Entry (`page_table_entry`)**:
   - Contains two fields: `phy_mem_addr_start` (the physical memory address) and `security_bits` (additional security-related information).

2. **Two-Level Page Table (`two_level_page_table`)**:
   - The class manages a two-level page table, where each level is dynamically allocated as needed.
   - It uses the virtual address to extract the **Level 1 index**, **Level 2 index**, and **Page offset**. This allows efficient translation of virtual addresses to physical memory addresses.
   - The table is represented as a pointer to an array of page table entries (`root`), where the first-level entries point to second-level page tables.
   - **Methods**:
     - `operator[]`: Overloaded to access a page table entry directly using a virtual address.
     - `set_page_table_entry`: Used to set the physical memory address and security bits for a specific virtual address.

## How to Use This Multilevel Page Table

### Step 1: Include the Header and Create the Page Table

To use the multilevel page table, instantiate the `two_level_page_table` class by providing the following parameters:
- `level_1_bits`: Number of bits for the first-level page table index.
- `level_2_bits`: Number of bits for the second-level page table index.
- `page_offset_bits`: Number of bits for the page offset.
- `addr_space_start`: The starting virtual address of the address space.
- `addr_space_end`: The ending virtual address of the address space.

For example:

```cpp
two_level_page_table page_table(10, 10, 12, 0, 1024);
