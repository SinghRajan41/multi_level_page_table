#include <bits/stdc++.h>
#define ll long long int
#define all(x) x.begin(), x.end()
using namespace std;

// Page table entry class to store physical memory address and security bits
class page_table_entry {
public:
    unsigned int phy_mem_addr_start; // Start of the physical memory address
    unsigned int security_bits;      // Security bits for the page

    // Default constructor initializes both fields to -1 (invalid value)
    page_table_entry() {
        phy_mem_addr_start = security_bits = -1;
    }

    // Parameterized constructor initializes the fields with provided values
    page_table_entry(const int phy_mem_addr_start, const int security_bits) {
        this->phy_mem_addr_start = phy_mem_addr_start; // Fix: Assign the value to the correct variable
        this->security_bits = security_bits;
    }
};

// Two-level page table class for memory management
class two_level_page_table {
    unsigned int level_1_bits, level_2_bits, page_offset_bits, addr_space_start, addr_space_end;
    page_table_entry **root; // Pointer to the root of the page table

public:
    // Constructor for initializing the two-level page table
    two_level_page_table(int level_1_bits, int level_2_bits, int page_offset_bits, int addr_space_start, int addr_space_end) {
        if((addr_space_end - addr_space_start + 1) > (1ll << 32)){
            throw invalid_argument("Virtual Address Space too large");
        }
        // Validate if the bits sum to 32 and are positive
        if(min({level_1_bits, level_2_bits, page_offset_bits}) > 0 && (level_1_bits + level_2_bits + page_offset_bits) == 32) {
            this->level_1_bits = level_1_bits;
            this->level_2_bits = level_2_bits;
            this->page_offset_bits = page_offset_bits;
            this->addr_space_start = addr_space_start;
            this->addr_space_end = addr_space_end;
            
            // Dynamically allocate memory for the root page table with entries for level 1
            root = new page_table_entry*[1 << level_1_bits];
            for(int i = 0; i < (1 << level_1_bits); i++) {
                root[i] = NULL;
            }
        } else {
            if(level_1_bits <= 0 || level_2_bits <= 0 || page_offset_bits <= 0) {
                throw invalid_argument("Bits should be positive integers.");
            } else {
                throw invalid_argument("Sum of bits should be 32.");
            }
        }
    }

    // Overloaded subscript operator to access page table entries by virtual address
    page_table_entry & operator[](int virtual_address) {
        // Check if the virtual address is within the specified address space
        if(virtual_address < addr_space_start || virtual_address > addr_space_end) {
            throw invalid_argument("Out of Bounds memory access");
        }

        int level_1_idx, level_2_idx, page_offset_idx;

        // Extract the page offset, level 2 index, and level 1 index from the virtual address
        page_offset_idx = virtual_address % (1 << page_offset_bits);
        virtual_address >>= page_offset_bits;
        level_2_idx = virtual_address % (1 << level_2_bits);
        virtual_address >>= level_2_bits;
        level_1_idx = virtual_address;

        // If the level 1 entry is not allocated, allocate memory for it
        if(root[level_1_idx] == NULL) {
            root[level_1_idx] = new page_table_entry[1 << level_2_bits];
            for(int i = 0; i < (1 << level_2_bits); i++) {
                root[level_1_idx][i] = page_table_entry(); // Initialize with default values
            }
        }

        // Return the page table entry for the given virtual address
        return root[level_1_idx][level_2_idx];
    }

    // Method to set a page table entry with the physical memory address and security bits
    void set_page_table_entry(int virtual_address, const unsigned int phy_mem_addr, const unsigned int security_bits) {
        int level_1_idx, level_2_idx, page_offset_idx;

        // Extract the page offset, level 2 index, and level 1 index from the virtual address
        page_offset_idx = virtual_address % (1 << page_offset_bits);
        virtual_address >>= page_offset_bits;
        level_2_idx = virtual_address % (1 << level_2_bits);
        virtual_address >>= level_2_bits;
        level_1_idx = virtual_address;

        // If the level 1 entry is not allocated, allocate memory for it
        if(root[level_1_idx] == NULL) {
            root[level_1_idx] = new page_table_entry[1 << level_2_bits];
            for(int i = 0; i < (1 << level_2_bits); i++) {
                root[level_1_idx][i] = page_table_entry(); // Initialize with default values
            }
        }

        // Set the physical memory address and security bits for the page table entry
        root[level_1_idx][level_2_idx] = page_table_entry(phy_mem_addr, security_bits);
    }
};

// Main function to demonstrate the use of the two-level page table
int main() {
    try {
        // Create a two-level page table with specific parameters (for example, 10 bits for level 1, 10 for level 2, 12 bits for the page offset)
        two_level_page_table page_table(10, 10, 12, 0, (1<<20) - 1); // 1KB address space (32-bit addressing)

        // Set a page table entry for a virtual address (example: address 0x1234)
        page_table.set_page_table_entry(1234, 54321, 11111);  // Physical address 0xABCD, security bits 0x1

        // Access the page table entry for the virtual address
        page_table_entry &entry = page_table[1234];

        // Output the physical address and security bits from the page table entry
        cout << "Physical Memory Address: " << entry.phy_mem_addr_start << endl;
        cout << "Security Bits: " << entry.security_bits << endl;
    } catch (const exception &e) {
        // Catch and print any exceptions thrown
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}
