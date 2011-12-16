#!/usr/local/bin/perl -w
# impl.pl.eventgen: GENERATOR FOR <code/eventgen.h>
#
# $Id: //info.ravenbrook.com/project/mps/version/1.100/code/eventgen.pl#1 $
# Copyright (c) 2001 Ravenbrook Limited.  See end of file for license.
#
# .how: Invoke this script in the src directory.  It works by scanning
# eventdef.h and then creating a file eventgen.h that includes the
# necessary types and macros.
#
# You will need to have eventgen.h claimed, and you should
# remember to check it in afterwards.

$ID = substr(q$Id: //info.ravenbrook.com/project/mps/version/1.100/code/eventgen.pl#1 $, 4, -1);

%Formats = ();

%Types = (
  "D", "double",
  "S", "EventStringStruct",
  "U", "unsigned",
  "W", "Word",
  "A", "Addr",
  "P", "void *",
	  );


#### Discover formats


open(C, "<eventdef.h") || die "Can't open $_";
while(<C>) {
  if(/RELATION\([^,]*,[^,]*,[^,]*,[^,]*, ([A-Z]+)\)/) {
    $Formats{$1} = 1 if(!defined($Formats{$1}));
  }
}
close(C);


#### Generate eventgen.h


open(H, ">eventgen.h") || die "Can't open eventgen.h for output";

print H "/* <code/eventgen.h> -- Automatic event header
 *
 * \$Id\$
 * Copyright (c) 2001 Ravenbrook Limited.  See end of file for license.
 *
 * DO NOT EDIT THIS FILE!
 * This file was generated by $ID
 */\n\n";


print H "#ifdef EVENT\n\n";


#### Generate structure definitions and accessors


foreach $format ("", sort(keys(%Formats))) {
  $fmt = ($format eq "") ? "0" : $format;
  print H "typedef struct {\n";
  print H "  Word code;\n  Word clock;\n";
  for($i = 0; $i < length($format); $i++) {
    $c = substr($format, $i, 1);
    if($c eq "S") {
      die "String must be at end of format" if($i+1 != length($format));
    }
    if(!defined($Types{$c})) {
      die "Can't find type for format code >$c<.";
    } else {
      print H "  ", $Types{$c}, " \l$c$i;\n";
    }
  }
  print H "} Event${fmt}Struct;\n\n";

  print H "#define EVENT_${fmt}_FIELD_PTR(event, i) \\\n  (";
  for($i = 0; $i < length($format); $i++) {
    $c = substr($format, $i, 1);
    print H "((i) == $i) ? (void *)&((event)->\L$fmt.$c\E$i) \\\n   : ";
  }
  print H "NULL)\n\n";
}


#### Generate union type


print H "\ntypedef union {\n  Event0Struct any;\n";

foreach $format (sort(keys(%Formats))) {
  print H "  Event${format}Struct \L$format;\n";
}
print H "} EventUnion;\n\n\n";


#### Generate writer macros


foreach $format ("", sort(keys(%Formats))) {
  $fmt = ($format eq "") ? "0" : $format;

  print H "#define EVENT_$fmt(type";
  for($i = 0; $i < length($format); $i++) {
    $c = substr($format, $i, 1);
    if($c eq "S") {
      print H ", _l$i, _s$i";
    } else {
      print H ", _\l$c$i";
    }
  }
  print H ") \\\n";

  print H "  EVENT_BEGIN(type) \\\n";

  if(($i = index($format, "S")) != -1) {
    print H "    size_t _string_len; \\\n";
  }

  for($i = 0; $i < length($format); $i++) {
    $c = substr($format, $i, 1);
    if($c eq "S") {
      print H "    _string_len = (_l$i); \\\n";
      print H "    AVER(_string_len < EventStringLengthMAX); \\\n";
      print H "    EventMould.\L$fmt.s$i.len = "
                 . "(EventStringLen)_string_len; \\\n";
      print H "    mps_lib_memcpy(EventMould.\L$fmt.s$i.str, "
                               . "_s$i, _string_len); \\\n";
    } else {
      print H "    EventMould.\L$fmt.$c$i = (_$c$i); \\\n";
    }
  }

  if(($i = index($format, "S")) != -1) {
    print H "  EVENT_END(type, $fmt, "
                      . "offsetof(Event${fmt}Struct, s$i.str) "
                      . "+ _string_len)\n\n";
  } else {
    print H "  EVENT_END(type, $fmt, "
                      . "sizeof(Event${fmt}Struct))\n\n";
  }
}


#### Generate format codes


$C = 0;
foreach $format ("0", sort(keys(%Formats))) {
  print H "#define EventFormat$format $C\n";
  $C++;
}


#### Generate dummies for non-event varieties


print H "\n#else /* EVENT not */\n\n";


print H "#define EVENT_0(type) NOOP\n";

foreach $format (sort(keys(%Formats))) {
  print H "#define EVENT_$format(type";
  for($i = 0; $i < length($format); $i++) {
    print H ", p$i";
  }
  if(($i = index($format, "S")) != -1) {
    print H ", l$i";
  }
  print H ") NOOP\n";
}


print H "\n#endif /* EVENT */\n";


close(H);


# C. COPYRIGHT AND LICENSE
#
# Copyright (C) 2001-2002 Ravenbrook Limited <http://www.ravenbrook.com/>.
# All rights reserved.  This is an open source license.  Contact
# Ravenbrook for commercial licensing options.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
# 
# 1. Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution.
# 
# 3. Redistributions in any form must be accompanied by information on how
# to obtain complete source code for this software and any accompanying
# software that uses this software.  The source code must either be
# included in the distribution or be available for no more than the cost
# of distribution plus a nominal fee, and must be freely redistributable
# under reasonable conditions.  For an executable file, complete source
# code means the source code for all modules it contains. It does not
# include source code for modules or files that typically accompany the
# major components of the operating system on which the executable file
# runs.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
# IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
# PURPOSE, OR NON-INFRINGEMENT, ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT HOLDERS AND CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
# USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
