#include <stdio.h>
#include <stdint.h>
#include <string.h>

/*
 * RSA FROM SCRATCH (Toy Example without BigInt Library)
 * This demonstrates the core mathematics of RSA. Because standard C does 
 * not have a Big Integer type, we use 64-bit unsigned integers and small 
 * primes to avoid overflow during exponentiation.
 */

// Modular Exponentiation: (base^exp) % mod
uint64_t modPow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1;
    base = base % mod;
    while (exp > 0) {
        // If exp is odd, multiply base with result
        if (exp % 2 == 1) res = (res * base) % mod;
        exp = exp >> 1; // exp = exp / 2
        base = (base * base) % mod; // base = base^2
    }
    return res;
}

// Euclidean Algorithm for GCD
uint64_t gcd(uint64_t a, uint64_t b) {
    uint64_t temp;
    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Extended Euclidean Algorithm for Modular Inverse
int64_t modInverse(int64_t a, int64_t m) {
    int64_t m0 = m, t, q;
    int64_t x0 = 0, x1 = 1;
    if (m == 1) return 0;
    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) x1 += m0;
    return x1;
}

int main() {
    // 1. Choose two distinct prime numbers
    uint64_t p = 61;
    uint64_t q = 53;
    
    // 2. Compute n = p*q
    uint64_t n = p * q;
    
    // 3. Compute Euler's totient function phi(n) = (p-1)*(q-1)
    uint64_t phi = (p - 1) * (q - 1);
    
    // 4. Choose an integer e such that 1 < e < phi and gcd(e, phi) = 1
    uint64_t e = 17;
    
    // 5. Determine d as d = e^-1 mod phi
    uint64_t d = modInverse(e, phi);
    
    printf("--- RSA FROM SCRATCH TEST (Toy Example) ---\n");
    printf("Public Key  : (e=%lu, n=%lu)\n", e, n);
    printf("Private Key : (d=%lu, n=%lu)\n", d, n);
    
    // Example string to encrypt
    const char* msg = "RSA";
    printf("\nOriginal Message: %s\n", msg);
    
    // Encryption: c = m^e mod n
    uint64_t encrypted[10];
    int len = strlen(msg);
    printf("Encrypted: ");
    for (int i = 0; i < len; i++) {
        encrypted[i] = modPow((uint64_t)msg[i], e, n);
        printf("%lu ", encrypted[i]);
    }
    printf("\n");
    
    // Decryption: m = c^d mod n
    printf("Decrypted: ");
    for (int i = 0; i < len; i++) {
        uint64_t decrypted = modPow(encrypted[i], d, n);
        printf("%c", (char)decrypted);
    }
    printf("\n");
    
    return 0;
}
