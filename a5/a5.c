/**
 * @file
 *
 * @author Jeremiah Via <jeremiah@cs.tufts.edu>
 * @version 2012-11-29
 *
 * @section DESCRIPTION
 *
 * Assignment 5 --- Paging
 */
#include <stdio.h> 
#include "t5.h"

#define RDM (1 << 0)
#define LRU (1 << 1)
#define PRD (1 << 2)
#define PSM (1 << 3)

#define PAGER RDM

// proc: process to work upon (0-19) 
// page: page to put in (0-19)
// returns
//   1 if pagein started or already started
//   0 if it can't start (e.g., swapping out) 
// int pagein(int proc, int page); 


#if PAGER == RDM
/**
 * A pager which places a random page in memory.
 *
 * @param q the state of every process
 */
void pageit(Pentry q[MAXPROCESSES]) { 
   int proc,pc,page,oldpage; 
   for (proc = 0; proc<MAXPROCESSES; proc++) { 
      if (q[proc].active) { 
	 pc = q[proc].pc; 
         page = pc / PAGESIZE;
         if (!q[proc].pages[page] &&!pagein(proc,page)) { 
           for (oldpage = 0; oldpage<q[proc].npages; oldpage++)
             if (oldpage != page && pageout(proc,oldpage)) {
               break; 
             } 
	 }
	 break;
      } 
   } 	
} 
#elif PAGER == LRU
/**
 * Least-Recently-Used: make a time variable, count ticks. Make an
 * array: what time each page was used for each process.
 *
 * @param q the process state
 */
void pageit(Pentry q[MAXPROCESSES]) { 
  static int tick = 0; // artificial time
  static int timestamps[MAXPROCESSES][MAXPROCPAGES];

  // these are regular dynamic variables on stack
  int proc,pc,page,oldpage,i,lru,lru_tick;

  // select first active process 
  for (proc = 0; proc < MAXPROCESSES; proc++) {
    if (q[proc].active) { 
      pc = q[proc].pc;
      page = pc / PAGESIZE;
      timestamps[proc][page] = tick;
      if (!q[proc].pages[page] && !pagein(proc,page)) {
        for (oldpage = 0; oldpage < q[proc].npages; oldpage++) {
          if (timestamps[proc][oldpage] <= lru_tick && oldpage != page) {
            lru = oldpage;
            lru_tick = timestamps[proc][oldpage];
          }
        }
        if (pageout(proc,lru)) {
          break;
        }
      } 
    }
    break;
  } 
  tick++;
} 



#elif PAGER == PRD
// second strategy: predictive
// track the PC for each process over time
// e.g., in a ring buffer. 
// int pc[PROCESSES][TIMES]; 
// at any time pc[i][0]-pc[i][TIMES-1] are the last TIMES
// locations of the pc. If these are near a page border (up 
// or down, and if there is an idle page, swap it in. 
void pageit(Pentry q[MAXPROCESSES]) { 
  //////////////////////////////////////////////////////////////////////
  // persistent function variables
   static int tick = 0; // artificial time
   static int timestamps[MAXPROCESSES][MAXPROCPAGES]; 

   // these are regular dynamic variables on stack
   int proc,pc,page,oldpage; 

   // select first active process 
   for (proc = 0; proc < MAXPROCESSES; proc++) { 
      if (q[proc].active) { 
	 pc = q[proc].pc;
         page = pc/PAGESIZE;
	 timestamps[proc][page] = tick;
         if (!q[proc].pages[page]) {
	    if (!pagein(proc,page)) {
	       for (oldpage = 0; oldpage<q[proc].npages; oldpage++) {
		  if (oldpage!=page && pageout(proc,oldpage)) break; 
 		} 
	    } 
	 }
	 break; // no more 
      } 
   } 	
   tick++; // advance time for next iteration
}

#elif PAGER == PSM
// third strategy: probabilistic state machine
// From each page, compute the pages that you can branch to. 
// Swap them in according to their probability of occurrence. 
void pageit(Pentry q[MAXPROCESSES]) { 
  //////////////////////////////////////////////////////////////////////
  // persistent function variables
   static int tick = 0; // artificial time
   static int timestamps[MAXPROCESSES][MAXPROCPAGES]; 

   // these are regular dynamic variables on stack
   int proc,pc,page,oldpage; 

   // select first active process 
   for (proc = 0; proc < MAXPROCESSES; proc++) { 
      if (q[proc].active) { 
	 pc = q[proc].pc; 		// program counter for process
         page = pc/PAGESIZE; 		// page the program counter needs
	 timestamps[proc][page] = tick;	// last access
         if (!q[proc].pages[page]) { 	// if page is not there: 
	    if (!pagein(proc,page)) {   // try to swap in, if this fails: 
	       // look at all old pages, swap out any other pages 
	       for (oldpage = 0; oldpage<q[proc].npages; oldpage++) {
		  // if I find a page that's not equal to the one I want, 
	          // swap it out => 100 ticks later, pagein will succeed. 
		  if (oldpage!=page && pageout(proc,oldpage)) break; 
                  //                   ^^^^^^^^^^^^^^^^^^^^^ swapout starts
                  //  ^^^^^^^^^^^^^ it's not the page I want
 		} 
	    } 
	 }
	 break; // no more 
      } 
   } 	
   tick++; // advance time for next iteration
} 
#endif

