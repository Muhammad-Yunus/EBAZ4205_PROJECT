import socket
import struct
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from collections import deque
import threading

# --- TCP settings ---
TCP_IP = '192.168.0.126'    # IP of the FPGA board
TCP_PORT = 7                # TCP port of the FPGA board
NUM_SAMPLES = 100
BUFFER_SIZE = NUM_SAMPLES * 2
PLOT_LEN = 5000

# --- Connect ---
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((TCP_IP, TCP_PORT))

# Send initial trigger
sock.send(b'x')

# --- Data buffer ---
data_buffer = deque([0]*PLOT_LEN, maxlen=PLOT_LEN)

# --- Thread to receive data continuously ---
def receive_data():
    while True:
        try:
            sock.send(b'x')  # request next packet
            raw = sock.recv(BUFFER_SIZE)
            if len(raw) >= 2:
                samples = struct.unpack(f'{len(raw)//2}h', raw)
                for s in samples:
                    if s > 100 and s < 3000:
                        data_buffer.append(s)
        except Exception as e:
            print("Receive error:", e)
            break

threading.Thread(target=receive_data, daemon=True).start()

# --- Plot setup ---
fig, ax = plt.subplots()
line, = ax.plot(range(PLOT_LEN), list(data_buffer))
#ax.set_ylim(0, 4095)
ax.set_ylim(2000, 2800)
ax.set_xlabel("Samples")
ax.set_ylabel("ADC Value")
ax.set_title("Live ADC Plot")

def update(frame):
    line.set_ydata(list(data_buffer))
    return line,

ani = FuncAnimation(fig, update, interval=50, blit=True)
plt.show()
