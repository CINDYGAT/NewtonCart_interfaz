# NewtonCart_interfaz
Enviar y recibir distancias con BLE Ubuntu 22.04 Lee datos del sensor ultrasónico y los muestra utilizando el archivo main.cpp Envía datos Bluethooth utilizando ble_reciever.py para habilitar a la computadora recibir los datos del sensor utilizando Bleak

Documentación breve del proyecto: Registro y subida de datos a NextCloud
🧾 Descripción general

Este programa permite a un ESP32:

    Conectarse a una red WiFi

    Leer y simular datos de sensores (tiempo, aceleración, distancia, fuerza, velocidad)

    Guardar los datos en un archivo .csv en una tarjeta SD

    Subir periódicamente el archivo completo a un servidor NextCloud usando WebDAV

⚙️ Componentes utilizados

    ESP32

    Tarjeta microSD + módulo lector

    Servidor NextCloud accesible por IP local

    Librerías: WiFi.h, HTTPClient.h, SD.h, SPI.h

🌐 Flujo de funcionamiento

    Al iniciar, el ESP32 se conecta al WiFi y genera un archivo CSV en la SD con encabezado.

    Cada 5 segundos, simula nuevos datos y los guarda en el archivo.

    Luego, abre el archivo completo y lo envía mediante HTTP PUT a NextCloud.

    Los datos en la nube se sobrescriben en cada envío.
