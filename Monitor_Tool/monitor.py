import serial, psutil, time

ser = serial.Serial("/dev/ttyUSB0", 115200)  # change COM port

while True:
    cpu = psutil.cpu_percent(interval=0.5)
    ram = psutil.virtual_memory().percent

    try:
        temp = psutil.sensors_temperatures()['cpu-thermal'][0].current
    except:
        temp = 45  # fallback

    packet = f"{cpu},{ram},{temp}\n"
    ser.write(packet.encode())
