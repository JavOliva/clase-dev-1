---
theme: seriph
title: Teoría de números
info: |
  ## Teoría de números
  Taller de Programación Competitiva · Universidad de Chile · Javier Oliva
layout: portada
colorSchema: light
canvasWidth: 900
transition: slide-left
mdc: true
fonts:
  sans: IBM Plex Sans
  serif: IBM Plex Serif
  mono: IBM Plex Mono
---

<span class="kicker">Taller de Programación Competitiva</span>

# Teoría de números

Primos, criba, aritmética modular, exponenciación binaria e inverso modular.

<div class="mt-10 text-sm leading-relaxed">
  <div class="font-semibold text-white">Javier Oliva</div>
  <div class="opacity-70">Universidad de Chile</div>
</div>

---

# Índice

<div class="flex flex-col gap-5 mt-8">
  <div class="flex items-center gap-4"><span class="badge-num">1</span><div><b class="text-lg">Números primos</b><div class="text-sm opacity-60">test de primalidad y descomposición prima en O(√n)</div></div></div>
  <div class="flex items-center gap-4"><span class="badge-num">2</span><div><b class="text-lg">Criba de Eratóstenes</b><div class="text-sm opacity-60">todos los primos hasta N y descomposición prima</div></div></div>
  <div class="flex items-center gap-4"><span class="badge-num">3</span><div><b class="text-lg">Aritmética modular</b><div class="text-sm opacity-60">suma y producto, con demostración</div></div></div>
  <div class="flex items-center gap-4"><span class="badge-num">4</span><div><b class="text-lg">Exponenciación binaria</b><div class="text-sm opacity-60">a<sup>b</sup> mod m en O(log b)</div></div></div>
  <div class="flex items-center gap-4"><span class="badge-num">5</span><div><b class="text-lg">Inverso modular</b><div class="text-sm opacity-60">dividir usando el pequeño teorema de Fermat</div></div></div>
</div>

---
layout: seccion
num: 1
---

<span class="kicker">Parte 1</span>

# Números primos

Y cómo saber si un número lo es sin probar todos sus posibles divisores.

---

# ¿Qué es un número primo?

<div class="mt-4">

Un entero $n \ge 2$ es **primo** si sus únicos divisores positivos son $1$ y $n$. Si no, es **compuesto**.

<div v-click class="flex flex-wrap gap-2 my-5 font-mono text-sm">
  <span v-for="p in [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37]" :key="p" class="px-2.5 py-1 rounded-md text-white font-bold" style="background: var(--primo)">{{ p }}</span>
  <span class="px-2 py-1 opacity-50">…</span>
</div>

<v-clicks>

- El $1$ **no** es primo (tiene un solo divisor).
- El $2$ es el **único primo par**.
- Todo entero $n \ge 2$ se escribe como **producto de primos**, de forma única salvo el orden. Por ejemplo, <span class="whitespace-nowrap">$60 = 2^2 \cdot 3 \cdot 5$</span>.

</v-clicks>

</div>

---

# Primer intento: probar todos los divisores

<div class="grid grid-cols-[1.1fr_1fr] gap-10 mt-4">
<div>

Si ningún $d$ entre $2$ y $n-1$ divide a $n$, entonces $n$ es primo.

```cpp
bool es_primo(long long n) {
    if (n < 2) return false;
    for (long long d = 2; d < n; d++)
        if (n % d == 0) return false;
    return true;
}
```

</div>
<div class="flex flex-col gap-4 justify-center">

<div v-click class="card primo">

### Costo: O(n)
Con $n = 10^9 + 7$ (que es primo) el ciclo da **mil millones** de vueltas.

</div>

<div v-click class="callout">

En un juez se hacen del orden de $10^8$ operaciones por segundo. Esto ya se pasa, y con $n = 10^{12}$ ni hablar.

</div>

</div>
</div>

---

# Los divisores vienen en pares

<div class="grid grid-cols-[1.35fr_1fr] gap-8 mt-2">
<div class="flex flex-col gap-4">
  <DivisorPairs :n="36" />

<div v-click="2" class="callout">

Basta revisar hasta $\sqrt{n}$: si ahí no hay divisores, **no hay ninguno**.

</div>

</div>
<div class="flex flex-col gap-3">

<div class="teorema">
<span class="teorema-tag">Lema</span>

Si $n$ es compuesto, tiene un divisor $d$ con $2 \le d \le \sqrt{n}$.

</div>

<div v-click="1" class="proof">
<span class="proof-tag">Demostración</span>

Sea $n = d \cdot e$ con $2 \le d \le e < n$. Supongamos que $d > \sqrt{n}$. Como $e \ge d$:

$$
n = d \cdot e \ge d \cdot d > \sqrt{n} \cdot \sqrt{n} = n
$$

Es decir, $n > n$: **contradicción**. Por lo tanto, $d \le \sqrt{n}$. $\blacksquare$

</div>

</div>
</div>

---

# Test de primalidad en O(√n)

<div class="grid grid-cols-[1.1fr_1fr] gap-10 mt-4">
<div>

```cpp {3}
bool es_primo(long long n) {
    if (n < 2) return false;
    for (long long d = 2; d * d <= n; d++)
        if (n % d == 0) return false;
    return true;
}
```

<div class="text-sm mt-4 leading-relaxed">

Usamos `d * d <= n` y no `d <= sqrt(n)`: **evita errores de punto flotante** con números grandes. Con `long long`, `d * d` no overflowea para $n \le 10^{18}$.

</div>

</div>
<div v-click class="flex flex-col gap-3 justify-center">

<div class="grid grid-cols-[auto_1fr_1fr] gap-x-4 gap-y-2 items-baseline text-sm">
  <span class="opacity-50 text-xs uppercase tracking-widest">n</span>
  <span class="opacity-50 text-xs uppercase tracking-widest">O(n)</span>
  <span class="opacity-50 text-xs uppercase tracking-widest">O(√n)</span>
  <span class="font-mono">10<sup>6</sup></span><span class="font-mono">10<sup>6</sup></span><span class="font-mono t-teal">1 000</span>
  <span class="font-mono">10<sup>9</sup> + 7</span><span class="font-mono t-primo">10<sup>9</sup></span><span class="font-mono t-teal">31 623</span>
  <span class="font-mono">10<sup>12</sup></span><span class="font-mono t-primo">10<sup>12</sup></span><span class="font-mono t-teal">10<sup>6</sup></span>
  <span class="font-mono">10<sup>18</sup></span><span class="font-mono t-primo">10<sup>18</sup></span><span class="font-mono t-teal">10<sup>9</sup></span>
</div>

<div class="callout mt-2">

Sirve para **un** número (o unos pocos). Para muchos números a la vez hay algo mejor.

</div>

</div>
</div>

---

<span class="kicker">Primos · Factorización</span>

# Descomposición prima en O(√n)

El mismo ciclo del test, pero cuando $d$ divide a $n$, lo **sacamos** todas las veces que se pueda.

<div class="grid grid-cols-[1.15fr_1fr] gap-8 mt-3">
<div class="min-w-0">

```cpp
vector<long long> factorizar(long long n) {
    vector<long long> f;
    for (long long d = 2; d * d <= n; d++)
        while (n % d == 0) {
            f.push_back(d);
            n /= d;
        }
    if (n > 1) f.push_back(n);  // es primo
    return f;
}
```

<div class="text-sm mt-3 opacity-80">

Ejemplo: $360 \to 2, 2, 2, 3, 3, 5$, en orden creciente.

</div>

<div v-click="3" class="callout text-sm mt-4">

Cuesta $O(\sqrt{n})$, igual que el test: sirve hasta $n \approx 10^{12}$.

</div>

</div>
<div class="flex flex-col gap-3">

<div v-click="1" class="proof">
<span class="proof-tag">Cada d que se agrega es primo</span>

Cuando $d$ divide a $n$, ya sacamos todos los primos menores que $d$. Si $d$ fuera compuesto, tendría un factor primo $q < d$ que también dividiría a $n$: contradicción.

</div>

<div v-click="2" class="proof">
<span class="proof-tag">Lo que queda al final es primo</span>

El ciclo termina con $d^2 > n$ sin encontrar divisores de $n$ hasta $\sqrt{n}$. Por el lema de los pares, $n$ no es compuesto: si $n > 1$, es primo.

</div>

</div>
</div>

---
layout: seccion
num: 2
---

<span class="kicker">Parte 2</span>

# Criba de Eratóstenes

Encontrar todos los primos hasta N tachando múltiplos, y usarla para factorizar.

---

# Todos los primos hasta N

<div class="grid grid-cols-2 gap-8 mt-6">
<div class="card">

### Usar el test en cada número
Probar $2, 3, \dots, N$ con el test de $O(\sqrt{n})$ cuesta $O(N\sqrt{N})$.

Con $N = 10^7$: unas $3 \cdot 10^{10}$ operaciones. **Demasiado.**

</div>
<div v-click class="card teal">

### Idea de Eratóstenes
En vez de preguntar *"¿quién divide a $n$?"*, cada primo **tacha a sus múltiplos**.

Lo que nunca se tacha es primo.

</div>
</div>

<div v-click class="flex flex-col gap-3 mt-8 text-sm">
  <div class="flex items-center gap-3"><span class="badge-num">1</span><span>Marcar todos los números desde 2 como candidatos.</span></div>
  <div class="flex items-center gap-3"><span class="badge-num">2</span><span>Tomar el menor candidato <i>p</i> que no esté tachado: <b>es primo</b>.</span></div>
  <div class="flex items-center gap-3"><span class="badge-num">3</span><span>Tachar sus múltiplos <i>p</i>², <i>p</i>² + <i>p</i>, <i>p</i>² + 2<i>p</i>, … mientras no pasen de <i>N</i>.</span></div>
  <div class="flex items-center gap-3"><span class="badge-num">4</span><span>Volver al paso 2 con el siguiente candidato, mientras <i>p</i>² ≤ <i>N</i>. Lo que quede sin tachar es primo.</span></div>
</div>

---

# La criba en acción

<div class="mt-2">
  <Sieve :n="100" />
</div>

<!--
Avanzar con la flecha del componente. Cada color es el primo que tachó al número primero (su menor factor primo). Al terminar con 7 (7² = 49 ≤ 100 < 121 = 11²) todo lo que queda sin tachar es primo: 25 primos.
-->

---

# ¿Por qué funciona?

<div class="grid grid-cols-2 gap-6 mt-4">
<div class="flex flex-col gap-3">

<div class="teorema">
<span class="teorema-tag">Afirmación 1</span>

Al procesar $p$, basta tachar **desde $p^2$**.

</div>

<div v-click class="proof">
<span class="proof-tag">Demostración</span>

Un múltiplo menor es $k = p \cdot j$ con $2 \le j < p$. Algún primo $q$ divide a $j$, y $q \le j < p$.

Así que $k$ ya fue tachado cuando procesamos $q$. $\blacksquare$

</div>

</div>
<div class="flex flex-col gap-3">

<div v-click class="teorema">
<span class="teorema-tag">Afirmación 2</span>

Basta procesar los primos con $p \le \sqrt{N}$.

</div>

<div v-click class="proof">
<span class="proof-tag">Demostración</span>

Todo compuesto $k \le N$ tiene un divisor primo $\le \sqrt{k} \le \sqrt{N}$ (el lema de los pares).

Ese primo ya lo tachó. Lo que sigue sin tachar es primo. $\blacksquare$

</div>

</div>
</div>

---

# Criba en C++

<div class="grid grid-cols-[1.15fr_1fr] gap-10 mt-4">
<div>

```cpp
vector<bool> criba(int N) {
    vector<bool> primo(N + 1, true);
    primo[0] = primo[1] = false;
    for (long long p = 2; p * p <= N; p++)
        if (primo[p])
            for (long long k = p * p; k <= N; k += p)
                primo[k] = false;
    return primo;
}
```

<div class="text-sm mt-4">

`vector<bool>` guarda un bit por número: con $N = 10^7$ son unos 1,25 MB.

</div>

</div>
<div class="flex flex-col gap-4 justify-center">

<div v-click class="card teal">

### Complejidad: O(N log log N)
Solo los **primos** tachan, y cada uno tacha unos $N/p$ números:

$$
\sum_{\substack{p \le N \\ p \text{ primo}}} \frac{N}{p} \approx N \ln \ln N
$$

Si tacháramos con **todo** $k$, sería la suma armónica: $\sum_{k \le N} \frac{N}{k} \approx N \ln N$.

</div>

<div v-click class="flex flex-col gap-2">
  <div class="text-sm font-semibold">Listar todos los primos hasta N = 10<sup>7</sup>:</div>
  <div class="flex gap-4 items-baseline">
    <span class="big-num t-teal whitespace-nowrap">≈ 3·10<sup>7</sup></span>
    <span class="text-sm opacity-70">operaciones con la criba</span>
  </div>
  <div class="flex gap-4 items-baseline">
    <span class="big-num t-primo whitespace-nowrap">≈ 3·10<sup>10</sup></span>
    <span class="text-sm opacity-70">aplicando el test O(√n)<br>a cada número de 2 a N</span>
  </div>
</div>

</div>
</div>

---
zoom: 0.9
---

<span class="kicker">Criba · Factorización</span>

# Guardar el menor factor primo

En vez de un `bool`, guardamos para cada número su **menor factor primo**, `spf[k]` (*smallest prime factor*). Los colores de la animación de la criba eran justamente `spf`.

<div class="grid grid-cols-[1.25fr_1fr] gap-6 mt-2">
<div class="min-w-0">

```cpp
vector<int> criba_spf(int N) {
    vector<int> spf(N + 1, 0);
    for (int i = 2; i <= N; i++)
        if (spf[i] == 0) {        // i es primo
            spf[i] = i;
            for (ll k = 1LL * i * i; k <= N; k += i)
                if (spf[k] == 0) spf[k] = i;
        }
    return spf;
}
```

<div class="text-sm mt-3 opacity-80">

Mismo costo que la criba: $O(N \log \log N)$. Aquí `ll` es `long long`.

</div>

</div>
<div class="flex flex-col gap-3">

<div class="teorema">
<span class="teorema-tag">Afirmación</span>

Para todo compuesto $k \le N$, `spf[k]` es su menor factor primo.

</div>

<div v-click class="proof">
<span class="proof-tag">Demostración</span>

Sea $p$ el menor primo que divide a $k$. Como $k$ es compuesto, $k \ge p^2$, así que $p$ lo alcanza al tachar desde $p^2$.

Los primos van en orden creciente y ninguno menor divide a $k$: $p$ escribe primero. $\blacksquare$

</div>

</div>
</div>

---

<span class="kicker">Criba · Factorización</span>

# Descomposición prima en O(log n)

<div class="grid grid-cols-[1.15fr_1fr] gap-8 mt-4">
<div class="min-w-0">

Para factorizar $n$: sacamos su menor factor primo `spf[n]`, dividimos y repetimos con lo que queda.

```cpp
vector<int> factorizar(int n) {  // spf global
    vector<int> f;
    while (n > 1) {
        f.push_back(spf[n]);
        n /= spf[n];
    }
    return f;
}
```

<div class="text-sm mt-3 opacity-80">

Los factores salen en orden creciente: $360 \to 2, 2, 2, 3, 3, 5$.

</div>

</div>
<div class="flex flex-col gap-4">

<div v-click class="proof">
<span class="proof-tag">¿Por qué O(log n)?</span>

Cada vuelta divide $n$ por un primo, que es $\ge 2$. Después de $t$ vueltas, $n$ se redujo al menos $2^t$ veces, así que hay a lo más $\log_2 n$ vueltas.

</div>

<div v-click class="callout text-sm">

**Cuándo conviene:** muchas consultas con $n \le N \approx 10^7$ (la tabla `spf` ocupa $4N$ bytes). Para un solo $n$ grande, hasta $10^{12}$, mejor división por tentativa hasta $\sqrt{n}$.

</div>

</div>
</div>

---

<span class="kicker">Criba · Factorización</span>

# Pruébalo: factorizar con spf

<div class="mt-4">
  <SpfFactor :n="360" />
</div>

<!--
Probar con 1024 (diez divisiones, el peor caso para su tamaño), con 9973 (primo: una sola) y con 97020 = 2²·3²·5·7²·11.
-->

---
layout: seccion
num: 3
---

<span class="kicker">Parte 3</span>

# Aritmética modular

Por qué podemos reducir módulo m en cualquier momento de una suma o un producto.

---

# "Imprime la respuesta módulo 10⁹ + 7"

<div class="grid grid-cols-[1.1fr_1fr] gap-10 mt-4">
<div>

Muchos problemas piden contar cosas cuya respuesta es **gigante**. Por ejemplo:

$$
25! = 15\,511\,210\,043\,330\,985\,984\,000\,000
$$

<div v-click class="text-sm mt-4">

Un `long long` llega hasta $\approx 9{,}2 \cdot 10^{18}$, y $25!$ ya **no cabe**: el cálculo overflowea sin avisar.

</div>

<div v-click class="callout mt-5">

Solución del enunciado: devolver solo el **resto** al dividir por $10^9 + 7$. Pero ¿podemos ir reduciendo *durante* el cálculo sin cambiar ese resto?

</div>

</div>
<div v-click class="flex flex-col gap-3 justify-center">

<div class="card teal">

### ¿Por qué 10⁹ + 7?
- Es **primo** (lo vamos a necesitar para dividir).
- Dos números menores que él se multiplican sin overflowear un `long long`: $(10^9)^2 = 10^{18}$.

</div>

</div>
</div>

---

# Definiciones

<div class="grid grid-cols-2 gap-6 mt-4">
<div class="flex flex-col gap-4">

<div class="teorema">
<span class="teorema-tag">División euclidiana</span>

Para todo entero $a$ y $m \ge 1$ existen **únicos** $q$ y $r$ con

$$
a = q \cdot m + r, \qquad 0 \le r < m.
$$

Llamamos $a \bmod m = r$.

</div>

<div v-click class="teorema">
<span class="teorema-tag">Congruencia</span>

$a \equiv b \pmod{m}$ significa que $m$ divide a $a - b$, es decir, que tienen **el mismo resto**.

</div>

</div>
<div class="flex flex-col gap-4">

<div v-click class="card">

### Lema (sumar múltiplos de m no cambia el resto)

$$
(k \cdot m + x) \bmod m = x \bmod m
$$

Si $x = q m + r$, entonces $k m + x = (k + q) m + r$, con el mismo $r$. Por la unicidad, ese es el resto.

</div>

<div v-click class="card primo">

### Ojo en C++
`%` conserva el signo: `-7 % 3 == -1`, no `2`. Para un resto entre $0$ y $m-1$:

```cpp
((a % m) + m) % m
```

</div>

</div>
</div>

---

<span class="kicker">Teorema</span>

# La suma respeta el módulo

<div class="teorema mt-2">

$$
(a + b) \bmod m \;=\; \big( (a \bmod m) + (b \bmod m) \big) \bmod m
$$

</div>

<div class="proof mt-5">
<span class="proof-tag">Demostración</span>

<v-clicks>

Escribimos $a = q_1 m + r_1$ y $b = q_2 m + r_2$, con $r_1 = a \bmod m$ y $r_2 = b \bmod m$.

Sumando: $\; a + b = (q_1 + q_2)\, m + (r_1 + r_2)$.

El primer término es un múltiplo de $m$, así que por el lema: $\;(a + b) \bmod m = (r_1 + r_2) \bmod m$.

Reemplazando $r_1$ y $r_2$ queda exactamente el lado derecho. $\blacksquare$

</v-clicks>

</div>

<div v-click class="text-sm mt-4 opacity-80">

Ojo: $r_1 + r_2$ puede llegar a $2m - 2$, por eso hace falta reducir otra vez al final.

</div>

---

<span class="kicker">Teorema</span>

# El producto también

<div class="teorema mt-2">

$$
(a \cdot b) \bmod m \;=\; \big( (a \bmod m) \cdot (b \bmod m) \big) \bmod m
$$

</div>

<div class="proof mt-5">
<span class="proof-tag">Demostración</span>

<v-clicks>

Con la misma notación, $a = q_1 m + r_1$ y $b = q_2 m + r_2$. Multiplicando:

$$
a \cdot b = q_1 q_2 m^2 + q_1 r_2\, m + q_2 r_1\, m + r_1 r_2 = \underbrace{(q_1 q_2 m + q_1 r_2 + q_2 r_1)}_{k}\, m + r_1 r_2
$$

Es un múltiplo de $m$ más $r_1 r_2$. Por el lema, $(a \cdot b) \bmod m = (r_1 r_2) \bmod m$. $\blacksquare$

</v-clicks>

</div>

<div v-click class="text-sm mt-4 opacity-80">

Con $m = 10^9 + 7$: $r_1 r_2 < m^2 \approx 10^{18}$, que **cabe** en un `long long`.

</div>

---

# Compruébalo con números

<div class="mt-4">
  <ModCheck a="123456789" b="987654321" m="1000000007" op="×" />
</div>

<div class="text-sm mt-6 opacity-70">

Prueba con números negativos, con la suma o con un $m$ pequeño como $7$. El resultado de ambos lados siempre coincide.

</div>

---

# Resta y división

<div class="grid grid-cols-2 gap-6 mt-4">
<div class="card teal">

### Resta: funciona, con cuidado
La misma demostración vale para $a - b$, pero en C++ el resultado puede salir **negativo**. Sumamos $m$ antes del último `%`.

</div>
<div v-click class="card primo">

### División: **no** funciona
Con $m = 4$:

$$
\tfrac{6}{2} \bmod 4 = 3 \quad\text{pero}\quad \tfrac{6 \bmod 4}{2 \bmod 4} = \tfrac{2}{2} = 1
$$

Para dividir necesitamos otra herramienta (parte 5).

</div>
</div>

<div v-click class="mt-6">

```cpp
const long long MOD = 1e9 + 7;

long long suma(long long a, long long b)  { return (a % MOD + b % MOD) % MOD; }
long long resta(long long a, long long b) { return ((a - b) % MOD + MOD) % MOD; }
long long mult(long long a, long long b)  { return (a % MOD) * (b % MOD) % MOD; }
```

</div>

---
layout: seccion
num: 4
---

<span class="kicker">Parte 4</span>

# Exponenciación binaria

Calcular a<sup>b</sup> mod m con b de hasta 10<sup>18</sup>.

---

# El problema

<div class="grid grid-cols-[1.1fr_1fr] gap-10 mt-4">
<div>

Calcular $a^b \bmod m$ con $b$ hasta $10^{18}$.

```cpp
long long res = 1;
for (long long i = 0; i < b; i++)
    res = res * a % m;          // O(b)
```

<div v-click class="text-sm mt-4">

Gracias a la parte 3 los números no overflowean, pero son $10^{18}$ vueltas: **siglos**.

</div>

</div>
<div v-click class="flex flex-col gap-3 justify-center">

<div class="card teal">

### Idea: elevar al cuadrado

$$
a^{b} =
\begin{cases}
\left(a^{\lfloor b/2 \rfloor}\right)^2 & b \text{ par} \\[4pt]
a \cdot \left(a^{\lfloor b/2 \rfloor}\right)^2 & b \text{ impar}
\end{cases}
$$

Cada paso **divide el exponente por 2**.

</div>

</div>
</div>

---

# Mirando b en binario

<div class="grid grid-cols-[1fr_1.2fr] gap-10 mt-4">
<div>

Si $b = 13 = 1101_2 = 8 + 4 + 1$:

$$
3^{13} = 3^{8} \cdot 3^{4} \cdot 3^{1}
$$

<v-clicks>

- Las potencias $a^1, a^2, a^4, a^8, \dots$ salen **elevando al cuadrado** la anterior: $a^{2^{i+1}} = \left(a^{2^i}\right)^2$.
- Multiplicamos al resultado solo las que tienen **bit 1** en $b$.
- $b$ tiene $\lfloor \log_2 b \rfloor + 1$ bits: con $b = 10^{18}$ son **60 iteraciones**.

</v-clicks>

</div>
<div>
  <BinExp :a="3" :b="13" :m="1000" />
</div>
</div>

---

# Exponenciación binaria en C++

<div class="grid grid-cols-2 gap-8 mt-4">
<div>

<div class="text-xs uppercase tracking-widest font-semibold mb-2 t-primo">Recursiva</div>

```cpp
ll potencia(ll a, ll b, ll m) {
    if (b == 0) return 1 % m;
    ll t = potencia(a, b / 2, m);
    t = t * t % m;
    if (b % 2 == 1) t = t * (a % m) % m;
    return t;
}
```

</div>
<div v-click>

<div class="text-xs uppercase tracking-widest font-semibold mb-2 t-teal">Iterativa</div>

```cpp
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
```

</div>
</div>

<div v-click class="callout mt-5">

Con `using ll = long long;` arriba del archivo. Ambas son $O(\log b)$. La iterativa recorre los bits de $b$ de menor a mayor: `b & 1` es el bit actual y `b >>= 1` pasa al siguiente.

</div>

---
layout: seccion
num: 5
---

<span class="kicker">Parte 5</span>

# Inverso modular

Cómo dividir cuando trabajamos módulo un primo.

---

# Dividir es multiplicar por el inverso

<div class="grid grid-cols-[1.1fr_1fr] gap-10 mt-4">
<div>

<div class="teorema">
<span class="teorema-tag">Definición</span>

El **inverso** de $b$ módulo $m$ es un $x$ tal que

$$
b \cdot x \equiv 1 \pmod{m}.
$$

Lo escribimos $b^{-1}$.

</div>

<div v-click class="text-sm mt-5 leading-relaxed">

Entonces "dividir por $b$" es **multiplicar por $b^{-1}$**:

$$
\frac{a}{b} \equiv a \cdot b^{-1} \pmod{m}
$$

</div>

</div>
<div v-click class="flex flex-col gap-4 justify-center">

<div class="card teal">

### Ejemplo con m = 7
$2 \cdot 4 = 8 \equiv 1$, así que $2^{-1} \equiv 4$.

$$
\frac{6}{2} \equiv 6 \cdot 4 = 24 \equiv 3 \pmod 7 \;\checkmark
$$

</div>

<div v-click class="card primo">

### ¿Siempre existe?
No: módulo $4$, el $2$ no tiene inverso ($2x$ siempre es par). Existe cuando $\gcd(b, m) = 1$. Con $m$ **primo**, todo $b \not\equiv 0$ lo tiene.

</div>

</div>
</div>

---

# Pequeño teorema de Fermat

<div class="teorema mt-2">
<span class="teorema-tag">Teorema</span>

Si $p$ es primo y $p \nmid a$, entonces $\; a^{p-1} \equiv 1 \pmod{p}$.

</div>

<div class="mt-5">
  <FermatPerm :p="7" :a="3" />
</div>

<!--
La clave de la demostración está en el dibujo: multiplicar 1..p-1 por a solo los cambia de orden. Mostrar p = 8 o 12 (compuestos) para ver que aparecen ceros y repetidos.
-->

---

# Demostración

<div class="flex flex-col gap-3 mt-2 text-[0.92rem]">

<div class="proof">
<span class="proof-tag">Paso 1 · los números a·1, a·2, …, a·(p−1) son distintos y no nulos (mod p)</span>

**No nulos:** si $p \mid a \cdot i$, como $p$ es primo, $p \mid a$ o $p \mid i$. Ninguna pasa, porque $p \nmid a$ y $1 \le i \le p - 1$.

**Distintos:** si $a i \equiv a j$, entonces $p \mid a(i - j)$, luego $p \mid i - j$. Como $|i - j| < p$, debe ser $i = j$.

</div>

<div v-click class="proof">
<span class="proof-tag">Paso 2 · multiplicarlos todos</span>

Son $p - 1$ restos distintos en $\{1, \dots, p-1\}$: es **el mismo conjunto** en otro orden. Multiplicando:

$$
(a \cdot 1)(a \cdot 2) \cdots (a \cdot (p-1)) \equiv 1 \cdot 2 \cdots (p-1)
\quad\Longrightarrow\quad
a^{p-1} \, (p-1)! \equiv (p-1)! \pmod p
$$

</div>

<div v-click class="proof">
<span class="proof-tag">Paso 3 · cancelar (p − 1)!</span>

$p \mid (a^{p-1} - 1)(p-1)!$, y $p \nmid (p-1)!$ porque es producto de números menores que $p$. Como $p$ es primo, $p \mid a^{p-1} - 1$, es decir, $a^{p-1} \equiv 1 \pmod p$. $\blacksquare$

</div>

</div>

---

# El inverso es a<sup>p−2</sup>

<div class="grid grid-cols-[1.1fr_1fr] gap-8 mt-4">
<div class="min-w-0">

<div class="teorema">
<span class="teorema-tag">Corolario</span>

Si $p$ es primo y $p \nmid a$, por Fermat:

$$
a \cdot a^{p-2} = a^{p-1} \equiv 1 \pmod p
$$

Luego $a^{p-2}$ es el inverso: $\; a^{-1} \equiv a^{p-2} \pmod p$.

</div>

<div v-click class="mt-5">

```cpp
ll inverso(ll a, ll p) {
    return potencia(a, p - 2, p);   // O(log p)
}
```

</div>

</div>
<div class="flex flex-col gap-4 justify-center">

<div v-click class="card teal">

### Ejemplo con p = 10⁹ + 7
`inverso(2, MOD)` da $500\,000\,004$, y efectivamente

$$
2 \cdot 500\,000\,004 = 1\,000\,000\,008 \equiv 1.
$$

</div>

<div v-click class="card primo">

### Condiciones
- El módulo tiene que ser **primo** (por eso $10^9 + 7$).
- $a \not\equiv 0 \pmod p$: el $0$ no tiene inverso.

</div>

</div>
</div>

---

# Resumen

<table class="resumen mt-6">
<thead><tr><th>Herramienta</th><th>Para qué</th><th>Costo</th></tr></thead>
<tbody>
<tr><td><b>Test de primalidad</b></td><td>¿Es primo un número?</td><td><code>O(√n)</code></td></tr>
<tr><td><b>División por tentativa</b></td><td>Factorizar un n (hasta ~10<sup>12</sup>)</td><td><code>O(√n)</code></td></tr>
<tr><td><b>Criba de Eratóstenes</b></td><td>Todos los primos hasta N</td><td><code>O(N log log N)</code></td></tr>
<tr><td><b>Criba con menor factor primo</b></td><td>Factorizar muchos n ≤ N</td><td><code>O(log n)</code> por consulta</td></tr>
<tr><td><b>Aritmética modular</b></td><td>Reducir en cada suma, resta y producto</td><td><code>O(1)</code></td></tr>
<tr><td><b>Exponenciación binaria</b></td><td>a<sup>b</sup> mod m</td><td><code>O(log b)</code></td></tr>
<tr><td><b>Inverso modular</b></td><td>Dividir módulo un primo: a<sup>p−2</sup></td><td><code>O(log p)</code></td></tr>
</tbody>
</table>

<div class="callout mt-8">

Todo el código de la clase, con pruebas, está en `codigo/teoria_de_numeros.cpp`.

</div>

---

# Problemas para practicar

<div class="grid grid-cols-2 gap-5 mt-6">

<a href="https://cses.fi/problemset/task/1095" target="_blank" class="card teal enlace">

### Exponentiation <span class="text-xs opacity-60 font-normal">CSES 1095</span>
Exponenciación binaria directa.

</a>

<a href="https://cses.fi/problemset/task/1712" target="_blank" class="card teal enlace">

### Exponentiation II <span class="text-xs opacity-60 font-normal">CSES 1712</span>
Calcular $a^{b^c}$: Fermat permite reducir el exponente módulo $p - 1$.

</a>

<a href="https://cses.fi/problemset/task/1713" target="_blank" class="card oro enlace">

### Counting Divisors <span class="text-xs opacity-60 font-normal">CSES 1713</span>
Muchas consultas: factoriza con `spf` y cuenta los divisores con los exponentes.

</a>

<a href="https://cses.fi/problemset/task/1079" target="_blank" class="card primo enlace">

### Binomial Coefficients <span class="text-xs opacity-60 font-normal">CSES 1079</span>
$\binom{a}{b} = \frac{a!}{b!\,(a-b)!}$: factoriales e inversos módulo $10^9 + 7$.

</a>

</div>
