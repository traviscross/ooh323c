/*
 * Copyright (C) 2004 by Objective Systems, Inc.
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

/**
 * @file ootypes.h
 * This file contains the definitions of common constants and data structures
 */

#ifndef _OOTYPES_H_
#define _OOTYPES_H_

#include "ooSocket.h"
#include "MULTIMEDIA-SYSTEM-CONTROL.h"
#include "H323-MESSAGES.h"
#include "ooasn1.h"


/* tracing */
#ifdef _OOWARNING
#define TRACELVL 2
#endif
#ifdef _OOINFO
#define TRACELVL 3
#endif
#ifdef _OODEBUGA
#define TRACELVL 4
#endif
#ifdef _OODEBUGB
#define TRACELVL 5
#endif
#ifdef _OODEBUGC
#define TRACELVL 6
#endif

/* Ensure we always log error messages */
#ifndef TRACELVL
#define TRACELVL 1
#endif

#define OOTRACEERR1(a)                ooTrace(a)
#define OOTRACEERR2(a,b)        ooTrace(a,b)
#define OOTRACEERR3(a,b,c)    ooTrace(a,b,c)
#define OOTRACEERR4(a,b,c,d)  ooTrace(a,b,c,d)
#define OOTRACEWARN1(a)       if(TRACELVL > 1) ooTrace(a)
#define OOTRACEWARN2(a,b)     if(TRACELVL > 1) ooTrace(a,b)
#define OOTRACEWARN3(a,b,c)   if(TRACELVL > 1) ooTrace(a,b,c)
#define OOTRACEWARN4(a,b,c,d) if(TRACELVL > 1) ooTrace(a,b,c,d)
#define OOTRACEINFO1(a)               if(TRACELVL > 2) ooTrace(a)
#define OOTRACEINFO2(a,b)     if(TRACELVL > 2) ooTrace(a,b)
#define OOTRACEINFO3(a,b,c)   if(TRACELVL > 2) ooTrace(a,b,c)
#define OOTRACEINFO4(a,b,c,d) if(TRACELVL > 2) ooTrace(a,b,c,d)
#define OOTRACEINFO5(a,b,c,d,e) if(TRACELVL > 2) ooTrace(a,b,c,d,e)
#define OOTRACEDBGA1(a)       if(TRACELVL > 3) ooTrace(a)
#define OOTRACEDBGA2(a,b)     if(TRACELVL > 3) ooTrace(a,b)
#define OOTRACEDBGA3(a,b,c)   if(TRACELVL > 3) ooTrace(a,b,c)
#define OOTRACEDBGA4(a,b,c,d) if(TRACELVL > 3) ooTrace(a,b,c,d)
#define OOTRACEDBGB1(a)       if(TRACELVL > 4) ooTrace(a)
#define OOTRACEDBGB2(a,b)     if(TRACELVL > 4) ooTrace(a,b)
#define OOTRACEDBGB3(a,b,c)   if(TRACELVL > 4) ooTrace(a,b,c)
#define OOTRACEDBGB4(a,b,c,d) if(TRACELVL > 4) ooTrace(a,b,c,d)
#define OOTRACEDBGC1(a)       if(TRACELVL > 5) ooTrace(a)
#define OOTRACEDBGC2(a,b)     if(TRACELVL > 5) ooTrace(a,b)
#define OOTRACEDBGC3(a,b,c)   if(TRACELVL > 5) ooTrace(a,b,c)
#define OOTRACEDBGC4(a,b,c,d) if(TRACELVL > 5) ooTrace(a,b,c,d)


/*  types */
#define OO_FAILED -1
#define OO_OK 1


/**
  Various states of master slave determination prcedure
*/
/*TODO: States for both local and remote initiation should be maintained
   separately */
#define OO_MasterSlave_Idle 2
#define OO_MasterSlave_DetermineSent 3
#define OO_MasterSlave_AckReceived 4
#define OO_MasterSlave_Master 6
#define OO_MasterSlave_Slave 7

#define OO_TermCapExchagne_Idle 9
#define OO_RemoteTermCapSetAcked 10
#define OO_LocalTermCapSetAcked  11
#define OO_TermCapExchangeDone 12

/** Call Clear Reasons */
#define OO_REMOTE_CLOSED_CONNECTION 20
#define OO_REMOTE_CLOSED_H245_CONNECTION 21
#define OO_REMOTE_CLEARED 22
#define OO_HOST_CLEARED 23
#define OO_NORMAL 24
/** call states */

#define OO_CALL_CONNECTING 51
#define OO_CALL_CONNECTED 52
#define OO_CALL_CLEARING 53
#define OO_CALL_CLEARED 54

#define OO_TotalMessages  4
/**
 * Reasons for ending call
 */
static char * messages[] = {
   "Remote endpoint closed H.225 TCP connection",
   "Remote endpoint closed H.245 TCP connection",
   "Remote Ended call",
   "Local host ended call",
   "Normal End of Call"
};
/**
   Terminal type of the endpoint. Default is 60.
*/
#define OOTERMTYPE 60

#define MAXLOGMSGLEN 1024
/**
   Various message types for H225 and H245 messages
*/
#define OOQ931MSG 101
#define OOH245MSG 102
#define OOSetup 103
#define OOCallProceeding 104
#define OOAlert 105
#define OOConnect 106
#define OOReleaseComplete 107
#define OOMasterSlaveDetermination 108
#define OOMasterSlaveAck 109
#define OOMasterSlaveReject 110
#define OOMasterSlaveRelease 111
#define OOTerminalCapabilitySet 112
#define OOTerminalCapabilitySetAck 113
#define OOTerminalCapabilitySetReject 114
#define OOOpenLogicalChannel 115
#define OOOpenLogicalChannelAck 116
#define OOOpenLogicalChannelReject 117
#define OOOpenLogicalChannelRelease 118
#define OOEndSessionCommand 119
#define OOCloseLogicalChannel 120
#define OORequestCloseLogicalChannel 121
/**
  Default port ranges used
*/
#define TCPPORTSSTART 2050
#define TCPPORTSEND 2150
#define UDPPORTSSTART 3050
#define UDPPORTSEND 3150
#define RTPPORTSSTART 4050
#define RTPPORTSEND 4250

/**
 Maximum length for received messages
*/
#define MAXMSGLEN 4096

#define OO_CMD_MAKECALL 201
#define OO_CMD_HANGCALL 202
#define OO_CMD_STOPMONITOR 203

/** Type of callback functions to be registered at the time of
 * channel creation.
 */
typedef int (*ChannelCallback)(void*);

/**
 * Type of callback function registered at initialization
 * for handling commands
 */
typedef int (*CommandCallback)();

/** Stores base value for generating new call token */
int gCallTokenBase;
/** Stores Max value for call token, at which token is reset */
int gCallTokenMax;
/** Stores current value for call token generation */
int gCurCallToken;

int gMonitor;
/** List of stack commands issued by application which have to
 *  be processed.
 */
DList gCmdList;
/** Context for stack commands list */
OOCTXT gCtxt;

/**
 * Mutex to protect access to stack commands list */
#ifdef _WIN32
CRITICAL_SECTION gCmdMutex;
#else
pthread_mutex_t gCmdMutex;
#endif

/**
 * Structure for stack commands */
typedef struct ooCommand {
   int type;
   void * param1;
   void * param2;
   void * param3;
} ooCommand;
/**
 * This structure is used to define the port ranges to be used
 * by the application.
 */
struct ooH323Ports
{
   int start;
   int max;
   int current;
};

/**
 Defines the Q931 message structure. Contains
 context for memory allocation, protocol Discriminator,
 call reference, meesage type and list of user user IEs.
*/
typedef struct Q931Message {
   OOCTXT* pctxt; /* context is used to allocate memory for */
                     /* different parts of the message */
   unsigned protocolDiscriminator;
   unsigned callReference;
   ASN1BOOL fromDestination;
   unsigned messageType; /* Q931MsgTypes  */

   DList ies;
   H225H323_UserInformation *userInfo;
} Q931Message;

/**
 Defines the H245 message structure. All request/response
 and command messages are represented using this structure.
*/
typedef struct H245Message {
   OOCTXT* pctxt;
   H245MultimediaSystemControlMessage h245Msg;
   ASN1UINT msgType;
} H245Message;

struct ooH323EpCapability;

/**
 * Structure to store information of logical channels for call.
 */
typedef struct ooLogicalChannel{
   int channelNo;
   int sessionID;
   char type[10]; /* audio/video/data */
   char dir[10]; /* receive/transmit */
   struct ooH323EpCapability *chanCap;
   struct ooLogicalChannel *next;
}ooLogicalChannel;


/**
 * Structure to store all the information related to a particular
 * call.
 */
typedef struct ooCallData {
   OOCTXT               *pctxt;
   char                 callToken[20]; /* ex: ooh323c_call_1 */
   char                 callType[10]; /* incoming/outgoing */
   int                  callState;
   int audioChannel;
   int videoChannel;
   int dataChannel;
   ASN1USINT            callReference;
   /**
      The call identifier for the active call.
   */
   H225CallIdentifier   callIdentifier;
   /**
      The h225 channel socket if the channel is established.
   */
   OOSOCKET             *h225Channel;
   /**
      The h225 channel port if the channel is established.
   */
   int                  *h225ChanPort;
  
   /**  
      H.245 channel socket, if the channel is established
   */
   OOSOCKET             *h245Channel;
   int                  *h245ChanPort;
   OOSOCKET             *h245listener;
   int                  *h245listenport;
   /** Remote RTP port */
   int                  remoteRtpPort;
   /** Remote RTCP port */
   int                  remoteRtcpPort;
   /** Remote IP address */
   char                 remoteIP[20];
   /** Local RTP port */
   int                  remotePort;
   int                  remoteH245Port;
   int                  localRtpPort;
   /** Local RTCP port */
   int                  localRtcpPort;
   /** Local IP address */
   char                 localIP[20];

   /**
      Number of outgoing H225/Q931 messages queued
   */
   int                  sendH225;
   /**
      Number of outgoing H245 messages queued
   */
   int                  sendH245;
  

   /**
      Outgoing H225/Q931 message queue
   */
   DList                outH225Queue;
  
   /**
      Outgoing H245 message queue
   */
   DList                outH245Queue;
  
   /**
      Master-Slave state
   */  
   int                  masterSlaveState;
   ASN1UINT             statusDeterminationNumber;
   ooLogicalChannel*     logicalChans;
   H245Message          *remoteTermCapSet;
   ASN1UINT8            remoteTermCapSeqNo;
   ASN1UINT8            localTermCapSeqNo;
   int                  capabilityExchangeState;
   int                  noOfLogicalChannels;
   int                  logicalChanNoBase;
   int                  logicalChanNoMax;
   int                  logicalChanNoCur;
   int                  isAudioActive;
  
   struct ooCallData*   next;
   struct ooCallData*   prev;
} ooCallData;


/** Call back for starting media receive channel */
typedef int (*cb_StartReceiveChannel)(ooCallData *call);
/** callback for starting media transmit channel */
typedef int (*cb_StartTransmitChannel)(ooCallData *call);
/** callback to stop media receive channel */
typedef int (*cb_StopReceiveChannel)(ooCallData *call);
/** callback to stop media transmit channel */
typedef int (*cb_StopTransmitChannel)(ooCallData *call);

/**
 * Structure to store information related to end point
 * capability
 */
typedef struct ooH323EpCapability{
   int t;
   void *cap;
   cb_StartReceiveChannel startReceiveChannel;
   cb_StartTransmitChannel startTransmitChannel;
   cb_StopReceiveChannel stopReceiveChannel;
   cb_StopTransmitChannel stopTransmitChannel;
}ooH323EpCapability;


typedef int (*cb_OnIncomingCall)(ooCallData* call );
typedef int (*cb_OnOutgoingCall)(ooCallData* call );
typedef int (*cb_OnCallCleared)(ooCallData* call );
typedef int (*cb_OnCallEstablished)(ooCallData* call );
typedef int (*cb_OnStartLogicalChannel)(ooCallData* call );

/** Structure to store all the config information related to the
 * endpoint created by an application */
typedef struct ooEndPoint{
   OOCTXT ctxt;
   FILE *               fptraceFile;
   /** Range of port numbers to be used for TCP connections */
   struct ooH323Ports tcpPorts;
   /** Range of port numbers to be used for UDP connections */
   struct ooH323Ports udpPorts;
   /** Range of port numbers to be used for RTP connections */
   struct ooH323Ports rtpPorts;
 
   int h245Tunneling;
   int fastStart;
   int termType;
   int t35CountryCode;
   int t35Extension;
   int manufacturerCode;
   char *productID;
   char *versionID;
   char *callerid;
   char *callername;
   OOSOCKET *stackSocket;
  
   int callType;
   DList audioCaps;
   DList dataApplicationCaps;
   DList videoCaps;
   cb_OnIncomingCall onIncomingCall;
   cb_OnOutgoingCall onOutgoingCall;
   cb_OnCallEstablished onCallEstablished;
   cb_OnCallCleared onCallCleared;
   cb_OnStartLogicalChannel onStartLogicalChannel;
   int listenPort;
   OOSOCKET *listener;
   ooCallData *callList;
  
   int txAudioChan;

} ooEndPoint;
/** Global endpoint structure */
ooEndPoint gH323ep;
#endif

