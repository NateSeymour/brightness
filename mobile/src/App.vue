<template>
  <main class="container">
    <CircleButton
        class="button"
        :label="isLocked ? 'Locked' : 'Unlocked'"
        :color="isLocked ? 'green' : 'red'"
        @click="isLocked = !isLocked"
    />
  </main>
</template>

<script setup lang="ts">
import {onMounted, ref} from "vue";
import {openUrl} from "@tauri-apps/plugin-opener";
import {buildLoginUrl} from "./util/auth.ts";
import {getCurrent} from "@tauri-apps/plugin-deep-link";

import CircleButton from "./component/CircleButton.vue";

const identity = ref({});

const isConnected = ref<boolean>(false);
const isLocked = ref<boolean>(false);

onMounted(async () => {
  // Check if callback from deeplink
  const startUrls = await getCurrent();
  if (startUrls) {
  }
  // Check if user credentials are stored
  const isLoggedIn = false;
  // IF YES
  // - Start pinging for access tokens
  // IF NO
  // - Redirect to login page
  if (!isLoggedIn) {
    const loginUrl = buildLoginUrl({
      authority: import.meta.env.VITE_AUTH_AUTHORITY,
      clientId: import.meta.env.VITE_AUTH_CLIENT_ID,
      redirectUri: 'doorctl://auth',
      responseType: 'code',
      scope: ['email', 'openid'],
    });
    await openUrl(loginUrl);
  }
});
</script>

<style scoped lang="scss">
.container {
  height: 100%;
  width: 100%;
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;

  .button {
    width: 10em;
    height: 10em;
  }
}
</style>