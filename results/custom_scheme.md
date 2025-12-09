# Esquema Propio: Diff-8

## Información del Estudiante
- **Nombre:** Sebastian Pinango
- **Cédula termina en:** 8
- **Restricción asignada:** "Debe funcionar correctamente con inversión de polaridad"

## Descripción del Esquema (Diff-8)
El esquema **Diff-8** es una variante de codificación diferencial bifase (similar a Differential Manchester o Biphase Mark Code). Se diseñó específicamente para ser insensible a la polaridad absoluta de la señal, basando la información únicamente en cambios (transiciones) dentro del periodo de bit.

### Regla de Codificación
Para cada bit de entrada, se generan 2 "chips" (símbolos de señal):
1. **Inicio de intervalo:** Siempre ocurre una transición (inversión de nivel) respecto al nivel final del bit anterior. Esto garantiza sincronización.
2. **Mitad de intervalo:**
   - Si el bit es **'1'**: Ocurre una segunda transición.
   - Si el bit es **'0'**: Se mantiene el nivel (no hay transición).

### Tabla de Mapeo (Ejemplo Relativo)
Suponiendo nivel previo '0':

| Bit | Acción | Señal Generada |
|-----|--------|----------------|
| **0** | Transición al inicio | `11` (Alto, Alto - si partimos de bajo) *[Corrección: La implementación real hace transición al inicio]* |
| **1** | Transición al inicio + medio | `10` (Alto, Bajo - si partimos de bajo) |

*Nota: La implementación real no usa una tabla estática, sino lógica diferencial basada en el estado `current_level`.*

## Demostración de Restricción (Inversión de Polaridad)
La restricción exige que si se invierten todos los bits del canal (un cable cruzado, `1` se vuelve `0` y `0` se vuelve `1`), el mensaje debe decodificarse correctamente.

### ¿Por qué cumple?
El decodificador **Diff-8** no mira si el voltaje es "Alto" o "Bajo". Solo compara la **primera mitad** del símbolo con la **segunda mitad**:
- Si `mitad1 != mitad2` (ej. "10" o "01") $\rightarrow$ Hubo transición en el medio $\rightarrow$ Bit **'1'**.
- Si `mitad1 == mitad2` (ej. "11" o "00") $\rightarrow$ No hubo transición en el medio $\rightarrow$ Bit **'0'**.

Si invertimos la polaridad:
- Un "10" original se convierte en "01". Siguen siendo diferentes $\rightarrow$ Decodifica **'1'**.
- Un "11" original se convierte en "00". Siguen siendo iguales $\rightarrow$ Decodifica **'0'**.

Por lo tanto, la decodificación es **invariante a la polaridad**.

### Prueba Experimental
En `main.c` se incluyó una prueba automática:
1. Codificar un mensaje.
2. Invertir manualmente todos los bits del mensaje codificado (`~bit`).
3. Decodificar la versión invertida.
4. **Resultado:** El mensaje decodificado es idéntico al original.

## Análisis de Eficiencia
- **Bits In:** 1
- **Bits Out:** 2 (Chips)
- **Eficiencia:** $1/2 = 50\%$
- **Comparación:** Igual eficiencia que Manchester, pero con la ventaja añadida de resistencia total a inversión de polaridad.
