
/*
 * File: multi-lookup.c
 * Author: Nick Henry
 * Project: CSCI 3753 Programming Assignment 3
 * Create Date: 2020/03/16
 * Description:
 * 	multi-lookup.c defines requester and resolver threads and then creates
 *  a specified number of each in the main function.
 */

#include "multi-lookup.h"

//Simple function for getting the tid
uint64_t gettid() {
    pthread_t ptid = pthread_self();
    uint64_t threadId = 0;
    memcpy(&threadId, &ptid, sizeof(threadId));
    return threadId;
}

void *requester(void *arg){

  //Local thread data
  thread_data_t *data = (thread_data_t *)arg;
  FILE * fp;
  char line[MAX_NAME];
  int tid = gettid();
  fp = fopen(data->serviced,"a");

  while(data->NxtFile != -1){

    pthread_mutex_lock(&data->qlock);
    //printf("%d Entering qlock \n",tid);
    //Add 'file to service' to Serviced.txt with tid
    fp = fopen(data->serviced,"a");
    if(data->NxtFile==-1){pthread_mutex_unlock(&data->qlock);break;}
    else fprintf(fp, "thread: %d is servicing %s\n", tid, data->dataFiles[data->NxtFile]);
    fclose(fp);

    //Open 'file to service'
    fp = fopen(data->dataFiles[data->NxtFile],"r");
    data->NxtFile--;
    //printf("%d Leaving qlock \n",tid);
    pthread_mutex_unlock(&data->qlock);


    //Read from 'file to service' and write to shared buffer
    pthread_mutex_lock(&data->Buff);
    //printf("%d Entering Buff \n",tid);
    while(fgets(line,MAX_NAME,fp)){
      line[strcspn ( line, "\n" )] = '\0';//remove '\n' character
      data->BuffPointer++;
      strcpy(data->sharedBuffer[data->BuffPointer],line);
    }
    //printf("%d Leaving Buff \n",tid);
    pthread_mutex_unlock(&data->Buff);

    if (data->BuffPointer != -1)
      pthread_cond_signal(&data->BuffCond);

  }

  pthread_cond_broadcast(&data->BuffCond);

  fclose(fp);
  pthread_exit(NULL);

}

void *resolver(void *arg){
  //printf("Resolver: Hullo\n");

  thread_data_t *data = (thread_data_t *)arg;
  FILE * fp;
  char hostname[MAX_NAME];
  int err;
  char ip[INET6_ADDRSTRLEN];

  int tid = gettid();
  fp = fopen(data->results,"a");

  while (data->BuffPointer == -1) {
    pthread_cond_wait(&data->BuffCond, &data->Buff);
  }

  while(data->BuffPointer != -1)
  {

    pthread_mutex_lock(&data->Buff);
    //printf("%d Entering Buff \n",tid);
    strcpy(hostname,data->sharedBuffer[data->BuffPointer]);
    data->BuffPointer--;
    //printf("%d Leaving Buff \n",tid);
    pthread_mutex_unlock(&data->Buff);

    //Critical Area: resolve hostnames to IPs
    err = dnslookup(hostname, ip, INET6_ADDRSTRLEN);

    pthread_mutex_lock(&data->rlock);
    //printf("%d Entering rlock \n",tid);
    if(err){
      printf("ERROR: Hostname not read, HOST: %s, ERR: %d\n",hostname,err);

      if(strcmp(hostname,""))fprintf(fp,"%s,\n",hostname);
      else pthread_exit(NULL);

    }
    else fprintf(fp,"%s,%s\n",hostname,ip);
    //printf("%d Leaving rlock \n",tid);
    pthread_mutex_unlock(&data->rlock);

    if(data->NxtFile != -1){
      while(data->BuffPointer==-1)
        pthread_cond_wait(&data->BuffCond, &data->Buff);
      }
  }

  fclose(fp);
  pthread_exit(NULL);
}


int main(int argc, char **argv){
	start_t = clock();
  //Create shared data struct
  thread_data_t data;

  //Initialize mutexes
  pthread_mutex_init(&data.Buff, NULL);
  pthread_mutex_init(&data.Files, NULL);
  pthread_mutex_init(&data.qlock, NULL);
  pthread_mutex_init(&data.rlock, NULL);

  //Set pointers
  data.BuffPointer = -1;
  data.NxtFile = -1;

  int num_requesters = atoi(argv[1]);
  int num_resolvers = atoi(argv[2]);

  //Check for incorrect input
  if(argc-5 > MAX_INFILES)
    printf("ERROR: Too many inFiles!\n");
  if(num_requesters > MAX_REQUESTER)
    printf("ERROR: Too many requesters!\n");
  if(num_resolvers > MAX_RESOLVER)
    printf("ERROR: Too many resolvers!\n");

  //Copy file names to a shared data struct
  strcpy(data.results,argv[4]);
  strcpy(data.serviced,argv[3]);

  //Check output file names
  if(strcmp(data.results,"results.txt"))
    printf("Warnning standard 'results.txt' not used\n");
  if(strcmp(data.serviced,"serviced.txt"))
    printf("Warnning standard 'serviced.txt' not used\n");

  //Empty both files or create files to store output
  fopen(data.results,"w");
  fopen(data.serviced,"w");

  for(int i = 5; i<argc; i++){
    if(!(fopen(argv[i],"r")==NULL)){
      data.NxtFile++;
      strcpy(data.dataFiles[data.NxtFile],argv[i]);
    }
    else printf("BAD inFILE!\n");
  }

  //printf("ARGS: %d\n", argc);

  pthread_t qthr[num_requesters];
  pthread_t rthr[num_resolvers];

  for(int i = 0; i < num_requesters;i++)//Create requester thread pool
    pthread_create(&qthr[i], NULL, requester, &data);

  for(int i = 0; i < num_resolvers;i++)//Create resolver thread pool
    pthread_create(&rthr[i], NULL, resolver, &data);

  for(int i = 0; i < num_requesters;i++)//Join requester threads
    pthread_join(qthr[i],NULL);

  for(int i = 0; i < num_resolvers;i++)//Join resolvers
    pthread_join(rthr[i],NULL);

  end_t = clock();
  printf("Time elapsed (Seconds) = %f\n", (double)(end_t-start_t)/CLOCKS_PER_SEC);
  return 0;
}
