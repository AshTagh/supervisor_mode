/***************************************************************************
* Project           		:  shakti devt board
* Name of the file	     	:  hello.c
* Brief Description of file     :  Does the printing of hello with the help of uart communication protocol.
* Name of Author    	        :  Sathya Narayanan N
* Email ID                      :  sathya281@gmail.com

 Copyright (C) 2019  IIT Madras. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.

***************************************************************************/
/**
@file hello.c
@brief Does the printing of hello with the help of uart communication
@detail 
*/
/*
#include<uart.h>

/** @fn void main()
 * @brief prints hello world
 *//*
void main()
{
	printf ("Hello World !\n");
	while(1);
} 
*/


#include <stdio.h>

#include <stdbool.h>

#include <string.h>

#define MAX_TASKS 4
#define MAX_QUEUE_SIZE 4

long stacks[MAX_TASKS][5];

#define STATE_INVALID 0 // no task/Invalid
#define STATE_READY 1 // ready to run
#define STATE_BLOCKED 2 // has run, but now blocked by semaphore
#define STATE_DELAYED 3 // has run, but now awaiting timer
// Maximum size of Queue to put processes waiting for Semaphore

int taskCurrent = 0xFF;
// total number of valid tasks
int taskCount = 0;
// stTime and endTime to store the process execution time of ech task
unsigned int stTime = 39999;
unsigned int endTime = 0;
// CpuTimeout to acknowledge caluculation of processor Usage calculation
int cpuTimeout = 1000;
// PrirotyInheritance Selector
bool PI = false;
// Variable to count how many times the timer overflowed in a particular process execution
int zeros = 0;

struct semaphore {
  // Count of the semaphore
  unsigned int count;
  // Current queuesize (no.of Processes waiting in queue)
  unsigned int queueSize;

  unsigned int processQueue[MAX_QUEUE_SIZE]; // store task Pid
  // name of the semaphore
  //string name;
  char name[15];
  // Read and write Indexes from the processQueue to make it circular queue
  unsigned int readIndex;
  unsigned int writeIndex;
  // current user of semaphore at the current instance (PID of process)
  int currentUser;
}* s, lock, pInheritance;

struct _tcb {
  char state; // Current state of the Task/Process
  void * pid; // Process ID used to uniquely identify process
  int * sp; // location of stack pointer for the process
  int priority; // 0=highest, 7=lowestchar
  int currentPriority;
  int skipcount; // Skipcount for scheduling task; Allowing the task to skip max of skipcount times
  int ticks;
  char name[15]; // Name of the task
  struct semaphore * current; // Semaphore used currently by this task if any
}
tcb[MAX_TASKS];

void initi(void * p, int count, char * name) {
  s = p;
  s -> count = count;
  s -> queueSize = 0;
  s -> readIndex = 0;
  s -> writeIndex = 0;
  s -> currentUser = 0xFF;
  strcpy(s -> name, name);
}

typedef void( * simpleFP)();
typedef struct functionMETA {
  simpleFP funcPtr;
  char * funcName;
}
functionMETA;

typedef void( * _fn)();
_fn get_functionName(char * str);

void * get_SP() {
  __asm(" mv x1, sp");
  __asm(" addi sp,sp,-8");
  //__asm(" sd x1, 1*8(sp)");
}

void update_SP(void * sp) {
  __asm(" addi x2,x2,8");
  __asm(" mv x2,a0");
  __asm(" addi x2,x2,-8");
}

void rtosInit() {
  int i;
  // No tasks Running or Created (At start of program)
  taskCount = 0;
  // clear out all tcb records
  for (i = 0; i < MAX_TASKS; i++) {
    tcb[i].state = STATE_INVALID;
    tcb[i].pid = 0;
  }

}

// RTOS schedular to dispatch next task based on Priority
int rtosScheduler() {
  bool ok;
  int prevTask = taskCurrent;
  static int task = 0xFF;
  int tDiff = 0;
  ok = false;
  int c = 0;
  while (c < 3) {
    printf("inside scheduler %d \n", task);
    c++;

    if (task >= MAX_TASKS) {
      task = 0;
      // printf("many tasks\n");
    }
    //printf("%d\n %s  %s   %s \n",task,tcb[0].name,tcb[1].name,tcb[2].name);
    if (tcb[task].state == STATE_READY) {
      printf("ready\n");
      if (tcb[task].skipcount == tcb[task].priority) {
        printf("yay\n");
        tcb[task].skipcount = 0;
        printf("Task : %s, pid : %d \n", tcb[task].name, tcb[task].pid);
        ok = true;
        void( * funPointer)() = get_functionName(tcb[task].name);
       // funPointer();
      } else {
        if (tcb[task].skipcount < tcb[task].priority)
          tcb[task].skipcount++;
      }

    }
    task++;
  }

  return task;
}

void rtosStart() {
  taskCurrent = rtosScheduler();
  printf("\n back to rtos start\n");
  update_SP(tcb[taskCurrent].sp);
 /* //__asm(" LREG { s0 } ");
  __asm(" ld  x8,1*8(sp) ");
  __asm(" ld  s1, 2*8(sp)");
  __asm(" ld  s2, 3*8(sp)");
  __asm(" ld  s3, 4*8(sp)");
  __asm(" ld  s4, 5*8(sp)");
  __asm(" ld  s5, 6*8(sp)");
  __asm(" ld  s6, 7*8(sp)");
  __asm(" ld  s7, 8*8(sp)");
  __asm(" ld  s8, 9*8(sp)");
  __asm(" ld  s9, 10*8(sp)");
  __asm(" ld  s10, 11*8(sp)");
  __asm(" ld  s11, 12*8(sp)");
  __asm(" ld  ra,13*8(sp) "); */
  tcb[taskCurrent].sp;
  update_SP(tcb[taskCurrent].sp);

}

// Waiting for a semaphore
void wait(void * pSemaphore) {
  s = pSemaphore;
/*  __asm(" addi sp,sp,-8*13 ");
  __asm(" sd ra,1*8(sp) ");
    __asm(" sd  x8,13*8(sp) ");
  __asm(" sd  s1, 2*8(sp)");
  __asm(" sd  s2, 3*8(sp)");
  __asm(" sd  s3, 4*8(sp)");
  __asm(" sd  s4, 5*8(sp)");
  __asm(" sd  s5, 6*8(sp)");
  __asm(" sd  s6, 7*8(sp)");
  __asm(" sd  s7, 8*8(sp)");
  __asm(" sd  s8, 9*8(sp)");
  __asm(" sd  s9, 10*8(sp)");
  __asm(" sd  s10, 11*8(sp)");
  __asm(" sd  s11, 12*8(sp)"); */
  printf("\n\nwaiting\n\n");
  tcb[taskCurrent].sp = get_SP();
  if (s -> count == 0) { // If semaphore not available
    // If priority Inheritance is selected, Inherit hiher priority to lower priority task.
    if (PI && (tcb[s -> currentUser].priority > tcb[taskCurrent].priority)) {
      tcb[s -> currentUser].currentPriority = tcb[taskCurrent].priority;
      tcb[s -> currentUser].skipcount = tcb[taskCurrent].priority;
    }
    // If task requested for semaphore is already assigned previously
    if (s -> currentUser == taskCurrent)
      s -> currentUser = 0xFF;
    tcb[taskCurrent].state = STATE_BLOCKED;
    // Write to current writeindex in process queue ringbuffer and increment writeindex
    s -> processQueue[s -> writeIndex++ % 10] = taskCurrent;
    s -> queueSize++;
    // Call schedular for next task as current process is blocked
    taskCurrent = rtosScheduler();
  } else {
    // If semaphore is already available
    tcb[taskCurrent].current = s;
    s -> currentUser = taskCurrent;
    s -> count--;

  }
  update_SP(tcb[taskCurrent].sp);
 /*   __asm(" ld  x8,1*8(sp) ");
  __asm(" ld  s1, 2*8(sp)");
  __asm(" ld  s2, 3*8(sp)");
  __asm(" ld  s3, 4*8(sp)");
  __asm(" ld  s4, 5*8(sp)");
  __asm(" ld  s5, 6*8(sp)");
  __asm(" ld  s6, 7*8(sp)");
  __asm(" ld  s7, 8*8(sp)");
  __asm(" ld  s8, 9*8(sp)");
  __asm(" ld  s9, 10*8(sp)");
  __asm(" ld  s10, 11*8(sp)");
  __asm(" ld  s11, 12*8(sp)");
  __asm(" ld  ra, 13*8(sp)");
 */
}
// Posting a Semaphore to its pool
void post(void * pSemaphore) {
  s = pSemaphore;
  tcb[taskCurrent].current = NULL;
  s -> currentUser = 0xFF;
  // Reverting task priority to its Normal priority if changed due to priority inheritance
  if (tcb[taskCurrent].currentPriority != tcb[taskCurrent].priority) {
    tcb[taskCurrent].currentPriority = tcb[taskCurrent].priority;
  }
  int task = 0xFF;
  s -> count++;
  if (s -> count >= 1) {
    if (s -> queueSize > 0) { // If process exits in queue
      task = s -> processQueue[s -> readIndex++ % 10];
      s -> queueSize--;
      // Assign semaphore only if task state is Blocked
      // This is to prevent if task is destroyed after adding to process queue.
      if (tcb[task].state == STATE_BLOCKED) {
        tcb[task].state = STATE_READY;
        tcb[task].current = s;
        s -> currentUser = task;
        s -> count--;
      }
    }
  }
  printf("\n\n leaving \n\n");
}

void myTask1() {
  printf("start of task1");
  int count = 15;
  wait( & lock);
  while (1) {
    printf("task1 : %d\r\n", count++);
    if (count == 20) {
      break;
    }
  }
  post( & lock);
}
void myTask2() {
  printf("start of task 2");
  int count = 18;
  wait( & lock);
  while (1) {
    printf("task2 : %d\r\n", count++);

    if (count == 20) {
      break;
    }
  }
  post( & lock);
}
void myTask3() {
  printf("\n\start of task 3\n\n");
  int count = 1;
  //wait(&lock);

  printf("task3 : ");

  //post(&lock);
}
_fn get_functionName(char * str) {
  _fn fn;
  // newPriority = 0xFF;

  if (strcmp(str, "task1") == 0) {
    fn = myTask1;
    // newPriority = 0;
  } else if (strcmp(str, "task2") == 0) {
    fn = myTask2;
    // newPriority = 6;
  } else if (strcmp(str, "task3") == 0) {
    fn = myTask3;
    // newPriority = 6;
  } else
    fn = 0;
  return fn;
}

bool createProcess(_fn fn, int priority, char * name) {

  bool ok = false;
  int i = 0;
  bool found = false;
  if (taskCount < MAX_TASKS) {
    // make sure fn not already in list (prevent reentrancy)

    if (!found) {
      // find first available tcb record
      //i = 0;
      while (tcb[taskCount].state != STATE_INVALID) {
        taskCount++;
      }
      // Task Initialization

      tcb[taskCount].pid = (int) fn;

      tcb[taskCount].sp = & stacks[taskCount][243];
      stacks[taskCount][251] = (long) fn;

      tcb[taskCount].priority = priority;
      tcb[taskCount].currentPriority = priority;
      tcb[taskCount].skipcount = priority;
      strcpy(tcb[taskCount].name, name);
      tcb[taskCount].state = STATE_READY;
      //printf("\nthis is the stack pointer of %d is %d %s\n",taskCount,stacks[taskCount][0],tcb[taskCount].name);
      ok = true;
      printf("created task: %s with pid: %d\n", tcb[taskCount].name, tcb[taskCount].pid);

      taskCount++;
    }
  }
  return ok;
}

bool destroyProcess(_fn fn, char * name) {
  int i = 0;
  bool ok = false;
  while (i <= MAX_TASKS) {
    if (tcb[i].pid == fn) {
      ok = true;
      break;
    } else
      i++;
  }

  if (ok == false)
    return ok;

  else {
    tcb[i].pid = 0;
    tcb[i].priority = 0;
    tcb[i].currentPriority = 0;
    tcb[i].skipcount = 0;
    tcb[i].state = STATE_INVALID;
    taskCount--;
    printf("%s is deleted", name);
    return true;
  }

} 

int main() {
  rtosInit();

  printf("\n\n starting now\n\n");
  
  char name[15] = "lock";
  strcpy(name, "lock");
  initi( & lock, 1, name);
  
  printf("\n\n starting now\n\n");
  char task[15];
  strcpy(task, "task1");
  bool ox = createProcess(myTask1, 1, task);
  // printf("%d\n",*myTask1);
  //printf("\n^^^^^^^^^^^^^^\n");
  //printf("in main %d\n %s\n",1,tcb[0].name);
  // destroyProcess(myTask1,task);
  strcpy(task, "task2");
  bool ol = createProcess(myTask2, 2, task);
  //printf("in main %d\n %s\n",2,tcb[1].name);
  strcpy(task, "task3");
  bool op = createProcess(myTask3, 3, task);
  printf("\n*************************************\n");
  int z = 0;
  while (z < 3) {
    printf("in main %d %s\n", z, tcb[z].name);
    z++;

  }
  rtosStart();
  //printf("aks");
  return 0;

}
