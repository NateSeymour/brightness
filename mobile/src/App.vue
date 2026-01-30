<template>
  <header>
    <div v-if="isLoggedIn">Email: {{ identity.email }}</div>
  </header>
  <main class="container">
    <div v-if="isReady" class="locking-mechanism">
      <CircleButton
          class="button"
          :label="isLocked ? 'Locked' : 'Unlocked'"
          :color="isLocked ? 'green' : 'red'"
          @click="sendDoorCommand"
      />
    </div>
    <div v-else class="loading">
      <div v-for="_ in 3" class="dot"></div>
    </div>
  </main>
  <footer>
    <div class="status">

    </div>
  </footer>
</template>

<script setup lang="ts">
import {computed, onMounted, ref} from "vue";
import {openUrl} from "@tauri-apps/plugin-opener";
import {buildLoginUrl} from "./util/auth.ts";
import {getCurrent} from "@tauri-apps/plugin-deep-link";
import {LazyStore} from "@tauri-apps/plugin-store";
import {useDevice} from "./composable/useDevice.ts";

import CircleButton from "./component/CircleButton.vue";

const store = new LazyStore("auth.json");

const { connected } = useDevice();

const token = ref<string | null>(null);
const identity = computed(() => {
  if (!token.value) return null;

  const [_, payload] = token.value.split(".");
  const identity = JSON.parse(atob(payload));
  console.log(identity);
  if (identity.exp > Date.now() / 1000) {
    return identity;
  }

  return null;
});

const isLoggedIn = computed(() => !!identity.value);
const isConnected = ref<boolean>(true);
const doorAuthorizationToken = ref<string | null>(null);

const isReady = computed(() => {
  return isLoggedIn.value && isConnected.value && doorAuthorizationToken.value;
});

const isLocked = ref<boolean>(false);

async function sendDoorCommand() {
  // Send to BLE device
  isLocked.value = !isLocked.value;

  doorAuthorizationToken.value = null;
  refreshDoorAuthorizationToken();
}

async function doAuthFlow(): Promise<boolean> {
  token.value = await store.get<string>("token") || null;

  // We loaded a valid token from store.
  if (isLoggedIn.value) {
    return true;
  }

  // Else check if this is an auth callback.
  const startUrls = await getCurrent();
  if (startUrls) {
    for (const raw of startUrls) {
      const url = new URL(raw);

      const code = url.searchParams.get("code");
      if (code) {
        const response = await fetch(`${import.meta.env.VITE_AUTH_AUTHORITY}/oauth2/token`, {
          method: "POST",
          body: new URLSearchParams({
            "grant_type": "authorization_code",
            "code": code,
            "client_id": import.meta.env.VITE_AUTH_CLIENT_ID,
            "redirect_uri": "doorctl://auth",
          }),
        });

        if (response.ok) {
          const payload = await response.json();

          await store.set("token", payload.id_token);
          location.href = "/";

          return true;
        } else {
          console.error(response);
        }
      }
    }
  } else { // Else start the auth process.
    const loginUrl = buildLoginUrl({
      authority: import.meta.env.VITE_AUTH_AUTHORITY,
      clientId: import.meta.env.VITE_AUTH_CLIENT_ID,
      redirectUri: 'doorctl://auth',
      responseType: 'code',
      scope: ['email', 'openid'],
    });
    await openUrl(loginUrl);
  }

  return false;
}

async function refreshDoorAuthorizationToken() {
  if (await doAuthFlow()) {
    try {
      const response = await fetch(`${import.meta.env.VITE_API_ENDPOINT}/v1/access`, {
        method: "POST",
        headers: {
          "Authorization": `Bearer ${token.value}`,
        },
      });

      if (response.ok) {
        const payload = await response.json();
        doorAuthorizationToken.value = payload.token;
        console.log(payload.token);
      } else {
        throw "Failed to retrieve authorization token!";
      }
    } catch (e) {
      console.error(e);
    }
  }
}

onMounted(() => {
  refreshDoorAuthorizationToken();
});
</script>

<style scoped lang="scss">
header {
  padding: 1em;
}

.container {
  width: 100%;
  display: grid;
  place-items: center;

  .button {
    width: 70vw;
    height: 70vw;
  }

  .loading {
    width: 70vw;
    height: 70vw;
    border-radius: 100%;
    border: 2px solid white;
    display: flex;
    flex-direction: row;
    align-items: center;
    justify-content: center;
    gap: 1em;

    .dot {
      width: 2em;
      height: 2em;
      background: white;
      border-radius: 100%;
      animation: dot-loading-anim 500ms ease-in-out alternate infinite;
    }

    @for $i from 1 through 3 {
      :nth-child(#{$i}n) {
        animation-delay: #{$i * 100}ms;
      }
    }
    
    @keyframes dot-loading-anim {
      from {
        transform: scaleY(0.5);
      }

      to {
        transform: scaleY(1.5);
      }
    }
  }
}

footer {
  padding: 1em;
}
</style>