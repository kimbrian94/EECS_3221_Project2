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
int lowestP;
extern struct node **head;
extern int totalTask;
struct node **head2;
struct node *temp;

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
  Task *task;
  temp = *head;
  task = temp->task;
  lowestP = temp->task->priority;
  while (temp != NULL) {
    if(temp->task->priority <= lowestP){
      lowestP = temp->task->priority;
      task = temp->task;
    }
    temp = temp->next;
  }
  return task;
}

void schedule(){
  int turnaround[totalTask], waiting[totalTask], response[totalTask];
  int totalTurn, totalWait, totalRes, avgTurn, avgWait, avgRes;
  totalTurn = 0;
  totalWait = 0;
  totalRes = 0;
  Task *newTask;
  temp = *head;
  lowestP = temp->task->priority;

  for (int k = 0; k < totalTask; k++) {
    turnaround[k] = 0; waiting[k] = 0; response[k] = 0;
  }

  for (int i = 0; i < totalTask; i++) {
    newTask = pickNextTask();
    run(newTask, newTask->burst);
    timeRunning += newTask->burst;
    if (i == 0) {
      waiting[i] = 0;
      response[i] = 0;
    } else {
      waiting[i] = timeRunning - newTask->burst;
      response[i] = timeRunning - newTask->burst;
    }
    turnaround[i] = timeRunning;

    delete(head, newTask);
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
