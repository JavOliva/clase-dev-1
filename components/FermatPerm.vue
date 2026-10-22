<script setup lang="ts">
import { computed, ref, watch } from 'vue'

/**
 * Multiplicar 1, 2, ..., p − 1 por a (mod p) solo los reordena cuando p es primo.
 * Con p compuesto aparecen repetidos o ceros y el argumento de Fermat se cae.
 */
const props = withDefaults(defineProps<{ p?: number, a?: number, step?: number }>(), { p: 7, a: 3, step: 99 })

const p = ref(props.p)
const a = ref(props.a)
watch(p, (v) => {
  if (a.value >= v)
    a.value = 2
})

const mod = (x: number, k: number) => ((x % k) + k) % k
const isPrime = (k: number) => k >= 2 && Array.from({ length: k - 2 }, (_, i) => i + 2).every(d => d * d > k || k % d !== 0)

const items = computed(() => Array.from({ length: p.value - 1 }, (_, i) => i + 1))
const images = computed(() => items.value.map(i => mod(a.value * i, p.value)))
const counts = computed(() => {
  const c = new Map<number, number>()
  for (const v of images.value) c.set(v, (c.get(v) ?? 0) + 1)
  return c
})
const isPerm = computed(() => images.value.every(v => v !== 0 && counts.value.get(v) === 1))
const power = computed(() => {
  let r = 1
  for (let i = 0; i < p.value - 1; i++) r = mod(r * a.value, p.value)
  return r
})

const W = 620
const cx = (i: number) => 30 + ((i - 1) / Math.max(p.value - 2, 1)) * (W - 60)
</script>

<template>
  <div class="fp">
    <div class="fp-opts">
      <span class="fp-label">p =</span>
      <button v-for="o in [5, 7, 11, 13, 8, 12]" :key="o" :class="{ sel: o === p, comp: !isPrime(o) }" @click="p = o">{{ o }}</button>
      <span class="fp-label ml-4">a =</span>
      <button v-for="o in items.slice(1, 8)" :key="o" :class="{ sel: o === a }" @click="a = o">{{ o }}</button>
    </div>

    <svg :viewBox="`0 0 ${W} 150`" class="fp-svg">
      <text x="0" y="28" class="fp-row-t">i</text>
      <text x="0" y="128" class="fp-row-t">a·i</text>
      <line
        v-for="(i, k) in items"
        :key="`l${i}`"
        :x1="cx(i)" y1="40" :x2="images[k] === 0 ? cx(i) : cx(images[k])" y2="108"
        class="fp-arrow" :class="{ bad: images[k] === 0 || counts.get(images[k])! > 1 }"
      />
      <g v-for="i in items" :key="`t${i}`">
        <circle :cx="cx(i)" cy="24" r="15" class="fp-top" />
        <text :x="cx(i)" y="29" class="fp-n">{{ i }}</text>
      </g>
      <g v-for="(i, k) in items" :key="`b${i}`">
        <circle
          :cx="images[k] === 0 ? cx(i) : cx(images[k])" cy="124" r="15"
          class="fp-bot" :class="{ bad: images[k] === 0 || counts.get(images[k])! > 1 }"
        />
        <text :x="images[k] === 0 ? cx(i) : cx(images[k])" y="129" class="fp-n">{{ images[k] }}</text>
      </g>
    </svg>

    <div class="fp-msg" :class="{ bad: !isPerm }">
      <template v-if="isPerm">
        <b>{{ a }}·1, {{ a }}·2, …, {{ a }}·{{ p - 1 }}</b> (mod {{ p }}) son los mismos números 1, …, {{ p - 1 }}, solo que en otro orden.
        Por eso {{ a }}<sup>{{ p - 1 }}</sup> ≡ {{ power }} (mod {{ p }}).
      </template>
      <template v-else>
        Con {{ p }} {{ isPrime(p) ? '' : 'compuesto' }} y a = {{ a }} hay <b>ceros o repetidos</b>: no es una permutación y Fermat no aplica
        ({{ a }}<sup>{{ p - 1 }}</sup> ≡ {{ power }} mod {{ p }}).
      </template>
    </div>
  </div>
</template>

<style scoped>
.fp { display: flex; flex-direction: column; gap: 0.5rem; }
.fp-opts { display: flex; align-items: center; gap: 0.3rem; flex-wrap: wrap; }
.fp-label { font-family: var(--slidev-code-font-family, monospace); margin-right: 0.15rem; }
.fp-opts button {
  min-width: 2rem; padding: 0.1rem 0.45rem; border-radius: 6px; border: 1px solid #cbd5e1; background: #fff; cursor: pointer;
  font-family: var(--slidev-code-font-family, monospace); font-size: 0.8rem;
}
.fp-opts button.comp { border-style: dashed; color: #64748b; }
.fp-opts button.sel { background: var(--tinta); border-color: var(--tinta); color: #fff; }
.fp-svg { width: 100%; overflow: visible; }
.fp-row-t { font-size: 12px; fill: #94a3b8; font-family: var(--slidev-code-font-family, monospace); }
.fp-arrow { stroke: var(--teal); stroke-width: 1.8; opacity: 0.7; transition: all 0.4s; }
.fp-arrow.bad { stroke: #dc2626; }
.fp-top { fill: #fff; stroke: #94a3b8; stroke-width: 1.5; }
.fp-bot { fill: var(--teal); transition: cx 0.4s; }
.fp-bot.bad { fill: #dc2626; }
.fp-n { text-anchor: middle; font-size: 13px; font-weight: 700; font-family: var(--slidev-code-font-family, monospace); fill: #14213d; }
.fp-bot + .fp-n { fill: #fff; }
.fp-msg { font-size: 0.85rem; padding: 0.5rem 0.8rem; border-radius: 10px; background: #e8f5f3; color: #155e56; }
.fp-msg.bad { background: #fdecec; color: #991b1b; }
</style>
