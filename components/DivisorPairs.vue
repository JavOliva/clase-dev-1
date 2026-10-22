<script setup lang="ts">
import { computed, ref } from 'vue'

/**
 * Divisores de n en una recta, unidos en pares d · (n / d) = n.
 * Cada par tiene un extremo a cada lado de √n.
 */
const props = withDefaults(defineProps<{ n?: number, options?: number[] }>(), {
  n: 36,
  options: () => [36, 60, 100, 97],
})

const n = ref(props.n)
const divisors = computed(() => {
  const out: number[] = []
  for (let d = 1; d <= n.value; d++) {
    if (n.value % d === 0)
      out.push(d)
  }
  return out
})
const pairs = computed(() => divisors.value.filter(d => d * d <= n.value).map(d => [d, n.value / d] as const))
const root = computed(() => Math.sqrt(n.value))
const isPrime = computed(() => divisors.value.length === 2)

// Escala logarítmica: así cada par queda simétrico respecto a √n.
const W = 640
const pad = 30
const x = (v: number) => pad + (Math.log(v) / Math.log(n.value)) * (W - 2 * pad)
const lineY = 120
</script>

<template>
  <div class="dp">
    <div class="dp-opts">
      <span class="dp-label">n =</span>
      <button v-for="o in options" :key="o" :class="{ sel: o === n }" @click="n = o">{{ o }}</button>
    </div>

    <svg :viewBox="`0 0 ${W} 170`" class="dp-svg">
      <rect :x="pad" :y="lineY - 2" :width="x(root) - pad" height="4" rx="2" class="dp-left" />
      <rect :x="x(root)" :y="lineY - 2" :width="W - pad - x(root)" height="4" rx="2" class="dp-right" />
      <path
        v-for="([a, b], i) in pairs"
        :key="`p${i}`"
        :d="`M ${x(a)} ${lineY} Q ${(x(a) + x(b)) / 2} ${lineY - 22 - (x(b) - x(a)) * 0.32} ${x(b)} ${lineY}`"
        class="dp-arc"
        :class="{ trivial: a === 1 }"
      />
      <line :x1="x(root)" :x2="x(root)" :y1="16" :y2="lineY + 12" class="dp-root" />
      <text :x="x(root)" y="10" class="dp-root-t">√{{ n }} ≈ {{ root.toFixed(2) }}</text>
      <g v-for="d in divisors" :key="d">
        <circle :cx="x(d)" :cy="lineY" r="7" class="dp-dot" :class="{ small: d <= root }" />
        <text :x="x(d)" :y="lineY + 24" class="dp-num">{{ d }}</text>
      </g>
    </svg>

    <div class="dp-foot">
      <template v-if="isPrime">
        <b>{{ n }}</b> solo tiene el par trivial 1 · {{ n }}: <b class="dp-prime">es primo</b>.
        Para saberlo bastó revisar d = 2, …, {{ Math.floor(root) }}.
      </template>
      <template v-else>
        Pares:
        <code v-for="([a, b], i) in pairs" :key="i">{{ a }}·{{ b }}</code>
        — el menor de cada par siempre es ≤ √{{ n }}.
      </template>
    </div>
  </div>
</template>

<style scoped>
.dp { display: flex; flex-direction: column; gap: 0.4rem; }
.dp-opts { display: flex; align-items: center; gap: 0.35rem; }
.dp-label { font-family: var(--slidev-code-font-family, monospace); margin-right: 0.2rem; }
.dp-opts button {
  padding: 0.1rem 0.6rem; border-radius: 6px; border: 1px solid #cbd5e1; background: #fff; cursor: pointer;
  font-family: var(--slidev-code-font-family, monospace); font-size: 0.8rem;
}
.dp-opts button.sel { background: var(--tinta); border-color: var(--tinta); color: #fff; }
.dp-svg { width: 100%; overflow: visible; }
.dp-left { fill: var(--teal); opacity: 0.35; }
.dp-right { fill: #cbd5e1; }
.dp-arc { fill: none; stroke: var(--primo); stroke-width: 2; opacity: 0.85; }
.dp-arc.trivial { stroke-dasharray: 4 4; opacity: 0.5; }
.dp-root { stroke: var(--tinta); stroke-width: 1.5; stroke-dasharray: 4 4; }
.dp-root-t { text-anchor: middle; font-size: 12px; fill: var(--tinta); font-weight: 700; }
.dp-dot { fill: #fff; stroke: #94a3b8; stroke-width: 2; }
.dp-dot.small { fill: var(--teal); stroke: var(--teal); }
.dp-num { text-anchor: middle; font-size: 12px; fill: #334155; font-family: var(--slidev-code-font-family, monospace); }
.dp-foot { font-size: 0.85rem; color: #475569; }
.dp-foot code { margin: 0 0.15rem; }
.dp-prime { color: var(--primo); }
</style>
