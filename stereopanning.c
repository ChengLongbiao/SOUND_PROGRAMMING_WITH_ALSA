//We need function that, a given parameter (in the range -1...+1), returns two values, representing the amplitude scaling factors(range 0..1)
// for the left and right output chanells.
typedef struct panpos{
	double left;
	double right;
}PANPOS;

// we can define the a panning function that return a PANPOS object:
PANPOS sinplepan(double position);
//

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

/*
 In the main Add a variable called panpos(type double) to the end of the variable list at the top main
 
 ALSO change the ENUM
 
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
		
		
		//Add this to the main loop, As the function simplepan returns a PANPOS, we need a variable to receive it:
		PANPOS thispos;  /*STAGE 1*/
		
		//processing loop to do the linear panning:
		
	thispos = simplepan(position);
	while((framesread = psf_sndReadFloatFrames(ifd, inframes, nframes)) > 0){
	 int i, out_i;
	 for(i = 0; out_i = 0; i < framesread; i++){
		outframe[out_i++] = (float)(inframe[i]*thispos.left);
		outframe[out_i++] = (float)(inframe[i]*thispos.right);
	 }
	  if (psf_sndWriteFloatFrames(ofd, outframe, framesread) != framesread{
		printf("Error writting to outfile\n);
		error++;
		break;
	  }
	}
		
		