import socket
import struct
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from collections import deque
import threading

# --- TCP settings ---
TCP_IP = '192.168.0.126'    # IP of the FPGA board
TCP_PORT = 7                # TCP port of the FPGA board
NUM_SAMPLES = 16384 * 2
BUFFER_SIZE = NUM_SAMPLES * 2  # 16-bit samples
FS = 64e6  # 64 MS/s

# --- Connect ---
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((TCP_IP, TCP_PORT))
sock.send(b'x')  # trigger first packet

# --- Data buffer ---
data_buffer = deque([0]*NUM_SAMPLES, maxlen=NUM_SAMPLES)

# --- IIR smoothing parameter ---
alpha = 0.05  # 0 < alpha <= 1; smaller = smoother

# --- Receive thread ---
def receive_data():
    while True:
        try:
            sock.send(b'x')
            raw = sock.recv(BUFFER_SIZE)
            if len(raw) >= 2:
                samples = struct.unpack(f'{len(raw)//2}h', raw)
                for s in samples:
                    if 100 < s < 3096:
                        data_buffer.append(s)
        except Exception as e:
            print("Receive error:", e)
            break

threading.Thread(target=receive_data, daemon=True).start()

# --- Plot setup ---
fig, ax = plt.subplots()
freqs = np.fft.rfftfreq(NUM_SAMPLES, 1/FS)
line, = ax.plot(freqs, np.zeros(len(freqs)), label="FFT Magnitude")
peak_line, = ax.plot([0, 0], [0, 0], 'r--', label="Peak Frequency")
peak_text = ax.text(0, 0, "", color='r', fontsize=10)

ax.set_xlabel("Frequency (Hz)")
ax.set_ylabel("Magnitude")
ax.set_title("Live FFT of ADC Data")
ax.set_xlim(0, FS/32) # show up to FS/32, e.g. 2 MHz for FS=64MHz
ax.set_ylim(0, 300000)  # adjust magnitude, may need tuning, default 300000
ax.legend()

# --- Previous smoothed data for IIR ---
prev_smoothed = np.zeros(NUM_SAMPLES)

def update(frame):
    global prev_smoothed
    if len(data_buffer) >= NUM_SAMPLES:
        y = np.array(data_buffer)
        y = y - np.mean(y)  # remove DC offset

        # --- Apply simple IIR smoothing ---
        prev_smoothed = alpha*y + (1-alpha)*prev_smoothed
        y_smooth = prev_smoothed

        # --- Compute FFT ---
        Y = np.fft.rfft(y_smooth)
        Y_mag = np.abs(Y)

        # Update FFT line
        line.set_ydata(Y_mag)

        # Find peak frequency
        peak_idx = np.argmax(Y_mag)
        peak_freq_val = freqs[peak_idx]
        peak_mag_val = Y_mag[peak_idx]

        # Update vertical line for peak
        peak_line.set_xdata([peak_freq_val, peak_freq_val])
        peak_line.set_ydata([0, ax.get_ylim()[1]])

        # Put peak text at top-right of plot
        ax_width = ax.get_xlim()[1]
        ax_height = ax.get_ylim()[1]
        peak_text.set_position((0.66*ax_width, 0.70*ax_height))
        peak_text.set_text(f"Peak: {peak_freq_val/1e3:.3f} KHz\nMag: {int(peak_mag_val)}")

    return line, peak_line, peak_text

ani = FuncAnimation(fig, update, interval=50, blit=True, cache_frame_data=False)
plt.show()
