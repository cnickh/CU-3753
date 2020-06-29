/*
 * File: pager-predict0.c
 * Author:       Nick
 * Project: CSCI 3753 Programming Assignment 4
 * Create Date: 04/2020
 * Description:
 * pager-predict
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "simulator.h"



void predict(int pc, int proc, int pc_prev, int pred_matrix[MAXPROCESSES][MAXPROCPAGES][MAXPROCPAGES]){
  int *moves;  //temp array to track future positions
  moves = pred_matrix[proc][pc_prev];
  for(int i=0; i<MAXPROCPAGES; i++){
    if((moves[i] == -1)|(moves[i]==pc)){
      moves[i] = pc; //Add in posible future page
      break;
    }
  }
}

void pageit(Pentry q[MAXPROCESSES]) {

  static int temp_pgs[MAXPROCESSES][MAXPROCPAGES][MAXPROCPAGES];
  static int prev_pgs[MAXPROCESSES];
  static int initialized = 0;

  int proctmp;
  int page;
  int cur;
  int prev;

if(!initialized){
  for(proctmp=0; proctmp < MAXPROCESSES; proctmp++){
    for(page=0; page < MAXPROCPAGES; page++){
      for(int i=0;i<MAXPROCPAGES;i++)
        temp_pgs[page][proctmp][i]=-1; //Create empty indicator
    }
  }
  initialized = 1;
}

for(proctmp=0; proctmp < MAXPROCESSES;proctmp++){
    //Get active process
    if(q[proctmp].active){
      cur = q[proctmp].pc/PAGESIZE;

      prev = prev_pgs[proctmp]; //create previous
      prev_pgs[proctmp] = cur;
      page = cur;
      if(prev != page)
      {
        pageout(proctmp, prev); //Get rid of previous pages
        predict(page, proctmp, prev, temp_pgs); //Add predictions
      }

      if(!q[proctmp].pages[cur]){
        for(int i=0;i<2;i++){
          if(!pagein(proctmp,cur+i))pageout(proctmp,rand() % q[proctmp].npages);
        }
      }
      int *pages; //temp array to point to predicted pages
      int future_page;

      if(q[proctmp].active)
      {
        future_page = (q[proctmp].pc + 101)/PAGESIZE;
        pages = temp_pgs[proctmp][future_page];
        for(int i=0;i<MAXPROCPAGES;i++)
        {
          if(pages[i]==-1){
            break; //no prediction
          }
          else{
            pagein(proctmp, pages[i]); //Add in prediction
          }
        }
      }
    }
  }
}
