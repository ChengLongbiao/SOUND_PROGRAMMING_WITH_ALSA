#include "interface.h"
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <stdio.h> 
#include <stdlib.h>
#include <math.h> 
#include <errno.h>
#include <poll.h>
#include <alsa/asoundlib.h>


PANPOS constpower_pan(double position){
	PANPOS pos;
	/* pi/2: 1/4 cycle of sinusoid*/

	const double pi_over_two = 4.0 * atan(1.0) * 0.5;
	const double root_two_over_two = sqrt(2.0) * 0.5;
	/*Scale the position to fit the pi/2 range*/
	double thispos = position * pi_over_two;
	/*each channel uses a 1/4 of a cycle */
	double angle = thispos * 0.5;
	
	pos.left = root_two_over_two * (cos(angle) - sin(angle));
	pos.right = root_two_over_two * (cos(angle) + sin(angle));
	return pos;
}


		
		
