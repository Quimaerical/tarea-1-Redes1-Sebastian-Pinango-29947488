#include "encoding.h"

// Helper portable strdup
char *my_strdup(const char *s) {
    if (!s) return NULL;
    char *d = malloc(strlen(s) + 1);
    if (d) strcpy(d, s);
    return d;
}

// --- NRZ (Non-Return to Zero Level) ---
// Mapeo directo: '1' -> '1' (High), '0' -> '0' (Low)
char *encode_nrz(const char *bitstream) {
    if (!bitstream) return NULL;
    return my_strdup(bitstream); // En NRZ-L la salida es igual a la entrada lógica
}

char *decode_nrz(const char *encoded) {
    if (!encoded) return NULL;
    return my_strdup(encoded);
}

// --- NRZI (Non-Return to Zero Inverted) ---
// '1' provoca transición, '0' mantiene nivel anterior. Asumimos nivel inicial Low ('0').
char *encode_nrzi(const char *bitstream) {
    int len = strlen(bitstream);
    char *out = (char *)malloc(len + 1);
    char current_level = '0'; // Estado inicial

    for (int i = 0; i < len; i++) {
        if (bitstream[i] == '1') {
            // Invertir estado
            current_level = (current_level == '0') ? '1' : '0';
        }
        // Si es '0', current_level se mantiene
        out[i] = current_level;
    }
    out[len] = '\0';
    return out;
}

char *decode_nrzi(const char *encoded) {
    int len = strlen(encoded);
    char *out = (char *)malloc(len + 1);
    char current_level = '0'; // Estado inicial conocido

    for (int i = 0; i < len; i++) {
        if (encoded[i] != current_level) {
            out[i] = '1'; // Hubo cambio -> era un 1
            current_level = encoded[i];
        } else {
            out[i] = '0'; // No hubo cambio -> era un 0
        }
    }
    out[len] = '\0';
    return out;
}

// --- Manchester ---
// '0' -> "10" (Alto a Bajo), '1' -> "01" (Bajo a Alto) - Convención IEEE
char *encode_manchester(const char *bitstream) {
    int len = strlen(bitstream);
    char *out = (char *)malloc((len * 2) + 1);
    
    for (int i = 0; i < len; i++) {
        if (bitstream[i] == '0') {
            out[2*i] = '1';
            out[2*i+1] = '0';
        } else {
            out[2*i] = '0';
            out[2*i+1] = '1';
        }
    }
    out[len * 2] = '\0';
    return out;
}

char *decode_manchester(const char *encoded) {
    int len = strlen(encoded);
    if (len % 2 != 0) return NULL; // Error de alineación
    
    char *out = (char *)malloc((len / 2) + 1);
    
    for (int i = 0; i < len; i += 2) {
        // "10" representa 0, "01" representa 1
        if (encoded[i] == '1' && encoded[i+1] == '0') {
            out[i/2] = '0';
        } else if (encoded[i] == '0' && encoded[i+1] == '1') {
            out[i/2] = '1';
        } else {
            // Violación de código (ej. "00" o "11"), manejamos como error o default
            out[i/2] = '?'; 
        }
    }
    out[len / 2] = '\0';
    return out;
}

// --- 4B/5B ---
// Tablas de búsqueda estáticas
static const char *codes_4b[] = {
    "11110", "01001", "10100", "10101", // 0-3
    "01010", "01011", "01110", "01111", // 4-7
    "10010", "10011", "10110", "10111", // 8-B
    "11010", "11011", "11100", "11101"  // C-F
};

// Función auxiliar para obtener el índice de un nibble hexadecimal
int hex_char_to_int(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0; 
}

// Convertir binario a entero (para bloques de 4 bits)
int bin4_to_int(const char *b) {
    int val = 0;
    for(int i=0; i<4; i++) {
        val = (val << 1) | (b[i] - '0');
    }
    return val;
}

char *encode_4b5b(const char *bitstream) {
    int len = strlen(bitstream);
    // Asumimos que la entrada es múltiplo de 4 para simplificar
    // En producción se haría padding.
    int groups = len / 4; 
    char *out = (char *)malloc((groups * 5) + 1);
    out[0] = '\0';

    for (int i = 0; i < groups; i++) {
        int val = bin4_to_int(bitstream + (i * 4));
        strcat(out, codes_4b[val]);
    }
    return out;
}

char *decode_4b5b(const char *encoded) {
    // La decodificación requiere búsqueda inversa en la tabla.
    // Por brevedad, aquí se implementa una búsqueda lineal simple.
    int len = strlen(encoded);
    int groups = len / 5;
    char *out = (char *)malloc((groups * 4) + 1);
    char *ptr = out;

    for (int i = 0; i < groups; i++) {
        char chunk[6];
        strncpy(chunk, encoded + (i*5), 5);
        chunk[5] = '\0';

        int found = -1;
        for (int j = 0; j < 16; j++) {
            if (strcmp(chunk, codes_4b[j]) == 0) {
                found = j;
                break;
            }
        }
        
        // Convertir 'found' (int 0-15) de vuelta a 4 bits "0000"-"1111"
        for (int b = 3; b >= 0; b--) {
            *ptr++ = ((found >> b) & 1) ? '1' : '0';
        }
    }
    *ptr = '\0';
    return out;
}

// --- CUSTOM SCHEME: Diff-8 (Differential Biphase Mark) Similar a NRZI---
// Restricción de mi cedula que termina en 8: Resistente a inversión de polaridad.

char *encode_custom(const char *bitstream) {
    int len = strlen(bitstream);
    // Cada bit se convierte en 2 "chips" (simbolos de señal)
    char *out = (char *)malloc((len * 2) + 1);
    
    // Estado inicial arbitrario (puede ser '0' o '1')
    char current_level = '0'; 

    for (int i = 0; i < len; i++) {
        // 1. Transición OBLIGATORIA al inicio del intervalo
        current_level = (current_level == '0') ? '1' : '0';
        out[2 * i] = current_level;

        // 2. Si es '1', hay OTRA transición a mitad del intervalo.
        //    Si es '0', se mantiene el nivel.
        if (bitstream[i] == '1') {
            current_level = (current_level == '0') ? '1' : '0';
        }
        out[2 * i + 1] = current_level;
    }
    out[len * 2] = '\0';
    return out;
}

char *decode_custom(const char *encoded) {
    int len = strlen(encoded);
    if (len % 2 != 0) return NULL; // Error de alineación

    char *out = (char *)malloc((len / 2) + 1);
    
    for (int i = 0; i < len; i += 2) {
        char first_half = encoded[i];
        char second_half = encoded[i+1];

        // Lógica de decodificación resistente a polaridad:
        // Comparamos las dos mitades del periodo.
        if (first_half != second_half) {
            // Si son diferentes ("10" o "01"), hubo transición en el medio -> '1'
            out[i/2] = '1';
        } else {
            // Si son iguales ("00" o "11"), no hubo transición en el medio -> '0'
            out[i/2] = '0';
        }
    }
    out[len / 2] = '\0';
    return out;
}

// --- HERRAMIENTAS DE VISUALIZACIÓN Y RUIDO ---

void plot_signal(const char *encoded, const char *filename, const char *label) {
    if (!encoded) return;

    FILE *f = fopen(filename, "a");
    if (!f) return;

    fprintf(f, "\n%s\n", label);
    fprintf(f, "Tiempo: ");
    int len = strlen(encoded);
    for (int i = 0; i < len; i++) {
        fprintf(f, "%4d", i);
    }
    fprintf(f, "\nSeñal:  ");

    // Dibujo simple: ALTO para '1', BAJO para '0'
    for (int i = 0; i < len; i++) {
        if (encoded[i] == '1') {
            fprintf(f, "----");
        } else {
            fprintf(f, "____");
        }
    }
    fprintf(f, "\n\n");
    fclose(f);
}

void add_noise(char *bitstream, double ber) {
    if (!bitstream) return;
    int len = strlen(bitstream);
    
    // Asumimos srand() ya fue llamado en main()
    for (int i = 0; i < len; i++) {
        double r = (double)rand() / (double)RAND_MAX;
        if (r < ber) {
            // Invertir bit (Error)
            bitstream[i] = (bitstream[i] == '0') ? '1' : '0';
        }
    }
}

void add_burst_noise(char *bitstream, double prob_burst) {
    if (!bitstream) return;
    int len = strlen(bitstream);
    double r = (double)rand() / (double)RAND_MAX;

    // Si ocurre ráfaga
    if (r < prob_burst) {
        // Longitud de ráfaga aleatoria entre 2 y 5 bits
        int burst_len = 2 + (rand() % 4); 
        // Evitar desbordamiento
        if (burst_len > len) burst_len = len;

        // Posición inicio segura
        int max_start = len - burst_len;
        int start_pos = (max_start > 0) ? (rand() % max_start) : 0;
        
        for (int i = 0; i < burst_len; i++) {
             bitstream[start_pos + i] = (bitstream[start_pos + i] == '0') ? '1' : '0';
        }
    }
}