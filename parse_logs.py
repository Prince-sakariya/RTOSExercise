import re
import subprocess
import csv
from collections import defaultdict

# Regex pattern matching the log format after the ESP-IDF prefix
log_pattern = re.compile(
    r"TickCount\(ticks\)=(\d+), Timestamp\(us\)=(\d+)\s*, EventType=(\S+), Task=(\S+), Queue=(\S+), TickWait\(ticks\)=(\S+)"
)
# Regex to remove ANSI escape codes
ansi_escape = re.compile(r'\x1B[@-_][0-?]*[ -/]*[@-~]')

# CSV file to save trace data
CSV_FILE = "log_entries.csv"

# Stats
total_entries = 0
event_counts = defaultdict(int)
timestamps = []

# Open CSV file and write header
csv_file = open(CSV_FILE, mode="w", newline="")
csv_writer = csv.writer(csv_file)
csv_writer.writerow(["tick", "Timestamp_us", "eventtype", "taskid", "Queue", "TickWait"])

# Start idf_monitor as a subprocess
process = subprocess.Popen(
    ["idf.py", "monitor"],
    stdout=subprocess.PIPE,
    stderr=subprocess.STDOUT,
    text=True,
    bufsize=1,
)

print("Listening for ESP32 logs... Press Ctrl+] to stop.")

try:
    for line in process.stdout:
        line = line.strip()
        # Remove ANSI codes
        line = ansi_escape.sub('', line)
        # Remove ESP-IDF prefix (e.g., I (19348) LOG: )
        if "LOG:" in line:
            line = line.split("LOG:")[1].strip()
        else:
            continue  # skip lines that are not log entries

        match = log_pattern.search(line)
        if match:
            total_entries += 1
            tick = int(match.group(1))
            ts = int(match.group(2))
            event = match.group(3)
            task = match.group(4)
            queue = match.group(5)
            wait = match.group(6)

            timestamps.append(ts)
            event_counts[event] += 1

            # Write row to CSV
            csv_writer.writerow([tick, ts, event, task, queue, wait])
            csv_file.flush()  # make sure it's written immediately

            # Optional: print parsed log line
            print(f"{tick} ticks | {ts} us | Event: {event} | Task: {task} | Queue: {queue} | Wait: {wait}")

except KeyboardInterrupt:
    print("Interrupted by user.")

finally:
    process.terminate() # Make sure monitor subprocess is killed. 
    csv_file.close()

    # Print summary on Ctrl+C
    time_span_us = max(timestamps) - min(timestamps) if timestamps else 0
    print("\n--- Trace Summary ---")
    print(f"Total log entries: {total_entries}")
    print("Entries per event type:")
    for e, c in event_counts.items():
        print(f"  {e}: {c}")
    print(f"Time span: {time_span_us} us ({time_span_us/1e6:.3f} s)")
    print(f"Trace data saved to {CSV_FILE}")
    print("Exiting parser.")
