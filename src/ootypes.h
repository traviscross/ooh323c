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
#define OO_CALL_ENDREASON_MIN            20
#define OO_UNKNOWN                       20
#define OO_REMOTE_CLOSED_CONNECTION      21
#define OO_REMOTE_CLOSED_H245_CONNECTION 22
#define OO_REMOTE_CLEARED                23
#define OO_HOST_CLEARED                  24
#define OO_NORMAL                        25
#define OO_CALL_ENDREASON_MAX            25




/** call states */
#define OO_CALL_STATE_MIN         50
#define OO_CALL_CREATED           50
#define OO_CALL_WAITING_ADMISSION 51    
#define OO_CALL_CONNECTING        52
#define OO_CALL_CONNECTED         53
#define OO_CALL_CLEAR             54 /* call marked for clearing */
#define OO_CALL_CLEAR_CLOLCS      55 /* Logical Channels closed*/
#define OO_CALL_CLEAR_CLELCS      56 /* Logical Channels cleared*/
#define OO_CALL_CLEAR_ENDSESSION  57 /* EndSession command sent*/
#define OO_CALL_CLEAR_CLOSEH245   58 /* H245 sockets closed*/
#define OO_CALL_CLEAR_RELEASE     59 /* Release Sent */
#define OO_CALL_CLEARED           60 /* Call Cleared */
#define OO_CALL_STATE_MAX         60



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

#define MAXLOGMSGLEN 2048
/**
   Various message types for H225 and H245 messages
*/
#define OO_MSGTYPE_MIN                 101
#define OOQ931MSG                      101
#define OOH245MSG                      102
#define OOSetup                        103
#define OOCallProceeding               104
#define OOAlert                        105
#define OOConnect                      106
#define OOReleaseComplete              107
#define OOFacility                     108
#define OOMasterSlaveDetermination     109
#define OOMasterSlaveAck               110
#define OOMasterSlaveReject            111
#define OOMasterSlaveRelease           112
#define OOTerminalCapabilitySet        113
#define OOTerminalCapabilitySetAck     114
#define OOTerminalCapabilitySetReject  115
#define OOTerminalCapabilitySetRelease 116
#define OOOpenLogicalChannel           117
#define OOOpenLogicalChannelAck        118
#define OOOpenLogicalChannelReject     119
#define OOOpenLogicalChannelRelease    120
#define OOEndSessionCommand            121
#define OOCloseLogicalChannel          122
#define OOCloseLogicalChannelAck       123
#define OORequestChannelClose          124
#define OORequestChannelCloseAck       125
#define OO_MSGTYPE_MAX                 125

/* Timer types */
#define OO_CALLESTB_TIMER  (1<<0)
#define OO_MSD_TIMER       (1<<1)
#define OO_TCS_TIMER       (1<<2)
#define OO_OLC_TIMER       (1<<3)
#define OO_CLC_TIMER       (1<<4)

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
#define OO_CMD_MAKECALL_3  202
#define OO_CMD_ANSCALL     203
#define OO_CMD_REJECTCALL  204
#define OO_CMD_HANGCALL    205
#define OO_CMD_STOPMONITOR 206


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

#define DEFAULT_MAX_RETRIES 3
/** Type of callback functions to be registered at the time of
 * channel creation.
 */
typedef int (*ChannelCallback)(void*);

/**
 * Type of callback function registered at initialization
 * for handling commands
 */
typedef int (*CommandCallback)(void);


/**
 * Mutex to protect ooGenerateCallReference function.
 * This is required as this function will be called by
 * multiple threads trying to place calls using stack commands
 */
#ifdef _WIN32
CRITICAL_SECTION gCallRefMutex;
#else
pthread_mutex_t gCallRefMutex;
#endif

/**
 * Mutex to protect access to global call token variables.
 * This is required as these variables will be used by
 * multiple threads trying to place calls using stack commands
 */
#ifdef _WIN32
CRITICAL_SECTION gCallTokenMutex;
#else
pthread_mutex_t gCallTokenMutex;
#endif

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
   H245MultimediaSystemControlMessage h245Msg;
   ASN1UINT msgType;
} H245Message;

struct ooH323EpCapability;
typedef struct ooCapPrefs {
  int order[20];
  int index;
}ooCapPrefs;

/* Store local and remote media endpoint info, for media type */
typedef struct ooMediaInfo{
   char  dir[15]; /* transmit/receive*/
   int   cap;
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
typedef struct OOH323Channel {
   OOSOCKET     sock;
   int          port;
   DList        outQueue;
} OOH323Channel;

typedef struct OORetries {
   ASN1USINT   msdRetries;
   ASN1USINT   olcRetries;
} OORetries;

struct ooCallData;

typedef struct ooTimerCallback{
   struct ooCallData* call;
   ASN1UINT    timerType;
   ASN1UINT    sequenceNumber;
} ooTimerCallback;

typedef struct ooCallData {
   OOCTXT               *pctxt;
   char                 callToken[20]; /* ex: ooh323c_call_1 */
   char                 callType[10]; /* incoming/outgoing */
   ASN1USINT            callReference;
  
   H225CallIdentifier   callIdentifier;/* The call identifier for the active
                                          call. */
   H225ConferenceIdentifier confIdentifier;
   int                  callState;
   int                  callEndReason;
   int                  h245SessionState;
   int                  isTunnelingActive;
   int                  isFastStartActive;
   int                  gkEngaged;  
   int                  dtmfmode;
   ooMediaInfo          *mediaInfo;
   char                 localIP[20];/* Local IP address */
   OOH323Channel*       pH225Channel;
   OOH323Channel*       pH245Channel;
   OOSOCKET             *h245listener;
   int                  *h245listenport;
   char                 remoteIP[20];/* Remote IP address */
   int                  remotePort;
   int                  remoteH245Port;
   ASN1OCTET            *remoteDisplayName;
   ooAliases            *remoteAliases;
   int                  masterSlaveState;   /* Master-Slave state */
   ASN1UINT             statusDeterminationNumber;
   int                  localTermCapState;
   int                  remoteTermCapState;
   struct ooH323EpCapability * remoteCaps;
   DList                remoteFastStartOLCs;
   ASN1UINT8            remoteTermCapSeqNo;
   ASN1UINT8            localTermCapSeqNo;
   ooCapPrefs           capPrefs;  
   ooLogicalChannel*    logicalChans;
   int                  noOfLogicalChannels;
   int                  logicalChanNoBase;
   int                  logicalChanNoMax;
   int                  logicalChanNoCur;
   int                  isAudioActive;
   DList                timerList;
   OORetries            retries;
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
   int dir;
   int cap;
   int capType;
   void *params;
   cb_StartReceiveChannel startReceiveChannel;
   cb_StartTransmitChannel startTransmitChannel;
   cb_StopReceiveChannel stopReceiveChannel;
   cb_StopTransmitChannel stopTransmitChannel;
   struct ooH323EpCapability *next;
}ooH323EpCapability;

typedef int (*cb_OnAlerting)(ooCallData * call);
typedef int (*cb_OnIncomingCall)(ooCallData* call );
typedef int (*cb_OnOutgoingCall)(ooCallData* call );
typedef int (*cb_OnCallAnswered)(ooCallData* call);
typedef int (*cb_OnCallCleared)(ooCallData* call );
typedef int (*cb_OnCallEstablished)(ooCallData* call );
typedef int (*cb_OnOutgoingCallAdmitted)(ooCallData* call );

/** Structure to store all the config information related to the
 * endpoint created by an application */
typedef struct ooEndPoint{
   /**
    * This context should be used for allocation of memory for
    * items within the endpoint structure.
    */
   OOCTXT ctxt;

   /**
    * This context should be used for allocation of memory for
    * message structures.
    */
   OOCTXT msgctxt;

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
   ooAliases *aliases;
   int callType;

   ooH323EpCapability *myCaps;
   ooCapPrefs     capPrefs;
   int noOfCaps;
   cb_OnAlerting onAlerting;
   cb_OnIncomingCall onIncomingCall;
   cb_OnOutgoingCall onOutgoingCall;
   cb_OnCallEstablished onCallEstablished;
   cb_OnCallCleared onCallCleared;
   cb_OnOutgoingCallAdmitted onOutgoingCallAdmitted;
   char signallingIP[20];
   int listenPort;
   OOSOCKET *listener;
   ooCallData *callList;
   int autoAnswer;   /* 1 to enable, 0 to disable */
   int callMode; /* audio/audiorx/audiotx/video/fax */
   int dtmfmode;
   ASN1UINT callEstablishmentTimeout;
   ASN1UINT msdTimeout;
   ASN1UINT tcsTimeout;
   ASN1UINT logicalChannelTimeout;
} ooEndPoint;

#endif

