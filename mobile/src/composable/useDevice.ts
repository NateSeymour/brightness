import {onMounted, ref} from "vue";
import {BleDevice, startScan} from "@mnlphlp/plugin-blec";

export const useDevice = () => {
    const devices = ref<BleDevice[]>([]);
    const device = ref<BleDevice | null>(null);
    const connected = ref(false);

    const sendCommand = (command: string) => {};

    const tryConnect = async () => {
        await startScan(dev => devices.value = dev, 1000);

        for (const device of devices) {

        }
    };

    onMounted(() => {
        tryConnect();
    });

    return { connected, device, sendCommand };
};