/*
 * Copyright (C) 1997-2004 by Objective Systems, Inc.
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
 * @file asn1CEvtHndlr.h
 * C event handler structure.  The ASN1CEventHandler is a structure type 
 * which can be used to define event handlers by the user.
 */
/**
 * @defgroup Asn1CEventHandler Asn1CEventHandler
 * Asn1CEventHandler is a structure type used for user-defined event handlers.
 * @ingroup cppruntime
 * @{
 */


#ifndef _ASN1CEVTHNDLR_H_
#define _ASN1CEVTHNDLR_H_

#include <stdio.h>
#include "ooasn1.h"

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


/**
 * This is a function pointer for a callback function which is invoked
 * from within a decode function when an element of a SEQUENCE, SET,
 * SEQUENCE OF, SET OF, or CHOICE construct is parsed.
 *
 * @param name         For SEQUENCE, SET, or CHOICE, this is the name of the
 *                       element as defined in the ASN.1 defination. For
 *                       SEQUENCE OF or SET OF, this is set to the name
 *                       "element".
 * @param index        For SEQUENCE, SET, or CHOICE, this is not used and is
 *                       set to the value
 *                       -1. For SEQUENCE OF or SET OF, this contains the
 *                       zero-based index of the element in the conceptual
 *                       array associated with the construct.
 * @return             - none
 */
typedef void (*rtStartElement) (const char* name, int index) ;


/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function when parsing is complete on an element of a
 * SEQUENCE, SET, SEQUENCE OF, SET OF, or CHOICE construct.
 *
 * @param name         For SEQUENCE, SET, or CHOICE, this is the name of the
 *                       element as defined in the ASN.1 defination. For
 *                       SEQUENCE OF or SET OF, this is set to the name
 *                       "element".
 * @param index        For SEQUENCE, SET, or CHOICE, this is not used and is
 *                       set to the value
 *                       -1. For SEQUENCE OF or SET OF, this contains the
 *                       zero-based index of the element in the conceptual
 *                       array associated with the construct.
 * @return             - none
 */
typedef void (*rtEndElement) (const char* name, int index) ;


/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function when a value of the BOOLEAN ASN.1 type is parsed.
 *
 * @param value        Parsed value.
 * @return             - none
 */
typedef void (*rtBoolValue) (ASN1BOOL value);

/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function when a value of the INTERGER ASN.1 type is parsed.
 *
 * @param value        Parsed value.
 * @return             - none
 */
typedef void (*rtIntValue) (ASN1INT value);

/**
 * This is a function pointer for a callback function which is invoked
 * from within a decode function when a value of the INTEGER ASN.1 type
 * is parsed. In this case, constraints on the integer value forced the
 * use of unsigned integer C type to represent the value.
 *
 * @param value        Parsed value.
 * @return             - none
 */
typedef void (*rtUIntValue) (ASN1UINT value);

/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function when a value of the BIT STRING ASN.1 type is
 * parsed.
 *
 * @param numbits      - Number of bits in the parsed value.
 * @param data         - Pointer to a byte array that contains the bit
 *                         string data.
 * @return             - none
 */
typedef void (*rtBitStrValue) (ASN1UINT numbits, const ASN1OCTET* data);

/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function when a value of one of the OCTET STRING ASN.1 type
 * is parsed.
 *
 * @param numocts      Number of octets in the parsed value.
 * @param data         Pointer to byte array containing the octet string
 *                       data.
 * @return             - none
 */
typedef void (*rtOctStrValue) (ASN1UINT numocts, const ASN1OCTET* data) ;

/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function when a value of one of the 8-bit ASN.1 character
 * string types is parsed.
 *
 * @param value        Null terminated character string value.
 * @return             - none
 */
typedef void (*rtCharStrValue) (const char* value) ;

/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function when a value of one of the 16-bit ASN.1 character
 * string types is parsed.
 *
 * This is used for the ASN.1 BmpString type.
 *
 * @param nchars       Number of characters in the parsed value.
 * @param data         Pointer to an array containing 16-bit values.
 *                       These are represented using unsigned short integer
 *                       values.
 * @return             - none
 */
typedef void (*rtCharStrValue16Bit) (ASN1UINT nchars, ASN116BITCHAR* data) ;

/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function when a value of one of the 32-bit ASN.1 characer
 * string types is parsed.
 *
 * This is used for the ASN.1 UniversalString type.
 *
 * @param nchars       Number of characters in the parsed value.
 * @param data         Pointer to an array containing 32-bit values.
 *                       Each 32-bit integer value is a universal character.
 * @return             - none
 */
typedef void (*rtCharStrValue32Bit) (ASN1UINT nchars, ASN132BITCHAR* data) ;

/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function when a value of the NULL ASN.1 type is parsed.
 *
 * @param             - none
 * @return             - none
 */
typedef void (*rtNullValue) () ;

/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function whn a value the OBJECT IDENTIFIER ASN.1 type is
 * parsed.
 *
 * @param numSubIds    Number of subidentifiers in the object identifier.
 * @param pSubIds      Pointer to array containing the subidentifier values.
 * @return             -none
 */
typedef void (*rtOidValue) (ASN1UINT numSubIds, ASN1UINT* pSubIds) ;

 /**
  * This is a function pointer for a callback function which is invoked from
  * within a decode function when a value the REAL ASN.1 type is parsed.
  *
  * @param value        Parsed value.
  * @return             - none
  */
typedef void (*rtRealValue) (double value) ;

/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function when a value of the ENUMERATED ASN.1 type is
 * parsed.
 *
 * @param value        - Parsed enumerated value
 * @return             - none
 */
typedef void (*rtEnumValue) (ASN1UINT value) ;

/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function when an ASN.1 open type is parsed.
 *
 * @param numocts      Number of octets in the parsed value.
 * @param data         Pointer to byet array contain in tencoded ASN.1
 *                       value.
 * @return             - none
 */
typedef void (*rtOpenTypeValue) (ASN1UINT numocts, const ASN1OCTET* data) ;


/**
 * This is a basic C based event handler structure, which can be used
 * to define user-defined event handlers.
 */
typedef struct Asn1NamedCEventHandler {
   rtStartElement      startElement;
   rtEndElement        endElement;
   rtBoolValue         boolValue;
   rtIntValue          intValue;
   rtUIntValue         uIntValue;
   rtBitStrValue       bitStrValue;
   rtOctStrValue       octStrValue;
   rtCharStrValue      charStrValue;
   rtCharStrValue16Bit charStrValue16Bit;
   rtCharStrValue32Bit charStrValue32Bit;
   rtNullValue         nullValue;
   rtOidValue          oidValue;
   rtRealValue         realValue;
   rtEnumValue         enumValue;
   rtOpenTypeValue     openTypeValue;
}Asn1NamedCEventHandler;


/**
 * This function is called to add a new event handler to the context event
 * handler list.
 *
 * @param pCtxt       Context to which event handler has to be added.
 * @param pHandler    Pointer to the event handler structure.
 * @return            none
 */

EXTERN void rtAddEventHandler (OOCTXT* pCtxt,
                                Asn1NamedCEventHandler* pHandler);

/**
 * This function is called to remove an event handler from the context event
 * handler list.  Note that it does not delete the event handler object.
 *
 * @param pCtxt       Context from which event handler has to be removed.
 * @param pHandler    Pointer to event handler structure.
 * @return            none
 */
EXTERN void rtRemoveEventHandler (OOCTXT* pCtxt,
                                   Asn1NamedCEventHandler* pHandler);

/**
 * The following functions are invoked from within the generated
 * code to call the various user-defined event handler methods ..
 */
EXTERN void rtInvokeStartElement (OOCTXT* pCtxt, const char* name, int index);
EXTERN void rtInvokeEndElement (OOCTXT* pCtxt, const char* name, int index);
EXTERN void rtInvokeBoolValue (OOCTXT* pCtxt, ASN1BOOL value);
EXTERN void rtInvokeIntValue (OOCTXT* pCtxt, ASN1INT value);
EXTERN void rtInvokeUIntValue (OOCTXT* pCtxt, ASN1UINT value);
EXTERN void rtInvokeBitStrValue (OOCTXT* pCtxt, ASN1UINT numbits,
                                  const ASN1OCTET* data);
EXTERN void rtInvokeOctStrValue (OOCTXT* pCtxt, ASN1UINT numocts,
                                  const ASN1OCTET* data);
EXTERN void rtInvokeCharStrValue (OOCTXT* pCtxt, const char* value);
EXTERN void rtInvokeCharStr16BitValue (OOCTXT* pCtxt, ASN1UINT nchars,
                                   ASN116BITCHAR* data);
EXTERN void rtInvokeCharStr32BitValue (OOCTXT* pCtxt, ASN1UINT nchars,
                                   ASN132BITCHAR* data);
EXTERN void rtInvokeNullValue (OOCTXT* pCtxt);
EXTERN void rtInvokeOidValue (OOCTXT* pCtxt, ASN1UINT numSubIds,
                               ASN1UINT* pSubIds);
EXTERN void rtInvokeRealValue (OOCTXT* pCtxt, double value);
EXTERN void rtInvokeEnumValue (OOCTXT* pCtxt, ASN1UINT value);
EXTERN void rtInvokeOpenTypeValue (OOCTXT* pCtxt, ASN1UINT numocts,
                                    const ASN1OCTET* data);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
