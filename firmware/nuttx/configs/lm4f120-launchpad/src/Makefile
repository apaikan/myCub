############################################################################
# configs/lm4f120-launchpad/src/Makefile
#
#   Copyright (C) 2013 Gregory Nutt. All rights reserved.
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

-include $(TOPDIR)/Make.defs

CFLAGS		+= -I$(TOPDIR)/sched -Dgcc -DPART_LM4F120H5QR

ASRCS		= 
AOBJS		= $(ASRCS:.S=$(OBJEXT))

## 3dparty board driver ##
CFLAGS		+= -I../include
CSRCS        = board_driver/adc.c
CSRCS       += board_driver/can.c
CSRCS       += board_driver/comp.c
CSRCS       += board_driver/cpu.c
CSRCS       += board_driver/eeprom.c
CSRCS       += board_driver/epi.c
CSRCS       += board_driver/ethernet.c
CSRCS       += board_driver/fan.c
CSRCS       += board_driver/flash.c
CSRCS       += board_driver/fpu.c
CSRCS       += board_driver/gpio.c
CSRCS       += board_driver/hibernate.c
CSRCS       += board_driver/i2c.c
CSRCS       += board_driver/i2s.c
CSRCS       += board_driver/interrupt.c
CSRCS       += board_driver/lpc.c
CSRCS       += board_driver/mpu.c
CSRCS       += board_driver/peci.c
CSRCS       += board_driver/pwm.c
CSRCS       += board_driver/qei.c
CSRCS       += board_driver/ssi.c
CSRCS       += board_driver/sysctl.c
CSRCS       += board_driver/sysexc.c
CSRCS       += board_driver/systick.c
CSRCS       += board_driver/timer.c
CSRCS       += board_driver/uart.c
CSRCS       += board_driver/udma.c
CSRCS       += board_driver/usb.c
CSRCS       += board_driver/watchdog.c

## Nuttx relevent board driver ## 
CSRCS		+= lm4f_boot.c lm4f_ssi.c up_servo.c up_range_srf04.c up_adc.c up_i2c.c

ifeq ($(CONFIG_ARCH_LEDS),y)
CSRCS		+= lm4f_autoleds.c
endif

ifeq ($(CONFIG_NSH_ARCHINIT),y)
CSRCS		+= lm4f_nsh.c
endif

COBJS		= $(CSRCS:.c=$(OBJEXT))

SRCS		= $(ASRCS) $(CSRCS)
OBJS		= $(AOBJS) $(COBJS)

ARCH_SRCDIR	= $(TOPDIR)/arch/$(CONFIG_ARCH)/src
ifeq ($(WINTOOL),y)
  CFLAGS	+= -I "${shell cygpath -w $(ARCH_SRCDIR)/chip}" \
  		   -I "${shell cygpath -w $(ARCH_SRCDIR)/common}" \
  		   -I "${shell cygpath -w $(ARCH_SRCDIR)/armv7-m}"
else
  CFLAGS	+= -I$(ARCH_SRCDIR)/chip -I$(ARCH_SRCDIR)/common -I$(ARCH_SRCDIR)/armv7-m
endif

all: libboard$(LIBEXT)

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS) $(LINKOBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

libboard$(LIBEXT): $(OBJS)
	$(call ARCHIVE, $@, $(OBJS))

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(CC) -- $(CFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call DELFILE, libboard$(LIBEXT))
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
