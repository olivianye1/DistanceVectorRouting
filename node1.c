/*
Olivia Nye 
Computer Networks 
Lab 2 
node1.c
*/

#include <stdio.h>
#include "prog.h"


static struct distance_table
{
  int costs[4][4];
} dt;

struct distance_table dt1;


/* students to write the following two routines, and maybe some others */

// helper function creates and sends routing functions to neighbors
void sendInfo1(){
  
  // instantiate rtpkt for node 0 going to node 1
  struct rtpkt pkt10 = {
    1, // sourceid
	0, // destid
    {dt1.costs[1][0], dt1.costs[1][1], dt1.costs[1][2], dt1.costs[1][3]} //mincost array
  };
   // instantiate rtpkt for node 0 going to node 2
  struct rtpkt pkt12 = {
    1, // sourceid
	2, // destid
    {dt1.costs[1][0], dt1.costs[1][1], dt1.costs[1][2], dt1.costs[1][3]} //mincost array
  };

  // print mincost info being sent in the packets
  printf("Min cost info being sent: ");
  for (int x = 0; x < 4; x++){
    printf("%d ", dt1.costs[1][x]);
  }
  printf("\n");

  // send minimum cost info held in the pkts to neighboring nodes
  tolayer2(pkt10); 
  tolayer2(pkt12);
};


void printdt1(int costs[4][4])
{
  printf("                Cost To   \n");
  printf("   D1 |     0      1      2     3 \n");
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

void rtinit1()
{

  printf("\nrtinit1() has been called\n");
  
 // set all dt values to infinity
  for (int x = 0; x < 4; x++){
    for (int y = 0; y < 4; y++){
      dt1.costs[x][y] = 999;
    };
  };

  // In the dt, set costs of links between node 1 and its neighboring nodes
  dt1.costs[1][1] = 0;
  dt1.costs[1][0] = 1;
  dt1.costs[1][2] = 1;
 
  printf("\nNode 1 set its dt to its initial costs\n");
  // print initial distance table for node 1
  printdt1(dt1.costs);
  
  // send initial cost info to neighbors
  printf("\nNode 1 is sending its initial costs to each of its neighbors\n");
  sendInfo1();
  
};


void rtupdate1(struct rtpkt *rcvdpkt)
{
	printf("\nrtupdate1() has been called\n");
	
	// ensure the packet was meant for us 
	if (rcvdpkt -> destid == 1){
	
		// save sourceId from message 
		int pktSrc = rcvdpkt -> sourceid;
	
		printf("\nNode 1 received packet from Node %d\n", pktSrc);
	
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
		  if (pktCosts[x] != dt1.costs[pktSrc][x]){
			dt1.costs[pktSrc][x] = pktCosts[x];
			// indicate that the dt has been updated
			updatedDt = 1;
		  };
		};		
		
		// if the packet contained any new info for us
		if (updatedDt == 1){
			// check for mincost updates (updates to row 1)
			for (int x = 0; x < 4; x ++){ 
				// check direct link between pktSrc and 1 for updates needed to 1's mincosts
				if (x == 1){
					// check if pktSrc has a cheaper cost to 1 than 1 does to pktSrc
					if (dt1.costs[pktSrc][x] < dt1.costs[x][pktSrc]){
						// update mincost from 0 to pktSrc
						 dt1.costs[x][pktSrc] = dt1.costs[pktSrc][x];
						 // indicate that mincosts have been updated
						 updatedMinCosts = 1;
					};
				// dt1.costs[pktSrc][pktSrc] is always 0 and will never change Node 1's mincosts
				}else if (x == pktSrc){
					// do nothing
				// all links from pktSrc (other than to 1 and to itself)
				}else{
					// check if its cheaper for 1 to go through packet src to get to node x than current mincost to node x
					if (dt1.costs[1][pktSrc] + dt1.costs[pktSrc][x] < dt1.costs[1][x]){
						//update mincost from 1 to x
						 dt1.costs[1][x] = dt1.costs[1][pktSrc] + dt1.costs[pktSrc][x];
						 updatedMinCosts = 1;
					};
				};
			};
			// print updated  distance table and alert that there has been a change
		  	printf("\nNode 1 has updated its dt\n");
		  	printdt1(dt1.costs);	
		  	if (updatedMinCosts == 1){
		  		// send min cost updates to neighbors and alert that there has been a min cost change
		  		printf("\nNode 1 is sending updated min cost info to its neighbors\n");
				sendInfo1();
		  	}else{
		  		// alert that no min cost changes have occur
				printf("\nNode 1's dt update did not change its min costs\n");
		  	};
		// spread the word that no changes were made at all 
		}else{
			printf("\nNode 1 did not have to update its dt, change its min cost info, or send any new packets\n");
		};
		
	}else{
		printf("\nThere has been a mistake! The packet received by Node 1 from Node %d was meant for Node %d\n",  rcvdpkt -> sourceid,  rcvdpkt -> destid);
	};
};


/* called when cost from 1 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
void linkhandler1(int linkid, int newcost)
{
    
};

