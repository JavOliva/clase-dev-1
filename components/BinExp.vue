<script setup lang="ts">
import { computed, ref, watch } from 'vue'

/** Exponenciación binaria paso a paso: un bit de b por iteración. */
const props = withDefaults(defineProps<{ a?: number, b?: number, m?: number }>(), { a: 3, b: 13, m: 1000 })

const a = ref(props.a)
const b = ref(props.b)
const m = ref(props.m)
const step = ref(0)
watch([a, b, m], () => (step.value = 0))

const mulmod = (x: bigint, y: bigint, k: bigint) => (x * y) % k

const rows = computed(() => {
  const out: { i: number, bit: number, base: bigint, res: bigint }[] = []
  if (!(b.value >= 1) || !(m.value >= 1))
    return out
  const M = BigInt(m.value)
  let base = ((BigInt(a.value) % M) + M) % M
  let res = 1n % M
  let e = BigInt(b.value)
  let i = 0
  while (e > 0n) {
    const bit = Number(e & 1n)
    if (bit)
      res = mulmod(res, base, M)
    out.push({ i, bit, base, res })
    base = mulmod(base, base, M)
    e >>= 1n
    i++
  }
  return out
})
const bits = computed(() => rows.value.map(r => r.bit).reverse().join(''))
const shown = computed(() => rows.value.slice(0, step.value))
const done = computed(() => step.value >= rows.value.length && rows.value.length > 0)
const used = computed(() => shown.value.filter(r => r.bit).map(r => `${a.value}^${2 ** r.i}`))
</script>

<template>
  <div class="be">
    <div class="be-inputs">
      <label>a <input v-model.number="a" type="number"></label>
      <label>b <input v-model.number="b" type="number" min="1"></label>
      <label>m <input v-model.number="m" type="number" min="1"></label>
      <span class="be-bin">b = {{ b }} = <b>{{ bits }}</b><sub>2</sub></span>
    </div>

    <table class="be-table">
      <thead>
        <tr>
          <th>iteración</th>
          <th>bit</th>
          <th>a<sup>2<sup>i</sup></sup> mod m</th>
          <th>res</th>
        </tr>
      </thead>
      <tbody>
        <tr v-for="r in shown" :key="r.i" :class="{ on: r.bit }">
          <td>{{ r.i }}</td>
          <td><span class="be-bit">{{ r.bit }}</span></td>
          <td><code>{{ a }}^{{ 2 ** r.i }}</code> ≡ {{ r.base }}</td>
          <td>{{ r.bit ? `res · ${r.base} → ${r.res}` : `${r.res} (no cambia)` }}</td>
        </tr>
        <tr v-if="!shown.length" class="be-empty">
          <td colspan="4">res = 1, base = a mod m</td>
        </tr>
      </tbody>
    </table>

    <div class="be-foot">
      <div class="be-ctrl">
        <button :disabled="step === 0" @click="step = 0"><span class="i-carbon-skip-back-filled" /></button>
        <button :disabled="step === 0" @click="step--"><span class="i-carbon-chevron-left" /></button>
        <button :disabled="done" @click="step++"><span class="i-carbon-chevron-right" /></button>
      </div>
      <div v-if="done" class="be-res">
        {{ a }}<sup>{{ b }}</sup> = {{ used.join(' · ') }} ≡ <b>{{ rows[rows.length - 1].res }}</b> (mod {{ m }})
        <span class="be-dim">· {{ rows.length }} iteraciones</span>
      </div>
    </div>
  </div>
</template>

<style scoped>
.be { display: flex; flex-direction: column; gap: 0.7rem; font-size: 0.8rem; }
.be-inputs { display: flex; align-items: center; gap: 0.8rem; flex-wrap: wrap; }
.be-inputs label { display: flex; align-items: center; gap: 0.35rem; font-family: var(--slidev-code-font-family, monospace); }
.be-inputs input {
  width: 5.5rem; padding: 0.15rem 0.45rem; border: 1px solid #cbd5e1; border-radius: 6px;
  font-family: var(--slidev-code-font-family, monospace);
}
.be-bin { margin-left: auto; font-family: var(--slidev-code-font-family, monospace); }
.be-bin b { color: var(--primo); letter-spacing: 0.08em; }
.be-table { width: 100%; border-collapse: separate; border-spacing: 0 3px; font-size: 0.78rem; }
.be-table th { text-align: left; font-size: 0.62rem; letter-spacing: 0.1em; text-transform: uppercase; color: #64748b; font-weight: 700; padding: 0 0.6rem; }
.be-table td { padding: 0.3rem 0.6rem; background: #f1f4f8; font-family: var(--slidev-code-font-family, monospace); }
.be-table td:first-child { border-radius: 8px 0 0 8px; }
.be-table td:last-child { border-radius: 0 8px 8px 0; }
.be-table tr.on td { background: #fdeee9; }
.be-table tr.on td:last-child { color: var(--primo); font-weight: 700; }
.be-bit { display: inline-block; width: 1.3rem; text-align: center; border-radius: 4px; background: #fff; }
tr.on .be-bit { background: var(--primo); color: #fff; }
.be-empty td { color: #94a3b8; }
.be-foot { display: flex; align-items: center; gap: 1rem; }
.be-ctrl { display: flex; gap: 0.3rem; }
.be-ctrl button {
  display: inline-flex; align-items: center; justify-content: center; padding: 0.2rem 0.6rem; min-height: 1.8rem;
  border-radius: 6px; border: 1px solid #cbd5e1; background: #fff; cursor: pointer;
}
.be-ctrl button:disabled { opacity: 0.35; cursor: default; }
.be-res { font-family: var(--slidev-code-font-family, monospace); }
.be-res b { color: var(--primo); font-size: 1rem; }
.be-dim { color: #94a3b8; }
</style>
