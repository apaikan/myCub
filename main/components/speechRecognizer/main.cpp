/*
 *  Yarp Modules Manager
 *  Copyright: (C) 2010 RobotCub Consortium
 *				Italian Institute of Technology (IIT)
 *				Via Morego 30, 16163, 
 *				Genova, Italy
 * 
 *  Copy Policy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *  Authors: Ali Paikan <ali.paikan@iit.it>
 *
 */


#include <stdio.h>
#include <string>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <string>

#include <yarp/os/all.h>
#include <yarp/os/Log.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Bottle.h>

#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>
#include <sphinxbase/cont_ad.h>
#include <pocketsphinx.h>

using namespace yarp::os;
using namespace std;

class SpeechRecognizer: public RFModule {
public:

    SpeechRecognizer(void) {
        ps = NULL;
        config = NULL;
        cont = NULL;
        ad = NULL;
    }

    bool configure(ResourceFinder &rf) {

        string lm = rf.check("lm", Value("speech.lm")).asString();
        string dict = rf.check("dic", Value("speech.dic")).asString();
        string adcdev = rf.check("adcdev", Value("")).asString();
        int samprate = rf.check("samprate", Value(16000)).asInt();
        string str_samprate = rf.check("samprate", Value("16000")).asString();
        string str_nfft = rf.check("nfft", Value("512")).asString();

        config = cmd_ln_init(NULL, ps_args(), TRUE,
                     "-lm", lm.c_str(),
                     "-dict", dict.c_str(),
                     "-nfft", str_nfft.c_str(),
                     "-samprate", str_samprate.c_str(),
                     NULL);   

        if(config == NULL) return false;
	    
        ps = ps_init(config);
	    if(ps == NULL) return false;

        const char* padcdev = (adcdev.size() > 0) ? adcdev.c_str() : NULL;
        if((ad = ad_open_dev(padcdev, samprate)) == NULL)
        {
            printf("Failed to open audio device\n");
            return false;
        }    

        /* Initialize continuous listening module */
        if((cont = cont_ad_init(ad, ad_read)) == NULL) {
            printf("Failed to initialize voice activity detection\n");
            return false;
        }    
        
        if(ad_start_rec(ad) < 0) {
            printf("Failed to start recording\n");
            return false;
        }    
        if(cont_ad_calib(cont) < 0) {
            printf("Failed to calibrate voice activity detection\n");
            return false;
        }     
        //rewind(rawfd);

        bool ret = speechPort.open("/SpeechRecognizer/words:o");
        shouldStop = !ret;
        return ret;
    }

    double getPeriod() {
        //makeRealTime();
        return 0.1; // run every 100ms 
    }
    
    bool updateModule() {
        printf("READY....\n");
        fflush(stdout);

        /* Wait data for next utterance */
        while ((k = cont_ad_read(cont, adbuf, 4096)) == 0)
            sleep_msec(100);

        if (k < 0) {
            printf("Failed to read audio\n");
            return false;
        }    

        /*
         * Non-zero amount of data received; start recognition of new utterance.
         * NULL argument to uttproc_begin_utt => automatic generation of utterance-id.
         */
        if (ps_start_utt(ps, NULL) < 0) {
            printf("Failed to start utterance\n");
            return false;
         }   

        ps_process_raw(ps, adbuf, k, FALSE, FALSE);
        printf("Listening...\n");
        fflush(stdout); 

       /* Note timestamp for this first block of data */
        ts = cont->read_ts;

        /* Decode utterance until end (marked by a "long" silence, >1sec) */
        while(!shouldStop) {
            /* Read non-silence audio data, if any, from continuous listening module */
            if ((k = cont_ad_read(cont, adbuf, 4096)) < 0) {            
                printf("Failed to read audio\n");
                return false;
            }    
            if (k == 0) {
                /*
                 * No speech data available; check current timestamp with most recent
                 * speech to see if more than 1 sec elapsed.  If so, end of utterance.
                 */
                if ((cont->read_ts - ts) > DEFAULT_SAMPLES_PER_SEC)
                    break;
            }
            else
                ts = cont->read_ts; /* New speech data received; note current timestamp */

            /*
             * Decode whatever data was read above.
             */
            rem = ps_process_raw(ps, adbuf, k, FALSE, FALSE);

            /* If no work to be done, sleep a bit */
            if ((rem == 0) && (k == 0))
                sleep_msec(20);
        }

        /*
         * Utterance ended; flush any accumulated, unprocessed A/D data and stop
         * listening until current utterance completely decoded
         */
        ad_stop_rec(ad);
        while ((ad_read(ad, adbuf, 4096) >= 0) && !shouldStop);
        cont_ad_reset(cont);

        printf("Stopped listening, please wait...\n"); fflush(stdout);
        /* Finish decoding, obtain and print result */
        ps_end_utt(ps);
        hyp = ps_get_hyp(ps, NULL, &uttid);
        printf("%s: %s\n", uttid, hyp);
        fflush(stdout);
        
        if(strlen(hyp) > 0) {
            Bottle& wb = speechPort.prepare();
            wb.clear();
            wb.addString(hyp);
            speechPort.write();
        }
        /* Exit if the first word spoken was GOODBYE */
        //if (hyp) {
        //    sscanf(hyp, "%s", word);
        //    if (strcmp(word, "goodbye") == 0)
        //        break;
        //}

        /* Resume A/D recording for next utterance */
        if (ad_start_rec(ad) < 0) {        
            printf("Failed to start recording\n");
            return false;
        }

        return true; 
    }

    bool interruptModule() { 
        shouldStop = true;
        return true; 
    }

    bool close() {
        speechPort.close(); 
        if(cont)
            cont_ad_close(cont);
        if(ps)
           ps_free(ps); 
        return true; 
    } 

    void makeRealTime() {
	    if(!bIsRT) {
	        struct sched_param thread_param;
    	    thread_param.sched_priority = 99;
   	        pthread_setschedparam(pthread_self(), SCHED_FIFO, &thread_param);
            bIsRT = true;
	    }
    }


private:
    void sleep_msec(int32 ms) {
        struct timeval tmo;
        tmo.tv_sec = 0;
        tmo.tv_usec = ms * 1000;
        select(0, NULL, NULL, NULL, &tmo);
    }


private:
    BufferedPort<Bottle> speechPort;
    bool shouldStop; 
    bool bIsRT;

	ps_decoder_t *ps;
	cmd_ln_t *config;    
    ad_rec_t *ad;
    int16 adbuf[4096];
    int32 k, ts, rem;
    char const *hyp;
    char const *uttid;
    cont_ad_t *cont;
    char word[256];
};



int main(int argc, char* argv[])
{
    yarp::os::Network yarp;
    ResourceFinder rf;
    //rf.configure(argc, argv);
	//rf.setDefaultConfigFile("face_detector.ini");
    //rf.setDefaultContext("faceDetector/conf");
    rf.configure("MYCUB_ROOT", argc, argv);

    SpeechRecognizer module;
    if(!module.configure(rf)) {
        fprintf(stderr, "Error opening SpeechRecognizer\n");
        return -1;
    }

    printf("Press Ctrl+C to exit...\n"); fflush(stdout);
    module.runModule(); 

    return 0;
}

