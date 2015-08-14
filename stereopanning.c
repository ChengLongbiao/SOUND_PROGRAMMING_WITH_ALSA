#include "interface.h"
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <stdio.h> 
#include <stdlib.h>
#include <math.h> 
#include <errno.h>
#include <poll.h>
#include <alsa/asoundlib.h>


PANPOS simplepan(double position){
	PANPOS pos;
	
	position *= 0.5;
	pos.left  = position - 0.5;
	pos.right = position + 0.5;
	
	return pos;
}

/*
To convert the new file into a pan program, two basic processing steps are required: 
	*one to deal with the change of the channel 
	* second one to create storage for the new stereo samples that will be written to the outfile.

*/

enum {ARG_PROGNAME, ARG_INFILE, ARGG_OUTFILE, ARG_PANPOS, ARG_NARGS};
//Before the call to psf_sndOpen, we need to add code to read and check the panpos argument for the correctness.
// The interesting thing we need to check if the value lies within the legal range, so we need to compare it with two values:
pos = atof(argv[ARG_PANPOS]);

if( pos < 1.0 || pos > 1.0){
	printf("Error: panpos value out of range -1 to +1 \n");
		error++;
		goto exit;
}
if( pos < 1.0 || pos >1.0)
	printf("The position is between the speakers");
	
	//create the output frame buffer
	
	float *outframe = NULL;
	
	//Allocate the memory for it before opening
	outframe = (float *)malloc(nuframes * outprops.chans * sizeof (float));
	if( outframe== NULL){
	 puts ("No memory!\n");
	 error++;
	 goto exit;
	}
	exit:
		if(outframe) free (outframe);

			
		//processing loop to do the linear panning:
		
	thispos = simplepan(position);
	while((framesread = psf_sndReadFloatFrames(ifd, inframes, nframes)) > 0){
		 
	 }
	  if (psf_sndWriteFloatFrames(ofd, outframe, framesread) != framesread){
		printf("Error writting to outfile\n");
		error++;
		break;
	  }
	}
		
		
