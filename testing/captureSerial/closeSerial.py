import serial.tools.list_ports

def close_all_serial_ports():
    # List all available serial ports
    ports = serial.tools.list_ports.comports()
    
    for port in ports:
        try:
            # Open the port
            ser = serial.Serial(port.device)
            # Close the port
            ser.close()
            print(f"Closed port: {port.device}")
        except Exception as e:
            print(f"Failed to close port {port.device}: {e}")

if __name__ == "__main__":
    close_all_serial_ports()