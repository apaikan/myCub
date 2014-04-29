/*
 * dispd.c Nokia LCD 5110 driver for the RaspberryPi
 * Copyright (c) 2014 Ali Paikan <ali.paikan@gmail.com>
 *
 * This program provides very similar functionality to servoblaster, except
 * that rather than implementing it as a kernel module, servod implements
 * the functionality as a usr space daemon.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <math.h>

#include <string> 
#include <vector> 

#include <wiringPi.h>
#include "PCD8544.h"


#define DEVFILE			"/dev/display"

// pin setup
int _din = 1;
int _sclk = 0;
int _dc = 2;
int _rst = 4;
int _cs = 3;

int _bk = 7;

// lcd contrast 
int contrast = 60;

// battery 
int battery_level = 0; 
std::vector<std::string> linebuff;

/*
static void udelay(int us)
{
	struct timespec ts = { 0, us * 1000 };

	nanosleep(&ts, NULL);
}*/

static void terminate(int dummy)
{	
    digitalWrite(_bk, LOW);
	unlink(DEVFILE);
	exit(1);
}

static void fatal(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	terminate(0);
}

static void setup_sighandlers(void)
{
	int i;
	// Catch all signals possible - it is vital we kill the DMA engine
	// on process exit!
	for (i = 0; i < 64; i++) {
		struct sigaction sa;
		memset(&sa, 0, sizeof(sa));
		sa.sa_handler = terminate;
		sigaction(i, &sa, NULL);
	}
}

void update_display()
{
    // update display
    LCDclear();
    for(int i=0; i<linebuff.size(); i++)                         
        LCDdrawstring(0, i*8, (char*)linebuff[i].c_str());

     // drawing buttom line    
     for (int i=0; i<83; i+=2)
        LCDdrawline(i, 40, i, 40, BLACK);
     // drawing battery
     LCDdrawrect(70, 42, 12, 5, BLACK);
     LCDdrawrect(81, 43, 3, 3, BLACK);
     // battery charge state     
     LCDfillrect(70, 43, battery_level/10+1, 4, BLACK); 

}


static void go_go_go(void)
{
	int fd;
	struct timeval tv;
	static char line[128];
	int nchars = 0;
    int ntimes_out = 0;
    int ntimes_reset = 0;
	if ((fd = open(DEVFILE, O_RDWR|O_NONBLOCK)) == -1)
		fatal("dispd: Failed to open %s: %m\n", DEVFILE);

	for (;;) {
		int n, width, servo;
		fd_set ifds;
		char width_arg[64];

		FD_ZERO(&ifds);
		FD_SET(fd, &ifds);
        tv.tv_sec = 1;
        tv.tv_usec = 0;
		if ((n = select(fd+1, &ifds, NULL, NULL, &tv)) != 1)
        {
            // turn off backlight after 5 sec
            if(++ntimes_out > 5)
            {
                digitalWrite(_bk, LOW);
                delay(10);
            }
            if(++ntimes_reset > 60)
            {
                ntimes_reset = 0;
                LCDInit(_sclk, _din, _dc, _cs, _rst, contrast);
                //for(int i=0; i<linebuff.size(); i++)                         
                //    LCDdrawstring(0, i*8, (char*)linebuff[i].c_str());
                LCDdisplay();
                delay(10);
            }
			continue;
        }   
		while (read(fd, line+nchars, 1) == 1) {
			if (line[nchars] == '\n') 
            {
				line[nchars] = '\0';
				nchars = 0;
                std::string str = line;

                // do whatever
                if(strcmp(line,"%clear%") == 0)                
                {
                    linebuff.clear();
                    update_display();
                }   
                else if(strcmp(line,"%logo%") == 0)
                {
                    linebuff.clear();
                    LCDclear();
                    LCDshowLogo();
                }
                else if(str.substr(0,5) == "%bat%")
                {
                    std::string val = str.substr(6, str.npos);
                    battery_level = atoi(val.c_str());
                    update_display();
                }
                else
                {
                    int start = 0;  
                    int size = 14;  
                    while (start < str.size()) 
                    {  
                        int end = ((start + size) < str.size()) ? size : str.size() - start;                        
                        if(linebuff.size()>4)
                            linebuff.erase(linebuff.begin());	                  
                        linebuff.push_back(str.substr(start, end));
                        start += size;  
                    } 
                    update_display();
                }
                ntimes_out = 0;
                digitalWrite(_bk, HIGH);
                LCDdisplay();
                delay(10);
			} 
            else 
            {
				if (++nchars >= 126) {
					fprintf(stderr, "Input too long\n");
					nchars = 0;
				}
			}
		} // end while
	} // end for
}

int main(int argc, char **argv)
{
	int i;
   setvbuf(stdout, NULL, _IOLBF, 0);

	setup_sighandlers();

    // check wiringPi setup
    if (wiringPiSetup() == -1)
    {
        printf("wiringPi-Error\n");
        exit(1);
    }

    // init and clear lcd
    pinMode(_bk, OUTPUT);
    digitalWrite(_bk, HIGH);

	unlink(DEVFILE);
	if (mkfifo(DEVFILE, 0666) < 0)
		fatal("dispd: Failed to create %s: %m\n", DEVFILE);
	if (chmod(DEVFILE, 0666) < 0)
		fatal("didpd: Failed to set permissions on %s: %m\n", DEVFILE);

	if (daemon(0,1) < 0)
		fatal("dispd: Failed to daemonize process: %m\n");

    LCDInit(_sclk, _din, _dc, _cs, _rst, contrast);
    LCDclear();
    LCDshowLogo();
    delay(2000);

	go_go_go();

	return 0;
}

