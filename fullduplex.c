/* Use the newer ALSA API */
#include "interface.h"
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <stdio.h> 
#include <stdlib.h>
#include <math.h> 
#include <errno.h>
#include <poll.h>
#include <alsa/asoundlib.h>
#include <snd_defs.h>




int main() {
/*you locate these in a heap */
	int 		restarting;
	int		nchannels = 2;
	int		buffer_size = 512;
	float 		sample_rate = 48000.f;
	int 		bits = 16;
	int 		err;
  	long		frame_size;
	double 		panpos//panning position of the stereo
	float       *sig; //signal buffer
    float 		freq = 10.f; //frequency
    float 		*delay; // delay memory
    float 		maxdel; //max delay time
    float 		vdtime; //variable delay time
    float 		dtime;	//delay time
    float 		gain;
    float 		fdb;	//feedbackgain
    int 		vecsize = def_vsize; //vectorsize
	char		*snd_device_in = "plughw:2,0";
	char 		*snd_device_out = "plughw:2,0";
	snd_pcm_t	*playback_handle;
	snd_pcm_t 	*capture_handle;

	//Set the device for capture and playback

		if ((err = snd_pcm_open(&capture_handle, snd_device_in, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        	  fprintf(stderr, "cannot open input audio device %s: %s\n", snd_device_out, snd_strerror(err));
        
    	exit(1);
     } 
      if ((err = snd_pcm_open(&playback_handle, snd_device_out, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
         	 fprintf(stderr, "cannot open input audio device %s: %s\n", snd_device_in, snd_strerror(err));
         
    	 exit(1);
      }

     configure_alsa_audio(snd_device_in,  nchannels);
     configure_alsa_audio(snd_device_out, nchannels);

	restarting = 1
		// preocess the data

		int  inframes, outframes;
  		char *rdbuf;
  		int i;
  	
		rdbuf = (char *)malloc(frame_size);
		long semitones = 3; // shift up by 3 semitones
		float pitchShift = pow(2.0, semitones/12.0);// convert semitones to factor
		delay = (float *)malloc(sizeoff(dtime*sample_rate));
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
            			fprintf(stderr, "Input buffer overrun\n");
            			restarting = 1;
            			snd_pcm_prepare(capture_handle);
       	       	 	}
        			
				if (inframes != frames)
	            		fprintf(stderr, "Short read from capture device: %d, expecting %d\n", inframes, frames);
		       				
		       			/* now processes the frames */
						
						int effects;
						printf("Choose the sound effect you want \n1.Pitch shift\n2.echo\n3.Room\n4.biquad\n5.stereo");
							scanf("%d", &effects);

								switch(effects){
									case 1:
									/*call smbPitchshift*/
										smbPitchShift(pitchShift, frames, 2048, 4,snd_pcm_readi(capture_handle, rdbuf, frames), inframes, outframes);
			
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
			           					//call lowpass effects
		           							lowpass(&sig, &freq, &delay, vecsize, sample_rate);
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
		           									 free(sig);
		           									 free(delay);
		           									 free(freq);
		           								 break;

	           							case 3: 
	           							//call highpass effects
	           								 highpass(&sig, &freq, &delay, vecsize, sample_rate);
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
												 		free (freq);
												 		free(sig);
													 	free(delay);
		           								 break;

		           						case 4: 
			           							//call streo effects
												PANPOS thispos;
			           								thispos = constpower_pan(panpos);
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

		           						case 5: 
	           								//call balance effects
	
			           							balance(buffer, comp, del1);
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

		           						case 6: 
			           							//call variable delay effects
												 vdelay(&sig, vdtime, maxdel, &delay, &p, vecsize, sample_rate);
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
															 free(p);
														 	 free(sig);
														 	 free(delay);
				           								 break;

		           						case 7: 
			           							//call franger effects
												
					           					flanger(rdbuf, line(.0001f, dur, dtime, &ts), 0.8f, dtime, del, &pt);
	    
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
															 free(ts);
															 free(pt);
				           								 break;

		           						case 8: 
			           							//allpass
												allpass(&sig,dtime,gain,&delay, &p, vecsize, sample_rate);
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
															 free(p);
														 	 free(sig);
														 	 free(delay);
				           								 break;
		           						case 9: 
	           									//call comb effects
												comb(&sig,dtime,gain,&delay, &p, vecsize, sample_rate);
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
														 free(p);
														 free(sig);
														 free(delay);
			           								 break;

			           					case 10: 
			           							//call franger effects
												delay(&sig, vdtime, fdb, maxdel,&delay, &p, vecsize, sample_rate)
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
														 free(p);
														 free(sig);
														 free(delay);
			           								 break;
			 					}
	        				}    

	}		
