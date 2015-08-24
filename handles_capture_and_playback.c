#include "interface.h"
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <stdio.h> 
#include <stdlib.h>
#include <math.h> 
#include <errno.h>
#include <poll.h>
#include <alsa/asoundlib.h>
#include <snd_defs.h>

//setup the handles

int configure_alsa_audio(snd_pcm_t *device, int channels){
    
    		snd_pcm_hw_params_t 	*hw_params;
    		int err;
    		int tmp;
    		snd_pcm_uframes_t  frames;
        	int 		   fragments = 2;
	
		 /*alocate memory for hardware parameter structure*/
  		if ((err = snd_hw_params_malloc(&hw_params)) < 0) {
 		   	 fprintf(stderr, "cannot allocate parameter structure (%s)\n", snd_strerror(err));
    	
     			return 1;
   		}

  		//Fill structure from current audio parameters
 		if ((err = snd_pcm_hw_params_any(device, hw_params)) < 0){
    			fprintf(stderr, "can not initialize parameter structure (%s)\n", snd_strerror(err));
    
               		return 1;
 		}

 		//set access type, sample rate sample format, channels
 		if ((err = snd_pcm_hw_params_set_access(device, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
   			 fprintf(stderr, "can not set access type: (%s)\n", snd_strerror(err));
  
    		 	 return 1;
   		}	

 		//bits = 16
  		if ((err = snd_pcm_hw_params_set_format(device, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
     			fprintf (stderr, "cannot set sample format: (%s)\n", snd_strerror(err));
     
     	     		return 1;
   		 }	

  	
		tmp = sample_rate;
  
		if ((err = snd_pcm_hw_params_set_rate_near(device, hw_params, &tmp, 0)) < 0) {
   			fprintf(stderr, "cannot set sample rate: (%s)\n", snd_strerror(err));
   
   			 return 1;
 		}

 		if (tmp != sample_rate) {
    			fprintf(stderr, "could not set requested sample rate, asked for %d got %d\n", sample_rate, tmp);
    			sample_rate = tmp;    
 		}

 		if ((err = snd_pcm_hw_params_set_channels(device, hw_params, channels)) < 0) {
    			fprintf (stderr, "can not set channel count: %s\n", snd_strerror(err));
   	   		
			return 1;
	 	}
	
		/*(Fragments are the number of equal-sized chunks the full audio buffer gets divided to, and alsa calls them periods.)*/ 	
 
    		if ((err = snd_pcm_hw_params_set_periods_near(device, hw_params, &fragments, 0)) < 0) {
        		fprintf(stderr, "Error setting # fragments to %d: %s\n", fragments, snd_strerror(err));
      	 		return 1;
    		}
		
		/*convert the buffer_size variable which holds the size of a fragment, 
		It need to be converted to frames, which are one sample data for all chanels involved, 
		If we are in 16-bit 2-channel operation, a frame is 4 bytes wide. 
		So, I multiply the frames with the number of fragments supported by hardware, and hope that the hardware can give me that buffer size*/

    		frame_size = channels * (bits / 8);
    		frames = buffer_size / frame_size * fragments;
	    
		if ((err = snd_pcm_hw_params_set_buffer_size_near(device, hw_params, &frames)) < 0) {
	        	fprintf(stderr, "Error setting buffer_size %d frames: %s\n", frames, snd_strerror(err));
		
		        return 1;
	 	}

	 	if (buffer_size != frames * frame_size / fragments) {
	        	fprintf(stderr, "Could not set requested buffer size, asked for %d got %d\n", buffer_size, frames * frame_size / fragments);
	        	buffer_size = frames * frame_size / fragments;
	 	}
	  
	   	/*Setting the parameters:*/
	 	if ((err = snd_pcm_hw_params(device, hw_params)) < 0) {
	        	fprintf(stderr, "Error setting HW params: %s\n",
	                snd_strerror(err));
	        
			return 1;
	  	}
		return 0;
}