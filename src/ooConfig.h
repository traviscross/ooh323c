/*
 * Copyright (C) 2004-2010 by Objective Systems, Inc.
 *
 * This software is furnished under an open source license and may be
 * used and copied only in accordance with the terms of this license.
 * The text of the license may generally be found in the root
 * directory of this installation in the LICENSE.txt file.  It
 * can also be viewed online at the following URL:
 *
 *   http://www.obj-sys.com/open/license.html
 *
 * Any redistributions of this file including modified versions must
 * maintain this copyright notice.
 *
 *****************************************************************************/
/**
 * @file ooConfig.h
 * Configuration file handling functions.
 */
/**
 * @defgroup ooConfig Configuration file handling functions.
 * @{
 * General utility functions for reading and searching configuration files.
 */
#ifndef _OOCONFIG_H_
#define _OOCONFIG_H_

#include "ooCommon.h"
#include "dlist.h"

/* Configuration name/value pair */

typedef struct OOConfigNVP {
   char* name;
   char* value;
} OOConfigNVP;

/* Configuration section */

typedef struct OOConfigSection {
   char* sectionName;
   DList nameValuePairs; /* list of OOConfigNVP */
} OOConfigSection;

/* Configuration file is a list of configuration sections */

typedef struct OOConfigFile {
   OOCTXT* pctxt;
   OOBOOL  bOwnContext;
   DList   sections;  /* List of OOConfigSection */
} OOConfigFile;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This function initializes a configuration file structure.  It should be
 * the first function called with the given list structure.
 *
 * @param pconfig   Pointer to configuration file variable.
 * @return          0 if successful, ASN_E_NOMEM if memory could not be
 *                    allocated for the context.
 */
EXTERN int ooConfigInit (OOConfigFile* pconfig);

/**
 * This function initializes a configuration file structure using an
 * existing context.  This is more efficient then the default initialization
 * which creates a new context.
 *
 * @param pctxt     Pointer to an existing initialized context.
 * @param pconfig   Pointer to configuration file variable.
 */
EXTERN void ooConfigCtxtInit (OOCTXT* pctxt, OOConfigFile* pconfig);

/**
 * This function reads the contents of a configuration file into a
 * configuration structure.
 *
 * @param filename  Full path to a configuration file.
 * @param pconfig   Pointer to configuration file variable.
 * @return          Status of operation: 0 if successful or a negative
 *                    status code if error.
 */
EXTERN int ooConfigFileRead (const char* filename, OOConfigFile* pconfig);

/**
 * This function destroys an existing in-memory configuration structure.
 *
 * @param pconfig   Pointer to configuration file variable.
 */
EXTERN void ooConfigDestroy (OOConfigFile* pconfig);

/**
 * This function prints the contents of an in-memory configuration structure.
 *
 * @param pconfig   Pointer to configuration file variable.
 */
EXTERN void ooConfigPrint (OOConfigFile* pconfig);

#ifdef __cplusplus
}
#endif
/**
 * @} ooConfig
 */

#endif
