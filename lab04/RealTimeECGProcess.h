#ifndef ECG_PROCESSOR_H
#define ECG_PROCESSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ---------- Constante ---------- */
#define MAX_PEAKS 100 
#define REAL_TIME_BUFFER_SIZE 20
#define SAMPLE_RATE 200.0   // frecventa de esantionare
#define REFRACTORY_SAMPLES 40   // intervalul de refractie pentru detectarea utrmatorului complex QRS
#define NUM_SECTIONS 3 

/* ---------- Functii protorip ---------- */
void real_time_ecg_init(void);
double real_time_bandpass_filter(double input); // Functia pentru filtrul trece banda
double real_time_derivative(double input,int ecg_sample_counter); // functia pentru derivarea semnalului
double real_time_moving_average(double input,int ecg_sample_counter); // funtie pentru media mobila
double real_time_peak_detection(double processed_signal, double bandpass_signal, double original_ecg, int ecg_sample_counter); // functia pentru extragerea undelor R din semnalul mediat


void process_ecg_sample(double ecg_sample, double *heart_rate); 
void get_ecg_results(double *heart_rate, int *peak_count, 
                    double *last_r_peak, int *last_r_location);
double get_average_heart_rate(void);
int get_peak_count(void);
int get_peak_detected(void);
void reset_real_time_ecg(void);

#endif