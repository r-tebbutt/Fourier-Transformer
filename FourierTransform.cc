#include <iostream>
#include <complex>
#include <valarray>
#include <cmath>
#include <fstream>


const double PI = 3.141592653589793238460;

using namespace std;

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

void ifft(CArray& x)
{
    // conjugate the complex numbers
    x = x.apply(conj);
 
    // forward fft
    fft( x );
 
    // conjugate the complex numbers again
    x = x.apply(conj);
 
    // scale the numbers
    x /= x.size();
}

int main() {
	
	double Fs = 5000; // sample frequency
	double T = 1/Fs; // time step
	double sample_time = 10; // sample time in seconds
	double L = sample_time * Fs; // total number of samples
	double t[int(L)]; // time array
	double f[int(L/2)]; // frequency array
	Complex t_domain_amp_1[int(L)]; // amplitude values in time domain
	
	
	for(int i = 0; i < L; i++){
		t[i] = i * T;
	}
	
	
	for (int i = 0; i < L; i++){
		t_domain_amp_1[int(i)] = cos(2*PI*30*t[i]) + cos(2*PI*200*t[i]);
	} // sample signal of 2 cos waves
	
	
	ofstream Fx("Fx.txt");
	for(int i = 0; i < L; i++){
		Fx << t[i] << " " << real(t_domain_amp_1[int(i)]) << endl;
	} // writing time and amplitude values to file to plot with GNUplot
	Fx.close();
	
	
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
	
	ofstream Gk("Gk.txt");
	for(int i = 0; i < L/2; i++){
		Gk << f[i] << " " << abs(f_domain_amp_3[i]) << endl;
	}
	Gk.close(); // writes frequency domain data to file to plot with GNUplot
	
}
	
	

