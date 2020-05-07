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
  Task *task;
  temp = *head;
  while(temp != NULL){
    if(temp->task->tid == counter){
      task = temp->task;
      counter = counter + 1;
    }
    temp = temp->next;
  }
  return task;
}

void schedule(){
  Task *newTask;
  int turnaround[totalTask], waiting[totalTask], response[totalTask];
  int i, j, totalTurn, totalWait, totalRes, avgTurn, avgWait, avgRes;
  totalTurn = 0;
  totalWait = 0;
  totalRes = 0;

  for (int k = 0; k < totalTask; k++) {
    turnaround[k] = 0; waiting[k] = 0; response[k] = 0;
  }

  for (i = 0; i < totalTask; i++) {
    newTask = pickNextTask();
    run(newTask, newTask->burst);
    timeRunning += newTask->burst;
    if(newTask->tid == 1){
      waiting[i] = 0;
      response[i] = 0;
    }else{
      waiting[i] = timeRunning - newTask->burst;
      response[i] = timeRunning - newTask->burst;
    }
    turnaround[i] = timeRunning;
  }
  for (j = 0; j < totalTask; j++) {
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
