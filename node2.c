/*
Olivia Nye 
Computer Networks 
Lab 2 
node2.c
*/

#include <stdio.h>
#include "prog.h"


static struct distance_table
{
  int costs[4][4];
} dt;

struct distance_table dt2;


/* students to write the following two routines, and maybe some others */


// helper function creates and sends routing functions to neighbors
void sendInfo2(){
  
  // instantiate rtpkt for node 2 going to node 0
  struct rtpkt pkt20 = {
    2, // sourceid
	0, // destid
    {dt2.costs[2][0], dt2.costs[2][1], dt2.costs[2][2], dt2.costs[2][3]} //mincost array
  };
   // instantiate rtpkt for node 2 going to node 1
  struct rtpkt pkt21 = {
    2, // sourceid
	1, // destid
    {dt2.costs[2][0], dt2.costs[2][1], dt2.costs[2][2], dt2.costs[2][3]} //mincost array
  };
   // instantiate rtpkt for node 2 going to node 3
  struct rtpkt pkt23 = {
    2, // sourceid
	3, // destid
    {dt2.costs[2][0], dt2.costs[2][1], dt2.costs[2][2], dt2.costs[2][3]} //mincost array
  };
  
  // print mincost info being sent in the packets
  printf("Min cost info being sent: ");
  for (int x = 0; x < 4; x++){
    printf("%d ", dt2.costs[2][x]);
  }
  printf("\n");
  
  // send minimum cost info held in the pkts to neighboring nodes
  tolayer2(pkt20); 
  tolayer2(pkt21); 
  tolayer2(pkt23); 
};


void printdt2(int costs[4][4])
{
  printf("                Cost To   \n");
  printf("   D2 |     0      1      2     3 \n");
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


void rtinit2() 
{
  printf("\nrtinit2() has been called\n");
  
  // set all dt values to infinity
  for (int x = 0; x < 4; x++){
    for (int y = 0; y < 4; y++){
      dt2.costs[x][y] = 999;
    };
  };

  // In the dt, set costs of links between node 2 and its neighboring nodes
  dt2.costs[2][2] = 0;
  dt2.costs[2][0] = 3;
  dt2.costs[2][1] = 1;
  dt2.costs[2][3] = 2;
 
  printf("\nNode 2 set its dt to its initial costs\n");
  // print initial distance table for node 2
  printdt2(dt2.costs);
  
  // send initial cost info to neighbors
  printf("\nNode 2 is sending its initial costs to each of its neighbors\n");
  sendInfo2();
  
};


void rtupdate2(struct rtpkt *rcvdpkt)
{
	printf("\nrtupdate2() has been called\n");
	
	// ensure the packet was meant for us 
	if (rcvdpkt -> destid == 2){
	
		// save sourceId from message 
		int pktSrc = rcvdpkt -> sourceid;
	
		printf("\nNode 2 received packet from Node %d\n", pktSrc);
	
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
		  if (pktCosts[x] != dt2.costs[pktSrc][x]){
			dt2.costs[pktSrc][x] = pktCosts[x];
			// indicate that the dt has been updated
			updatedDt = 1;
		  };
		};		
		
		// if the packet contained any new info for us
		if (updatedDt == 1){
			// check for mincost updates (updates to row 1)
			for (int x = 0; x < 4; x ++){ 
				// check direct link between pktSrc and 2 for updates needed to 2's mincosts
				if (x == 2){
					// check if pktSrc has a cheaper cost to 2 than 2 does to pktSrc
					if (dt2.costs[pktSrc][x] < dt2.costs[x][pktSrc]){
						// update mincost from 2 to pktSrc
						 dt2.costs[x][pktSrc] = dt2.costs[pktSrc][x];
						 // indicate that mincosts have been updated
						 updatedMinCosts = 1;
					};
				// dt2.costs[pktSrc][pktSrc] is always 0 and will never change Node 2's mincosts
				}else if (x == pktSrc){
					// do nothing
				// all links from pktSrc (other than to 2 and to itself)
				}else{
					// check if its cheaper for 2 to go through packetSrc to get to node x than current mincost to node x
					if (dt2.costs[2][pktSrc] + dt2.costs[pktSrc][x] < dt2.costs[2][x]){
						//update mincost from 2 to x
						 dt2.costs[2][x] = dt2.costs[2][pktSrc] + dt2.costs[pktSrc][x];
						 updatedMinCosts = 1;
					};
				};
			};
			// print updated  distance table and alert that there has been a change
		  	printf("\nNode 2 has updated its dt\n");
		  	printdt2(dt2.costs);
		  	if (updatedMinCosts == 1){
		  		// send min cost updates to neighbors and alert that there has been a min cost change
		  		printf("\nNode 2 is sending updated min cost info to its neighbors\n");
				sendInfo2();
		  	}else{
		  		// alert that no min cost changes have occur
				printf("\nNode 2's dt update did not change its min costs\n");
		  	};
		// spread the word that no changes were made at all 
		}else{
			printf("\nNode 2 did not have to update its dt, change its min cost info, or send any new packets\n");
		};
		
	}else{
		printf("\nThere has been a mistake! The packet received by Node 2 from Node %d was meant for Node %d\n",  rcvdpkt -> sourceid,  rcvdpkt -> destid);
	};
};
