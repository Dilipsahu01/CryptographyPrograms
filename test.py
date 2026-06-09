import subprocess
import os

tests = [
    {"cmd": ["python3", "aespy.py"], "name": "AES (Python)", "expected": "Ciphertext: 69c4e0d86a7b0430d8cdb78070b4c55a"},
    {"cmd": ["python3", "despy.py"], "name": "DES (Python)", "expected": "Ciphertext: 0x85E813540F0AB405"},
    {"cmd": ["python3", "rsapy.py"], "name": "RSA (Python)", "expected": "Decrypted : Hello RSA From Scratch"},
    {"cmd": ["./aesc"], "name": "AES (C)", "expected": "Ciphertext: 69 c4 e0 d8 6a 7b 04 30 d8 cd b7 80 70 b4 c5 5a"},
    {"cmd": ["./desc"], "name": "DES (C)", "expected": "Ciphertext: 0x85E813540F0AB405"},
    {"cmd": ["./rsac"], "name": "RSA (C)", "expected": "Decrypted: RSA"},
]

def compile_c_files():
    print("--- Compiling C files ---")
    c_files = [("aesc.c", "aesc"), ("desc.c", "desc"), ("rsac.c", "rsac")]
    for src, out in c_files:
        try:
            subprocess.run(["gcc", "-o", out, src], check=True)
            print(f"Compiled {src} -> {out}")
        except subprocess.CalledProcessError:
            print(f"Failed to compile {src}")
            return False
    return True

def run_all_tests():
    if not compile_c_files():
        print("Compilation failed, aborting tests.")
        return

    print("\n--- Running Cryptographic Tests ---")
    passed = 0
    total = len(tests)

    for test in tests:
        print(f"\n> Testing {test['name']}...")
        try:
            result = subprocess.run(test["cmd"], capture_output=True, text=True, check=True)
            output = result.stdout.strip()
            print(output)
            
            if test["expected"] in output:
                print(f"[{test['name']}] ✅ PASS")
                passed += 1
            else:
                print(f"[{test['name']}] ❌ FAIL (Expected missing)")
        except Exception as e:
            print(f"[{test['name']}] ❌ FAIL (Error executing: {e})")

    print(f"\n========================================")
    print(f"              TEST SUMMARY              ")
    print(f"========================================")
    print(f" Passed: {passed} / {total}")
    if passed == total:
        print(" SUCCESS: All implementations match standard vectors!")
    else:
        print(" WARNING: Some implementations failed.")
    print(f"========================================")

if __name__ == '__main__':
    run_all_tests()
