<script setup lang="ts">
// Tablero 1..100 con los primos encendidos: el motivo visual de la clase.
const isPrime = (k: number) => {
  if (k < 2)
    return false
  for (let d = 2; d * d <= k; d++) {
    if (k % d === 0)
      return false
  }
  return true
}
</script>

<template>
  <div class="slidev-layout portada">
    <div class="portada-body">
      <slot />
    </div>
    <div class="portada-grid" aria-hidden="true">
      <span v-for="k in 100" :key="k" :class="{ p: isPrime(k) }">{{ k }}</span>
    </div>
  </div>
</template>

<style scoped>
.portada {
  height: 100%; display: grid; grid-template-columns: 1.15fr 1fr; gap: 2.5rem; align-items: center;
  background: var(--tinta); color: #fff;
}
.portada-grid { display: grid; grid-template-columns: repeat(10, 1fr); gap: 5px; }
.portada-grid span {
  aspect-ratio: 1; display: flex; align-items: center; justify-content: center; border-radius: 6px;
  font-family: var(--slidev-code-font-family, monospace); font-size: 0.62rem;
  color: rgba(255, 255, 255, 0.28); border: 1px solid rgba(255, 255, 255, 0.08);
}
.portada-grid span.p { background: var(--primo); border-color: var(--primo); color: #fff; font-weight: 700; }
</style>
