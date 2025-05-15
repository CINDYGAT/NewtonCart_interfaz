from fastapi import FastAPI, Form
from datetime import datetime
import csv
import os

app = FastAPI()
DATA_DIR = "data"

# Asegúrate de que exista una carpeta para guardar los datos
if not os.path.exists(DATA_DIR):
    os.makedirs(DATA_DIR)

@app.post("/recolectar/")
async def recolectar(
    tiempo: float = Form(...),
    aceleracion: float = Form(...),
    distancia: float = Form(...),
    fuerza: float = Form(...),
    velocidad: float = Form(...)
):
    now = datetime.now()
    nombre_archivo = now.strftime("%Y-%m-%d") + ".csv"
    ruta = os.path.join(DATA_DIR, nombre_archivo)

    archivo_nuevo = not os.path.exists(ruta)

    with open(ruta, mode="a", newline="") as file:
        writer = csv.writer(file)
        if archivo_nuevo:
            writer.writerow(["timestamp", "tiempo", "aceleracion", "distancia", "fuerza", "velocidad"])
        writer.writerow([
            now.strftime("%Y-%m-%d %H:%M:%S"),
            tiempo,
            aceleracion,
            distancia,
            fuerza,
            velocidad
        ])
    return {"status": "ok", "mensaje": "Datos guardados exitosamente"}
