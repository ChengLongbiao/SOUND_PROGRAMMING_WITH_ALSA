#Innovation Project
SOUND_PROGRAMMING_WITH_ALSA
#ALSA Full duplex

#Sound 
consists of waves of varying air pressure, is converted to its electrical form by a transducer, such as a microphone. An analog-to-digital converter (ADC) converts the analog voltages into discrete values, called samples, at regular intervals in time, known as the sampling rate. By sending the samples to a digital-to-analog converter and an output transducer, such as a loudspeaker, the original sound can be reproduced. 
The size of the samples, expressed in bits, is one factor that determines how accurately the sound is represented in digital form. The other major factor affecting sound quality is the sampling rate. The NYQuist Theorem states that the highest frequency that can be represented accurately is at most one-half the sampling rate. 
ALSA consists of a series of kernel device drivers for many different sound cards, and it also provides an API library, libasound. Application developers are encouraged to program using the library API and not the kernel interface. The library provides a higher-level and more developer-friendly programming interface along with a logical naming of devices so that developers do not need to be aware of low-level details such as device files


This project is about the sound control interface whereby there is input and output. The input is filtered with DSP (digital signals processing) so as to get the required output. The digital audio programming project will be implemented with #C in Linux/Unix environment. 
The image bellow shows the project idea.

https://cloud.githubusercontent.com/assets/5812481/7043048/26ad86ec-ddef-11e4-9a06-b9868c4eace1.png
 
#Goals of the project
The project will tackle main areas of the audio effects which include the following, reverb, delay pitch shift and compression


