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





/*  types */
#define OO_FAILED -1
#define OO_OK 1


/**
  Various states of master slave determination prcedure
*/
/*TODO: States for both local and remote initiation should be maintained
   separately */
#define OO_MasterSlave_Idle          2
#define OO_MasterSlave_DetermineSent 3
#define OO_MasterSlave_AckReceived   4
#define OO_MasterSlave_Master        5
#define OO_MasterSlave_Slave         6

/** States for Capability Exchange Procedure */
#define OO_LocalTermCapExchange_Idle  9
#define OO_LocalTermCapSetSent        10
#define OO_LocalTermCapSetAckRecvd    11
#define OO_RemoteTermCapExchange_Idle 12
#define OO_RemoteTermCapSetRecvd      13
#define OO_RemoteTermCapSetAckSent    14


#define OO_FASTSTART_SENT            15
#define OO_FASTSTART_RECEIVED        16
#define OO_FASTSTART_ACCEPTED        17
#define OO_FASTSTART_REFUSED         18

/** Call Clear Reasons */
#define OO_UNKNOWN                       20
#define OO_REMOTE_CLOSED_CONNECTION      21
#define OO_REMOTE_CLOSED_H245_CONNECTION 22
#define OO_REMOTE_CLEARED                23
#define OO_HOST_CLEARED                  24
#define OO_NORMAL                        25

#define OO_TotalMessages  5

/**
 * Reasons for ending call
 */
static char * messages[] = {
   "Reason unknown",
   "Remote endpoint closed H.225 TCP connection",
   "Remote endpoint closed H.245 TCP connection",
   "Remote Ended call",
   "Local host ended call",
   "Normal End of Call"
};


/** call states */
#define OO_CALL_CREATED          50
#define OO_CALL_CONNECTING       51
#define OO_CALL_CONNECTED        52
#define OO_CALL_CLEAR            53 /* call marked for clearing */
#define OO_CALL_CLEAR_CLOLCS     54 /* Logical Channels closed*/
#define OO_CALL_CLEAR_CLELCS     55 /* Logical Channels cleared*/
#define OO_CALL_CLEAR_ENDSESSION 56 /* EndSession command sent*/
#define OO_CALL_CLEAR_CLOSEH245  57 /* H245 sockets closed*/
#define OO_CALL_CLEAR_RELEASE    58 /* Release Sent */
#define OO_CALL_CLEARED          59 /* Call Cleared */

#define CALL_STATE_BASE 50
static char* callStates[] = {
   "OO_CALL_CREATED",
   "OO_CALL_CONNECTING",
   "OO_CALL_CONNECTED",
   "OO_CALL_CLEAR",
   "OO_CALL_CLEAR_CLOLCS",
   "OO_CALL_CLEAR_CLELCS",
   "OO_CALL_CLEAR_ENDSESSION",
   "OO_CALL_CLEAR_CLOSEH245",
   "OO_CALL_CLEAR_RELEASE",
   "OO_CALL_CLEARED"
};

/** H245 Session state */
#define OO_H245SESSION_INACTIVE 61
#define OO_H245SESSION_ACTIVE 62

/** Logical Channel states */
#define OO_LOGICALCHAN_IDLE        70
#define OO_LOGICALCHAN_PROPOSED    71
#define OO_LOGICALCHAN_ESTABLISHED 72




/**
   Terminal type of the endpoint. Default is 60.
*/
#define OOTERMTYPE 60

#define MAXLOGMSGLEN 1024
/**
   Various message types for H225 and H245 messages
*/
#define OOQ931MSG                     101
#define OOH245MSG                     102
#define OOSetup                       103
#define OOCallProceeding              104
#define OOAlert                       105
#define OOConnect                     106
#define OOReleaseComplete             107
#define OOFacility                    108
#define OOMasterSlaveDetermination    109
#define OOMasterSlaveAck              110
#define OOMasterSlaveReject           111
#define OOMasterSlaveRelease          112
#define OOTerminalCapabilitySet       113
#define OOTerminalCapabilitySetAck    114
#define OOTerminalCapabilitySetReject 115
#define OOOpenLogicalChannel          116
#define OOOpenLogicalChannelAck       117
#define OOOpenLogicalChannelReject    118
#define OOOpenLogicalChannelRelease   119
#define OOEndSessionCommand           120
#define OOCloseLogicalChannel         121
#define OOCloseLogicalChannelAck      122
#define OORequestChannelClose  123
#define OORequestChannelCloseAck 124

#define OO_MSGTYPE_BASE 101
static char *msgTypes[]={
   "OOQ931MSG",
   "OOH245MSG",
   "OOSetup",
   "OOCallProceeding",
   "OOAlert",
   "OOConnect",
   "OOReleaseComplete",
   "OOFacility",
   "OOMasterSlaveDetermination",
   "OOMasterSlaveAck",
   "OOMasterSlaveReject",
   "OOMasterSlaveRelease",
   "OOTerminalCapabilitySet",
   "OOTerminalCapabilitySetAck",
   "OOTerminalCapabilitySetReject",
   "OOOpenLogicalChannel",
   "OOOpenLogicalChannelAck",
   "OOOpenLogicalChannelReject",
   "OOOpenLogicalChannelRelease",
   "OOEndSessionCommand",
   "OOCloseLogicalChannel",
   "OOCloseLogicalChannelAck",
   "OORequestChannelClose",
   "OORequestChannelCloseAck"
};

/**
  Default port ranges used
*/
#define TCPPORTSSTART 12030
#define TCPPORTSEND   12230
#define UDPPORTSSTART 13030
#define UDPPORTSEND   13230
#define RTPPORTSSTART 14030
#define RTPPORTSEND   14230

/**
 Maximum length for received messages
*/
#define MAXMSGLEN 4096

#define OO_CMD_MAKECALL    201
#define OO_CMD_ANSCALL     202
#define OO_CMD_REJECTCALL  203
#define OO_CMD_HANGCALL    204
#define OO_CMD_STOPMONITOR 205


/**
 Endpoint call modes. The call mode of the endpoint dictates what type
 of channels are created for the calls placed by the endpoint or received
 by the endpoint.
*/
#define OO_CALLMODE_AUDIOCALL 301
#define OO_CALLMODE_AUDIORX   302
#define OO_CALLMODE_AUDIOTX   303
#define OO_CALLMODE_VIDEOCALL 304
#define OO_CALLMODE_FAX       305

/** Type of callback functions to be registered at the time of
 * channel creation.
 */
typedef int (*ChannelCallback)(void*);

/**
 * Type of callback function registered at initialization
 * for handling commands
 */
typedef int (*CommandCallback)(void);

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

/* Store local and remote media endpoint info, for media type */
typedef struct ooMediaInfo{
   char  dir[15]; /* tx/rx/tx&rx*/
   char  mediaType[10]; /* audio/video/data*/
   int   capType;
   int   lMediaPort;
   int   lMediaCntrlPort;
   char  lMediaIP[20];
   struct ooMediaInfo *next;
} ooMediaInfo;

/**
 * Structure to store information of logical channels for call.
 */

typedef struct ooLogicalChannel{
   int channelNo;
   int sessionID;
   char type[10]; /* audio/video/data */
   char dir[10]; /* receive/transmit */
   char                 remoteIP[20];
   int                  remoteRtpPort; /* TODO: Should be renamed mediaPort */
   int                  remoteRtcpPort; /* TODO: Should be renamed mediaControlPort */
   int                  localRtpPort;
   int                  localRtcpPort;
   char                 localIP[20];
   int                  state;        
   struct ooH323EpCapability *chanCap;
   struct ooLogicalChannel *next;
}ooLogicalChannel;

typedef struct ooAliases{
   int type;
   char *value;
   struct ooAliases *next;
}ooAliases;

/**
 * Structure to store all the information related to a particular
 * call.
 */
typedef struct ooCallData {
   OOCTXT               *pctxt;
   char                 callToken[20]; /* ex: ooh323c_call_1 */
   char                 callType[10]; /* incoming/outgoing */
   ASN1USINT            callReference;
  
   H225CallIdentifier   callIdentifier;/* The call identifier for the active
                                          call. */
   int                  callState;
   int                  callEndReason;
   int                  h245SessionState;
   int                  isTunnelingActive;
   int                  isFastStartActive; /***TODO - Need to update this field */
  
   ooMediaInfo          *mediaInfo;
   char                 localIP[20];/* Local IP address */
   OOSOCKET             *h225Channel;/* The h225 channel socket if the channel
                                        is established.*/
   int                  *h225ChanPort;/* The h225 channel port if the channel
                                         is established.*/
   OOSOCKET             *h245Channel;/* H.245 channel socket, if the channel is
                                        established */
   int                  *h245ChanPort;
   OOSOCKET             *h245listener;
   int                  *h245listenport;
   char                 remoteIP[20];/* Remote IP address */
   int                  remotePort;
   int                  remoteH245Port;
  
   int                  sendH225;/* Number of outgoing H225/Q931 messages
                                    queued */
   int                  sendH245; /* Number of outgoing H245 messages queued */
   DList                outH225Queue;/* Outgoing H225/Q931 message queue */
   DList                outH245Queue;/* Outgoing H245 message queue */
   ASN1OCTET            *remoteDisplayName;
   ooAliases            *remoteAliases;
   ooAliases            *localAliases;
  
   int                  masterSlaveState;   /* Master-Slave state */
   ASN1UINT             statusDeterminationNumber;
   int                  localTermCapState;
   int                  remoteTermCapState;
   H245Message          *remoteTermCapSet;
   DList                remoteFastStartOLCs;
   ASN1UINT8            remoteTermCapSeqNo;
   ASN1UINT8            localTermCapSeqNo;
  
   ooLogicalChannel*    logicalChans;
   int                  noOfLogicalChannels;
   int                  logicalChanNoBase;
   int                  logicalChanNoMax;
   int                  logicalChanNoCur;
   int                  isAudioActive;
   struct ooCallData*   next;
   struct ooCallData*   prev;
} ooCallData;


/** Call back for starting media receive channel */
typedef int (*cb_StartReceiveChannel)(ooCallData *call, ooLogicalChannel *pChannel);
/** callback for starting media transmit channel */
typedef int (*cb_StartTransmitChannel)(ooCallData *call, ooLogicalChannel *pChannel);
/** callback to stop media receive channel */
typedef int (*cb_StopReceiveChannel)(ooCallData *call, ooLogicalChannel *pChannel);
/** callback to stop media transmit channel */
typedef int (*cb_StopTransmitChannel)(ooCallData *call, ooLogicalChannel *pChannel);

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
typedef int (*cb_OnCallAnswered)(ooCallData* call);
typedef int (*cb_OnAlerting)(ooCallData* call);
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
   int termType; /* 50 - Terminal entity with No MC,
                    60 - Gateway entity with no MC,
                    70 - Terminal Entity with MC, but no MP etc.*/
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
   int autoAnswer;   /* 1 to enable, 0 to disable */
   int callMode; /* audio/audiorx/audiotx/video/fax */

} ooEndPoint;
/** Global endpoint structure */
ooEndPoint gH323ep;
#endif

