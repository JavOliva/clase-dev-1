import { defineConfig } from 'vite'

export default defineConfig({
  build: {
    // lightningcss falla al minificar un CSS interno de Slidev (números de línea del código).
    cssMinify: false,
  },
})
