
/*
 * mac-simple.cc
 * Copyright (C) 2003 by the University of Southern California
 * $Id: mac-simple.cc,v 1.8 2010/03/08 05:54:52 tom_henderson Exp $
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 *
 *
 * The copyright of this module includes the following
 * linking-with-specific-other-licenses addition:
 *
 * In addition, as a special exception, the copyright holders of
 * this module give you permission to combine (via static or
 * dynamic linking) this module with free software programs or
 * libraries that are released under the GNU LGPL and with code
 * included in the standard release of ns-2 under the Apache 2.0
 * license or under otherwise-compatible licenses with advertising
 * requirements (or modified versions of such code, with unchanged
 * license).  You may copy and distribute such a system following the
 * terms of the GNU GPL for this module and the licenses of the
 * other code concerned, provided that you include the source code of
 * that other code when and as the GNU GPL requires distribution of
 * source code.
 *
 * Note that people who make modified versions of this module
 * are not obligated to grant this special exception for their
 * modified versions; it is their choice whether to do so.  The GNU
 * General Public License gives permission to release a modified
 * version without this exception; this exception also makes it
 * possible to release a modified version which carries forward this
 * exception.
 *
 */

#include "ll.h"
#include "mac.h"
#include "MncPrj.h"
#include "random.h"

// Added by Sushmita to support event tracing (singal@nunki.usc.edu)
#include "agent.h"
#include "basetrace.h"

#include "cmu-trace.h"

static class MncPrjClass : public TclClass {
public:
	MncPrjClass() : TclClass("Mac/MncPrj") {}
	TclObject* create(int, const char*const*) {
		return new MncPrj();
	}
} class_MncPrj;


// Added by Sushmita to support event tracing (singal@nunki.usc.edu).
void MncPrj::trace_event(char *eventtype, Packet *p)
{
	if (et_ == NULL) return;
	char *wrk = et_->buffer();
	char *nwrk = et_->nbuffer();

	hdr_ip *iph = hdr_ip::access(p);
	char *src_nodeaddr =
		Address::instance().print_nodeaddr(iph->saddr());
	char *dst_nodeaddr =
		Address::instance().print_nodeaddr(iph->daddr());

	if (wrk != 0) 
	{
		sprintf(wrk, "E -t "TIME_FORMAT" %s %s %s",
			et_->round(Scheduler::instance().clock()),
			eventtype,
			src_nodeaddr,
			dst_nodeaddr);
	}
	if (nwrk != 0)
	{
		sprintf(nwrk, "E -t "TIME_FORMAT" %s %s %s",
		et_->round(Scheduler::instance().clock()),
		eventtype,
		src_nodeaddr,
		dst_nodeaddr);
	}
	et_->dump();
}

MncPrj::MncPrj() : Mac() {
	rx_state_ = tx_state_ = MAC_IDLE;
	tx_active_ = 0;
	waitTimer = new MncPrjWaitTimer(this);
	sendTimer = new MncPrjSendTimer(this);
	recvTimer = new MncPrjRecvTimer(this);
	// Added by Sushmita to support event tracing (singal@nunki.usc.edu)
	et_ = new EventTrace();
	busy_ = 0;
	//bind("fullduplex_mode_", &fullduplex_mode_);
	
	//modifications for proj3
	Tcl& tcl = Tcl::instance(); 
	tcl.evalf("Mac/MncPrj set repeatnum");
	bind("repeatnum",&repeatnum);
}

// Added by Sushmita to support event tracing (singal@nunki.usc.edu)
int 
MncPrj::command(int argc, const char*const* argv)
{
	if (argc == 3) {
		if(strcmp(argv[1], "eventtrace") == 0) {
			et_ = (EventTrace *)TclObject::lookup(argv[2]);
			return (TCL_OK);
		}
	}
	return Mac::command(argc, argv);
}

void MncPrj::recv(Packet *p, Handler *h) {

	struct hdr_cmn *hdr = HDR_CMN(p);
	/* Handle the packet sending to lower layer */
	if (hdr->direction() == hdr_cmn::DOWN) {
		send(p,h);
		return;
	}
        #if 0
	printf("recv , state :%d",rx_state_);
        #endif
	/* Handle the packet sending to upper layer */
	if (rx_state_ == MAC_IDLE) {
		/*
		 * We aren't already receiving any packets, so go ahead
		 * and try to receive this one.
		 */
		rx_state_ = MAC_RECV;
		pktRx_ = p;
		/* schedule reception of the packet */
		//recvTimer->start(txtime(p));
		recvTimer->start(0);
	} 
	else { /*handle RxRx collision, free the packet in either case */
		drop(p, DROP_MAC_COLLISION);
		Packet::free(p);
	}
}


double
MncPrj::txtime(Packet *p)
 {
	 struct hdr_cmn *ch = HDR_CMN(p);
	 double t = ch->txtime();
	 if (t < 0.0)
	 	t = 0.0;
	 return t;
 }



void MncPrj::send(Packet *p, Handler *h)
{
	hdr_cmn* ch = HDR_CMN(p);

	/* store data tx time */
 	ch->txtime() = Mac::txtime(ch->size());

	// Added by Sushmita to support event tracing (singal@nunki.usc.edu)
	trace_event("SENSING_CARRIER",p);

	/* check whether we're idle */
	if (tx_state_ != MAC_IDLE) { /*collision identified and hence drop packet */
		drop(p, DROP_MAC_COLLISION);
		Packet::free(p);
		return;
	}
	pktTx_ = p;
    txHandler_ = h;
	/*otherwise schedule X copies of the packet */
	int num_copies =0;
#if 0
	printf("\n repeatnum :%d",repeatnum);
#endif
	count = 0;
	while(num_copies < repeatnum)
	{
		double delay = (Random::random())%20;
		delay = delay/(repeatnum*1000);
		send_time[num_copies] = delay;
#if 0
		printf("print - %f : %f , %d",send_time[num_copies],delay, num_copies);
#endif
		num_copies++;
	}
	sendTimer->start(send_time[count]);
#if 0
	int i=0;
	for(i=0;i<repeatnum;i++)
		printf("\n sendnum :%f",send_time[i]);
#endif
}

void MncPrj::recvHandler()
{
	hdr_cmn *ch = HDR_CMN(pktRx_);
	Packet* p = pktRx_;
	MacState state = rx_state_;
	pktRx_ = 0;
	int dst = hdr_dst((char*)HDR_MAC(p));
	
	rx_state_ = MAC_IDLE;
#if 0
	printf("recv hdlr");
#endif
	if (state == MAC_COLL) {
		// recv collision, so discard the packet
		drop(p, DROP_MAC_COLLISION);
		//Packet::free(p);
	} else if (dst != index_ && (u_int32_t)dst != MAC_BROADCAST) {
		
		/*  address filtering
		 *  We don't want to log this event, so we just free
		 *  the packet instead of calling the drop routine.
		 */
		Packet::free(p);
	} else if (ch->error()) {
		// packet has errors, so discard it
		//Packet::free(p);
		drop(p, DROP_MAC_PACKET_ERROR);
	
	} else {
		#if 0
		printf("\n packet recv");	
                #endif
		uptarget_->recv(p, (Handler*) 0);
	}
}

void MncPrj::waitHandler()
{
	tx_state_ = MAC_SEND;
	tx_active_ = 1;

	downtarget_->recv(pktTx_, txHandler_);
}

void MncPrj::sendHandler()
{
	tx_state_ = MAC_SEND;
	tx_active_ = 1;
	
	//make copy of the packet
	Packet *cp = pktTx_->copy();
	Handler *h = txHandler_;
	Packet *p = pktTx_;
	downtarget_->recv(cp, h);
	count++;
#if 0
	printf("in main , count :%d ; repeatnum :%d",count,repeatnum);
	int i=0;
	for(i=0;i<repeatnum;i++)
		printf("\n num :%f",send_time[i]);
#endif
	//send the copy
	if(count < repeatnum)
	{
		sendTimer->start(send_time[count]);
	}
	else
	{
		pktTx_ = 0;
		txHandler_ = 0;
		tx_state_ = MAC_IDLE;
		tx_active_ = 0;
#if 0
		printf("\n packet sent");	
#endif
	}
		// I have to let the guy above me know I'm done with the packet
		h->handle(cp);
}

//  Timers

void MncPrjTimer::restart(double time)
{
	if (busy_)
		stop();
	start(time);
}

	

void MncPrjTimer::start(double time)
{
	Scheduler &s = Scheduler::instance();

	assert(busy_ == 0);
	
	busy_ = 1;
	stime = s.clock();
	rtime = time;
	assert(rtime >= 0.0);

	s.schedule(this, &intr, rtime);
}

void MncPrjTimer::stop(void)
{
	Scheduler &s = Scheduler::instance();

	assert(busy_);
	s.cancel(&intr);
	
	busy_ = 0;
	stime = rtime = 0.0;
}


void MncPrjWaitTimer::handle(Event *)
{
	busy_ = 0;
	stime = rtime = 0.0;

	mac->waitHandler();
}

void MncPrjSendTimer::handle(Event *)
{
	busy_ = 0;
	stime = rtime = 0.0;

	mac->sendHandler();
}

void MncPrjRecvTimer::handle(Event *)
{
	busy_ = 0;
	stime = rtime = 0.0;

	mac->recvHandler();
}


