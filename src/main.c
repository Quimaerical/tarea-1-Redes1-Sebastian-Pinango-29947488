#include "encoding.h"
#include <math.h>

#define NUM_SIMULATIONS 1000
#define ID_DIGIT 8
#define MY_BER 0.08

// Function to calculate errors
int count_errors(const char *original, const char *decoded) {
    if (!original || !decoded) return -1;
    int errors = 0;
    int len = strlen(original);
    for (int i=0; i<len; i++) {
        if (original[i] != decoded[i]) errors++;
    }
    return errors;
}

// Stats helper
typedef struct {
    double mean;
    double std_dev;
    int min;
    int max;
} Stats;

Stats calculate_stats(int *errors, int n) {
    long sum = 0;
    int min = 1000000, max = -1;
    for(int i=0; i<n; i++) {
        sum += errors[i];
        if (errors[i] < min) min = errors[i];
        if (errors[i] > max) max = errors[i];
    }
    double mean = (double)sum / n;
    
    double variance_sum = 0;
    for(int i=0; i<n; i++) {
        variance_sum += pow(errors[i] - mean, 2);
    }
    double std_dev = sqrt(variance_sum / n);
    
    Stats s = {mean, std_dev, min, max};
    return s;
}

// Generic Simulation Function
void run_simulation(const char *name, char* (*encoder)(const char*), char* (*decoder)(const char*), const char *data, double ber, int n, FILE *csv) {
    int *error_counts = malloc(n * sizeof(int));
    
    for(int i=0; i<n; i++) {
        char *encoded = encoder(data);
        
        // Simular canal
        add_noise(encoded, ber);
        
        char *decoded = decoder(encoded);
        int err = count_errors(data, decoded);
        error_counts[i] = (err >= 0) ? err : strlen(data); // Penalizar si no decodifica
        
        free(encoded);
        free(decoded);
    }
    
    Stats s = calculate_stats(error_counts, n);
    printf("| %-10s | %6.4f | %6.4f | %3d | %3d |\n", name, s.mean, s.std_dev, s.min, s.max);
    
    if (csv) {
        fprintf(csv, "%s,%.5f,%.4f\n", name, ber, s.mean);
    }
    
    free(error_counts);
}

int main() {
    srand(time(NULL));
    
    printf("=== TAREA 1: ANALISIS DE CODIFICACION ===\n");
    printf("Estudiante: Sebastian Pinango\n");
    printf("Cedula termina en: %d -> BER = %.2f\n\n", ID_DIGIT, MY_BER);
    
    const char *data_short = "11001010"; // Para signal plots
    const char *data_long = "11001010110010101100101011001010110010101100101011001010110010101100101011001010"; // 80 bits para stats
    
    // 1. Visualización (Signal Plot)
    // Limpiar archivo
    FILE *f = fopen("results/signals.txt", "w");
    if(f) { fprintf(f, "DIAGRAMAS DE SEÑAL\n==================\n"); fclose(f); }

    char *enc;
    
    enc = encode_nrz(data_short); plot_signal(enc, "results/signals.txt", "NRZ"); free(enc);
    enc = encode_nrzi(data_short); plot_signal(enc, "results/signals.txt", "NRZI"); free(enc);
    enc = encode_manchester(data_short); plot_signal(enc, "results/signals.txt", "Manchester"); free(enc);
    enc = encode_4b5b(data_short); plot_signal(enc, "results/signals.txt", "4B/5B"); free(enc);
    enc = encode_custom(data_short); plot_signal(enc, "results/signals.txt", "Custom (Diff-8)"); free(enc);
    
    printf("Diagramas generados en results/signals.txt\n\n");
    
    // 2. Overhead Analysis
    printf("### 1. Eficiencia y Overhead (Mensaje 80 bits)\n");
    printf("| Esquema    | Bits In | Bits Out | Overhead | Eficiencia |\n");
    printf("|------------|---------|----------|----------|------------|\n");
    
    struct { char *name; char* (*func)(const char*); } schemes[] = {
        {"NRZ", encode_nrz},
        {"NRZI", encode_nrzi},
        {"Manchester", encode_manchester},
        {"4B/5B", encode_4b5b},
        {"Custom", encode_custom}
    };
    
    for(int i=0; i<5; i++) {
        char *e = schemes[i].func(data_long);
        int in = strlen(data_long);
        int out = strlen(e);
        double overhead = ((double)(out - in) / in) * 100;
        double eff = ((double)in / out) * 100;
        printf("| %-10s | %7d | %8d | %7.1f%% | %9.1f%% |\n", schemes[i].name, in, out, overhead, eff);
        free(e);
    }
    printf("\n");
    
    // 3. Statistical Error Report (Fixed BER)
    printf("### 2. Analisis Estadistico de Errores (BER=%.2f, N=%d)\n", MY_BER, NUM_SIMULATIONS);
    printf("| Esquema    | Media  | StdDev | Min | Max |\n");
    printf("|------------|--------|--------|-----|-----|\n");
    
    run_simulation("NRZ", encode_nrz, decode_nrz, data_long, MY_BER, NUM_SIMULATIONS, NULL);
    run_simulation("NRZI", encode_nrzi, decode_nrzi, data_long, MY_BER, NUM_SIMULATIONS, NULL);
    run_simulation("Manchester", encode_manchester, decode_manchester, data_long, MY_BER, NUM_SIMULATIONS, NULL);
    run_simulation("4B/5B", encode_4b5b, decode_4b5b, data_long, MY_BER, NUM_SIMULATIONS, NULL);
    run_simulation("Custom", encode_custom, decode_custom, data_long, MY_BER, NUM_SIMULATIONS, NULL);
    printf("\n");
    
    // 4. BER Curve Analysis
    printf("Generando curva BER vs Tasa de Error Efectiva...\n");
    FILE *csv = fopen("results/ber_curve.csv", "w");
    if(csv) fprintf(csv, "Scheme,InjectBER,MeanErrors\n");
    
    for(double b = 0.001; b <= 0.1; b *= 1.5) { // Escala logarítmica aprox
        run_simulation("NRZ", encode_nrz, decode_nrz, data_long, b, 100, csv); // Menos simulaciones para velocidad
        run_simulation("Manchester", encode_manchester, decode_manchester, data_long, b, 100, csv);
    }
    if(csv) fclose(csv);
    printf("Datos exportados a results/ber_curve.csv\n\n");
    
    // 5. Burst Noise Test
    printf("### 3. Analisis de Rafagas (Burst Noise)\n");
    printf("Probabilidad de rafaga: 10%%\n");
    char *enc_burst = encode_custom(data_long);
    // Simular error de rafaga simple
    int hits = 0;
    for(int i=0; i<100; i++) {
        char *c = my_strdup(enc_burst);
        add_burst_noise(c, 0.1); // 10% chance
        char *d = decode_custom(c);
        if(count_errors(data_long, d) == 0) hits++;
        free(c); free(d);
    }
    printf("Custom Scheme decodifico correctamente %d/100 veces con ruido de rafaga presente.\n", hits);
    free(enc_burst);

    return 0;
}