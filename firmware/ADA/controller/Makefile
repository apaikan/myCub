
# PREFIX is the usual prefix for GNAT commands
PREFIX=

PART=LM4F120H5QR
CPU=-mcpu=cortex-m4
FPU=-mfpu=fpv4-sp-d16 -mfloat-abi=softfp
LM4F_ROOT=../ravenscar/lm4f120
#CFLAGS?=-Os -g -Wall -Werror -ffunction-sections -fdata-sections -MD -std=c99  -mthumb ${CPU} ${FPU} \
  -I$(LM4F_ROOT) \
  -DPART_$(PART) -DTARGET_IS_BLIZZARD_RA1 -Dgcc

CFLAGS?=-Os -g -Wall -ffunction-sections -fdata-sections -MD -std=c99  -mthumb ${CPU} ${FPU} \
  -I$(LM4F_ROOT) \
  -DPART_$(PART) -DTARGET_IS_BLIZZARD_RA1 -Dgcc



all: peripherals
	$(PREFIX)arm-none-eabi-gnatmake -P controller.gpr
	$(PREFIX)arm-none-eabi-objcopy -Obinary main.out main.bin
	$(PREFIX)arm-none-eabi-size main.out

peripherals:
	arm-none-eabi-gcc -c ./src_c/servo.c -o build/servo.o $(CFLAGS)
	arm-none-eabi-gcc -c ./src_c/uart.c -o build/uart.o $(CFLAGS)
	arm-none-eabi-ar rcs ./build/libperipherals.a build/servo.o build/uart.o

flash:
	sudo lm4flash main.bin

clean:
	$(PREFIX)arm-none-eabi-gnatclean -P controller.gpr
	rm -f main.bin
	rm -f build/*.*

