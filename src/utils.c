#include "encoding.h"

// Genera un diagrama ASCII. 
// "1" se dibuja arriba (----), "0" se dibuja abajo (____).
void plot_signal(const char *encoded, const char *filename, const char *label) {
    FILE *f = fopen(filename, "a"); // Append mode
    if (!f) return;

    fprintf(f, "\n=== %s ===\n", label);
    
    int len = strlen(encoded);
    
    // Eje de tiempo
    fprintf(f, "Tiempo: ");
    for (int i = 0; i < len; i++) fprintf(f, "%-2d", i % 10);
    fprintf(f, "\n");

    // Señal
    fprintf(f, "Señal:  ");
    char prev = ' ';
    
    for (int i = 0; i < len; i++) {
        if (encoded[i] == '1') {
            // Transición de abajo a arriba si el anterior era 0
            if (i > 0 && prev == '0') fprintf(f, "|"); 
            else if (i > 0) fprintf(f, " ");
            fprintf(f, "-");
        } else {
            // Transición de arriba a abajo si el anterior era 1
            if (i > 0 && prev == '1') fprintf(f, "|");
            else if (i > 0) fprintf(f, " ");
            fprintf(f, "_");
        }
        prev = encoded[i];
    }
    fprintf(f, "\n\n");
    fclose(f);
}

// Simula ruido invirtiendo bits según probabilidad BER
void add_noise(char *bitstream, double ber) {
    int len = strlen(bitstream);
    for (int i = 0; i < len; i++) {
        // Generar un número aleatorio entre 0 y 1
        double r = (double)rand() / (double)RAND_MAX;
        if (r < ber) {
            // Invertir bit (Error)
            bitstream[i] = (bitstream[i] == '0') ? '1' : '0';
        }
    }
}