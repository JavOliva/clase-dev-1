<script setup lang="ts">
import { computed, ref } from 'vue'

/**
 * Comprueba con números concretos que reducir antes o después da lo mismo.
 * Usa BigInt: los productos de números de 9 dígitos superan la precisión de Number.
 */
const props = withDefaults(defineProps<{ a?: string, b?: string, m?: string, op?: '+' | '×' }>(), {
  a: '17',
  b: '25',
  m: '7',
  op: '+',
})

const a = ref(String(props.a))
const b = ref(String(props.b))
const m = ref(String(props.m))
const op = ref(props.op)

function parse(s: string): bigint | null {
  return /^\s*-?\d+\s*$/.test(s) ? BigInt(s.trim()) : null
}
const A = computed(() => parse(a.value))
const B = computed(() => parse(b.value))
const M = computed(() => parse(m.value))
const ok = computed(() => A.value !== null && B.value !== null && M.value !== null && M.value >= 2n)

const mod = (x: bigint, k: bigint) => ((x % k) + k) % k
const apply = (x: bigint, y: bigint) => (op.value === '+' ? x + y : x * y)

const left = computed(() => (ok.value ? apply(A.value!, B.value!) : 0n))
const ra = computed(() => (ok.value ? mod(A.value!, M.value!) : 0n))
const rb = computed(() => (ok.value ? mod(B.value!, M.value!) : 0n))
const inner = computed(() => apply(ra.value, rb.value))
const bound = computed(() => {
  const k = (M.value ?? 2n) - 1n
  return op.value === '+' ? `2(m − 1) = ${2n * k}` : `(m − 1)² = ${k * k}`
})
</script>

<template>
  <div class="mc">
    <div class="mc-inputs">
      <label>a <input v-model="a" inputmode="numeric"></label>
      <label>b <input v-model="b" inputmode="numeric"></label>
      <label>m <input v-model="m" inputmode="numeric"></label>
      <div class="mc-ops">
        <button :class="{ sel: op === '+' }" @click="op = '+'">+</button>
        <button :class="{ sel: op === '×' }" @click="op = '×'">×</button>
      </div>
    </div>

    <template v-if="ok">
      <div class="mc-cols">
        <div class="mc-col">
          <span class="mc-tag">Operar y después reducir</span>
          <div class="mc-line">({{ a }} {{ op }} {{ b }}) mod {{ m }}</div>
          <div class="mc-line">= {{ left }} mod {{ m }}</div>
          <div class="mc-line mc-res">= {{ mod(left, M!) }}</div>
        </div>
        <div class="mc-col">
          <span class="mc-tag">Reducir, operar y reducir</span>
          <div class="mc-line">(({{ a }} mod {{ m }}) {{ op }} ({{ b }} mod {{ m }})) mod {{ m }}</div>
          <div class="mc-line">= ({{ ra }} {{ op }} {{ rb }}) mod {{ m }}</div>
          <div class="mc-line">= {{ inner }} mod {{ m }}</div>
          <div class="mc-line mc-res">= {{ mod(inner, M!) }}</div>
        </div>
      </div>
      <div class="mc-check" :class="{ bad: mod(left, M!) !== mod(inner, M!) }">
        <span class="i-carbon-checkmark-filled" />
        <span>Ambos lados dan {{ mod(left, M!) }}. El lado derecho nunca pasa de {{ bound }}.</span>
      </div>
    </template>
    <div v-else class="mc-check bad">
      Ingresa enteros, con m ≥ 2.
    </div>
  </div>
</template>

<style scoped>
.mc { display: flex; flex-direction: column; gap: 0.9rem; }
.mc-inputs { display: flex; align-items: center; gap: 0.8rem; flex-wrap: wrap; }
.mc-inputs label { display: flex; align-items: center; gap: 0.35rem; font-family: var(--slidev-code-font-family, monospace); }
.mc-inputs input {
  width: 9rem; padding: 0.15rem 0.45rem; border: 1px solid #cbd5e1; border-radius: 6px;
  font-family: var(--slidev-code-font-family, monospace);
}
.mc-ops { display: flex; gap: 0.25rem; margin-left: 0.4rem; }
.mc-ops button { width: 2rem; height: 1.8rem; border-radius: 6px; border: 1px solid #cbd5e1; background: #fff; font-weight: 700; cursor: pointer; }
.mc-ops button.sel { background: var(--tinta); border-color: var(--tinta); color: #fff; }
.mc-cols { display: grid; grid-template-columns: 1fr 1fr; gap: 1rem; }
.mc-col { padding: 0.7rem 0.9rem; border-radius: 12px; background: #f1f4f8; display: flex; flex-direction: column; gap: 0.2rem; min-width: 0; }
.mc-tag { font-size: 0.62rem; font-weight: 700; letter-spacing: 0.12em; text-transform: uppercase; color: #64748b; margin-bottom: 0.2rem; }
.mc-line { font-family: var(--slidev-code-font-family, monospace); font-size: 0.78rem; overflow-wrap: anywhere; }
.mc-res { font-size: 1.15rem; font-weight: 700; color: var(--primo); }
.mc-check { display: flex; align-items: center; gap: 0.5rem; font-size: 0.85rem; color: #1f7a6f; }
.mc-check.bad { color: #b91c1c; }
</style>
