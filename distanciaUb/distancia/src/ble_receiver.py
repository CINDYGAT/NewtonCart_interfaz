import asyncio
from bleak import BleakScanner, BleakClient

CHARACTERISTIC_UUID = "abcdef12-3456-789a-bcde-f0123456789a"

async def find_esp32():
    devices = await BleakScanner.discover()
    for device in devices:
        if "ESP32_Ultrasonico" in device.name:  # Buscar por nombre en lugar de MAC
            return device.address
    return None

async def read_ble():
    esp32_mac = await find_esp32()
    if not esp32_mac:
        print("No se encontró ESP32, revisa que esté encendido y publicando.")
        return

    async with BleakClient(esp32_mac) as client:
        print(f"Conectado a {esp32_mac}")

        while True:
            distancia = await client.read_gatt_char(CHARACTERISTIC_UUID)
            print(f"Distancia recibida: {distancia.decode()} cm")
            await asyncio.sleep(1)

asyncio.run(read_ble())

