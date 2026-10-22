// Código de la clase "Teoría de números" — Taller de Programación Competitiva.
// Compilar y probar: g++ -O2 -std=c++17 teoria_de_numeros.cpp -o tdn && ./tdn
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll MOD = 1e9 + 7;

// 1. Test de primalidad en O(sqrt(n))
bool es_primo(ll n) {
    if (n < 2) return false;
    for (ll d = 2; d * d <= n; d++)
        if (n % d == 0) return false;
    return true;
}

// 1b. Descomposición prima por división por tentativa en O(sqrt(n))
vector<ll> factorizar_tentativa(ll n) {
    vector<ll> f;
    for (ll d = 2; d * d <= n; d++)
        while (n % d == 0) {
            f.push_back(d);
            n /= d;
        }
    if (n > 1) f.push_back(n);   // lo que queda es primo
    return f;
}

// 2. Criba de Eratóstenes en O(N log log N)
vector<bool> criba(int N) {
    vector<bool> primo(N + 1, true);
    primo[0] = primo[1] = false;
    for (ll p = 2; p * p <= N; p++)
        if (primo[p])
            for (ll k = p * p; k <= N; k += p)
                primo[k] = false;
    return primo;
}

// 2b. Criba con el menor factor primo (spf) y factorización en O(log n)
vector<int> criba_spf(int N) {
    vector<int> spf(N + 1, 0);
    for (int i = 2; i <= N; i++)
        if (spf[i] == 0) {                       // nadie lo tachó: es primo
            spf[i] = i;
            for (ll k = (ll)i * i; k <= N; k += i)
                if (spf[k] == 0) spf[k] = i;     // el primer primo que lo tacha es el menor
        }
    return spf;
}

// En las diapositivas spf es global; aquí se pasa como parámetro para poder probarla.
vector<int> factorizar(int n, const vector<int>& spf) {
    vector<int> f;
    while (n > 1) {
        f.push_back(spf[n]);
        n /= spf[n];
    }
    return f;
}

// 3. Aritmética modular
ll suma(ll a, ll b) { return (a % MOD + b % MOD) % MOD; }
ll resta(ll a, ll b) { return ((a - b) % MOD + MOD) % MOD; }
ll mult(ll a, ll b) { return (a % MOD) * (b % MOD) % MOD; }

// 4. Exponenciación binaria en O(log b)
ll potencia(ll a, ll b, ll m) {
    ll res = 1;
    a %= m;
    while (b > 0) {
        if (b & 1) res = res * a % m;
        a = a * a % m;
        b >>= 1;
    }
    return res;
}

// 5. Inverso modular (m primo, a no múltiplo de m)
ll inverso(ll a, ll m) { return potencia(a, m - 2, m); }

int main() {
    // es_primo contra la criba
    auto p = criba(100000);
    for (int n = 0; n <= 100000; n++) assert(es_primo(n) == p[n]);
    assert(es_primo(1000000007));
    assert(!es_primo(1000000007LL * 3));
    int cuenta = 0;
    for (int n = 0; n <= 100; n++) cuenta += p[n];
    assert(cuenta == 25);

    // factorización con spf contra división por tentativa
    auto spf = criba_spf(1000000);
    for (int n = 2; n <= 1000000; n++) {
        assert((spf[n] == n) == (n <= 100000 ? (bool)p[n] : es_primo(n)));
        vector<int> esperado;
        int m = n;
        for (int d = 2; d * d <= m; d++)
            while (m % d == 0) esperado.push_back(d), m /= d;
        if (m > 1) esperado.push_back(m);
        assert(factorizar(n, spf) == esperado);
    }
    assert((factorizar(360, spf) == vector<int>{2, 2, 2, 3, 3, 5}));

    // división por tentativa contra spf, y con números grandes
    for (int n = 2; n <= 1000000; n++) {
        auto a = factorizar_tentativa(n);
        auto b = factorizar(n, spf);
        assert(vector<ll>(b.begin(), b.end()) == a);
    }
    assert((factorizar_tentativa(999983LL * 1000003) == vector<ll>{999983, 1000003}));
    assert((factorizar_tentativa(1LL << 40) == vector<ll>(40, 2)));
    for (ll n = 999999999989LL - 100; n <= 999999999989LL; n++) {    // cerca de 10^12
        ll prod = 1;
        for (ll q : factorizar_tentativa(n)) assert(es_primo(q)), prod *= q;
        assert(prod == n);
    }

    // propiedades de suma y producto
    mt19937_64 rng(42);
    for (int i = 0; i < 100000; i++) {
        ll a = rng() % (ll)1e9, b = rng() % (ll)1e9;
        assert((a + b) % MOD == suma(a, b));
        assert((__int128)a * b % MOD == mult(a, b));
    }
    assert(resta(3, 5) == MOD - 2);

    // potencia contra el producto ingenuo
    for (ll a = 0; a < 20; a++)
        for (ll b = 0; b < 40; b++) {
            ll ing = 1 % MOD;
            for (int k = 0; k < b; k++) ing = ing * a % MOD;
            assert(potencia(a, b, MOD) == ing);
        }
    assert(potencia(3, 13, 1000) == 323);  // 3^13 = 1594323

    // inversos
    assert(inverso(2, MOD) == 500000004);
    assert(inverso(2, 7) == 4);
    for (ll a = 1; a < 2000; a++) assert(a * inverso(a, MOD) % MOD == 1);
    assert(6 * inverso(2, 7) % 7 == 3);

    puts("Todas las pruebas pasaron.");
}
