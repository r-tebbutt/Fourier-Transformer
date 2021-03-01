#include <iostream>
#include <complex>
#include <valarray>
#include <cmath>
#include <fstream>
#include "AudioFile.h"
#include <vector>

const double PI = 3.141592653589793238460;

using namespace std;


AudioFile<double> audioFile;
typedef complex<double> Complex;
typedef valarray<Complex> CArray;

void fft(CArray& x)
{
    const size_t N = x.size();
    if (N <= 1) return;
 
    // divide
    CArray even = x[slice(0, N/2, 2)];
    CArray  odd = x[slice(1, N/2, 2)];
 
    // conquer
    fft(even);
    fft(odd);
 
    // combine
    for (size_t k = 0; k < N/2; ++k)
    {
        Complex t = polar(1.0, -2 * PI * k / N) * odd[k];
        x[k    ] = even[k] + t;
        x[k+N/2] = even[k] - t;
    }
}


int main() {
	
	audioFile.load ("TestWav.wav");
	
	int numSamples = audioFile.getNumSamplesPerChannel();
	double spectrum_fps = 25; //frame rate of spectogram
	double Fs = audioFile.getSampleRate(); // sample frequency
	double T = 1/Fs; // time step
	double lengthInSeconds = audioFile.getLengthInSeconds();
	double sample_time = 1/spectrum_fps; // sample time of each spectrum frame in seconds
	double L = sample_time * Fs; // total number of amplitude samples in each spectrum frame
	double t[int(L)]; // time array
	double f[int(L/2)]; // frequency array
	Complex t_domain_amp_1[int(L)]; // amplitude values in time domain
	
	
	
	ofstream Gk("Gk.txt");
	for(int j = 0; j < spectrum_fps * lengthInSeconds; j++){
	if(audioFile.isStereo()){
	for (int i = 0; i < L; i++){
		//extracts time domain data from Wav file, adding data from both channels if 2 exist
		t_domain_amp_1[int(i)] = audioFile.samples[0][int(j*L + i)] + audioFile.samples[1][int(j*L + i)];
	}
	}
	
	else{
		for (int i = 0; i < L; i++){
		//or just from one channel if mono audio
		t_domain_amp_1[int(i)] = audioFile.samples[0][int(j*L + i)];
	}
	}
	
	
	CArray t_domain_amp_2(t_domain_amp_1, L); // create valarray for fft
	
	fft(t_domain_amp_2); //fourier transform time domain data
	
	CArray f_domain_amp_1 = t_domain_amp_2;
	f_domain_amp_1 /= L;
	CArray f_domain_amp_2 = abs(f_domain_amp_1);
	CArray f_domain_amp_3 = f_domain_amp_2[slice(0,L/2+1,1)];
	
	for(int i = 1; i < (L/2-1); i++){
		f_domain_amp_3[i]*=2;
	}
	
	
	for(double i = 0; i < L/2;i++){
		f[int(i)] = Fs * i / L;
	} // sets the correct frequency axis values
	
	
	for(int i = 0; i < L/2; i++){
		Gk << f[i] << " " << abs(f_domain_amp_3[i]) << endl;
	}
	Gk << "\n\n";
	}
	Gk.close(); // writes frequency domain data to file to plot with GNUplot
	
}
	
	

