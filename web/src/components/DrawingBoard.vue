<script setup lang="ts">
import DrawingCell from "./DrawingCell.vue";
import {computed, ref} from "vue";
import ColorSelect from "./ColorSelect.vue";

const { width, height } = defineProps<{
  width: number,
  height: number,
}>();

const isPainting = ref<boolean>(false);
const color = ref<string>('red');

const canvasSize = computed(() => width * height);
const canvas = ref<string[]>(Array.from({ length: canvasSize.value }, () => 'white'));

function onDraw(index: number) {
  if(isPainting.value) {
    canvas.value[index] = color.value;
  }
}

function onMouseDown() {
  isPainting.value = true;
}

function onMouseUp() {
  isPainting.value = false;
}

function onColorSelect(newColor: string) {
  color.value = newColor;
}
</script>

<template>
  <div
      class="drawing-board"
      @mousedown="onMouseDown"
      @mouseup="onMouseUp"
      @mouseleave="onMouseUp"
  >
    <div class="cells">
      <DrawingCell v-for="i in canvasSize" :index="i" :color="canvas[i]!" @draw="onDraw"/>
    </div>

    <ColorSelect @color-selected="onColorSelect" />
  </div>
</template>

<style scoped>
.drawing-board {
  .cells {
    display: grid;
    grid-template-columns: repeat(v-bind(width), 1fr);
    border: 1px solid black;
  }
}
</style>