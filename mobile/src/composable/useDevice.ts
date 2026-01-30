import {onMounted, ref, watch} from "vue";
import {BleDevice, startScan} from "@mnlphlp/plugin-blec";
import {LazyStore} from "@tauri-apps/plugin-store";

export const useDevice = () => {
    const deviceHistory = new LazyStore("device-history.json");

    const devices = ref<BleDevice[]>([]);
    const device = ref<BleDevice | null>(null);

    const connected = ref(false);
    const autoRetry = ref(true);

    const sendCommand = (command: string) => {};

    const discover = async () => {};

    const connect = async (retry: boolean = true) => {
        
    };

    watch(connected, (old, current) => {
        if (old === true && current === false && autoRetry.value)
        {
            connect();
        }
    });

    return { connected, device, sendCommand, connect };
};