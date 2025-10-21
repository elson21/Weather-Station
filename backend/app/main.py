from fastapi import FastAPI
from pydantic import BaseModel

app = FastAPI()

class Sensors(BaseModel):
    temperature: float
    humidity: float
    pressure: float
    light_level: float
    uv_index: int

@app.get("/")
def read_root():
    return {"message": "Welcome to my Weather Station"}

@app.post("/api/readings")
def sensor_readings(reading: Sensors) -> Sensors:
    global last_reading
    last_reading = reading
    print(f"Received: {reading}")

    return reading

@app.get("/api/readings")
def get_readings():
        if last_reading is None:
            return {"error": "No data yet"}
        return last_reading
    