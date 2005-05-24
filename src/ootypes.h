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
/*! \mainpage
 * <H1>ooH323c Stack Functions</H1>
 *
 * The <B>Objective Open H.323 for C (ooH323c)</B> protocol stack is an
 * open source applications program interface (API) for building H.323 based
 * applications.   The stack implements Q.931/H.225 call signaling procedures,
 * H.245 logical channel operations, and Registration, Admission, and Status
 * (RAS) messaging for Gatekeeper communications.
 *
 * The categories of user functions provided are as follows:<UL>
 * <LI>Stack command functions.  These are high level functions used to
 * initiate common H.323 telephony operations (for example, to make a
 * call).</LI>
 * <LI>Gatekeeper functions.  These are high level functions for
 * managing communications with a gatekeeper using RAS messages.</LI>
 * <LI>H.323 endpoint management functions.  These are function for
 * used for managing the global H.323 endpoint.</LI>
 * <LI>Call management functions.  These are functions used to manage
 * active calls within the stack.</LI>
 * <LI>Capability management functions.  These functions are used for
 * negotiating capabilities between two different terminals.</LI>
 * <LI>H.225 and H.245 message handling functions.  Functions for
 * creating and handling H.323 standard ASN.1 messages.</LI>
 * <LI>Q.931 functions.  Functions for the execution of various
 * standard Q.931 operations.</LI>
 * <LI>TCP/IP and UDP socket communication functions.  Low-level
 * functions for writing data to and receiving data from sockets.</LI>
 * </UL>
 */
/**
 * @file ootypes.h
 * This file contains definitions of common constants and data structures.
 */
#ifndef _OOTYPES_H_
#define _OOTYPES_H_

#include "ooSocket.h"
#include "MULTIMEDIA-SYSTEM-CONTROL.h"
#include "H323-MESSAGES.h"
#include "ooasn1.h"


#define OOH323C_VERSION "v0.6.1"

/**
 * @defgroup ootypes Common type and constant definitions.
 * @{
 */
/* Function return codes */
#define OO_FAILED       -1
#define OO_OK           1

/* TODO: States for both local and remote initiation should be maintained
   separately */
/**
 * States defined for master/slave determination procedure.
 */
typedef enum OOMasterSlaveState {
   OO_MasterSlave_Idle,
   OO_MasterSlave_DetermineSent,
   OO_MasterSlave_AckReceived,
   OO_MasterSlave_Master,
   OO_MasterSlave_Slave
} OOMasterSlaveState;

/**
 * States defined for the capability exchange procedure.
 */
typedef enum {
   OO_LocalTermCapExchange_Idle,
   OO_LocalTermCapSetSent,
   OO_LocalTermCapSetAckRecvd,
   OO_RemoteTermCapExchange_Idle,
   OO_RemoteTermCapSetRecvd,
   OO_RemoteTermCapSetAckSent
} OOCapExchangeState;

/**
 * Call clear reason codes.
 */
typedef enum OOCallClearReason {
   OO_REASON_UNKNOWN=0,
   OO_REASON_INVALIDMESSAGE,
   OO_REASON_TRANSPORTFAILURE,
   OO_REASON_NOROUTE,
   OO_REASON_NOUSER,
   OO_REASON_NOBW,
   OO_REASON_GK_NOCALLEDUSER,
   OO_REASON_GK_NOCALLERUSER,
   OO_REASON_GK_NORESOURCES,
   OO_REASON_GK_UNREACHABLE,
   OO_REASON_GK_CLEARED,
   OO_REASON_NOCOMMON_CAPABILITIES,
   OO_REASON_REMOTE_FWDED,  
   OO_REASON_LOCAL_FWDED,
   OO_REASON_REMOTE_CLEARED,
   OO_REASON_LOCAL_CLEARED,
   OO_REASON_REMOTE_BUSY,
   OO_REASON_LOCAL_BUSY,
   OO_REASON_REMOTE_NOANSWER,
   OO_REASON_LOCAL_NOTANSWERED,
   OO_REASON_REMOTE_REJECTED,
   OO_REASON_LOCAL_REJECTED,
   OO_REASON_REMOTE_CONGESTED,
   OO_REASON_LOCAL_CONGESTED
} OOCallClearReason;

/**
 * Call states.
 */
typedef enum {
   OO_CALL_CREATED,               /*!< Call created. */
   OO_CALL_WAITING_ADMISSION,     /*!< Call waiting for admission by GK */
   OO_CALL_CONNECTING,            /*!< Call in process of connecting */
   OO_CALL_CONNECTED,             /*!< Call currently connected. */
   OO_CALL_CLEAR,                 /*!< Call marked for clearing */
   OO_CALL_CLEAR_RELEASERECVD,    /*!< Release command received. */
   OO_CALL_CLEAR_RELEASESENT,     /*!< Release sent */
   OO_CALL_CLEARED                /*!< Call cleared */
} OOCallState;

/**
 * H.245 session state
 */
typedef enum {
   OO_H245SESSION_IDLE,
   OO_H245SESSION_ACTIVE,
   OO_H245SESSION_ENDSENT,
   OO_H245SESSION_ENDRECVD,
   OO_H245SESSION_CLOSED
} OOH245SessionState;

/**
 * Logical channel states.
 */
typedef enum {
   OO_LOGICAL_CHAN_UNKNOWN,
   OO_LOGICALCHAN_IDLE,
   OO_LOGICALCHAN_PROPOSED,
   OO_LOGICALCHAN_ESTABLISHED
} OOLogicalChannelState;

/**
 * Terminal type of the endpoint. Default is 60.
 */
#define OOTERMTYPE 60
#define MAX_IP_LENGTH 15
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
#define OOOpenLogicalChannelConfirm    121
#define OOCloseLogicalChannel          122
#define OOCloseLogicalChannelAck       123
#define OORequestChannelClose          124
#define OORequestChannelCloseAck       125
#define OORequestChannelCloseReject    126
#define OORequestChannelCloseRelease   127
#define OOEndSessionCommand            128
#define OO_MSGTYPE_MAX                 128


/* Timer types */
#define OO_CALLESTB_TIMER  (1<<0)
#define OO_MSD_TIMER       (1<<1)
#define OO_TCS_TIMER       (1<<2)
#define OO_OLC_TIMER       (1<<3)
#define OO_CLC_TIMER       (1<<4)
#define OO_RCC_TIMER       (1<<5)
#define OO_SESSION_TIMER   (1<<6)

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
 * Maximum length for received messages.
 */
#define MAXMSGLEN 4096

/**
 * Maximum length of a filename.
 */
#define MAXFILENAME 256

#define OO_CMD_MAKECALL      201
#define OO_CMD_ANSCALL       202
#define OO_CMD_FWDCALL       203
#define OO_CMD_HANGCALL      204
#define OO_CMD_STOPMONITOR   205


/**
 Endpoint call modes. The call mode of the endpoint dictates what type
 of channels are created for the calls placed by the endpoint or received
 by the endpoint.
*/
typedef enum OOCallMode {
   OO_CALLMODE_AUDIOCALL,
   OO_CALLMODE_AUDIORX,
   OO_CALLMODE_AUDIOTX,
   OO_CALLMODE_VIDEOCALL,
   OO_CALLMODE_FAX
} OOCallMode;

/*
 * Flag macros - these operate on bit mask flags using mask values
 */
#define OO_SETFLAG(flags,mask) (flags |= mask)
#define OO_CLRFLAG(flags,mask) (flags &= ~mask)
#define OO_TESTFLAG(flags,mask) ((flags & mask) != 0)

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

/* Define common mutex type */
#ifdef _WIN32
#define OO_MUTEX CRITICAL_SECTION
#else
#define OO_MUTEX pthread_mutex_t
#endif

/**
 * Structure for stack commands */
typedef struct ooCommand {
   int type;
   void * param1;
   void * param2;
   void * param3;
} ooCommand;


/*TODO: Should add caller-id, callername etc. So that they can be changed per
  call basis*/
typedef struct ooCallOptions {
   OOBOOL fastStart; /* faststart for this call. overrides endpoint setting*/
   OOBOOL tunneling; /* tunneling for this call. overrides endpoint setting*/
   OOBOOL disableGk; /* If gk is enabled, then you can avoid going through gk for a specific call */
   OOCallMode callMode; /* Effective when faststart is used. Tells stack which type of channels to setup*/
}ooCallOptions;

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

struct Q931InformationElement;
/**
 Defines the Q931 message structure. Contains
 context for memory allocation, protocol Discriminator,
 call reference, meesage type and list of user user IEs.
*/
typedef struct Q931Message {
   ASN1UINT protocolDiscriminator;
   ASN1UINT callReference;
   ASN1BOOL fromDestination;
   ASN1UINT messageType;      /* Q931MsgTypes */
   ASN1UINT tunneledMsgType;  /* The H245 message this message is tunneling*/
   ASN1INT  logicalChannelNo; /* channel number associated with tunneled */
                              /* message, 0 if no channel */
   DList ies;   
   struct Q931InformationElement *bearerCapabilityIE;
   struct Q931InformationElement *callingPartyNumberIE;
   struct Q931InformationElement *calledPartyNumberIE;
   struct Q931InformationElement *causeIE;
   H225H323_UserInformation *userInfo;
} Q931Message;

/**
 Defines the H245 message structure. All request/response
 and command messages are represented using this structure.
*/
typedef struct H245Message {
   H245MultimediaSystemControlMessage h245Msg;
   ASN1UINT msgType;
   ASN1INT  logicalChannelNo;
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
typedef struct ooLogicalChannel {
   int  channelNo;
   int  sessionID;
   char type[10]; /* audio/video/data */
   char dir[10];  /* receive/transmit */
   char remoteIP[20];
   int  mediaPort;
   int  mediaControlPort;
   int  localRtpPort;
   int  localRtcpPort;
   char localIP[20];
   OOLogicalChannelState state;        
   struct ooH323EpCapability *chanCap;
   struct ooLogicalChannel *next;
} ooLogicalChannel;

typedef struct ooAliases{
   int type;
   char *value;
   OOBOOL registered;
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


struct OOH323CallData;

typedef struct ooTimerCallback{
   struct OOH323CallData* call;
   ASN1UINT    timerType;
   ASN1UINT    channelNumber;
} ooTimerCallback;

typedef struct OOCallFwdData{
   char ip[20];
   int port;
   ooAliases *aliases;
   OOBOOL fwdedByRemote; /*Set when we are being fwded by remote*/
}OOCallFwdData;     

/**
 * These are message callbacks which can be used by user applications
 * to perform application specific things on receiving a particular
 * message or before sending a particular message. For ex. user application
 * can change values of some parameters of setup message before it is actually
 * sent out.
 */
typedef int (*cb_OnReceivedSetup)
   (struct OOH323CallData *call, Q931Message *pmsg);

typedef int (*cb_OnReceivedConnect)
   (struct OOH323CallData *call, Q931Message *pmsg);

typedef int (*cb_OnBuiltSetup)
   (struct OOH323CallData *call, Q931Message *pmsg);

typedef int (*cb_OnBuiltConnect)
   (struct OOH323CallData *call, Q931Message *pmsg);

typedef struct OOH225MsgCallbacks{
   cb_OnReceivedSetup onReceivedSetup;
   cb_OnReceivedConnect onReceivedConnect;
   cb_OnBuiltSetup onBuiltSetup;
   cb_OnBuiltConnect onBuiltConnect;
}OOH225MsgCallbacks;


/**
 * This callback function is triggered when a new call structure is
 * created inside the stack for an incoming or outgoing call.
 *
 * @param call H.323 call data structure
 * @return 0 if callback was successful, non-zero error code if failure.
 */
typedef int (*cb_OnNewCallCreated)(struct OOH323CallData * call);

typedef int (*cb_OnAlerting)(struct OOH323CallData * call);
typedef int (*cb_OnIncomingCall)(struct OOH323CallData* call );
typedef int (*cb_OnOutgoingCall)(struct OOH323CallData* call );
typedef int (*cb_OnCallAnswered)(struct OOH323CallData* call);
typedef int (*cb_OnCallEstablished)(struct OOH323CallData* call);
typedef int (*cb_OnOutgoingCallAdmitted)(struct OOH323CallData* call );
typedef int (*cb_OnCallCleared)(struct OOH323CallData* call);
typedef int (*cb_OpenLogicalChannels)(struct OOH323CallData* call);
typedef int (*cb_OnCallForwarded)(struct OOH323CallData* call);

struct ooGkClient;

typedef struct OOH323CALLBACKS{
   cb_OnAlerting onNewCallCreated;
   cb_OnAlerting onAlerting;
   cb_OnIncomingCall onIncomingCall;
   cb_OnOutgoingCall onOutgoingCall;
   cb_OnCallAnswered onCallAnswered;
   cb_OnCallEstablished onCallEstablished;
   cb_OnCallForwarded onCallForwarded;
   cb_OnOutgoingCallAdmitted onOutgoingCallAdmitted;
   cb_OnCallCleared onCallCleared;
   cb_OpenLogicalChannels openLogicalChannels;
} OOH323CALLBACKS;

/**
 * @}
 */
#endif

