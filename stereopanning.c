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
	
	
	position *= 0.5;
	pos.left  = position - 0.5;
	pos.right = position + 0.5;
	
	return pos;
}


		
		
