
PART=TM4C123GH6PM
CPU=-mcpu=cortex-m4

CFLAGS=-ffunction-sections \
        -fdata-sections     \
        -MD                 \
        -Wall               \
        -pedantic           \
        -DPART_$(PART) -DTARGET_IS_BLIZZARD_RB1 -Dgcc -DDEPRECATED 
 

all:
	/usr/local/gnat/bin/g++ -c -fdump-ada-spec $(CFLAGS) *.h

clean:
	rm -f *.s *.ads *.d

