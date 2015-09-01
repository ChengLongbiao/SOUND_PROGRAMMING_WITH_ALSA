/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <stdio.h> 
#include <stdlib.h>
#include <math.h> 
#include <errno.h>
#include <poll.h>
#include <alsa/asoundlib.h>
#include <memory.h>
#include <sndfile.h>

/** default signal vectorsize.
*/
const int   def_vsize = 64;
/** default sampling rate
*/
const float def_sr = 44100.f;
/** default control rate
*/
const float def_cr = def_sr/def_vsize;
/** default audio channels.
*/
const int   def_chans = 1;
/** default function table length
*/
const int   def_len = 1024;

/** butterworth modes 
*/

/** the pi definition. 
*/ 
const double pi = 4*atan(1.); 


enum {ARG_PROGNAME, ARG_INFILE, ARGG_OUTFILE, ARG_PANPOS, ARG_NARGS, BP,BR,LP,HP };
//We need function that, a given parameter (in the range -1...+1), returns two values, representing the amplitude scaling factors(range 0..1)
// for the left and right output channels.
typedef struct panpos{
	double left;
	double right;
}PANPOS;
 
int configure_alsa_audio(snd_pcm_t *device, int channels);
PANPOS constpower_pan(double position);// we can define the function that return a PANPOS object:
void smbFft(float *fftBuffer, long fftFrameSize, long sign);
double smbAtan2(double x, double y);
int configure_alsa_audio(snd_pcm_t *device, int channels);
void smbPitchShift(float pitchShift, long numSampsToProcess, long fftFrameSize, long osamp, float sampleRate, float *indata, float *outdata);
float lowpass(float* sig, float freq, float *del, int vecsize=def_vsize, 
			  float sr=def_sr);

/** 1st order highpass filter
    
	 sig: signal buffer \n
	 freq: cutoff frequency \n
	 del: delay memory (1 sample) \n
	 vecsize: vector size \n
	 sr: sampling rate \n
	 \n
	 returns: first vector sample
*/
float highpass(float* sig, float freq, float *del, int vecsize=def_vsize, 
			   float sr=def_sr);

/** 2nd order all-pole filter (resonating bp)
    
	 sig: signal buffer \n
	 freq: cutoff frequency \n
	 bw: bandwidth \n
	 del: delay memory (2 samples) \n
	 vecsize: vector size \n
	 sr: sampling rate \n
	 \n
	 returns: first vector sample
*/

float bandpass(float* sig, float freq, float bw, float *del, 
			   int vecsize=def_vsize,float sr=def_sr);

/** 2nd order butterworth filter
    
	 sig: signal buffer \n
	 freq: cutoff frequency \n
	 bw: bandwidth \n
	 del: delay memory (2 samples) \n
	 mode: either BP, BR, LP or HP
	 vecsize: vector size \n
	 sr: sampling rate \n
	 \n
	 returns: first vector sample
*/

float  balance(float *sig, float *cmp, float *del, float freq=10.f, 
			   int vecsize=def_vsize, float sr=def_sr);

/** fixed delay line

	 sig: signal buffer \n
	 dtime: delay time (s) \n
	 delay: delay memory (enough samples for delay length) \n
     pnt: external memory for circular buffer pointer
	 vecsize: vector size \n
	 sr: sampling rate \n
	 \n
	 returns: first vector sample

*/
float delay(float *sig, float dtime,
			float *delay, int *p, int vecsize=def_vsize, float sr=def_sr);

/** comb filter

	 sig: signal buffer \n
	 dtime: delay time (s) \n
	 gain: feedback gain  \n
	 delay: delay memory (enough samples for delay length) \n
     pnt: external memory for circular buffer pointer  \n
	 vecsize: vector size \n 
	 sr: sampling rate \n
	 \n
	 returns: first vector sample

*/
float comb(float *sig, float dtime, float gain,
			float *delay, int *p, int vecsize=def_vsize, float sr=def_sr);

/** allpass filter

	 sig: signal buffer
	 dtime: delay time (s)
	 gain: feedback/feedforward gain (allpass coefficient)
	 delay: delay memory (enough samples for delay length)
     pnt: external memory for circular buffer pointer
	 vecsize: vector size \n
	 sr: sampling rate \n
	 \n
	 returns: first vector sample

*/
float allpass(float *sig, float dtime, float gain,
			float *delay, int *p, int vecsize=def_vsize, float sr=def_sr);


/** variable delay line

	 sig: signal buffer  \n
	 vdtime: variable delay time (s) \n
	 maxdel: max delay time (s) \n
	 delay: delay memory (enough samples for maxdelay length) \n
     pnt: external memory for circular buffer pointer \n
	 vecsize: vector size \n
	 sr: sampling rate \n
	 \n
	 returns: first vector sample

*/
float vdelay(float *sig, float vdtime, float maxdel,
			float *delay, int *p,int vecsize=def_vsize, float sr=def_sr);

/** variable delay line with feedback

	 sig: signal buffer \n
	 vdtime: variable delay time (s) \n
	 fdb: feedback gain \n
	 maxdel: max delay time (s) \n
	 delay: delay memory (enough samples for maxdelay length) \n
     pnt: external memory for circular buffer pointer \n
	 vecsize: vector size \n
	 sr: sampling rate \n
	 \n
	 returns: first vector sample

*/
float flanger(float *sig, float vdtime, float fdb, float maxdel,
			float *delay, int *p, int vecsize=def_vsize, float sr=def_sr);


/**  convolution

	 sig: signal buffer \n
	 imp: impulse response buffer \n
         del: delay memory (enough samples for impulse length) \n
	 length: size of impulse response \n
         pnt: external memory for circular buffer pointer \n
	 vecsize: vector size \n
	 sr: sampling rate \n
	 \n
	 returns: first vector sample

*/