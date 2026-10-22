<script setup lang="ts">
import { computed, ref, watch } from 'vue'

/** Factoriza n dividiendo una y otra vez por su menor factor primo spf[n]. */
const props = withDefaults(defineProps<{ n?: number, max?: number }>(), { n: 360, max: 100000 })

const spf = (() => {
  const s = new Int32Array(props.max + 1)
  for (let i = 2; i <= props.max; i++) {
    if (s[i] === 0) {
      s[i] = i
      for (let k = i * i; k <= props.max; k += i) {
        if (s[k] === 0)
          s[k] = i
      }
    }
  }
  return s
})()

const input = ref(String(props.n))
const n = computed(() => {
  const v = Number(input.value)
  return Number.isInteger(v) && v >= 2 && v <= props.max ? v : null
})
const step = ref(0)
watch(n, () => (step.value = 0))

const chain = computed(() => {
  const out: { n: number, p: number }[] = []
  let m = n.value ?? 1
  while (m > 1) {
    out.push({ n: m, p: spf[m] })
    m /= spf[m]
  }
  return out
})
const done = computed(() => step.value >= chain.value.length)
const shown = computed(() => chain.value.slice(0, step.value))
const current = computed(() => (done.value ? 1 : chain.value[step.value]?.n ?? 1))

const powers = computed(() => {
  const m = new Map<number, number>()
  for (const { p } of chain.value) m.set(p, (m.get(p) ?? 0) + 1)
  return [...m.entries()]
})
</script>

<template>
  <div class="sf">
    <div class="sf-opts">
      <span class="sf-label">n =</span>
      <button v-for="o in [360, 1001, 1024, 9973, 97020]" :key="o" :class="{ sel: String(o) === input }" @click="input = String(o)">{{ o }}</button>
      <input v-model.trim="input" inputmode="numeric" :class="{ bad: n === null }">
    </div>

    <div class="sf-body">
      <div class="sf-steps">
        <div v-for="(s, i) in shown" :key="i" class="sf-step">
          <span class="sf-n">{{ s.n }}</span>
          <span class="sf-op">spf = <b>{{ s.p }}</b></span>
          <span class="sf-arrow">→ {{ s.n }} / {{ s.p }} = {{ s.n / s.p }}</span>
        </div>
        <div v-if="!done" class="sf-step next">
          <span class="sf-n">{{ current }}</span>
          <span class="sf-op">spf[{{ current }}] = ?</span>
        </div>
        <div v-else class="sf-step end">
          <span class="sf-n">1</span>
          <span class="sf-op">listo</span>
        </div>
      </div>

      <div class="sf-side">
        <div class="sf-ctrl">
          <button :disabled="step === 0" @click="step = 0"><span class="i-carbon-skip-back-filled" /></button>
          <button :disabled="step === 0" @click="step--"><span class="i-carbon-chevron-left" /></button>
          <button :disabled="done" @click="step++"><span class="i-carbon-chevron-right" /></button>
          <button :disabled="done" @click="step = chain.length"><span class="i-carbon-skip-forward-filled" /></button>
        </div>
        <div class="sf-chips">
          <span v-for="(s, i) in shown" :key="i" class="sf-chip">{{ s.p }}</span>
        </div>
        <div v-if="done && n" class="sf-res">
          {{ n }} =
          <template v-for="([p, e], i) in powers" :key="p">
            <span v-if="i" class="sf-dot">·</span>{{ p }}<sup v-if="e > 1">{{ e }}</sup>
          </template>
        </div>
        <div class="sf-count">
          {{ chain.length }} divisiones · log₂ {{ n ?? '?' }} ≈ {{ n ? Math.log2(n).toFixed(1) : '?' }}
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>
.sf { display: flex; flex-direction: column; gap: 0.8rem; font-size: 0.82rem; }
.sf-opts { display: flex; align-items: center; gap: 0.3rem; flex-wrap: wrap; }
.sf-label { font-family: var(--slidev-code-font-family, monospace); margin-right: 0.15rem; }
.sf-opts button, .sf-ctrl button {
  display: inline-flex; align-items: center; justify-content: center;
  min-height: 1.7rem; padding: 0.1rem 0.55rem; border-radius: 6px; border: 1px solid #cbd5e1; background: #fff; cursor: pointer;
  font-family: var(--slidev-code-font-family, monospace); font-size: 0.8rem;
}
.sf-opts button.sel { background: var(--tinta); border-color: var(--tinta); color: #fff; }
.sf-ctrl button:disabled { opacity: 0.35; cursor: default; }
.sf-opts input {
  width: 6.5rem; margin-left: 0.4rem; padding: 0.15rem 0.45rem; border: 1px solid #cbd5e1; border-radius: 6px;
  font-family: var(--slidev-code-font-family, monospace);
}
.sf-opts input.bad { border-color: #dc2626; }
.sf-body { display: grid; grid-template-columns: 1fr 13rem; gap: 1.2rem; align-items: start; }
.sf-steps { display: flex; flex-direction: column; gap: 0.3rem; max-height: 17rem; overflow: auto; }
.sf-step {
  display: grid; grid-template-columns: 4.5rem 6.5rem 1fr; align-items: baseline; gap: 0.5rem;
  padding: 0.3rem 0.7rem; border-radius: 8px; background: #f1f4f8;
  font-family: var(--slidev-code-font-family, monospace);
}
.sf-step.next { background: #fff; border: 1.5px dashed #cbd5e1; color: #94a3b8; }
.sf-step.end { background: #e8f5f3; color: var(--teal); }
.sf-n { font-weight: 700; color: var(--tinta); }
.sf-step.next .sf-n { color: #94a3b8; }
.sf-op b { color: var(--primo); }
.sf-arrow { color: #64748b; }
.sf-side { display: flex; flex-direction: column; gap: 0.7rem; }
.sf-ctrl { display: flex; gap: 0.3rem; }
.sf-chips { display: flex; flex-wrap: wrap; gap: 0.3rem; min-height: 1.8rem; }
.sf-chip {
  padding: 0.15rem 0.55rem; border-radius: 6px; background: var(--primo); color: #fff; font-weight: 700;
  font-family: var(--slidev-code-font-family, monospace);
}
.sf-res { font-family: var(--slidev-code-font-family, monospace); font-size: 1.1rem; font-weight: 700; color: var(--tinta); }
.sf-dot { margin: 0 0.25rem; color: #94a3b8; }
.sf-count { font-size: 0.72rem; color: #64748b; }
</style>
