<script setup lang="ts">
import DrawingBoard from "./components/DrawingBoard.vue";
</script>

<template>
  <div class="drawing-area">
    <DrawingBoard :width="32" :height="16" />
  </div>
</template>

<style scoped lang="scss">
</style>
