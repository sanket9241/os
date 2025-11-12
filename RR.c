#include <stdio.h>
#include <stdlib.h>
#define N 100

struct process {
    int process_id;
    int arrival_time;
    int brust_time;
    int completion_time;
    int waiting_time;
    int turn_around_time;
    int remaining_time;
    int started; // flag to mark if process has started execution
};

struct process proc[N];
int queue[N];
int front = 0, rear = 0;

void push(int process_id) {
    queue[rear] = process_id;
    rear = (rear + 1) % N;
}

int pop() {
    if (front == rear)
        return -1;
    int ret = queue[front];
    front = (front + 1) % N;
    return ret;
}

int is_in_queue(int process_id) {
    for (int i = front; i != rear; i = (i + 1) % N) {
        if (queue[i] == process_id)
            return 1;
    }
    return 0;
}

int main() {
    int n, time_quantum;
    float total_waiting_time = 0, total_turnaround_time = 0;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("Enter arrival time for process %d: ", i + 1);
        scanf("%d", &proc[i].arrival_time);
        printf("Enter burst time for process %d: ", i + 1);
        scanf("%d", &proc[i].brust_time);
        proc[i].process_id = i + 1;
        proc[i].remaining_time = proc[i].brust_time;
        proc[i].started = 0;
    }

    printf("Enter time quantum: ");
    scanf("%d", &time_quantum);

    int time = 0;
    int completed = 0;
    int current = -1;
    int time_slice = 0;

    // Enqueue all processes that arrive at time 0
    for (int i = 0; i < n; i++) {
        if (proc[i].arrival_time == time) {
            push(i);
            proc[i].started = 1;
        }
    }

    while (completed < n) {
        if (current == -1) {
            current = pop();
            time_slice = 0;
        }

        if (current != -1) {
            proc[current].remaining_time--;
            time_slice++;
            time++;

            // Enqueue new arrivals at current time
            for (int i = 0; i < n; i++) {
                if (proc[i].arrival_time == time && !proc[i].started) {
                    push(i);
                    proc[i].started = 1;
                }
            }

            if (proc[current].remaining_time == 0) {
                proc[current].completion_time = time;
                proc[current].turn_around_time = proc[current].completion_time - proc[current].arrival_time;
                proc[current].waiting_time = proc[current].turn_around_time - proc[current].brust_time;
                total_turnaround_time += proc[current].turn_around_time;
                total_waiting_time += proc[current].waiting_time;
                completed++;
                current = -1;
                time_slice = 0;
            } else if (time_slice == time_quantum) {
                push(current);
                current = -1;
            }
        } else {
            // CPU is idle, move time forward
            time++;
            for (int i = 0; i < n; i++) {
                if (proc[i].arrival_time == time && !proc[i].started) {
                    push(i);
                    proc[i].started = 1;
                }
            }
        }
    }

    printf("\nProcess\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               proc[i].process_id,
               proc[i].arrival_time,
               proc[i].brust_time,
               proc[i].completion_time,
               proc[i].turn_around_time,
               proc[i].waiting_time);
    }

    printf("\nAverage Waiting Time: %.2f\n", total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround_time / n);

    return 0;
}

