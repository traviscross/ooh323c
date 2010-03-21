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

#include <ctype.h>
#include <stdio.h>
#include "dlist.h"
#include "ooasn1.h" /* for error codes */
#include "ooConfig.h"
#include "ootrace.h"

int ooConfigInit (OOConfigFile* pconfig)
{
   if (0 != pconfig) {
      pconfig->pctxt = (OOCTXT*) OOMEMALLOC (sizeof(OOCTXT));
      if (0 == pconfig->pctxt) return ASN_E_NOMEM;

      initContext (pconfig->pctxt);
      pconfig->bOwnContext = TRUE;

      dListInit (&pconfig->sections);
   }

   return 0;
}

void ooConfigCtxtInit (OOCTXT* pctxt, OOConfigFile* pconfig)
{
   if (0 != pconfig) {
      pconfig->pctxt = pctxt;
      pconfig->bOwnContext = FALSE;
      dListInit (&pconfig->sections);
   }
}

int ooConfigFileRead (const char* filename, OOConfigFile* pconfig)
{
   char lbuf[256];
   OOCTXT* pctxt = pconfig->pctxt;
   OOConfigSection* pConfigSect = 0;
   FILE* fp = fopen (filename, "r");
   if (0 == fp) return (ASN_E_FILNOTFOU);

   while (fgets (lbuf, sizeof(lbuf), fp)) {
      if (lbuf[0] == '[') {
         /* Start of section */
         size_t i, len = strlen (lbuf);
         pConfigSect = (OOConfigSection*)
            memAlloc (pctxt, sizeof(OOConfigSection));

         if (0 == pConfigSect) {
            OOTRACEERR1 ("ERROR: no memory for config section\n");
            return ASN_E_NOMEM;
         }

         pConfigSect->sectionName = (char*) memAlloc (pctxt, len);

         if (0 == pConfigSect->sectionName) {
            OOTRACEERR1 ("ERROR: no memory for config section name\n");
            return ASN_E_NOMEM;
         }

         for (i = 1; i < len; i++) {
            if (lbuf[i] != ']') {
               pConfigSect->sectionName[i-1] = lbuf[i];
            }
            else break;
         }
         pConfigSect->sectionName[i-1] = '\0';

         dListInit (&pConfigSect->nameValuePairs);

         dListAppend (pctxt, &pconfig->sections, (void*)pConfigSect);
      }
      else if (isalpha (lbuf[0])) {
         /* Start of name/value pair */
         /* format is name=value */
         size_t i, len = strlen (lbuf), len2 = 0;
         OOConfigNVP* pnvp = (OOConfigNVP*)
            memAlloc (pctxt, sizeof(OOConfigNVP));

         if (0 == pnvp) {
            OOTRACEERR1 ("ERROR: no memory for name/value pair\n");
            return ASN_E_NOMEM;
         }

         /* remove comment from line */
         for (i = 0; i < len; i++) {
            if (lbuf[i] == ';') {
               lbuf[i] = '\0';
               len = i;
               break;
            }
         }

         /* trim whitespace from end of line */
         i = len - 1;
         while (isspace (lbuf[i]) && i > 0) { i--; }
         len = i + 1;
         lbuf[len] = '\0';

         /* parse name */
         for (i = 0; i < len; i++) {
            if (lbuf[i] == '=') break;
         }
         if (i < len) {
            pnvp->name = (char*) memAlloc (pctxt, i+1);
            if (0 == pnvp->name) {
               OOTRACEERR1 ("ERROR: no memory for name\n");
               return ASN_E_NOMEM;
            }
            strncpy (pnvp->name, lbuf, i);
            pnvp->name[i] = '\0';
         }
         else { /* error */
            OOTRACEERR1 ("ERROR: malformed name/value pair:\n");
            OOTRACEERR1 (lbuf);
            continue;
         }

         /* parse value */
         len2 = len - i - 1;
         if (len2 > 0) {
            pnvp->value = (char*) memAlloc (pctxt, len2+1);
            if (0 == pnvp->value) {
               OOTRACEERR1 ("ERROR: no memory for value\n");
               return ASN_E_NOMEM;
            }
            strncpy (pnvp->value, &lbuf[i+1], len2);
            pnvp->value[len2] = '\0';
         }

         /* add name/value to list */
         if (0 != pConfigSect) {
            dListAppend (pctxt, &pConfigSect->nameValuePairs, (void*)pnvp);
         }
         else {
            OOTRACEERR1 ("ERROR: no configuration section for this NVP:\n");
            OOTRACEERR1 (lbuf);
         }
      }
      else if (lbuf[0] != ';' && !isspace(lbuf[0])) {
         OOTRACEERR1 ("ERROR: unknown configuration line:\n");
         OOTRACEERR1 (lbuf);
      }
   }

   fclose (fp);

   return 0;
}

void ooConfigDestroy (OOConfigFile* pconfig)
{
   if (!pconfig->bOwnContext) {
      /* release memory used for config */
      OOCTXT* pctxt = pconfig->pctxt;
      OOConfigSection* pConfigSect;
      for (;;) {
         pConfigSect = (OOConfigSection*)
            dListDeleteHead (pctxt, &pconfig->sections);

         if (0 != pConfigSect) {
            OOConfigNVP* pnvp;
            memFreePtr (pctxt, pConfigSect->sectionName);
            for (;;) {
               pnvp = (OOConfigNVP*)
                  dListDeleteHead (pctxt, &pConfigSect->nameValuePairs);

               if (0 != pnvp) {
                  memFreePtr (pctxt, pnvp->name);
                  memFreePtr (pctxt, pnvp->value);
               }
               else break;
            }
            memFreePtr (pctxt, pConfigSect);
         }
         else break;
      }
   }
   else {
      freeContext (pconfig->pctxt);
      OOMEMFREE (pconfig->pctxt);
      memset (pconfig, 0, sizeof(OOConfigFile));
   }
}

void ooConfigPrint (OOConfigFile* pconfig)
{
   OOConfigSection* pConfigSect;
   DListNode* pnode = pconfig->sections.head;
   while (0 != pnode) {
      pConfigSect = (OOConfigSection*) pnode->data;

      if (0 != pConfigSect) {
         OOConfigNVP* pnvp;
         DListNode* pnode2 = pConfigSect->nameValuePairs.head;

         printf ("section = '%s'\n", pConfigSect->sectionName);

         while (0 != pnode2) {
            pnvp = (OOConfigNVP*) pnode2->data;
            printf ("%s = %s\n", pnvp->name, pnvp->value);
            pnode2 = pnode2->next;
         }

         printf ("\n");
      }
      pnode = pnode->next;
   }
}
