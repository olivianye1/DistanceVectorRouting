//
//  prog.h
//  
//
//  Created by ZIZHAN ZHENG on 4/19/17.
//
//

#ifndef prog_h
#define prog_h

/* a rtpkt is the packet sent from one routing update process to
 another via the call tolayer2() */
struct rtpkt {
    int sourceid;       /* id of sending router sending this pkt */
    int destid;         /* id of router to which pkt being sent
                         (must be an immediate neighbor) */
    int mincost[4];    /* min cost to node 0 ... 3 */
};

void tolayer2(struct rtpkt packet);

void rtinit0();
void rtupdate0(struct rtpkt *rcvdpkt);

void rtinit1();
void rtupdate1(struct rtpkt *rcvdpkt);

void rtinit2();
void rtupdate2(struct rtpkt *rcvdpkt);

void rtinit3();
void rtupdate3(struct rtpkt *rcvdpkt);

void linkhandler0(int linkid, int newcost);
void linkhandler1(int linkid, int newcost);




#endif /* prog_h */