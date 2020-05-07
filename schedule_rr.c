#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "schedulers.h"
#include "task.h"
#include "list.h"
#include "cpu.h"
int not = 0;
int counter = 1;
int timeRunning = 0;
extern struct node **head;
extern int totalTask;

void add(char *name, int priority, int burst){
  not++;
  Task *newTask = malloc(sizeof(Task));
  newTask->name = name;
  newTask->tid = not;
  newTask->priority = priority;
  newTask->burst = burst;
  insert(head, newTask);
}

Task * pickNextTask(){
  struct node *temp;
  Task *task = NULL;
  temp = *head;
  while (temp != NULL) {
    if (temp->task->tid == counter) {
      task = temp->task;
    }
    temp = temp->next;
  }
  if (counter < totalTask) {
    counter++;
  }else{
    counter = 1;
  }
  return task;
}

int sum(int arr[], int n){
  int s = 0;
  for (int i = 0; i < n; i++) {
    s += arr[i];
  }
  return s;
}

void schedule(){
  int burst[totalTask], turnaround[totalTask], waiting[totalTask], response[totalTask];
  int totalTurn, totalWait, totalRes, avgTurn, avgWait, avgRes;
  totalTurn = 0;
  totalWait = 0;
  totalRes = 0;
  struct node *temp;
  Task *task;
  temp = *head;

  for (int k = 0; k < totalTask; k++) {
    burst[k] = 0; turnaround[k] = 0; waiting[k] = 0; response[k] = 0;
  }

  while (temp != NULL) {
    burst[temp->task->tid - 1] = temp->task->burst;
    temp = temp->next;
  }

  while (sum(burst, totalTask) > 0) {
    task = pickNextTask();
    if (burst[task->tid - 1] > 0 && burst[task->tid - 1] <= QUANTUM) {
      run(task, burst[task->tid - 1]);
      if (response[task->tid - 1] == 0 && task->tid != 1) {
        response[task->tid - 1] = timeRunning;
      }
      timeRunning += burst[task->tid - 1];
      burst[task->tid - 1] = 0;
      turnaround[task->tid - 1] = timeRunning;
      waiting[task->tid - 1] = turnaround[task->tid - 1] - task->burst;
    } else if (burst[task->tid - 1] > QUANTUM) {
      run(task, QUANTUM);
      if (response[task->tid - 1] == 0) {
        response[task->tid - 1] = timeRunning;
      }
      timeRunning += QUANTUM;
      burst[task->tid - 1] -= QUANTUM;
    }
  }

  for (int j = 0; j < totalTask; j++) {
    totalTurn += turnaround[j];
    totalWait += waiting[j];
    totalRes += response[j];
  }
  avgTurn = totalTurn / totalTask;
  avgWait = totalWait / totalTask;
  avgRes = totalRes / totalTask;
  printf("\nAverage Turnaround Time: %dms\n", avgTurn);
  printf("Average Waiting Time: %dms\n", avgWait);
  printf("Average Response Time: %dms\n", avgRes);
}
