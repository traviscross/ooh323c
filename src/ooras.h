/*
 * Copyright (C) 2005 by Page Iberica, S.A.
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
 * @file ooras.h
 * This file contains functions to support RAS protocol.
 *
 *
 */


#ifndef _OORASHDR_H_
#define _OORASHDR_H_

#include "ooasn1.h"
#include "ootypes.h"
#include "H323-MESSAGES.h"



/*-------------------------------------------------------------------*/
/*  This constant is used to tell the toolchain whether to copile or */
/*  not the RAS functionality. Define this constant to compile and   */
/*  link RAS code. Undefine it to save memory by not linking RAS     */
/*  functionality.                                                   */
/*-------------------------------------------------------------------*/

#define __USING_RAS


/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

#ifdef __USING_RAS

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EXTERN
#ifdef _WIN32
#define EXTERN __declspec(dllexport)
#else
#define EXTERN
#endif /* _WIN32 */
#endif /* EXTERN */


/*-------------------------------------------------------------------*/
/*  Public definitions                                               */
/*-------------------------------------------------------------------*/


#define OORAS_DEFAULT_REMOTE_PORT   1719

/*-------------------------------------------------------------------*/
/*  Public data types                                                */
/*-------------------------------------------------------------------*/


/**
 * @defgroup ras RAS Channel and Message handling
 * @{
 */

enum RasGatekeeperMode {
   RasNoGatekeeper = 0,
   RasDiscoverGatekeeper = 1,
   RasUseSpecificGatekeeper = 2,
};

enum RasCallModel {
   RasDirect = 0,
   RasGkRouted,
};

enum RasCallType{
   RasPointToPoint =0,
   RasOneToN,
   RasnToOne,
   RasnToN,
};
/**
 Defines the RAS message structure. Contains
 context for memory allocation, protocol Discriminator,
 call reference, meesage type and list of user user IEs.
*/

typedef struct ooRasMessage
{
   OOCTXT* psContext; /*context is used to allocate memory for different parts of the message */
   unsigned char ucBuilt;
   H225RasMessage sMessage;
} ooRasMessage;



/*
  Structure containing RAS parameters.
  All timing parameters in milliseconds.
*/


typedef struct {
   unsigned int uiGRQTimeout;   /* Timeout to resend GRQ's. 5 secs. minimun */
   unsigned int uiRRQTimeout;   /* Timeout to resend RRQ's. 5 secs. minimun */
   unsigned int uiRRQRetries;   /* Max RRQ retries with no answer before go back to send RRQs.*/
   unsigned int uiAddRegPeriod; /* Additive registration period. 0 if additive registrations */
                                /*are not desired.*/
} ooRasParams;




/*-------------------------------------------------------------------*/
/*  Functions exported by this module                                 */
/*-------------------------------------------------------------------*/


/**
 * This function is used to initialize the Ras module by setting gatekeeper
 * mode and Creating Ras Channel
 */
EXTERN int ooInitRas(int localRasPort, enum RasGatekeeperMode eGkMode,
                     char *szGkAddr, int iGkPort );


/**
 * This function is called periodically to monitor the process the RAS
 * channel.
 */
EXTERN void ooRasExplorer();


/**
 * Populate Alias list
 * @param psContext    Context to be used for allocating memory for
 *                      alias list.
 * @param psAliases     List of aliases from which alias list structure has
 *                      to be populated.
 * @param psAliasList   Alias list structure which has to be populated.
 *
 * @return              OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooRasFillAlias( OOCTXT *psContext, ooAliases *psAliases,
                           H225_SeqOfH225AliasAddress *psAliasList );


/**
 * This function is invoked to get current RAS socket.
 *
 * @return  RAS socket.
 */
EXTERN OOSOCKET ooRasGetSocket();


/**
 * This function is invoked receive data from RAS port.
 *
 * @return         Completion status - 0 on success, -1 on failure
 */
EXTERN int ooRasReceive();

/**
 * This function is invoked to set a gatekeeper mode.
 *
 * @param eGkMode  Gatekeeper mode selected. One of the following:
 *                    - RasNoGatekeeper (DEFAULT), No Gatekeeper.             
 *                    - RasDiscoverGatekeeper, to discover a gatekeeper
 *                      automatically.                                        
 *                    - RasUseSpecificGatekeeper, to use a specific gatekeeper.
 * @param szGkAddr  Gatekeeper address (only when using specific gatekeeper).
 * @param iGkPort  Gatekeeper RAS port
 *
 * @return         Completion status - 0 on success, -1 on failure
 */
EXTERN int ooRasSetGatekeeperMode( enum RasGatekeeperMode eGkMode,
                                   char *szGkAddr,
                                   int iGkPort );


/**
 * This function is invoked to get current gatekeeper mode.
 *
 * @return         Gatekeeper mode selected. One of the following:            
 *                    - RasNoGatekeeper (DEFAULT), No Gatekeeper.             
 *                    - RasDiscoverGatekeeper, to discover a gatekeeper
 *                      automatically.
 *                    - RasUseSpecificGatekeeper, to use a specific gatekeeper.
 */
EXTERN enum RasGatekeeperMode ooRasGetGatekeeperMode();


/**
 * This function is used to determine the registration status
 * of the endpoint with the gatekeeper.
 *
 * @return         TRUE, if registered, FALSE otherwise.
 */
EXTERN ASN1BOOL ooRasIsRegistered();


/**
 * This function is invoked to request bandwith admission for a call.
 *
 * @param call          Handle to the call.
 * @param eModel        Call Model (RasDirect/RasGkRouted)
 * @param psSrcAliases  Pointer to the calling party's aliases.    
 * @param psDestAliases Pointer to the called party's aliases.
 *
 * @return              OO_OK, on success. OO_FAILED, on failure.
 * @return  Completion status - 0 on success, -1 on failure
 */

EXTERN int ooRasSendAdmissionRequest
   (ooCallData * call, enum RasCallModel eModel, ooAliases *psSrcAliases,
    ooAliases *psDestAliases);

/**
 * This function is invoked to request call disengage to gatekeeper.
 *
 * @param call    Call Handle
 *
 * @return        Completion status - 0 on success, -1 on failure
 */
EXTERN int ooRasSendDisengageRequest(ooCallData *call);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#else /* __USING_RAS */
#define ooInitRas( iLocalPort, mode, ip, port )
#define ooRasExplorer()
#define ooRasGetSocket() 0
#define ooRasReceive()
#define ooRasSetGatekeeperMode( eGkMode, szGkAddr, usGkPort )
#define ooRasGetGatekeeperMode( peGkMode, szGkAddr, usGkPort )
#define ooRasGetGatekeeperStatus( szGatekeeperId, psGkAddr )
#define ooRasSetParams( psParams )
#define ooRasAdmissionRequest( szCallToken, uiBandwith )
#define ooRasDisengageRequest( szCallToken )

#endif /* __USING_RAS */

#endif /* __RASHDR_H */
