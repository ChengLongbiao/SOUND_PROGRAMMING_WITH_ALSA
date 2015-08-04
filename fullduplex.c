/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <stdio.h> 
#include <stdlib.h>
#include <math.h> 
#include <errno.h>
#include <poll.h>
#include <alsa/asoundlib.h>


int main() {
/*you locate these in a heap */
	int 		restarting;
	int		nchannels = 2;
	int		buffer_size = 512;
	int 		sample_rate = 48000;
	int 		bits = 16;
	 int 		err;
  	int 		frame_size;

	char		*snd_device_in = "plughw:2,0";
	char 		*snd_device_out = "plughw:2,0";
	snd_pcm_t	*playback_handle;
	snd_pcm_t 	*capture_handle;

	//Set the device for capture and playback

      if ((err = snd_pcm_open(&playback_handle, snd_device_out, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
         	 fprintf(stderr, "cannot open input audio device %s: %s\n", snd_device_in, snd_strerror(err));
         
    	 exit(1);
      }

     if ((err = snd_pcm_open(&capture_handle, snd_device_in, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        	  fprintf(stderr, "cannot open input audio device %s: %s\n", snd_device_out, snd_strerror(err));
        
    	exit(1);
     } 

     int configure_alsa_audio(snd_device_in,  nchannels);
     int configure_alsa_audio(snd_device_out, nchannels);

	restarting = 1;

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
	}

		// preocess the data

		int  inframes, outframes;
  		char *rdbuf;
  		int i;
  	
		rdbuf = (char *)malloc(frame_size);
		long semitones = 3; // shift up by 3 semitones
		float pitchShift = pow(2., semitones/12.);// convert semitones to factor

    		while (1) {
        		frame_size = channels * (bits / 8);
        		frames = buffer_size / frame_size;

        		if (restarting) {
            			restarting = 0;
        	      		/* drop any output we might got and stop */
              			snd_pcm_drop(capture_handle);
              			snd_pcm_drop(playback_handle);
            
        			     /* prepare for use */
                		snd_pcm_prepare(capture_handle);
               			snd_pcm_prepare(playback_handle);

          			 /* fill the whole output buffer */
          			 for (i = 0; i < fragments; i += 1)
        				    snd_pcm_writei(playback_handle, rdbuf, frames);
        		}

        		while ((inframes = snd_pcm_readi(capture_handle, rdbuf, frames)) < 0) {
            			if (inframes == -EAGAIN)
                			continue;
        			// by the way, writing to terminal emulator is costly if you use
        			// bad emulators like gnome-terminal, so don't do this.
            			fprintf(stderr, "Input buffer overrun\n");
            			restarting = 1;
            			snd_pcm_prepare(capture_handle);
       	       	 	}
        			
				if (inframes != frames)
            				fprintf(stderr, "Short read from capture device: %d, expecting %d\n", inframes, frames);
	       				/* now processes the frames */
					int effects;
					printf("Choose the sound effect you want \n1.echo\n2Pitch shift\n3.Room\n4.biquad\n5.phasor");
						scanf("%d", &effects);

							switch(effects){
								case 1:
								/*call smbPitchshift*/
									smbPitchShift(pitchShift, frames, 2048, 2,snd_pcm_readi(capture_handle, rdbuf, frames), data[0], data[0]);
		
	        								while ((outframes = snd_pcm_writei(playback_handle, rdbuf, inframes)) < 0) {
		            								if (outframes == -EAGAIN)
		                							continue;
	        		    								fprintf(stderr, "Output buffer underrun\n");
	            		      
	                      									restarting = 1;
	            					
	                			      						snd_pcm_prepare(playback_handle);
	        								}
	      			  						if (outframes != inframes)
	           									 fprintf(stderr, "Short write to playback device: %d, expecting %d\n", outframes, frames);
	           									 free(rdbuf);
	           								 break;

	    							case 2:
    									smoothdelay(do it);//echo
	    									while ((outframes = snd_pcm_writei(playback_handle, rdbuf, inframes)) < 0) {
	            									if (outframes == -EAGAIN)
	                								continue;
	            									
											fprintf(stderr, "Output buffer underrun\n");
	            		      
	                      								restarting = 1;
	            					
	                			      					snd_pcm_prepare(playback_handle);
	        								}
	      			  					       if (outframes != inframes)
	           									 fprintf(stderr, "Short write to playback device: %d, expecting %d\n", outframes, frames);
	           									free(rdbuf);
	           				        			 break;
	
		           					case 3:
	           							Room();
		           							while ((outframes = snd_pcm_writei(playback_handle, rdbuf, inframes)) < 0) {
	            									if (outframes == -EAGAIN)
	                								continue;
	            									
											fprintf(stderr, "Output buffer underrun\n");
	            		      
	                      								restarting = 1;
	            					
	                			      					snd_pcm_prepare(playback_handle);
	        								}
	      			  						if (outframes != inframes)
	           									 fprintf(stderr, "Short write to playback device: %d, expecting %d\n", outframes, frames);
	           									 free(rdbuf);
	           								 break;

           							case 4: 
           								biquad();
		           							while ((outframes = snd_pcm_writei(playback_handle, rdbuf, inframes)) < 0) {
		            								if (outframes == -EAGAIN)
		                							continue;
		            							
											fprintf(stderr, "Output buffer underrun\n");
		            		      
		                      							restarting = 1;
		            					
		                			      				snd_pcm_prepare(playback_handle);
		        							}
		      				  				if (outframes != inframes)
		           								 fprintf(stderr, "Short write to playback device: %d, expecting %d\n", outframes, frames);
											 free(rdbuf);
	           								 break;
		 					}
        				      	}

			
  return 0;         

}		
