import serial
import csv
import time

# Configure serial connection
arduino_port = "COM3"  # Replace with your Arduino's port (e.g., COM3 on Windows, /dev/ttyUSB0 on Linux)
baud_rate = 9600
output_file = "sensor_data.csv"

# Open serial connection
try:
    ser = serial.Serial(arduino_port, baud_rate, timeout=1)
    print(f"Connected to {arduino_port}")
except Exception as e:
    print(f"Error: {e}")
    exit()

# Open CSV file to save data
with open(output_file, mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(["Sensor", "Centroid 1", "Centroid 2"])  # CSV header

    try:
        while True:
            # Read line from Arduino
            line = ser.readline().decode('utf-8').strip()
            if line.startswith("Centroids:"):
                print("Starting to receive centroid data...")
                continue

            # Parse centroid data and write to CSV
            if "," in line:
                data = line.split(",")
                if len(data) == 2:  # Ensure we have two values
                    sensor_id = len(data) - 2
                    writer.writerow([sensor_id, data[0], data[1]])
                    print(f"Saved: Sensor {sensor_id}, Centroid 1 = {data[0]}, Centroid 2 = {data[1]}")

            # Stop after a fixed duration or manual break
            time.sleep(0.1)

    except KeyboardInterrupt:
        print("Data collection stopped by user.")
        ser.close()
    except Exception as e:
        print(f"Error: {e}")
        ser.close()

# Close serial connection
ser.close()
