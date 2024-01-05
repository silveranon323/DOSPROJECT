#include <stdio.h>
struct Process
{
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int startTime;
    int completionTime;
};

void FCFS(struct Process processes[], int n);
void roundRobin(struct Process processes[], int n, int quantum);
void calculateAverages(struct Process processes[], int n);

int main()
{
    int choice;
    do
    {
        printf("Choose scheduling algorithm:\n");
        printf("1. First Come First Served (FCFS)\n");
        printf("2. Round Robin (RR)\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            int n, i;
            printf("Enter the number of processes: ");
            scanf("%d", &n);
            struct Process processes[n];

            for (i = 0; i < n; i++)
            {
                printf("Enter details for process %d (Arrival Time Burst Time): ", i + 1);
                scanf("%d %d", &processes[i].arrivalTime, &processes[i].burstTime);
                processes[i].id = i + 1;
            }

            FCFS(processes, n);
            break;
        }
        case 2:
        {
            int n, i, quantum;
            printf("Enter the number of processes: ");
            scanf("%d", &n);
            struct Process processes[n];

            for (i = 0; i < n; i++)
            {
                printf("Enter details for process %d (Arrival Time Burst Time): ", i + 1);
                scanf("%d %d", &processes[i].arrivalTime, &processes[i].burstTime);
                processes[i].id = i + 1;
                processes[i].remainingTime = processes[i].burstTime;
                processes[i].startTime = -1;
            }

            printf("Enter time quantum for Round Robin: ");
            scanf("%d", &quantum);

            roundRobin(processes, n, quantum);
            break;
        }
        case 5:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}

void FCFS(struct Process processes[], int n)
{
    int currentTime = 0;
    float totalWaitingTime = 0, totalTurnaroundTime = 0, totalResponseTime = 0;

    printf("Gantt Chart for FCFS:\n");
    printf("Process\tStart Time\tCompletion Time\n");

    for (int i = 0; i < n; i++)
    {
        if (currentTime < processes[i].arrivalTime)
        {
            currentTime = processes[i].arrivalTime;
        }

        processes[i].startTime = currentTime;
        processes[i].completionTime = currentTime + processes[i].burstTime;

        printf("P%d\t%d\t\t%d\n", processes[i].id, processes[i].startTime, processes[i].completionTime);

        totalWaitingTime += (currentTime - processes[i].arrivalTime);
        totalTurnaroundTime += (processes[i].completionTime - processes[i].arrivalTime);
        totalResponseTime += (processes[i].startTime - processes[i].arrivalTime);

        currentTime = processes[i].completionTime;
    }

    calculateAverages(processes, n);
}

void roundRobin(struct Process processes[], int n, int quantum)
{
    int currentTime = 0;
    float totalWaitingTime = 0, totalTurnaroundTime = 0, totalResponseTime = 0;

    printf("Gantt Chart for Round Robin with quantum %d:\n", quantum);
    printf("Process\tStart Time\tEnd Time\n");

    int completedProcesses = 0;
    while (completedProcesses < n)
    {
        for (int i = 0; i < n; i++)
        {
            if (processes[i].remainingTime > 0)
            {
                if (processes[i].remainingTime > quantum)
                {
                    currentTime += quantum;
                    processes[i].remainingTime -= quantum;
                    printf("P%d\t%d\t\t%d\n", processes[i].id, currentTime - quantum, currentTime);
                }
                else
                {
                    currentTime += processes[i].remainingTime;
                    processes[i].completionTime = currentTime;
                    printf("P%d\t%d\t\t%d\n", processes[i].id, currentTime - processes[i].remainingTime, currentTime);

                    totalTurnaroundTime += (currentTime - processes[i].arrivalTime);
                    totalResponseTime += (processes[i].startTime - processes[i].arrivalTime);

                    processes[i].remainingTime = 0;
                    completedProcesses++;
                }
            }
            if (processes[i].remainingTime > 0)
            {
                if (processes[i].arrivalTime <= currentTime)
                {
                    if (processes[i].startTime == -1)
                    {
                        processes[i].startTime = currentTime;
                        totalWaitingTime += (currentTime - processes[i].arrivalTime);
                    }
                }
                else
                {
                    processes[i].startTime = -1;
                }
            }
        }
    }

    calculateAverages(processes, n);
}

void calculateAverages(struct Process processes[], int n)
{
    float totalWaitingTime = 0, totalTurnaroundTime = 0, totalResponseTime = 0;

    for (int i = 0; i < n; i++)
    {
        totalWaitingTime += (processes[i].completionTime - processes[i].arrivalTime - processes[i].burstTime);
        totalTurnaroundTime += (processes[i].completionTime - processes[i].arrivalTime);
        totalResponseTime += (processes[i].startTime - processes[i].arrivalTime);
    }

    float avgWaitingTime = totalWaitingTime / n;
    float avgTurnaroundTime = totalTurnaroundTime / n;
    float avgResponseTime = totalResponseTime / n;

    printf("\n");
    printf("--------------------------------------------------\n");
    printf("| Metric             |   Average Value           |\n");
    printf("--------------------------------------------------\n");
    printf("| Waiting Time        |   %.2f                   |\n", avgWaitingTime);
    printf("| Turnaround Time     |   %.2f                   |\n", avgTurnaroundTime);
    printf("| Response Time       |   %.2f                   |\n", avgResponseTime);
    printf("--------------------------------------------------\n");
}
