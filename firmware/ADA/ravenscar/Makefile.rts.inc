CROSS=arm-none-eabi-

PART=LM4F120H5QR
CPU=-mcpu=cortex-m4
FPU=-mfpu=fpv4-sp-d16 -mfloat-abi=softfp

CFLAGS?=-Os -g -Wall -Werror -ffunction-sections -fdata-sections -MD -std=c99  -mthumb ${CPU} ${FPU} \
  -I$(RAVENSCAR_SRC)/lm4f120 \
  -I$(RAVENSCAR_SRC)/lm4f120/inc \
  -I$(RAVENSCAR_SRC)/lm4f120/driverlib \
  -DPART_$(PART) -DTARGET_IS_BLIZZARD_RA1 -Dgcc

KERN_OBJS = startup_lm4f120.o main.o context_switch-bb-arm.o
LM4F_FILE=$(wildcard $(RAVENSCAR_SRC)/lm4f120/driverlib/*.c)
LM4F_TMP=$(filter-out %template.c,$(LM4F_FILE))
LM4F_OBJ=$(notdir $(LM4F_TMP:.c=.o))

RTS_FILE=$(shell grep -L "^separate" $(RTS_BASE)/adainclude/*.ad[sb])
RTS_OBJ=$(notdir $(patsubst %.adb,%.o,$(RTS_FILE:.ads=.o)))

all: libgnat.a liblm4f120.a

libgnat.a: $(KERN_OBJS) $(RTS_OBJ) force
	$(CROSS)ar rcs $@ $(KERN_OBJS) $(RTS_OBJ)

liblm4f120.a: $(LM4F_OBJ)
	$(CROSS)ar rcs $@ $(LM4F_OBJ)

GCC_COMPILE=$(CROSS)gcc -c -o $@ $(CFLAGS) $<

%.o: $(RAVENSCAR_SRC)/%.s
	$(GCC_COMPILE)

%.o: $(RAVENSCAR_SRC)/%.c
	$(GCC_COMPILE)

%.o: $(GCC_SRC)/%.c
	$(GCC_COMPILE)

%.o: $(RAVENSCAR_SRC)/lm4f120/driverlib/%.c
	$(GCC_COMPILE)

%.o: $(RTS_BASE)/adainclude/%.adb
	$(CROSS)gnatmake -gnata -a -c --RTS=$(RTS_BASE) -o $@ $(CFLAGS) $<

%.o: $(RTS_BASE)/adainclude/%.ads
	$(CROSS)gnatmake -gnata -a -c --RTS=$(RTS_BASE) -o $@ $(CFLAGS) $<

clean:
	$(RM) -f *.o *~ b~* $(PRG) *.ali *.map *.owav

force:

