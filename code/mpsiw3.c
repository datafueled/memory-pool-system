/* mpsint.c: WIN32 MEMORY POOL SYSTEM INTERFACE LAYER EXTRAS
 *
 *  $Id: //info.ravenbrook.com/project/mps/master/code/mpsiw3.c#9 $
 *
 *  Copyright (c) 2001 Ravenbrook Limited.  See end of file for license.
 */

#include "mpm.h"
#include "mps.h"

#include "mpswin.h"

SRCID(mpsiw3, "$Id: //info.ravenbrook.com/project/mps/master/code/mpsiw3.c#9 $");


/* This is defined in protw3.c */
extern LONG WINAPI ProtSEHfilter(LPEXCEPTION_POINTERS info);

LONG mps_SEH_filter(LPEXCEPTION_POINTERS info,
                    void **hp_o, size_t *hs_o)
{
  UNUSED(hp_o);
  UNUSED(hs_o);
  return ProtSEHfilter(info);
}

void mps_SEH_handler(void *p, size_t s)
{
  UNUSED(p); UNUSED(s);
  NOTREACHED;
}


/* C. COPYRIGHT AND LICENSE
 *
 * Copyright (C) 2001-2002 Ravenbrook Limited <http://www.ravenbrook.com/>.
 * All rights reserved.  This is an open source license.  Contact
 * Ravenbrook for commercial licensing options.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * 3. Redistributions in any form must be accompanied by information on how
 * to obtain complete source code for this software and any accompanying
 * software that uses this software.  The source code must either be
 * included in the distribution or be available for no more than the cost
 * of distribution plus a nominal fee, and must be freely redistributable
 * under reasonable conditions.  For an executable file, complete source
 * code means the source code for all modules it contains. It does not
 * include source code for modules or files that typically accompany the
 * major components of the operating system on which the executable file
 * runs.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, OR NON-INFRINGEMENT, ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS AND CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
