#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int max[MAX_PROCESSES][MAX_RESOURCES];
int allocation[MAX_PROCESSES][MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];
int available[MAX_RESOURCES];
int numOfProcesses;
int numOfResources;

void calculateNeed() {
    
    for (int i = 0; i < numOfProcesses; i++) {
        for (int j = 0; j < numOfResources; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}

bool isSafeState() {
    
    int work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES];
    int count = 0;
    for (int i = 0; i < numOfResources; i++) {
        work[i] = available[i];
    }
    for (int i = 0; i < numOfProcesses; i++) {
        finish[i] = false;
    }
    while (count < numOfProcesses) {
        bool found = false;
        for (int i = 0; i < numOfProcesses; i++) {
            if (!finish[i]) {
                bool canAllocate = true;
                for (int j = 0; j < numOfResources; j++) {
                    if (need[i][j] > work[j]) {
                        canAllocate = false;
                        break;
                    }
                }
                if (canAllocate) {
                    for (int j = 0; j < numOfResources; j++) {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    count++;
                    found = true;
                }
            }
        }
        if (!found) {
            break;
        }
    }
    return count == numOfProcesses;
}

int requestResources(int processNum, int request[]) {
   
    for (int i = 0; i < numOfResources; i++) {
        if (request[i] > available[i] || request[i] > need[processNum][i]) {
            return -1;
        }
    }
    for (int i = 0; i < numOfResources; i++) {
        available[i] -= request[i];
        allocation[processNum][i] += request[i];
        need[processNum][i] -= request[i];
    }
    if (!isSafeState()) {
        for (int i = 0; i < numOfResources; i++) {
            available[i] += request[i];
            allocation[processNum][i] -= request[i];
            need[processNum][i] += request[i];
        }
        return 0;
    }
    return 1;
}

void getSafeSequence(int safeSeq[]) {
    
    int work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES];
    for (int i = 0; i < numOfResources; i++) {
        work[i] = available[i];
    }
    for (int i = 0; i < numOfProcesses; i++) {
        finish[i] = false;
    }
    int count = 0;
    while (count < numOfProcesses) {
        bool found = false;
        for (int i = 0; i < numOfProcesses; i++) {
            if (!finish[i]) {
                bool canAllocate = true;
                for (int j = 0; j < numOfResources; j++) {
                    if (need[i][j] > work[j]) {
                        canAllocate = false;
                        break;
                    }
                }
                if (canAllocate) {
                    for (int j = 0; j < numOfResources; j++) {
                        work[j] += allocation[i][j];
                    }
                    safeSeq[count] = i;
                    finish[i] = true;
                    count++;
                    found = true;
                }
            }
        }
        if (!found) {
            break;
        }
    }
}

int main() {
   
    printf("Enter the number of processes: ");
    scanf("%d", &numOfProcesses);
    printf("Enter the number of resources: ");
    scanf("%d", &numOfResources);
    printf("Enter the Max matrix:\n");
    for (int i = 0; i < numOfProcesses; i++) {
        for (int j = 0; j < numOfResources; j++) {
            scanf("%d", &max[i][j]);
        }
    }
    printf("Enter the Allocation matrix:\n");
    for (int i = 0; i < numOfProcesses; i++) {
        for (int j = 0; j < numOfResources; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }
    printf("Enter the Available resources:\n");
    for (int i = 0; i < numOfResources; i++) {
        scanf("%d", &available[i]);
    }

   
    calculateNeed();

    
    if (isSafeState()) {
        printf("The system is in a safe state.\n");

        
        int safeSeq[MAX_PROCESSES];
        getSafeSequence(safeSeq);
        printf("Safe sequence: ");
        for (int i = 0; i < numOfProcesses; i++) {
            printf("%d ", safeSeq[i]);
        }
        printf("\n");

       
        printf("Enter the number of instances of type R2 that P3 wants to request:\n");
        int requestR2;
        scanf("%d", &requestR2);
        int processNum = 2;
        int request[MAX_RESOURCES] = {0, requestR2, 0, 0};
        int result = requestResources(processNum, request);
        if (result == 1) {
            printf("Request can be granted immediately.\n");
        } else if (result == 0) {
            printf("Request denied as it leads to an unsafe state.\n");
        } else {
            printf("Requested resources exceed available or need.\n");
        }
    } else {
        printf("The system is not in a safe state.\n");
    }

    return 0;
}