/*
 * Copyright (C) 2004-2005 by Objective Systems, Inc.
 *
 * This software is furnished under an open source license and may be
 * used and copied only in accordance with the terms of this license.
 * The text of the license may generally be found in the root
 * directory of this installation in the COPYING file.  It
 * can also be viewed online at the following URL:
 *
 *   http://www.obj-sys.com/open/license.html
 *
 * Any redistributions of this file including modified versions must
 * maintain this copyright notice.
 *
 *****************************************************************************/


#include "ooports.h"
#include "oo.h"


int ooSetTCPPorts( int start, int max)
{
   if(start < 1024)
      gH323ep.tcpPorts.start = 1024;
   else
      gH323ep.tcpPorts.start = start;
   if(max > 65500)
      gH323ep.tcpPorts.max = 65500;
   else
      gH323ep.tcpPorts.max = max;

   if(gH323ep.tcpPorts.max<gH323ep.tcpPorts.start)
   {
      OOTRACEERR1("Error: Failed to set tcp ports- "
                  "Max port number less than Start port number\n");
      return OO_FAILED;
   }
   gH323ep.tcpPorts.current = start;
       
   OOTRACEINFO1("TCP port range initialize - successful\n");
   return OO_OK;
}

/* Sets the range of ports that can be potentially used for UDP
   transport
*/
int ooSetUDPPorts(int start, int max)
{
   if(start < 1024)
      gH323ep.udpPorts.start = 1024;
   else
      gH323ep.udpPorts.start = start;
   if(max > 65500)
      gH323ep.udpPorts.max = 65500;
   else
      gH323ep.udpPorts.max = max;
       
   if(gH323ep.udpPorts.max<gH323ep.udpPorts.start)
   {
      OOTRACEERR1("Error: Failed to set udp ports- Max port number"
                  " less than Start port number\n");
      return OO_FAILED;
   }
       
   gH323ep.udpPorts.current = start;
       
   OOTRACEINFO1("UDP port range initialize - successful\n");

   return OO_OK;
}

/* Sets the range of ports that can be potentially used for RTP
   RTCP transport
*/
int ooSetRTPPorts(int start, int max)
{
   if(start < 1024)
      gH323ep.rtpPorts.start = 1024;
   else
      gH323ep.rtpPorts.start = start;
   if(max > 65500)
      gH323ep.rtpPorts.max = 65500;
   else
      gH323ep.rtpPorts.max = max;
       
   if(gH323ep.rtpPorts.max<gH323ep.rtpPorts.start)
   {
      OOTRACEERR1("Error: Failed to set rtp ports- Max port number"
                  " less than Start port number\n");
      return OO_FAILED;
   }
       
   gH323ep.rtpPorts.current = start;
   OOTRACEINFO1("RTP port range initialize - successful\n");
   return OO_OK;
}

/* Get the next port of type TCP/UDP/RTP */
int ooGetNextPort(ooEndPoint *ep, int type)
{
   if(type==OOTCP)
   {
      if(ep->tcpPorts.current <= ep->tcpPorts.max)
         return ep->tcpPorts.current++;
      else
      {
         ep->tcpPorts.current = ep->tcpPorts.start;
         return ep->tcpPorts.current++;
      }
   }
   if(type==OOUDP)
   {
      if(ep->udpPorts.current <= ep->udpPorts.max)
         return ep->udpPorts.current++;
      else
      {
         ep->udpPorts.current = ep->udpPorts.start;
         return ep->udpPorts.current++;
      }
   }
   if(type==OORTP)
   {
      if(ep->rtpPorts.current <= ep->rtpPorts.max)
         return ep->rtpPorts.current++;
      else
      {
         ep->rtpPorts.current = ep->rtpPorts.start;
         return ep->rtpPorts.current++;
      }
   }
   return OO_FAILED;
}

int ooBindPort(ooEndPoint *ep, int type,
               OOSOCKET socket)
{
   int initialPort, bindPort, ret;
   OOIPADDR ipAddrs;
   /*char localip[40];
   ooGetLocalIPAddress(localip);*/
   initialPort = ooGetNextPort(ep, type);
   bindPort = initialPort;
   /*ret= ooSocketStrToAddr (localip, &ipAddrs);*/
   ret= ooSocketStrToAddr (ep->signallingIP, &ipAddrs);
   while(1)
   {
      if((ret=ooSocketBind(socket, ipAddrs,
                     bindPort))==ASN_OK)
      {
        
         return bindPort;
      }
      else
      {
         bindPort = ooGetNextPort(ep, type);
         if(bindPort == initialPort)
            return OO_FAILED;
      }
   }
}

#ifdef _WIN32       
int ooBindOSAllocatedPort(OOSOCKET socket)
{
   OOIPADDR ipAddrs;
/*   char localip[40];*/
   int size, ret;
   struct sockaddr_in name;
   size = sizeof(struct sockaddr_in);
   /*ooGetLocalIPAddress(localip);
   ret= ooSocketStrToAddr (localip, &ipAddrs);*/
   ret= ooSocketStrToAddr (gH323ep.signallingIP, &ipAddrs);
   if((ret=ooSocketBind(socket, ipAddrs,
                     0))==ASN_OK)
   {
      ret = ooGetSockName(socket, &name, &size);
      if(ret == ASN_OK)
      {
         return name.sin_port;
        
      }
   }

   return OO_FAILED;
}
#endif
