#include <stdio.h>
#include <stdint.h>

/*
 * 1. STANDARD DES PERMUTATION TABLES
 */

// Initial Permutation (IP)
static const int IP_TABLE[64] = {
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9,  1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};

// Final Permutation (FP / IP-1)
static const int FP_TABLE[64] = {
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41,  9, 49, 17, 57, 25
};

// Expansion P-Box (32 to 48 bits)
static const int EXPANSION_TABLE[48] = {
    32, 1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,
    8,  9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
};

// Straight P-Box (32 bits)
static const int P_BOX_TABLE[32] = {
    16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25
};

// Parity Drop (PC-1: 64 to 56 bits)
static const int PC1_TABLE[56] = {
    57, 49, 41, 33, 25, 17, 9,  1, 58, 50, 42, 34, 26, 18,
    10, 2,  59, 51, 43, 35, 27, 19, 11, 3,  60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15, 7,  62, 54, 46, 38, 30, 22,
    14, 6,  61, 53, 45, 37, 29, 21, 13, 5,  28, 20, 12, 4
};

// Compression P-Box (PC-2: 56 to 48 bits)
static const int PC2_TABLE[48] = {
    14, 17, 11, 24, 1,  5,  3,  28, 15, 6,  21, 10,
    23, 19, 12, 4,  26, 8,  16, 7,  27, 20, 13, 2,
    41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

// Key Shift Table for 16 Rounds
static const int SHIFT_TABLE[16] = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

/*
 * 2. ALL 8 S-BOXES
 */
static const int S_BOX[8][4][16] = {
    { // S1
        {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
        {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
        {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
        {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
    },
    { // S2
        {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
        {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
        {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
        {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
    },
    { // S3
        {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
        {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
        {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
        {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
    },
    { // S4
        {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
        {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
        {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
        {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
    },
    { // S5
        {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
        {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
        {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
        {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
    },
    { // S6
        {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
        {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
        {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
        {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
    },
    { // S7
        {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
        {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
        {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
        {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
    },
    { // S8
        {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
        {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
        {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
        {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
    }
};

/*
 * 3. UTILITY BIT-LEVEL FUNCTIONS
 */

// Universal routing function for all tables
uint64_t permute(uint64_t input, const int* table, int input_bits, int output_bits) {
    uint64_t output = 0;
    for (int i = 0; i < output_bits; i++) {
        int bit_pos = input_bits - table[i]; 
        uint64_t bit = (input >> bit_pos) & 1;
        output |= (bit << (output_bits - 1 - i));
    }
    return output;
}

// 28-bit circular left shift for key generation
uint32_t shift_left(uint32_t val, int shifts) {
    return ((val << shifts) | (val >> (28 - shifts))) & 0x0FFFFFFF;
}

/*
 * 4. CORE DES COMPONENT FUNCTIONS 
 */

uint64_t ip_set(uint64_t plaintext) {
    return permute(plaintext, IP_TABLE, 64, 64);
}

uint64_t parity_drop(uint64_t key) {
    return permute(key, PC1_TABLE, 64, 56);
}

uint64_t compression_p_box(uint64_t combined_key) {
    return permute(combined_key, PC2_TABLE, 56, 48);
}

void key_generator(uint64_t main_key, uint64_t subkeys[16]) {
    uint64_t key56 = parity_drop(main_key);
    uint32_t C = (key56 >> 28) & 0x0FFFFFFF;
    uint32_t D = key56 & 0x0FFFFFFF;

    for (int i = 0; i < 16; i++) {
        C = shift_left(C, SHIFT_TABLE[i]);
        D = shift_left(D, SHIFT_TABLE[i]);
        uint64_t combined = ((uint64_t)C << 28) | D;
        subkeys[i] = compression_p_box(combined);
    }
}

uint64_t expansion_p_box(uint32_t right_half) {
    return permute(right_half, EXPANSION_TABLE, 32, 48);
}

// Extracts 6 bits for each of the 8 S-Boxes, runs substitution, merges to 32 bits
uint32_t s_box_expansion(uint64_t expanded_input) {
    uint32_t output = 0;
    for (int i = 0; i < 8; i++) {
        int chunk = (expanded_input >> (42 - i * 6)) & 0x3F;
        int row = ((chunk & 0x20) >> 4) | (chunk & 0x01); 
        int col = (chunk & 0x1E) >> 1;                   
        
        int val = S_BOX[i][row][col]; 
        output |= (val << (28 - i * 4));
    }
    return output;
}

uint32_t straight_p_box(uint32_t sbox_output) {
    return permute(sbox_output, P_BOX_TABLE, 32, 32);
}

uint32_t f_function(uint32_t R, uint64_t K) {
    uint64_t expanded_R = expansion_p_box(R);
    uint64_t xor_result = expanded_R ^ K;       
    uint32_t s_box_out = s_box_expansion(xor_result);
    return straight_p_box(s_box_out);
}

void swap(uint32_t *L, uint32_t *R) {
    uint32_t temp = *L;
    *L = *R;
    *R = temp;
}

void des_round(uint32_t *L, uint32_t *R, uint64_t subkey) {
    uint32_t L_prev = *L;
    uint32_t R_prev = *R;
    *L = R_prev; 
    *R = L_prev ^ f_function(R_prev, subkey);
}

uint64_t final_permutation(uint64_t pre_output) {
    return permute(pre_output, FP_TABLE, 64, 64);
}

/*
 * 5. MAIN ENCRYPTION LOOP & EXECUTION
 */

uint64_t des_encrypt(uint64_t plaintext, uint64_t key) {
    uint64_t subkeys[16];
    key_generator(key, subkeys);

    uint64_t block = ip_set(plaintext);
    uint32_t L = (block >> 32) & 0xFFFFFFFF;
    uint32_t R = block & 0xFFFFFFFF;

    for (int i = 0; i < 16; i++) {
        des_round(&L, &R, subkeys[i]);
    }

    // Standard DES dictates we don't swap on the 16th round
    swap(&L, &R); 
    
    uint64_t pre_output = ((uint64_t)L << 32) | (uint64_t)R;
    return final_permutation(pre_output);
}

int main() {
    // Standard test vector.
    // Plaintext: "0123456789ABCDEF"
    // Key:       "133457799BBCDFF1"
    // Expected Output Ciphertext: "85E813540F0AB405"
    
    uint64_t plaintext = 0x0123456789ABCDEF; 
    uint64_t key       = 0x133457799BBCDFF1; 
    
    printf("--- DES ENCRYPTION TEST ---\n");
    printf("Plaintext:  0x%016lX\n", plaintext);
    printf("Key:        0x%016lX\n", key);
    
    uint64_t ciphertext = des_encrypt(plaintext, key);
    
    printf("Ciphertext: 0x%016lX\n", ciphertext);
    
    return 0;
}
