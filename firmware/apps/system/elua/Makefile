############################################################################
#
#   Copyright (C) 2008, 2010-2013 Gregory Nutt. All rights reserved.
#   Author: Gregory Nutt <gnutt@nuttx.org>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. Neither the name NuttX nor the names of its contributors may be
#    used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
############################################################################

-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs
include $(APPDIR)/Make.defs

# Hello, World! built-in application info

APPNAME		= elua
PRIORITY	= SCHED_PRIORITY_DEFAULT
STACKSIZE	= 2048

# Hello, World! Example

CFLAGS		+= -I./inc -DLUA_CROSS_COMPILER -DBUFSIZ=128

ASRCS		=
CSRCS		= elua.c

CSRCS      += ./lua/lapi.c
CSRCS      += ./lua/lauxlib.c
CSRCS      += ./lua/lbaselib.c
CSRCS      += ./lua/lcode.c
CSRCS      += ./lua/ldblib.c
CSRCS      += ./lua/ldebug.c
CSRCS      += ./lua/ldo.c
CSRCS      += ./lua/ldump.c
CSRCS      += ./lua/legc.c
CSRCS      += ./lua/lfunc.c
CSRCS      += ./lua/lgc.c
CSRCS      += ./lua/linit.c
#CSRCS      += ./lua/liolib.c
CSRCS      += ./lua/llex.c
CSRCS      += ./lua/lmathlib.c
CSRCS      += ./lua/lmem.c
CSRCS      += ./lua/loadlib.c
CSRCS      += ./lua/lobject.c
CSRCS      += ./lua/lopcodes.c
#CSRCS      += ./lua/loslib.c
CSRCS      += ./lua/lparser.c
CSRCS      += ./lua/lrotable.c
CSRCS      += ./lua/lstate.c
CSRCS      += ./lua/lstring.c
CSRCS      += ./lua/lstrlib.c
CSRCS      += ./lua/ltable.c
CSRCS      += ./lua/ltablib.c
CSRCS      += ./lua/ltm.c
CSRCS      += ./lua/lua.c
CSRCS      += ./lua/luac.c
CSRCS      += ./lua/lundump.c
CSRCS      += ./lua/lvm.c
CSRCS      += ./lua/lzio.c
CSRCS      += ./lua/print.c

AOBJS		= $(ASRCS:.S=$(OBJEXT))
COBJS		= $(CSRCS:.c=$(OBJEXT))

SRCS		= $(ASRCS) $(CSRCS)
OBJS		= $(AOBJS) $(COBJS)

ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  BIN		= ..\..\libapps$(LIBEXT)
else
ifeq ($(WINTOOL),y)
  BIN		= ..\\..\\libapps$(LIBEXT)
else
  BIN		= ../../libapps$(LIBEXT)
endif
endif

ROOTDEPPATH	= --dep-path .

# Common build

VPATH		= 

all: .built
.PHONY: clean depend distclean

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

.built: $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))
	@touch .built

ifeq ($(CONFIG_NSH_BUILTIN_APPS),y)
$(BUILTIN_REGISTRY)$(DELIM)$(APPNAME)_main.bdat: $(DEPCONFIG) Makefile
	$(call REGISTER,$(APPNAME),$(PRIORITY),$(STACKSIZE),$(APPNAME)_main)

context: $(BUILTIN_REGISTRY)$(DELIM)$(APPNAME)_main.bdat
else
context:
endif

.depend: Makefile $(SRCS)
	@$(MKDEP) $(ROOTDEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	@touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
