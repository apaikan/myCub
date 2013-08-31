/****************************************************************************
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/


/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include <mycub_interface.h>

 
#include <termios.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <fcntl.h>

#include <apps/readline.h>
  
/****************************************************************************
 * Public Functions
 ****************************************************************************/
#define USEC_PER_MSEC 1000
#define MSEC_PER_SEC  1000
#define USEC_PER_SEC  (USEC_PER_MSEC * MSEC_PER_SEC)
#define SHORT_DELAY   (USEC_PER_SEC / 3)


#define FRONT_ARM       RIGHT_JOINT
#define BACK_ARM        LEFT_JOINT
#define RIGHT_ARM       FRONT_JOINT
#define LEFT_ARM        BACK_JOINT

#define MAX_CMD_NUM     16
#define MAX_CMD_LEN     32


extern "C"
{

static char mycub_cmd[MAX_CMD_NUM][MAX_CMD_LEN];

void status_serial(long int fd) 
{
    /*
	int status;
	unsigned int arg;
	status = ioctl(fd, TIOCMGET, &arg);
	fprintf(stderr, "[STATUS]: ");
	if(arg & TIOCM_RTS) fprintf(stderr, "RTS ");
	if(arg & TIOCM_CTS) fprintf(stderr, "CTS ");
	if(arg & TIOCM_DSR) fprintf(stderr, "DSR ");
	if(arg & TIOCM_CAR) fprintf(stderr, "DCD ");
	if(arg & TIOCM_DTR) fprintf(stderr, "DTR ");
	if(arg & TIOCM_RNG) fprintf(stderr, "RI ");
	fprintf(stderr, "\r\n");
    */
}



int init_serial(const char* devicename, int baudrate)
{
	int comfd;
	struct termios oldtio, newtio;       //place for old and new port settings for serial port
	struct termios oldkey, newkey;       //place tor old and new port settings for keyboard teletype

	comfd = open(devicename, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (comfd < 0)
	{
		perror(devicename);
		return comfd;
	}

    tcgetattr(comfd, &oldtio); // save current port settings 
	newtio.c_cflag = baudrate | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0;
	newtio.c_lflag = 0;
	newtio.c_cc[VMIN]=1;
	newtio.c_cc[VTIME]=0;
	tcflush(comfd, TCIFLUSH);
	tcsetattr(comfd,TCSANOW,&newtio);
    
    status_serial(comfd);
    return comfd;
}


inline void write_serial(int outfd, char ch)
{
  char buffer = ch;
  ssize_t nwritten;
  do
    {

      nwritten = write(outfd, &buffer, 1);
      if (nwritten < 0 && errno != EINTR)
        {
          break;
        }
    }
  while (nwritten < 1);
}

inline char read_serial(int infd)
{
  char buffer;
  ssize_t nread;

  do 
  {
      nread = read(infd, &buffer, 1);
      if (nread == 0)
        {
          return EOF;
        }

      else if (nread < 0)
        {
          int errcode = errno;
          if (errcode != EINTR)
              return EOF;
        }
    }
  while (nread < 1);

  return buffer;
}


void ack_serial(int comfd, char* msg) 
{
    for(int i=0; i<(int)strlen(msg); i++)
        write_serial(comfd, msg[i]);
}

size_t readline_serial(int comfd, char* buff, size_t maxlen, char* prompt) 
{
    char ch;
    size_t len = 0;

    write_serial(comfd, '\n');
    if(prompt)
    {
        for(int i=0; i<(int)strlen(prompt); i++)
            write_serial(comfd, prompt[i]);
    }

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(comfd, &fds);

    do {

        int ret = select(comfd+1, &fds, NULL, NULL, NULL);
        if(ret == -1)
        {
            perror("select");
            buff[len] = '\0';
            return len;
        }        
        
        if(FD_ISSET(comfd, &fds)) 
        {
            ch = read_serial(comfd);
            write_serial(comfd, ch);            
            if(isprint(ch))
                buff[len++] = ch;
        }

    } while ((ch != '\n') && (ch != '\r') && (len < maxlen));
    
    buff[len] = '\0';
    return len;
}

size_t parse_string(char* str)
{
    char* token = strtok(str, " ");
    int i = 0;
    while((token != NULL) && (i<MAX_CMD_NUM))
    {
        strcpy(mycub_cmd[i++], token);
        token = strtok(NULL, " ");
    }
    return i;
}

bool should_stop;
void siguser_action(int signo, siginfo_t *siginfo, void *arg)
{
    if(signo == SIGUSR1)
    {
        printf("SIGUSR1 received\n");
        should_stop = true;
    }
}


int myCub_main(int argc, char *argv[])
{
    struct sigaction act;
    struct sigaction oact;
    int status;

    memset(&act, 0, sizeof(struct sigaction));
    act.sa_sigaction = siguser_action;
    act.sa_flags     = SA_SIGINFO;
    (void)sigemptyset(&act.sa_mask);
    status = sigaction(SIGUSR1, &act, &oact);
    if(status != 0)
    {
        fprintf(stderr, "Failed to install SIGUSR1 handler, errno=%d\n", errno);
        exit(2);
    }

    printf("-- myCub Intreface --\n");  
    int comfd = init_serial("/dev/ttyS1", B115200);
    if(comfd < 0 )
    {
        printf("Error in intializing serial port %s.\n", "/dev/ttyS1");
        return 0;
    }

    MyCubInterface mycub;
    mycub.init();

    char buf[256];
    while(!should_stop) 
    {
        ssize_t len = readline_serial(comfd, buf, 256, "[myCub] ");
        if(len > 0)
        {
            buf[len] = '\0';
            int n = parse_string(buf);            
            //for (int i=0; i<n; i++)
            //    printf("command: [%s]\n", mycub_cmd[i]);
            
            // executing commands
            if(strcmp(mycub_cmd[0], "setPose")==0 ) {
                if(n >= 3)
                {
                    mycub.setPose(atoi(mycub_cmd[1]), atoi(mycub_cmd[2]));
                    ack_serial(comfd, "[ok]");
                }
                else
                    ack_serial(comfd, "[error]");
            }
            else if(strcmp(mycub_cmd[0], "getPose")==0 ) {
                if(n >= 2)
                {
                    int pos = mycub.getPose(atoi(mycub_cmd[1]));
                    sprintf(buf, "%d", pos);
                    ack_serial(comfd, buf);
                }
                else
                    ack_serial(comfd, "[error]");
            }
            else if(strcmp(mycub_cmd[0], "getDistance")==0 ) {
                if(n >= 2)
                {
                    int dist = mycub.getDistance(atoi(mycub_cmd[1]));
                    sprintf(buf, "%d", dist);
                    ack_serial(comfd, buf);
                }
                else
                    ack_serial(comfd, "[error]");
            }


            else if(strcmp(mycub_cmd[0], "gotoPose")==0 ) {
                if(n >= 3)
                {
                    double t = 0.0;
                    if(n>=4)
                        t = atoi(mycub_cmd[3]) / 1000.0;
                    mycub.gotoPose(atoi(mycub_cmd[1]), atoi(mycub_cmd[2]), t);
                    ack_serial(comfd, "[ok]");
                }
                else
                    ack_serial(comfd, "[error]");
            }

            else if(strcmp(mycub_cmd[0], "gotoPoseSync")==0 ) {
                if(n >= 3)
                {
                    double t = 0.0;
                    if(n>=4)
                        t = atoi(mycub_cmd[3]) / 1000.0;
                    mycub.gotoPoseSync(atoi(mycub_cmd[1]), atoi(mycub_cmd[2]), t);
                    ack_serial(comfd, "[ok]");
                }
                else
                    ack_serial(comfd, "[error]");
            }            
            else if(strcmp(mycub_cmd[0], "runDemo")==0 ) {
                JointContext ctx = mycub.getContext();
                mycub.gotoPose(FRONT_ARM, 160, 0.5);
                mycub.gotoPose(BACK_ARM, 100, 0.5);
                mycub.gotoPose(LEFT_ARM, 120, 0.5);
                mycub.gotoPose(RIGHT_ARM, 120, 0.5);
                while(!mycub.checkMotionDone()) sleep(1);
                
                should_stop = false;
                while(!should_stop)
                {
                    int dist = mycub.getDistance(RIGHT_RANGE);
                    if(dist == 0)
                        printf("No obstacle in the range\n");
                    else
                        printf("Obstacle at distance %d\n", dist);

                    if((dist == 0) || (dist > 150))
                    {
                        mycub.gotoPoseSync(BACK_ARM, 100, 0.2);
                        mycub.gotoPoseSync(FRONT_ARM, 8, 1.0);
                        
                        mycub.gotoPose(LEFT_ARM, 70, 0.2);
                        mycub.gotoPose(RIGHT_ARM, 70, 0.2);        
                        while(!mycub.checkMotionDone()) sleep(1);

                        mycub.gotoPoseSync(FRONT_ARM, 160, 1.0);
                        
                        mycub.gotoPoseSync(BACK_ARM, 80, 0.2);
                        mycub.gotoPose(LEFT_ARM, 120, 0.2);
                        mycub.gotoPose(RIGHT_ARM, 120, 0.2);        
                        while(!mycub.checkMotionDone()) sleep(1);
                    }
                    else
                        sleep(1);   //wait a bit
                 }
            }
        }
        //usleep(100000);
	}

	//tcsetattr(comfd,TCSANOW,&oldtio);
	close(comfd);
    mycub.fini();
    printf("done!\n");


    /*
     */
        
    return 0;
}

}
