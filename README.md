# Teoría de números

Clase del Taller de Programación Competitiva (Universidad de Chile), hecha con [Slidev](https://sli.dev/).

**Ver la presentación:** https://javoliva.github.io/clase-dev-1/

Contenido: números primos, test de primalidad y descomposición prima en O(√n), criba de Eratóstenes (y factorización con el menor factor primo), aritmética modular con demostraciones, exponenciación binaria e inverso modular con el pequeño teorema de Fermat.

## Presentar localmente

```bash
npm install
npm run dev
```

Se abre en http://localhost:3030. Atajos: `F` pantalla completa, `O` vista general, y `/presenter` para la vista del presentador.

## Publicación

Cada push a `main` compila la presentación y la publica en GitHub Pages (`.github/workflows/pages.yml`).

## Código

`codigo/teoria_de_numeros.cpp` tiene todas las funciones de la clase con pruebas:

```bash
g++ -O2 -std=c++17 codigo/teoria_de_numeros.cpp -o tdn && ./tdn
```
