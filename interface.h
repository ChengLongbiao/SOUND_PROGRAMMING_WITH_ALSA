/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <stdio.h> 
#include <stdlib.h>
#include <math.h> 
#include <errno.h>
#include <poll.h>
#include <alsa/asoundlib.h>
#include <snd_defs.h>

enum {ARG_PROGNAME, ARG_INFILE, ARGG_OUTFILE, ARG_PANPOS, ARG_NARGS};
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
float delay(float *sig, float dtime, float *del, int *p, int vecsize, float sr);
float comb(float *sig, float dtime, float gain,float *delay, int *p, int vecsize, float sr);
float allpass(float *sig, float dtime, float gain,float *delay, int *p, int vecsize, float sr);
float vdelay(float *sig, float vdtime, float maxdel,float *delay, int *p, int vecsize, float sr);
float flanger(float *sig, float vdtime, float fdb, float maxdel, float *delay, int *p, int vecsize, float sr);
float fir(float *sig, float *imp, float *del, int length, int *p, int vecsize, float sr);
