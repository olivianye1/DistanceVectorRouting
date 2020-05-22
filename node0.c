/*
Olivia Nye 
Computer Networks 
Lab 2 
node0.c
*/


#include <stdio.h>
#include <stdlib.h>
#include "prog.h"


static struct distance_table
{
  int costs[4][4];
  
} dt;

struct distance_table dt0;



/* students to write the following two routines, and maybe some others */

// helper function creates and sends routing functions to neighbors
void sendInfo0(){
  
  // instantiate rtpkt for node 0 going to node 1
  struct rtpkt pkt01 = {
    0, // sourceid
	1, // destid
    {dt0.costs[0][0], dt0.costs[0][1], dt0.costs[0][2], dt0.costs[0][3]} //mincost array
  };
   // instantiate rtpkt for node 0 going to node 2
  struct rtpkt pkt02 = {
    0, // sourceid
	2, // destid
    {dt0.costs[0][0], dt0.costs[0][1], dt0.costs[0][2], dt0.costs[0][3]} //mincost array
  };
   // instantiate rtpkt for node 0 going to node 3
  struct rtpkt pkt03 = {
    0, // sourceid
	3, // destid
    {dt0.costs[0][0], dt0.costs[0][1], dt0.costs[0][2], dt0.costs[0][3]} //mincost array
  };
  
  // print mincost info being sent in the packets
  printf("Min cost info being sent: ");
  for (int x = 0; x < 4; x++){
    printf("%d ", dt0.costs[0][x]);
  }
  printf("\n");
  
  // send minimum cost info held in the pkts to neighboring nodes
  tolayer2(pkt01); 
  tolayer2(pkt02); 
  tolayer2(pkt03); 
};


void printdt0(int costs[4][4])
{
  printf("                Cost To   \n");
  printf("   D0 |     0      1      2     3 \n");
  printf("  ----|---------------------------\n");
  printf("     0|   %3d    %3d    %3d    %3d\n",costs[0][0],
	 costs[0][1],costs[0][2],costs[0][3]);
  printf("via  1|   %3d    %3d    %3d    %3d\n",costs[1][0],
	 costs[1][1],costs[1][2],costs[1][3]);
  printf("     2|   %3d    %3d    %3d    %3d\n",costs[2][0],
	 costs[2][1],costs[2][2],costs[2][3]);
  printf("     3|   %3d    %3d    %3d    %3d\n",costs[3][0],
	 costs[3][1],costs[3][2],costs[3][3]);

};


void rtinit0() 
{
  printf("\nrtinit0() has been called\n");
  
  // set all dt values to infinity
  for (int x = 0; x < 4; x++){
    for (int y = 0; y < 4; y++){
      dt0.costs[x][y] = 999;
    };
  };

  // In the dt, set costs of links between node 0 and its neighboring nodes
  dt0.costs[0][0] = 0;
  dt0.costs[0][1] = 1;
  dt0.costs[0][2] = 3;
  dt0.costs[0][3] = 7;
  
  printf("\nNode 0 set its dt to its initial costs\n");
  printdt0(dt0.costs);
  
  // send initial cost info to neighbors
  printf("\n Node 0 is sending its initial costs to each of its neighbors\n");
  sendInfo0();
};



void rtupdate0(struct rtpkt *rcvdpkt)
{
	printf("\nrtupdate0() has been called\n");
	
	// ensure the packet was meant for us 
	if (rcvdpkt -> destid == 0){
	
		// save sourceId from message 
		int pktSrc = rcvdpkt -> sourceid;
	
		printf("\nNode 0 received packet from Node %d\n", pktSrc);
	
		// save mincost array from packet
		int pktCosts[4];
		for (int i = 0; i < 4; i++){
		  pktCosts[i] = rcvdpkt -> mincost[i];
		};
	
		int updatedDt = 0;
		int updatedMinCosts = 0;
		
		
		// check for updates to the pktSrc's row of the dt
		for (int x = 0; x < 4; x ++){ 
		  // add cost from srcNode to node x to dt if it is different from what we have
		  if (pktCosts[x] != dt0.costs[pktSrc][x]){
			dt0.costs[pktSrc][x] = pktCosts[x];
			// indicate that the dt has been updated
			updatedDt = 1;
		  };
		};		
		
		// if the packet contained any new info for us
		if (updatedDt == 1){
			// check for mincost updates (updates to row 0)
			for (int x = 0; x < 4; x ++){ 
				// check direct link between pktSrc and 0 for updates needed to 0's mincosts
				if (x == 0){
					// check if pktSrc has a cheaper cost to 0 than 0 does to pktSrc
					if (dt0.costs[pktSrc][x] < dt0.costs[x][pktSrc]){
						// update mincost from 0 to pktSrc
						 dt0.costs[x][pktSrc] = dt0.costs[pktSrc][x];
						 // indicate that mincosts have been updated
						 updatedMinCosts = 1;
					};
				// dt0.costs[pktSrc][pktSrc] is always 0 and will never change Node 0's mincosts
				}else if (x == pktSrc){
					// do nothing
				// all links from pktSrc (other than to 0 and to itself)
				}else{
					// check if its cheaper for 0 to go through packet src to get to node x than current mincost to node x
					if (dt0.costs[0][pktSrc] + dt0.costs[pktSrc][x] < dt0.costs[0][x]){
						//update mincost from 0 to x
						 dt0.costs[0][x] = dt0.costs[0][pktSrc] + dt0.costs[pktSrc][x];
						 updatedMinCosts = 1;
					};
				};
			};
			
			
			// print updated  distance table and alert that there has been a change
		  	printf("\nNode 0 has updated its dt\n");
		  	printdt0(dt0.costs);	
		  	if (updatedMinCosts == 1){
		  		// send min cost updates to neighbors and alert that there has been a min cost change
		  		printf("\nNode 0 is sending updated min cost info to its neighbors\n");
				sendInfo0();
		  	}else{
		  		// alert that no min cost changes have occur
				printf("\nNode 0's dt update did not change its min costs\n");
		  	};
		// spread the word that no changes were made at all dt3
		}else{
			printf("\nNode 0 did not have to update its dt, change its min cost info, or send any new packets\n");
		};
		
	}else{
		printf("\nThere has been a mistake! The packet received by Node 0 from Node %d was meant for Node %d\n",  rcvdpkt -> sourceid,  rcvdpkt -> destid);
	};
};



/* called when cost from 0 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
void linkhandler0(int linkid, int newcost)
{

    
};
















