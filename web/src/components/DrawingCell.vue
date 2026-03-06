<script setup lang="ts">
const { index, color } = defineProps<{
  index: number;
  color: string;
}>();

const emit = defineEmits(['draw']);

function onDraw() {
  emit('draw', index);
}
</script>

<template>
<div
    class="cell"
    @mouseenter="onDraw"
/>
</template>

<style scoped lang="scss">
.cell {
  width: auto;
  height: auto;
  min-width: 1em;
  min-height: 1em;
  border: 1px solid black;
  background: v-bind(color);
  user-select: none;
}
</style>