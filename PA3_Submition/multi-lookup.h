
/*
 * File: multi-lookup.h
 * Author: Nick Henry
 * Project: CSCI 3753 Programming Assignment 3
 * Create Date: 2020/03/16
 * Description:
 * 	This file contains declarations for multi-lookup.c
 *
 */
 #ifndef MULTI_LOOKUP_H
 #define MULTI_LOOKUP_H

 #include "util.h"
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <pthread.h>
 #include <sys/syscall.h>
 #include <sys/types.h>

 #define MAX_REQUESTER 5
 #define MAX_RESOLVER 10
 #define MAX_INFILES 10
 #define MAX_NAME 1025
 #define MAX_IPS 200

 clock_t start_t, end_t;

 typedef enum {true, false} bool;

 typedef struct _thread_data_t {

   //Shared arrays and filenames
   char sharedBuffer[MAX_IPS][MAX_NAME];
   char dataFiles[MAX_INFILES][MAX_NAME];
   char results[MAX_NAME];
   char serviced[MAX_NAME];

   //Current Locations
   short BuffPointer;
   short NxtFile;

   //Mutexes
   pthread_mutex_t Buff;
   pthread_mutex_t Files;
   pthread_mutex_t qlock;
   pthread_mutex_t rlock;

   //Condition Variable
   pthread_cond_t BuffCond;

 } thread_data_t;



void *requester(void *arg);

uint64_t gettid();

void *resolver(void *arg);


#endif
