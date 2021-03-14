# Fourier-Transformer

FourierTransform.cc : 

This C++ code can create a frequency spectrum from a time domain signal using the Fast Fourier Transform algorithm.

It outputs the data as 2 columns: time and amplitude in 'Fx.txt'; frequency and amplitude in 'Gk.txt'. 
I have then been plotting this data with gnuplot to view the time and frequency domain signals.

Spectrogram.cc : 

This program extracts time domain audio date from a WAV file and creates a file from which an animation of the frequency spectrum
changing with time can be created (I used GNUPlot to do this). In this sense, it creates a spectrogram of an audiofile.

The file created has blocks of x-y data to plot, corresponding to frequency and amplitude, and each block corresponding to one frame
of animation. In this form, a simple GNUPlot script can create an animated gif of the spectrogram.

The framerate of the spectrogram can be adjusted, but this will have an effect on the resolution of the specta as it changes
the sample time allocated to each frame. The sample rate of the WAV file also affects the resolution of the spectra.
The maximum frequency that will be displayed is half of the sample frequency of the WAV file in accordance with Nyquist's theorem,
though I adjust this in creating the animations as most of the action happens at lower frequencies.

"Audio.h" Header file found at https://github.com/adamstark/AudioFile/blob/master/AudioFile.h
