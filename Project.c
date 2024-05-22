#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int process_id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int priority;
} Process;

void displayProcesses(Process processes[], int n) {
    printf("Process ID | Arrival Time | Burst Time | Priority\n");
    for (int i = 0; i < n; i++) {
        printf("%11d | %13d | %10d | %8d\n", processes[i].process_id, processes[i].arrival_time, processes[i].burst_time, processes[i].priority);
    }
    printf("\n");
}

void calculateWaitingTime(Process processes[], int n, int waiting_time[]) {
    int completion_time[n];
    waiting_time[0] = 0;

    for (int i = 1; i < n; i++) {
        waiting_time[i] = waiting_time[i - 1] + processes[i - 1].burst_time;
    }
}

void calculateTurnaroundTime(Process processes[], int n, int waiting_time[], int turnaround_time[], int completion_time[]) {
    for (int i = 0; i < n; i++) {
        completion_time[i] = waiting_time[i] + processes[i].burst_time;
        turnaround_time[i] = completion_time[i] - processes[i].arrival_time;
    }
}

float calculateAverageTime(int time[], int n) {
    float total_time = 0;
    for (int i = 0; i < n; i++) {
        total_time += time[i];
    }
    return total_time / n;
}

void fcfsScheduling(Process processes[], int n) {
    int completion_time[n], waiting_time[n], turnaround_time[n];

    completion_time[0] = processes[0].burst_time;
    for (int i = 1; i < n; i++) {
        completion_time[i] = completion_time[i - 1] + processes[i].burst_time;
    }

    for (int i = 0; i < n; i++) {
        waiting_time[i] = completion_time[i] - processes[i].burst_time - processes[i].arrival_time;
    }

    for (int i = 0; i < n; i++) {
        turnaround_time[i] = completion_time[i] - processes[i].arrival_time;
    }


    printf("Processes after FCFS Scheduling:\n");
    printf("Process ID | Arrival Time | Burst Time | Completion Time | Waiting Time | Turnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("%11d | %13d | %10d | %16d | %13d | %16d\n",
               processes[i].process_id, processes[i].arrival_time, processes[i].burst_time,
               completion_time[i], waiting_time[i], turnaround_time[i]);
    }

    printf("Average Waiting Time: %.2f\n", calculateAverageTime(waiting_time, n));
    printf("Average Turnaround Time: %.2f\n", calculateAverageTime(turnaround_time, n));

    writeToFile("output.txt", processes, n, waiting_time, turnaround_time, completion_time);
}

void sjfScheduling(Process processes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].arrival_time > processes[j + 1].arrival_time ||
                (processes[j].arrival_time == processes[j + 1].arrival_time &&
                 processes[j].burst_time > processes[j + 1].burst_time)) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    int currentTime = 0;
    int completion_time[n];
    int remaining_processes = n;

    printf("Processes after SJF Scheduling:\n");
    printf("Process ID | Arrival Time | Burst Time | Completion Time | Waiting Time | Turnaround Time\n");

    while (remaining_processes > 0) {
        int nextProcessIndex = -1;
        int shortestBurstTime = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= currentTime && processes[i].remaining_time > 0 &&
                processes[i].burst_time < shortestBurstTime) {
                nextProcessIndex = i;
                shortestBurstTime = processes[i].burst_time;
            }
        }

        if (nextProcessIndex != -1) {
            completion_time[nextProcessIndex] = currentTime + processes[nextProcessIndex].burst_time;
            currentTime = completion_time[nextProcessIndex];
            processes[nextProcessIndex].remaining_time = 0;
            remaining_processes--;

            int turnaround_time = currentTime - processes[nextProcessIndex].arrival_time;
            int waiting_time = turnaround_time - processes[nextProcessIndex].burst_time;

            printf("%11d | %13d | %10d | %16d | %13d | %16d\n",
                   processes[nextProcessIndex].process_id, processes[nextProcessIndex].arrival_time,
                   processes[nextProcessIndex].burst_time, completion_time[nextProcessIndex],
                   waiting_time, turnaround_time);
        } else {
            currentTime++;
        }
    }

    int waiting_time[n];
    int turnaround_time[n];
    calculateWaitingTime(processes, n, waiting_time);
    calculateTurnaroundTime(processes, n, waiting_time, turnaround_time, completion_time);

    printf("Average Waiting Time: %.2f\n", calculateAverageTime(waiting_time, n));
    printf("Average Turnaround Time: %.2f\n", calculateAverageTime(turnaround_time, n));

    writeToFile("output.txt", processes, n, waiting_time, turnaround_time, completion_time);
}

void priorityScheduling(Process processes[], int n) {

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].priority > processes[j + 1].priority) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }


    int waiting_time[n];
    waiting_time[0] = 0;
    for (int i = 1; i < n; i++) {
        waiting_time[i] = waiting_time[i - 1] + processes[i - 1].burst_time;
    }


    int turnaround_time[n];
    int completion_time[n];
    for (int i = 0; i < n; i++) {
        completion_time[i] = waiting_time[i] + processes[i].burst_time;
        turnaround_time[i] = waiting_time[i] + processes[i].burst_time;
    }


    printf("Processes after Priority Scheduling:\n");
    printf("Process ID | Arrival Time | Burst Time | Waiting Time \n");
    for (int i = 0; i < n; i++) {
        printf("%11d | %13d | %10d | %13d \n",
               processes[i].process_id, processes[i].arrival_time, processes[i].burst_time,
               waiting_time[i]);
    }


    float avg_waiting_time = calculateAverageTime(waiting_time, n);
    float avg_turnaround_time = calculateAverageTime(turnaround_time, n);

    printf("Average Waiting Time: %.2f\n", avg_waiting_time);



    writeToFile("output.txt", processes, n, waiting_time, turnaround_time, completion_time);
}


void roundRobinScheduling(Process processes[], int n, int quantum_time) {
    int remaining_processes = n;
    int current_time = 0;
    int *remaining_time = (int *)malloc(n * sizeof(int));
    int *completion_time = (int *)malloc(n * sizeof(int));
    int *waiting_time = (int *)malloc(n * sizeof(int));
    int *turnaround_time = (int *)malloc(n * sizeof(int));


    for (int i = 0; i < n; i++) {
        remaining_time[i] = processes[i].burst_time;
    }

    while (1) {
        int done = 1;


        for (int i = 0; i < n; i++) {
            if (remaining_time[i] > 0) {
                done = 0;


                int execution_time = (remaining_time[i] < quantum_time) ? remaining_time[i] : quantum_time;
                current_time += execution_time;


                remaining_time[i] -= execution_time;


                if (remaining_time[i] == 0) {
                    completion_time[i] = current_time;
                    remaining_processes--;
                }

                printf("Time %d: Process %d is running\n", current_time, processes[i].process_id);
            }
        }


        if (done)
            break;
    }


    for (int i = 0; i < n; i++) {
        turnaround_time[i] = completion_time[i] - processes[i].arrival_time;
        waiting_time[i] = turnaround_time[i] - processes[i].burst_time;
    }


    printf("\nRound-Robin Scheduling Complete!\n");


    printf("Process ID | Arrival Time | Burst Time | Completion Time | Waiting Time | Turnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("%11d | %13d | %10d | %16d | %13d | %16d\n",
               processes[i].process_id, processes[i].arrival_time, processes[i].burst_time,
               completion_time[i], waiting_time[i], turnaround_time[i]);
    }


    float avg_waiting_time = calculateAverageTime(waiting_time, n);
    float avg_turnaround_time = calculateAverageTime(turnaround_time, n);
    printf("Average Waiting Time: %.2f\n", avg_waiting_time);
    printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);

    writeToFile("output.txt", processes, n, waiting_time, turnaround_time, completion_time);

    free(remaining_time);
    free(completion_time);
    free(waiting_time);
    free(turnaround_time);
}


void readProcessFromFile(char filename[], Process originalProcesses[], int *n) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d", n);

    for (int i = 0; i < *n; i++) {
        fscanf(file, "%d %d %d %d", &originalProcesses[i].process_id, &originalProcesses[i].arrival_time, &originalProcesses[i].burst_time, &originalProcesses[i].priority);
        originalProcesses[i].remaining_time = originalProcesses[i].burst_time;
    }

    fclose(file);
}

void displayProcessesBeforeScheduling(char filename[]) {
    int n;
    Process originalProcesses[100];
    readProcessFromFile(filename, originalProcesses, &n);

    printf("Processes before Scheduling:\n");
    displayProcesses(originalProcesses, n);
}
void writeToFile(const char filename[], Process processes[], int n, int waiting_time[], int turnaround_time[], int completion_time[]) {
    FILE *file = fopen(filename, "a");

    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "Processes after scheduling:\n");
    fprintf(file, "Process ID | Arrival Time | Burst Time | Completion Time | Waiting Time | Turnaround Time\n");

    for (int i = 0; i < n; i++) {
        fprintf(file, "%11d | %13d | %10d | %16d | %13d | %16d\n",
                processes[i].process_id, processes[i].arrival_time, processes[i].burst_time,
                completion_time[i], waiting_time[i], turnaround_time[i]);
    }


    float avg_waiting_time = calculateAverageTime(waiting_time, n);
    float avg_turnaround_time = calculateAverageTime(turnaround_time, n);

    fprintf(file, "Average Waiting Time: %.2f\n", avg_waiting_time);
    fprintf(file, "Average Turnaround Time: %.2f\n", avg_turnaround_time);

    fclose(file);
}

int main() {
    int n, choice, quantum_time;
    char filename[100];
    Process originalProcesses[100];

    while (1) {
        printf("Enter the  file name or 0 to exit : ");
        scanf("%s", filename);

        if (filename[0] == '0') {
            exit(0);
        }

        readProcessFromFile(filename, originalProcesses, &n);

        while (1) {
            system("cls");
            printf("Choose a scheduling algorithm:\n");
            printf("1. First Come First Serve (FCFS)\n");
            printf("2. Shortest Job First (SJF)\n");
            printf("3. Round Robin Scheduling\n");
            printf("4. Priority Scheduling\n");
            printf("0. Exit\n\n");

            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 0:
                    exit(0);
                case 1:
                    displayProcessesBeforeScheduling(filename);
                    fcfsScheduling(originalProcesses, n);
                    break;
                case 2:
                    displayProcessesBeforeScheduling(filename);
                    sjfScheduling(originalProcesses, n);
                    break;
                case 3:

                    printf("Enter the time quantum for Round Robin Scheduling between (2-3): ");
                    scanf("%d", &quantum_time);
                    displayProcessesBeforeScheduling(filename);
                    roundRobinScheduling(originalProcesses, n, quantum_time);
                    break;

                case 4:

                    displayProcessesBeforeScheduling(filename);
                    priorityScheduling(originalProcesses, n);
                    break;
                default:
                    printf("Invalid choice\n");
                    break;
            }

            printf("Press Any Key To Continue...");
            getch();
        }
    }

    return 0;
}
