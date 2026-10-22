<script setup lang="ts">
defineProps<{ num?: string | number }>()
</script>

<template>
  <div class="slidev-layout seccion">
    <div v-if="num" class="seccion-num">
      {{ String(num).padStart(2, '0') }}
    </div>
    <div class="seccion-body">
      <slot />
    </div>
  </div>
</template>

<style scoped>
.seccion {
  height: 100%; display: flex; align-items: center; gap: 2.5rem;
  background: var(--tinta); color: #fff;
}
.seccion-num {
  font-family: var(--slidev-code-font-family, monospace);
  font-size: 7rem; font-weight: 700; line-height: 1; color: var(--primo);
}
.seccion-body { max-width: 36rem; }
</style>
