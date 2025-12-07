#include "encoding.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ============================================
// NRZ (Non-Return to Zero)
// ============================================

char *encode_nrz(const char *bitstream) {
    // TODO: Implementar
    // En NRZ: '1' = nivel alto, '0' = nivel bajo
    return NULL;
}

char *decode_nrz(const char *encoded) {
    // TODO: Implementar
    return NULL;
}

// ============================================
// NRZI (Non-Return to Zero Inverted)
// ============================================

char *encode_nrzi(const char *bitstream) {
    // TODO: Implementar
    // En NRZI: '1' = transición, '0' = sin transición
    return NULL;
}

char *decode_nrzi(const char *encoded) {
    // TODO: Implementar
    return NULL;
}

// ============================================
// Manchester
// ============================================

char *encode_manchester(const char *bitstream) {
    // TODO: Implementar
    // En Manchester: '0' = transición bajo->alto, '1' = transición alto->bajo
    // (o viceversa según convención IEEE/Thomas)
    return NULL;
}

char *decode_manchester(const char *encoded) {
    // TODO: Implementar
    return NULL;
}

// ============================================
// 4B/5B
// ============================================

char *encode_4b5b(const char *bitstream) {
    // TODO: Implementar
    // Cada grupo de 4 bits se convierte en 5 bits según tabla estándar
    return NULL;
}

char *decode_4b5b(const char *encoded) {
    // TODO: Implementar
    return NULL;
}

// ============================================
// Visualización de señales
// ============================================

void plot_signal(const char *encoded, const char *filename) {
    // TODO: Implementar
    // Genera diagrama ASCII de la señal y lo guarda en el archivo
    (void)encoded;
    (void)filename;
}

// ============================================
// Simulación de ruido
// ============================================

void add_noise(char *bitstream, double ber) {
    // TODO: Implementar
    // Invierte cada bit con probabilidad 'ber'
    (void)bitstream;
    (void)ber;
}