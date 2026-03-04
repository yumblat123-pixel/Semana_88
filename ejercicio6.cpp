#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;

using u128 = __uint128_t;
using u64  = uint64_t;

// Multiplicación modular segura para evitar overflow (usa __uint128_t)
static inline u64 mul_mod(u64 a, u64 b, u64 mod) {
    return (u128)a * b % mod;
}

// Exponenciación modular rápida: (a^d) % mod
static inline u64 pow_mod(u64 a, u64 d, u64 mod) {
    u64 result = 1;
    while (d > 0) {
        if (d & 1) result = mul_mod(result, a, mod);
        a = mul_mod(a, a, mod);
        d >>= 1;
    }
    return result;
}

// Test de una base 'a' para n (devuelve false si detecta compuesto)
static bool check_witness(u64 a, u64 s, u64 d, u64 n) {
    u64 x = pow_mod(a % n, d, n);
    if (x == 1 || x == n - 1) return true;

    for (u64 r = 1; r < s; r++) {
        x = mul_mod(x, x, n);
        if (x == n - 1) return true;
    }
    return false; // compuesto
}

// Miller–Rabin: probable primo (Monte Carlo)
bool is_probable_prime(u64 n) {
    if (n < 2) return false;
    // Primos pequeños
    for (u64 p : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL}) {
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    // Escribimos n-1 = d * 2^s con d impar
    u64 d = n - 1;
    u64 s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    // Bases (a) elegidas: con este conjunto, para uint64, suele considerarse determinístico en práctica
    // (aun así, conceptualmente es Monte Carlo; más bases => menor prob. de error)
    vector<u64> bases = {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};

    for (u64 a : bases) {
        if (a % n == 0) continue;
        if (!check_witness(a, s, d, n)) return false; // compuesto
    }
    return true; // probablemente primo (en este set, muy confiable para 64-bit)
}

int main() {
    u64 n;
    cout << "Ingrese un numero (0..2^64-1): ";
    cin >> n;

    if (is_probable_prime(n)) {
        cout << "Probablemente primo\n";
    } else {
        cout << "Compuesto\n";
    }
    return 0;
}