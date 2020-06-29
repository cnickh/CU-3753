/*
 * File: pager-lru.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 4
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains an lru pageit
 *      implmentation.
 */

#include <stdio.h>
#include <stdlib.h>

#include "simulator.h"

void pageit(Pentry q[MAXPROCESSES]) {

    /* This file contains the stub for an LRU pager */
    /* You may need to add/remove/modify any part of this file */

    /* Static vars */
    static int initialized = 0;
    static int tick = 1; // artificial time
    static int timestamps[MAXPROCESSES][MAXPROCPAGES];

    /* Local vars */
    int proctmp;
    int pagetmp;
    int cur;
    int lru;
    int min_time = 9999999;

    /* initialize static vars on first run */
  if(!initialized){
  	for(proctmp=0; proctmp < MAXPROCESSES; proctmp++){
    	for(pagetmp=0; pagetmp < MAXPROCPAGES; pagetmp++){
        timestamps[proctmp][pagetmp] = 0;
    	}
  	}
  	initialized = 1;
  }

    /* TODO: Implement LRU Paging */
  for(proctmp=0; proctmp < MAXPROCESSES;proctmp++){
    //Get active process
    if(q[proctmp].active){
      cur = q[proctmp].pc/PAGESIZE;
      //set its time stamp
      timestamps[proctmp][cur] = tick;
      //if active and not in pagein()
      if(!q[proctmp].pages[cur]){
        //if cant pagein pageout
        if(!pagein(proctmp,cur)){
          //iterate through all pages and find lru
          for (pagetmp=0; pagetmp < q[proctmp].npages;pagetmp++){
            if(timestamps[proctmp][pagetmp] < min_time && q[proctmp].pages[pagetmp]){
              lru = pagetmp;
              min_time = timestamps[proctmp][lru];
            }
          }
          //pageout lru
          pageout(proctmp,lru);
        }
      }
    }
  }

    /* advance time for next pageit iteration */
    tick++;
}
