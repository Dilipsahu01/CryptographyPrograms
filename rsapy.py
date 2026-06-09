import random

# RSA Implementation from scratch in Python
# No external crypto libraries are used. Demonstrates the core mathematics.

def gcd(a, b):
    """Euclidean algorithm to find greatest common divisor."""
    while b != 0:
        a, b = b, a % b
    return a

def mod_inverse(a, m):
    """Extended Euclidean algorithm to find modular inverse."""
    m0, y, x = m, 0, 1
    if m == 1: return 0
    while a > 1:
        q = a // m
        a, m = m, a % m
        x, y = y, x - q * y
    if x < 0: x += m0
    return x

def is_prime(n, k=5):
    """Miller-Rabin primality test."""
    if n <= 1 or n == 4: return False
    if n <= 3: return True
    d = n - 1
    s = 0
    while d % 2 == 0:
        d //= 2
        s += 1
    for _ in range(k):
        a = random.randint(2, n - 2)
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            continue
        for _ in range(s - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True

def generate_prime(bits):
    """Generates a prime number of the specified bit length."""
    while True:
        p = random.getrandbits(bits)
        p |= (1 << bits - 1) | 1  # ensure it's odd and has the correct bit length
        if is_prime(p):
            return p

def generate_keypair(bits=512):
    """Generates RSA public and private keys."""
    p = generate_prime(bits)
    q = generate_prime(bits)
    n = p * q
    phi = (p - 1) * (q - 1)
    
    e = 65537 # Common choice for public exponent
    if gcd(e, phi) != 1:
        e = 3
        
    d = mod_inverse(e, phi)
    return ((e, n), (d, n))

def encrypt(pk, plaintext):
    """Encrypts plaintext strings into integer arrays."""
    key, n = pk
    return [pow(ord(char), key, n) for char in plaintext]

def decrypt(pk, ciphertext):
    """Decrypts integer arrays back into plaintext strings."""
    key, n = pk
    return ''.join([chr(pow(char, key, n)) for char in ciphertext])

if __name__ == '__main__':
    print("--- RSA FROM SCRATCH TEST ---")
    print("Generating keypair (this might take a second)...")
    public, private = generate_keypair(512)
    
    msg = "Hello RSA From Scratch"
    print(f"Original  : {msg}")
    
    enc = encrypt(public, msg)
    print(f"Encrypted : {enc[:3]}... (truncated)")
    
    dec = decrypt(private, enc)
    print(f"Decrypted : {dec}")
