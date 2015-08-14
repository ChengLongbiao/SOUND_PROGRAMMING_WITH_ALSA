/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <stdio.h> 
#include <stdlib.h>
#include <math.h> 
#include <errno.h>
#include <poll.h>
#include <alsa/asoundlib.h>

//We need function that, a given parameter (in the range -1...+1), returns two values, representing the amplitude scaling factors(range 0..1)
// for the left and right output channels.
typedef struct panpos{
	double left;
	double right;
}PANPOS;


PANPOS sinplepan(double position);// we can define the a panning function that return a PANPOS object:
void smbFft(float *fftBuffer, long fftFrameSize, long sign);
double smbAtan2(double x, double y);
int configure_alsa_audio(snd_pcm_t *device, int channels);
void smbPitchShift(float pitchShift, long numSampsToProcess, long fftFrameSize, long osamp, float sampleRate, float *indata, float *outdata);