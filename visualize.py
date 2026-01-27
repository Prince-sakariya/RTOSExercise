import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import re
import seaborn as sns
from typing import List, Tuple, Dict, Optional

def load_data(csv_file_path: str) -> Optional[pd.DataFrame]:
    try:
        df = pd.read_csv(csv_file_path)
    except Exception as e:
        print(f"Error reading CSV file: {e}")
        return None
    if df.empty: return None

    # Clean numeric columns
    df['tick'] = pd.to_numeric(df['tick'], errors='coerce')
    df['Timestamp_us'] = pd.to_numeric(df['Timestamp_us'], errors='coerce')
    df.dropna(subset=['tick', 'Timestamp_us'], inplace=True)
    
    # Normalize Microseconds to start at 0
    df['Timestamp_us'] = df['Timestamp_us'] - df['Timestamp_us'].min()
    return df

def get_segments(df: pd.DataFrame, time_column: str):
    """General function to extract segments based on ticks or microseconds"""
    task_ids = sorted(df['taskid'].unique())
    task_segments = {}
    
    for task_id in task_ids:
        task_df = df[df['taskid'] == task_id].sort_values(time_column)
        segments = []
        current_start = None
        
        for _, row in task_df.iterrows():
            etype = row['eventtype']
            ts = row[time_column]
            
            if etype == 'traceTASK_SWITCHED_IN':
                current_start = ts
            elif etype in ['traceTASK_SWITCHED_OUT', 'traceTASK_DELETE'] and current_start is not None:
                # If using ticks and duration is 0, give it a 0.5 tick width for visibility
                duration = ts - current_start
                if time_column == 'tick' and duration == 0:
                    duration = 0.5
                segments.append((current_start, current_start + duration))
                current_start = None
            elif etype in ['traceTASK_SWITCHED_OUT', 'traceTASK_DELETE'] and current_start is not None:
                # This closes the bar immediately at the deletion timestamp
                segments.append((current_start, ts)) 
                current_start = None
        
        task_segments[task_id] = segments
    return task_segments, df[time_column].max(), task_ids

def plot_basic_schedule(df: pd.DataFrame, output_name: str):
    """Generates the original Tick-based plot (Requirement 5.1)"""
    segments, max_val, task_ids = get_segments(df, 'tick')
    
    sns.set_theme(style="whitegrid")
    fig, ax = plt.subplots(figsize=(15, len(task_ids) * 0.8 + 2))
    palette = sns.color_palette("muted", n_colors=len(task_ids))

    for i, task_id in enumerate(task_ids):
        for start, end in segments.get(task_id, []):
            ax.barh(y=i, width=end-start, left=start, height=0.6, color=palette[i], edgecolor='black')

    ax.set_yticks(range(len(task_ids)))
    ax.set_yticklabels(task_ids)
    ax.set_xlabel('Tick Count')
    ax.set_title('Basic Task Schedule (Tick Resolution)')
    plt.tight_layout()
    plt.savefig(output_name)
    print(f"Basic plot saved as {output_name}")

def plot_extended_schedule(df: pd.DataFrame, output_name: str):
    """Generates the high-res Microsecond plot with markers for additional events"""
    # Filter out 'Logger', 'esp_timer', and 'IDLE' tasks
    df = df[~df['taskid'].isin(['Logger', 'esp_timer', 'IDLE'])]
    segments, max_val, task_ids = get_segments(df, 'Timestamp_us')
    
    sns.set_theme(style="white")
    fig, ax = plt.subplots(figsize=(15, len(task_ids) * 1.0 + 2))
    palette = sns.color_palette("husl", n_colors=len(task_ids))

    # Sort task_ids by the numeric part of the task name (assuming task_id is like 'J1', 'J2', etc.)
    task_ids = sorted(task_ids, key=lambda x: int(re.search(r'(\d+)', x).group()))
    task_ids.reverse()

    for i, task_id in enumerate(task_ids):
        # Baseline
        ax.axhline(y=i, color='black', linewidth=0.5, alpha=0.3)
        
        # Execution bars
        for start, end in segments.get(task_id, []):
            ax.barh(y=i, width=end-start, left=start, height=0.6, color=palette[i], edgecolor='black', zorder=3)

        # Markers for Task Events and Queue Operations
        task_df = df[df['taskid'] == task_id]
        
        # # Trace Task Delete
        # deletes = task_df[task_df['eventtype'] == 'traceTASK_DELETE']
        # ax.scatter(deletes['Timestamp_us'], [i]*len(deletes), marker='x', color='red', s=100, zorder=5, label="Task Deleted")
        
        # # Trace Task Create (Success or Failure)
        # create = task_df[task_df['eventtype'] == 'traceTASK_CREATE']
        # ax.scatter(create['Timestamp_us'], [i+0.2]*len(create), marker='o', color='green', s=100, zorder=5, label="Task Created")

        # create_failed = task_df[task_df['eventtype'] == 'traceTASK_CREATE_FAILED']
        # ax.scatter(create_failed['Timestamp_us'], [i+0.2]*len(create_failed), marker='o', color='orange', s=100, zorder=5, label="Task Create Failed")

        # Trace Task Switch In/Out
        switched_in = task_df[task_df['eventtype'] == 'traceTASK_SWITCHED_IN']
        ax.scatter(switched_in['Timestamp_us'], [i+0.3]*len(switched_in), marker='^', color='blue', s=80, zorder=4, label="Task Switched In")

        switched_out = task_df[task_df['eventtype'] == 'traceTASK_SWITCHED_OUT']
        ax.scatter(switched_out['Timestamp_us'], [i+0.3]*len(switched_out), marker='v', color='purple', s=80, zorder=4, label="Task Switched Out")
        
        # Queue Operations
        queue_send = task_df[task_df['eventtype'] == 'traceQUEUE_SEND']
        ax.scatter(queue_send['Timestamp_us'], [i+0.4]*len(queue_send), marker='d', color='blue', s=40, alpha=0.6, zorder=4, label="Queue Sent")
        
        queue_send_failed = task_df[task_df['eventtype'] == 'traceQUEUE_SEND_FAILED']
        ax.scatter(queue_send_failed['Timestamp_us'], [i+0.4]*len(queue_send_failed), marker='d', color='red', s=40, alpha=0.6, zorder=4, label="Queue Send Failed")

        queue_receive = task_df[task_df['eventtype'] == 'traceQUEUE_RECEIVE']
        ax.scatter(queue_receive['Timestamp_us'], [i+0.5]*len(queue_receive), marker='s', color='green', s=40, alpha=0.6, zorder=4, label="Queue Received")

        # queue_receive_failed = task_df[task_df['eventtype'] == 'traceQUEUE_RECEIVE_FAILED']
        # ax.scatter(queue_receive_failed['Timestamp_us'], [i+0.5]*len(queue_receive_failed), marker='s', color='red', s=40, alpha=0.6, zorder=4, label="Queue Receive Failed")

        # # Other queue events (For ISR)
        # queue_send_from_isr = task_df[task_df['eventtype'] == 'traceQUEUE_SEND_FROM_ISR']
        # ax.scatter(queue_send_from_isr['Timestamp_us'], [i+0.6]*len(queue_send_from_isr), marker='*', color='purple', s=50, zorder=4, label="Queue Send from ISR")
        
        # queue_receive_from_isr = task_df[task_df['eventtype'] == 'traceQUEUE_RECEIVE_FROM_ISR']
        # ax.scatter(queue_receive_from_isr['Timestamp_us'], [i+0.6]*len(queue_receive_from_isr), marker='*', color='cyan', s=50, zorder=4, label="Queue Receive from ISR")
        
        # # Trace Task Delay (if needed)
        # delay = task_df[task_df['eventtype'] == 'traceTASK_DELAY']
        # ax.scatter(delay['Timestamp_us'], [i+0.7]*len(delay), marker='s', color='yellow', s=50, zorder=4, label="Task Delayed")


    # Global Tick Increments (Vertical Lines)
    tick_inc = df[df['eventtype'] == 'traceTASK_INCREMENT_TICK']['Timestamp_us']
    for t in tick_inc:
        ax.axvline(x=t, color='gray', linestyle=':', alpha=0.2, zorder=1)

    # Legend with distinct markers for new event types
    from matplotlib.lines import Line2D
    legend_elements = [
        # Line2D([0], [0], color='gray', lw=2, label='Task Execution'),
        # Line2D([0], [0], marker='x', color='w', label='Task Deleted',
        #        markerfacecolor='red', markeredgecolor='red', markersize=10),
        # Line2D([0], [0], marker='o', color='w', label='Task Created',
        #        markerfacecolor='green', markeredgecolor='green', markersize=10),
        # Line2D([0], [0], marker='o', color='w', label='Task Create Failed',
        #        markerfacecolor='orange', markeredgecolor='orange', markersize=10),
        Line2D([0], [0], marker='^', color='w', label='Task Switched In',
               markerfacecolor='blue', markeredgecolor='blue', markersize=10),
        Line2D([0], [0], marker='v', color='w', label='Task Switched Out',
               markerfacecolor='purple', markeredgecolor='purple', markersize=10),
        Line2D([0], [0], marker='d', color='w', label='Semaphore Give',
               markerfacecolor='blue', markeredgecolor='blue', markersize=8),
        # Line2D([0], [0], marker='d', color='w', label='Queue Send Failed',
        #        markerfacecolor='red', markeredgecolor='red', markersize=8),
        # Line2D([0], [0], marker='s', color='w', label='Queue Received',
        #        markerfacecolor='green', markeredgecolor='green', markersize=8),
        # Line2D([0], [0], marker='s', color='w', label='Queue Receive Failed',
        #        markerfacecolor='red', markeredgecolor='red', markersize=8),
        # Line2D([0], [0], marker='*', color='w', label='Queue Send from ISR',
        #        markerfacecolor='purple', markeredgecolor='purple', markersize=8),
        # Line2D([0], [0], marker='*', color='w', label='Queue Receive from ISR',
        #        markerfacecolor='cyan', markeredgecolor='cyan', markersize=8),
        # Line2D([0], [0], color='gray', linestyle=':', lw=1, label='System Tick')
    ]
    ax.legend(handles=legend_elements, loc='upper right', frameon=True, shadow=True)
    

    ax.set_yticks(range(len(task_ids)))
    ax.set_yticklabels(task_ids, fontweight='bold')
    ax.set_xlabel('Time (microseconds)')
    ax.set_title('Extended Schedule Diagram (High Resolution & Events)')
    plt.tight_layout()
    plt.savefig(output_name)
    print(f"Extended plot saved as {output_name}")

if __name__ == "__main__":
    data = load_data("log_entries.csv")
    if data is not None:
        # Generate the two separate files
        # plot_basic_schedule(data, "task_schedule_basic.pdf")
        plot_extended_schedule(data, "task_schedule_extended.pdf")