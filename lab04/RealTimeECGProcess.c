#include "RealTimeECGProcess.h"

/* ---------- Coeficientii filtrului trece banda pentru semnalele ECG esantionate cu o frecventa de esantionare de 200 Hz ---------- */
static const double b[13] = {1, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 0, 1};
static const double a[3] = {1, -2, 1};

/* ---------- Variabilele Globale ---------- */
static double ecg_x_buffer[13] = {0.0};  // Buffer pentru intrtri (x[n] pâna la x[n-12])
static double ecg_y_buffer[2] = {0.0};   // Buffer pentru iesiri (y[n-1], y[n-2])
static int ecg_buffer_index = 0;

static double ecg_derivative_buffer[5] = {0.0};
static int ecg_derivative_index = 0;
static int ecg_derivative_ready = 0;

static double ecg_mov_avg_buffer[REAL_TIME_BUFFER_SIZE] = {0.0};
static int ecg_mov_avg_index = 0;
static const int ecg_mov_avg_win = REAL_TIME_BUFFER_SIZE;
static double ecg_mov_avg_sum = 0.0;
static int ecg_mov_avg_ready = 0;

static double ecg_last_sample = 0.0;
static double ecg_current_sample = 0.0;
static int ecg_peak_detected = 0;
static int ecg_peak_count = 0;
static double ecg_r_peaks[MAX_PEAKS] = {0.0};
static int ecg_r_locations[MAX_PEAKS] = {0};
static unsigned long ecg_sample_counter = 0;

static double ecg_last_r_time = 0.0;
static double ecg_current_heart_rate = 0.0;
static double ecg_rr_intervals[MAX_PEAKS] = {0.0};
static int ecg_rr_count = 0;

static int ecg_initialized = 0;

/* ---------- Variabile pentru praguri adaptive Pan-Tompkins ---------- */
static double THR_SIG = 0.0;          // Prag pentru semnalul integrat
static double THR_NOISE = 0.0;        // Prag pentru zgomot
static double SIG_LEV = 0.0;          // Nivel semnal
static double NOISE_LEV = 0.0;        // Nivel zgomot

static double THR_SIG1 = 0.0;         // Prag pentru semnalul filtrat trece-banda
static double THR_NOISE1 = 0.0;       // Prag pentru zgomot în semnalul filtrat
static double SIG_LEV1 = 0.0;         // Nivel semnal în filtrul trece-band?
static double NOISE_LEV1 = 0.0;       // Nivel zgomot în filtrul trece-band?

static int training_phase = 1;        // Faza de iniaializare (primele 2 secunde)
static int training_samples = 0;      // Numar de eaantioane în faza de training
static double max_train_integrated = 0.0;
static double mean_train_integrated = 0.0;
static double max_train_bandpass = 0.0;
static double mean_train_bandpass = 0.0;

static double integrated_signal_buffer[REAL_TIME_BUFFER_SIZE] = {0.0};
static double bandpass_signal_buffer[REAL_TIME_BUFFER_SIZE] = {0.0};
static int signal_buffer_index = 0;

/* ---------- Initializarea variabilelor pentru procesul de analiza a semnalelor ECG esantionate cu o frecventa de esantionare de 200 Hz ---------- */
void real_time_ecg_init(void)
{
    int i;
    
    /* Resetarea buffer-urilor noului filtru */
    for (i = 0; i < 13; i++) {
        ecg_x_buffer[i] = 0.0;
    }
    for (i = 0; i < 2; i++) {
        ecg_y_buffer[i] = 0.0;
    }
    ecg_buffer_index = 0;
    
    /* Resetarea buffer-ului derivatei */
    for (i = 0; i < 5; i++) {
        ecg_derivative_buffer[i] = 0.0;
    }
    ecg_derivative_index = 0;
    ecg_derivative_ready = 0;
    
    /* Resetarea buffer-ului mediei mobile */
    for (i = 0; i < REAL_TIME_BUFFER_SIZE; i++) {
        ecg_mov_avg_buffer[i] = 0.0;
        integrated_signal_buffer[i] = 0.0;
        bandpass_signal_buffer[i] = 0.0;
    }
    ecg_mov_avg_index = 0;
    ecg_mov_avg_sum = 0.0;
    ecg_mov_avg_ready = 0;
    signal_buffer_index = 0;
    
    /* Resetarea st?rii de detectare a vârfurilor */
    ecg_last_sample = 0.0;
    ecg_current_sample = 0.0;
    ecg_peak_detected = 0;
    ecg_peak_count = 0;
    ecg_sample_counter = 0;
    ecg_last_r_time = 0.0;
    ecg_current_heart_rate = 0.0;
    ecg_rr_count = 0;
    
    /* Resetarea pragurilor adaptive */
    THR_SIG = 0.0;
    THR_NOISE = 0.0;
    SIG_LEV = 0.0;
    NOISE_LEV = 0.0;
    THR_SIG1 = 0.0;
    THR_NOISE1 = 0.0;
    SIG_LEV1 = 0.0;
    NOISE_LEV1 = 0.0;
    
    training_phase = 1;
    training_samples = 0;
    max_train_integrated = 0.0;
    mean_train_integrated = 0.0;
    max_train_bandpass = 0.0;
    mean_train_bandpass = 0.0;
    
    ecg_initialized = 1;
}


/* ---------- Filtrul Trece Banda ---------- */
double real_time_bandpass_filter(double input)
{
    double output = 0.0;
    int i;
    
    /* Actualizeaza buffer-ul de intr?ri (folosind index circular) */
    ecg_x_buffer[ecg_buffer_index] = input;
    
    /* Calculeaza partea FIR (numaratorul) */
    for (i = 0; i < 13; i++) {
        int index = (ecg_buffer_index - i + 13) % 13;
        output += b[i] * ecg_x_buffer[index];
    }
    
    /* Calculeaza partea IIR (numitorul) si o scade*/
    for (i = 1; i < 3; i++) {
        output -= a[i] * ecg_y_buffer[i-1];
    }
    
    /* Actualizeaza buffer-ul de iesiri */
    ecg_y_buffer[1] = ecg_y_buffer[0];  // y[n-2] = y[n-1]
    ecg_y_buffer[0] = output;           // y[n-1] = y[n]
    
    /* Actualizeaza indexul buffer-ului circular */
    ecg_buffer_index = (ecg_buffer_index + 1) % 13;
    
    return output;
}

/* ---------- Derivata ---------- */
double real_time_derivative(double input, int ecg_sample_counter)
{
        double derivative = 0.0;
    
    /* Stocheaza esantionul curent în buffer-ul circular */
    ecg_derivative_buffer[ecg_derivative_index] = input;
    
    /* Actualizeaza indexul buffer-ului */
    if (++ecg_derivative_index >= 5) {
        ecg_derivative_index = 0;
        ecg_derivative_ready = 1;
    }
    
    /* Calculeaza derivata discreta când sunt suficiente esantioane */
    if (ecg_sample_counter >= 4) {
        int i0 = ecg_derivative_index;
        int i1 = (i0 + 1) % 5;
        int i2 = (i0 + 2) % 5;
        int im1 = (i0 + 4) % 5;
        int im2 = (i0 + 3) % 5;
        
        derivative = 25.0 * (2.0 * ecg_derivative_buffer[i1] + 
                            ecg_derivative_buffer[i2] - 
                            ecg_derivative_buffer[im2] - 
                            2.0 * ecg_derivative_buffer[im1]);
	}
    
    return derivative;
}

/* ---------- Media mobila---------- */
double real_time_moving_average(double input, int ecg_sample_counter)
{
     double average;
    int buffer_size;
    
     /* Actualizeaza buffer-ul circular */
    ecg_mov_avg_sum -= ecg_mov_avg_buffer[ecg_mov_avg_index];
    ecg_mov_avg_buffer[ecg_mov_avg_index] = input;
    ecg_mov_avg_sum += input;
    
    /* Actualizeaza indexul buffer-ului */
    if (++ecg_mov_avg_index >= ecg_mov_avg_win) {
        ecg_mov_avg_index = 0;
    }
    
     /* Seteaza flag-ul de pregatire când buffer-ul este umplut */
    if (!ecg_mov_avg_ready && ecg_sample_counter >= (unsigned long)ecg_mov_avg_win) {
        ecg_mov_avg_ready = 1;
    }
    
    /* Calculeaza media */
    buffer_size = (ecg_sample_counter < (unsigned long)ecg_mov_avg_win) ? 
                  (ecg_sample_counter + 1) : ecg_mov_avg_win;
    
    average = ecg_mov_avg_sum / buffer_size;
    
    return average;
}

/* ---------- Detectarea vârfurilor ---------- */
double real_time_peak_detection(double processed_signal, double bandpass_signal, double original_ecg, int ecg_sample_counter)
{
    static int skip = 0;
    static int not_nois = 0;
    static int ser_back = 0;
    static int noise_count = 0;
    
    ecg_current_heart_rate = 0.0;
    int peak_found = 0;
    
    /* Stocarea semnalelor pentru faza de training */
    integrated_signal_buffer[signal_buffer_index] = processed_signal;
    bandpass_signal_buffer[signal_buffer_index] = bandpass_signal;
    signal_buffer_index = (signal_buffer_index + 1) % REAL_TIME_BUFFER_SIZE;
    
    /* Faza de training - primele 2 secunde (400 de esantioane la 200Hz) */
    if (training_phase) {
        training_samples++;
        
        /* Actualizeaza maximele si mediile pentru semnale */
        if (processed_signal > max_train_integrated) {
            max_train_integrated = processed_signal;
        }
        mean_train_integrated += processed_signal;
        
        if (bandpass_signal > max_train_bandpass) {
            max_train_bandpass = bandpass_signal;
        }
        mean_train_bandpass += bandpass_signal;
        
        /* Dupa 2 secunde, se vor initializa pragurile */
        if (training_samples >= 400) {
            THR_SIG = max_train_integrated * (1.0/3.0);
            THR_NOISE = (mean_train_integrated / 400) * 0.5;
            SIG_LEV = THR_SIG;
            NOISE_LEV = THR_NOISE;
            
            THR_SIG1 = max_train_bandpass * (1.0/3.0);
            THR_NOISE1 = (mean_train_bandpass / 400) * 0.5;
            SIG_LEV1 = THR_SIG1;
            NOISE_LEV1 = THR_NOISE1;
            
            training_phase = 0;
        }
        
        return 0;
    }
    
    /* Actualizarea valorilor esantioanelor */
    ecg_last_sample = ecg_current_sample;
    ecg_current_sample = processed_signal;
    
    /* Iesire rapida in cazul in care buffer-urile nu sunt pregatite */
    if (!ecg_derivative_ready || !ecg_mov_avg_ready) {
        return 0;
    }
    
    /* === Reguli de decizie si praguri adaptive === */
    
    /* Detectarea vârfurilor discrete: curent > anterior SI curent > prag */
    if (ecg_current_sample > ecg_last_sample && ecg_current_sample > THR_SIG && !skip) {
        double samples_since_last_peak = ecg_sample_counter - ecg_last_r_time;
        
        /* Verificarea perioadei refractare discrete (40 esantioane = 200ms la 200Hz) */
        if (ecg_peak_count == 0 || samples_since_last_peak > REFRACTORY_SAMPLES) {
            
            /* Verificarea undei T - discriminare bazata pe panta */
            if (ecg_peak_count >= 3) {
                if (samples_since_last_peak <= (int)(0.3600 * SAMPLE_RATE)) {
                    /* Calculul pantei pentru discriminarea undei T */
                    double current_slope = ecg_current_sample - ecg_last_sample;
                    double prev_slope = 0.0;
                    
                    /* Panta undei R anterioare */
                    if (ecg_peak_count > 0) {
                        int prev_peak_index = (signal_buffer_index - (int)samples_since_last_peak + REAL_TIME_BUFFER_SIZE) % REAL_TIME_BUFFER_SIZE;
                        if (prev_peak_index >= 1) {
                            prev_slope = integrated_signal_buffer[prev_peak_index] - integrated_signal_buffer[prev_peak_index - 1];
                        }
                    }
                    
                    if (fabs(current_slope) <= fabs(0.5 * prev_slope)) {
                        /* Ignorarea undei T */
                        noise_count++;
                        skip = 1;
                        /* Ajustarea nivelurilor de zgomot */
                        NOISE_LEV1 = 0.125 * bandpass_signal + 0.875 * NOISE_LEV1;
                        NOISE_LEV = 0.125 * processed_signal + 0.875 * NOISE_LEV;
                    }
                }
            }
            
            if (!skip && ecg_peak_count < MAX_PEAKS) {
                int current_peak = ecg_peak_count;
                
                /* Stocarea informatiile despre undele R*/
                ecg_r_peaks[current_peak] = original_ecg;
                ecg_r_locations[current_peak] = (int)ecg_sample_counter;
                
                /* Calcularea frecventei cardiace daca existaun vârf anterior */
                if (current_peak >= 1) {
                    /* Conversia in timp a esantioanelor*/
                    double rr_interval = samples_since_last_peak / SAMPLE_RATE;
                    
                    if (ecg_rr_count < MAX_PEAKS) {
                        ecg_rr_intervals[ecg_rr_count++] = rr_interval;
                    }
                    
                    /* Ajustare dinamica a pragurilor bazate pe intervalul RR */
                    if (ecg_peak_count >= 9) {
                        double mean_rr = 0.0;
                        int valid_intervals = 0;
                        
                        /* Calcularea mediei ultimelor 8 intervale RR */
                        for (int i = ecg_rr_count - 8; i < ecg_rr_count; i++) {
                            if (i >= 0 && ecg_rr_intervals[i] > 0.3 && ecg_rr_intervals[i] < 2.0) {
                                mean_rr += ecg_rr_intervals[i];
                                valid_intervals++;
                            }
                        }
                        
                        if (valid_intervals > 0) {
                            mean_rr /= valid_intervals;
                            double current_rr = rr_interval;
                            
                            /* In cazul unui interval QRS abnormal, se vor ajusta pragurile */
                            if (current_rr <= 0.92 * mean_rr || current_rr >= 1.16 * mean_rr) {
                                THR_SIG = 0.5 * THR_SIG;
                                THR_SIG1 = 0.5 * THR_SIG1;
                            }
                        }
                    }
                    
                    /* Validarea si calcularea frecventei cardiace */
                    if (rr_interval > 0.3 && rr_interval < 2.0) {
                        ecg_current_heart_rate = 60.0 / rr_interval;
                    }
                }
                
                ecg_last_r_time = ecg_sample_counter;
                ecg_peak_count++;
                peak_found = 1;
                
                /* Actualizarea nivelurilor de semnal */
                SIG_LEV = 0.125 * processed_signal + 0.875 * SIG_LEV;
                SIG_LEV1 = 0.125 * bandpass_signal + 0.875 * SIG_LEV1;
            }
        }
    }
    /* Semnal slab - actualizarea nivelurilor de zgomot */
    else if (THR_NOISE <= processed_signal && processed_signal < THR_SIG) {
        NOISE_LEV1 = 0.125 * bandpass_signal + 0.875 * NOISE_LEV1;
        NOISE_LEV = 0.125 * processed_signal + 0.875 * NOISE_LEV;
    }
    /* Zgomot - actualizarea nivelurilor de zgomot */
    else if (processed_signal < THR_NOISE) {
        noise_count++;
        NOISE_LEV1 = 0.125 * bandpass_signal + 0.875 * NOISE_LEV1;
        NOISE_LEV = 0.125 * processed_signal + 0.875 * NOISE_LEV;
    }
    
    /* === Ajustarea pragurilor cu SNR === */
    if (NOISE_LEV != 0 || SIG_LEV != 0) {
        THR_SIG = NOISE_LEV + 0.25 * (fabs(SIG_LEV - NOISE_LEV));
        THR_NOISE = 0.5 * THR_SIG;
    }
    
    /* Ajustarea pragurilor pentru semnalul filtrat trece-banda */
    if (NOISE_LEV1 != 0 || SIG_LEV1 != 0) {
        THR_SIG1 = NOISE_LEV1 + 0.25 * (fabs(SIG_LEV1 - NOISE_LEV1));
        THR_NOISE1 = 0.5 * THR_SIG1;
    }
    
    /* Resetarea parametrilor */
    skip = 0;
    not_nois = 0;
    ser_back = 0;
    
    return ecg_current_heart_rate;
}


/* ---------- Procesarea un esantion ECG ---------- */
void process_ecg_sample(double ecg_sample, double *heart_rate)
{
    double bandpass_output, derivative_output, squared_output, integrated_output;
    
	double used_ecg_val = ecg_sample/1023.0;
	
    /* Initializare */
    if (!ecg_initialized) {
        real_time_ecg_init();
    }
    
    ecg_sample_counter++;
    
    /* Procesare */
    bandpass_output = real_time_bandpass_filter(used_ecg_val);
    derivative_output = real_time_derivative(bandpass_output,ecg_sample_counter);
    squared_output = derivative_output * derivative_output;
    integrated_output = real_time_moving_average(squared_output,ecg_sample_counter);
    ecg_current_heart_rate = real_time_peak_detection(integrated_output, bandpass_output, used_ecg_val, ecg_sample_counter);
    
    /* Frecventa Cardiaca Rezultata */
    if (heart_rate != NULL) {
        *heart_rate = ecg_current_heart_rate;
    }
}

/* ---------- Ob?ine rezultatele recente ---------- */
void get_ecg_results(double *heart_rate, int *peak_count, 
                    double *last_r_peak, int *last_r_location)
{
    if (heart_rate != NULL) {
        *heart_rate = ecg_current_heart_rate;
    }
    
    if (peak_count != NULL) {
        *peak_count = ecg_peak_count;
    }
    
    if (last_r_peak != NULL && last_r_location != NULL) {
        if (ecg_peak_count > 0) {
            int last_index = ecg_peak_count - 1;
            *last_r_peak = ecg_r_peaks[last_index];
            *last_r_location = ecg_r_locations[last_index];
        } else {
            *last_r_peak = 0.0;
            *last_r_location = 0;
        }
    }
}

/* ---------- Obtinerea frecventei cardiace medie (optimized) ---------- */
double get_average_heart_rate(void)
{
    double total_rr = 0.0;
    int valid_intervals = 0;
    int i;
    
    if (ecg_rr_count < 1) {
        return 0.0;
    }
    
    for (i = 0; i < ecg_rr_count; i++) {
        double interval = ecg_rr_intervals[i];
        if (interval > 0.3 && interval < 2.0) {
            total_rr += interval;
            valid_intervals++;
        }
    }
    
    return (valid_intervals > 0) ? (60.0 * valid_intervals / total_rr) : 0.0;
}

/* ---------- Numarul de varfuri ---------- */
int get_peak_count(void)
{
    return ecg_peak_count;
}

/* ---------- Starea de detectie a varfurilor ---------- */
int get_peak_detected(void)
{
    return ecg_peak_detected;
}

/* ---------- Resetarea procesului ---------- */
void reset_real_time_ecg(void)
{
    real_time_ecg_init();
}