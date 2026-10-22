<script setup lang="ts">
import { computed, onBeforeUnmount, ref } from 'vue'

/**
 * Criba de Eratóstenes paso a paso.
 * Cada paso procesa un primo p: tacha p², p² + p, p² + 2p, ...
 * Cada compuesto queda pintado con el color del primo que lo tachó primero.
 */
const props = withDefaults(defineProps<{ n?: number, cols?: number }>(), { n: 100, cols: 10 })

const COLORS = ['#e76f51', '#2a9d8f', '#e9a03b', '#6c5ce7', '#d6336c', '#1d7fbf']

// Primos p con p² <= N, en orden: son los pasos de la criba.
const steps = computed(() => {
  const ps: number[] = []
  for (let p = 2; p * p <= props.n; p++) {
    if (ps.every(q => p % q !== 0))
      ps.push(p)
  }
  return ps
})

// Quién tacha a cada número (el primer primo que lo marca, partiendo de p²).
const crossedBy = computed(() => {
  const by = new Map<number, number>()
  for (const p of steps.value) {
    for (let k = p * p; k <= props.n; k += p) {
      if (!by.has(k))
        by.set(k, p)
    }
  }
  return by
})

// step: 0 = nada procesado; 1..S = procesados los primeros k primos; S+1 = terminado
const step = ref(0)
const total = computed(() => steps.value.length + 1)
const current = computed(() => (step.value >= 1 && step.value <= steps.value.length ? steps.value[step.value - 1] : null))
const done = computed(() => step.value === total.value)

function state(k: number) {
  if (k === 1)
    return 'one'
  const by = crossedBy.value.get(k)
  const idx = by !== undefined ? steps.value.indexOf(by) : -1
  if (by !== undefined && idx < step.value)
    return 'crossed'
  if (steps.value.slice(0, step.value).includes(k) || (current.value === k))
    return 'prime'
  if (done.value)
    return 'prime'
  return 'idle'
}
function colorOf(k: number) {
  const s = state(k)
  if (s === 'crossed')
    return COLORS[steps.value.indexOf(crossedBy.value.get(k)!) % COLORS.length]
  if (s === 'prime') {
    const i = steps.value.indexOf(k)
    return i >= 0 ? COLORS[i % COLORS.length] : '#14213d'
  }
  return undefined
}

const crossedNow = computed(() => {
  const p = current.value
  if (!p)
    return 0
  let c = 0
  for (const [k, by] of crossedBy.value) {
    if (by === p && k)
      c++
  }
  return c
})
const primesCount = computed(() => {
  let c = 0
  for (let k = 2; k <= props.n; k++) {
    if (!crossedBy.value.has(k))
      c++
  }
  return c
})

let timer: ReturnType<typeof setInterval> | undefined
const playing = ref(false)
function pause() {
  playing.value = false
  if (timer)
    clearInterval(timer)
  timer = undefined
}
function play() {
  if (done.value)
    step.value = 0
  playing.value = true
  timer = setInterval(() => (step.value < total.value ? step.value++ : pause()), 1100)
}
onBeforeUnmount(pause)
</script>

<template>
  <div class="sv">
    <div class="sv-grid" :style="{ gridTemplateColumns: `repeat(${cols}, 1fr)` }">
      <div
        v-for="k in n"
        :key="k"
        class="sv-cell"
        :class="[state(k), { cur: current === k, start: current !== null && k === current * current }]"
        :style="{ '--c': colorOf(k) }"
      >
        {{ k }}
      </div>
    </div>

    <div class="sv-side">
      <div class="sv-row">
        <button :disabled="step === 0" @click="pause(); step = 0"><span class="i-carbon-skip-back-filled" /></button>
        <button :disabled="step === 0" @click="pause(); step--"><span class="i-carbon-chevron-left" /></button>
        <button class="wide" @click="playing ? pause() : play()">
          <span :class="playing ? 'i-carbon-pause-filled' : 'i-carbon-play-filled-alt'" /> {{ playing ? 'pausa' : 'reproducir' }}
        </button>
        <button :disabled="done" @click="pause(); step++"><span class="i-carbon-chevron-right" /></button>
      </div>

      <div class="sv-msg">
        <template v-if="step === 0">
          Todos los números desde 2 parten como <b>candidatos</b> a primo.
        </template>
        <template v-else-if="current">
          <b :style="{ color: colorOf(current) }">{{ current }}</b> no fue tachado, así que es primo.
          Tachamos sus múltiplos desde <b>{{ current }}² = {{ current * current }}</b>
          &nbsp;<span class="sv-dim">({{ crossedNow }} nuevos)</span>.
        </template>
        <template v-else>
          El siguiente candidato es mayor que √{{ n }} = {{ Math.sqrt(n).toFixed(1) }}: todo lo que quedó sin tachar es primo.
          <b>{{ primesCount }} primos</b> hasta {{ n }}.
        </template>
      </div>

      <div class="sv-legend">
        <span v-for="(p, i) in steps" :key="p" :style="{ '--c': COLORS[i % COLORS.length] }" :class="{ off: i >= step }">
          tachado por {{ p }}
        </span>
      </div>
    </div>
  </div>
</template>

<style scoped>
.sv { display: grid; grid-template-columns: 1fr 14.5rem; gap: 1.4rem; align-items: start; }
.sv-grid { display: grid; gap: 4px; }
.sv-cell {
  aspect-ratio: 1.45; display: flex; align-items: center; justify-content: center;
  border-radius: 6px; font-family: var(--slidev-code-font-family, monospace); font-size: 0.72rem;
  background: #fff; border: 1.5px solid #dbe2ea; color: #14213d;
  transition: all 0.35s ease;
}
.sv-cell.one { opacity: 0.25; }
.sv-cell.crossed {
  background: color-mix(in srgb, var(--c) 12%, #fff); border-color: color-mix(in srgb, var(--c) 35%, #fff);
  color: color-mix(in srgb, var(--c) 55%, #94a3b8); text-decoration: line-through;
}
.sv-cell.prime { background: var(--c, #14213d); border-color: var(--c, #14213d); color: #fff; font-weight: 700; }
.sv-cell.cur { transform: scale(1.12); box-shadow: 0 3px 12px rgba(0, 0, 0, 0.2); }
.sv-cell.start { outline: 2px solid var(--tinta); outline-offset: 1px; }

.sv-side { display: flex; flex-direction: column; gap: 0.8rem; font-size: 0.78rem; }
.sv-row { display: flex; gap: 0.3rem; }
.sv-row button {
  display: inline-flex; align-items: center; justify-content: center; gap: 0.3rem;
  padding: 0.2rem 0.55rem; min-height: 1.8rem; border-radius: 6px; border: 1px solid #cbd5e1; background: #fff; cursor: pointer;
}
.sv-row button.wide { flex: 1; }
.sv-row button:disabled { opacity: 0.35; cursor: default; }
.sv-msg { padding: 0.6rem 0.75rem; border-radius: 10px; background: #f1f4f8; line-height: 1.45; min-height: 5.2rem; }
.sv-dim { color: #64748b; }
.sv-legend { display: flex; flex-direction: column; gap: 0.25rem; }
.sv-legend span { display: flex; align-items: center; gap: 0.45rem; color: #475569; transition: opacity 0.3s; }
.sv-legend span::before { content: ''; width: 0.8rem; height: 0.8rem; border-radius: 3px; background: var(--c); }
.sv-legend span.off { opacity: 0.3; }
</style>
