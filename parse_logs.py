import re
import subprocess
import csv
import time
from collections import defaultdict

# Regex pattern matching the log format after the ESP-IDF prefix
log_pattern = re.compile(
    r"TickCount\(ticks\)=(\d+), Timestamp\(us\)=(\d+)\s*, EventType=(\S+), Task=(\S+), Queue=(\S+), TickWait\(ticks\)=(\S+), CoreID=(\d+)"
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
    ["./monitor.sh"],
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
            if queue in ["0x0", "NULL", "0"]:
                queue = "None"
            wait = match.group(6)
            core_id = match.group(7)

            timestamps.append(ts)
            event_counts[event] += 1

            # Append CoreID to the task name to create separate rows for Core 0 and Core 1
            task_with_core = f"{match.group(4)}_C{core_id}"
            
            # Write row to CSV
            csv_writer.writerow([tick, ts, event, task, queue, wait])
            csv_file.flush()  # make sure it's written immediately

            # Optional: print parsed log line
            print(f"{tick} ticks | {ts} us | Event: {event} | Task: {task} | Queue: {queue} | Wait: {wait}| Core: {task_with_core}")

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

    # Launch visualization script
    try: 
        print("Launching visualization script ...")
        proc = subprocess.Popen(["python3", "./visualize.py"])
        # Wait for some time
        time.sleep(2)  # 2 seconds
        
        # Terminate the process cleanly
        proc.terminate()  # send SIGTERM
        proc.wait(timeout=5)  # wait for process to exit
        print("Visualization script terminated.")
    except Exception as e:
        print(f"Failed to launch visualize.py: {e}")
