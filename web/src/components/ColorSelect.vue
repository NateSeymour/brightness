<script setup lang="ts">
const colors = [
    'black', 'white',
    'red', 'green', 'blue',
];
const colorCount = colors.length;

const emit = defineEmits(['color-selected']);
</script>

<template>
<div class="color-select">
  <div
      v-for="color in colors"
      class="color"
      :style="{ background: color }"
      @click="() => emit('color-selected', color)"
  />
</div>
</template>

<style scoped lang="scss">
.color-select {
  display: grid;
  grid-template-columns: repeat(v-bind(colorCount), 1fr);
  place-items: center;

  .color {
    width: 2em;
    height: 2em;
    border: 1px solid black;
    user-select: none;
  }
}
</style>