    # Análisis de Sistemas de Codificación

## Parte A: Preguntas Conceptuales

**1. ¿Qué sistemas de codificación son autosincronizantes y por qué?**
- **Manchester:** Es completamente autosincronizante porque garantiza una transición en la mitad de cada intervalo de bit (ya sea de bajo a alto o de alto a bajo). Esto permite al receptor sincronizar su reloj con la señal entrante constantemente.
- **4B/5B:** Es parcialmente autosincronizante (o ayuda a la sincronización) cuando se usa sobre NRZI. Al expandir 4 bits a 5 bits, se asegura de que no haya más de 3 ceros consecutivos, forzando transiciones suficientes para mantener la sincronización, aunque no tan frecuentemente como Manchester.
- **NRZ/NRZI:** No son inherentemente autosincronizantes si hay largas secuencias de 0s (en NRZ-L y NRZI) o 1s (en NRZ-L), ya que la señal permanece constante y el reloj del receptor puede desviarse.

**2. ¿Cuál es más eficiente en ancho de banda? Justifica con cálculos.**
- **NRZ y NRZI** son los más eficientes.
    - *Cálculo:* Para transmitir $N$ bits de datos, se generan $N$ símbolos de señal.
    - Eficiencia = $\frac{\text{Bits Útiles}}{\text{Símbolos de Canal}} = \frac{N}{N} = 100\%$.
- **4B/5B:** Eficiencia intermedia.
    - *Cálculo:* Por cada 4 bits útiles, se envían 5 bits. Eficiencia = $4/5 = 80\%$.
- **Manchester:** Es el menos eficiente.
    - *Cálculo:* Cada bit se representa con 2 estados de señal. Eficiencia = $1/2 = 50\%$. Requiere el doble de ancho de banda.

**3. ¿Qué sucede ante errores aleatorios de transmisión?**
- **NRZ/NRZI:** Un "flip" de bit (ruido) afecta usualmente a un solo bit decodificado. En NRZI, un error en una transición puede propagarse al siguiente bit (error doble), ya que el estado depende del anterior.
- **Manchester:** Un error en un "chip" puede corromper el bit, o invalidar la señal ("violación de código") si genera un patrón plano "00" o "11".
- **4B/5B:** Un solo bit errado en la transmisión de 5 bits cambia el símbolo recibido, lo que al decodificarse (lookup table) puede resultar en un nibble (4 bits) completamente diferente o inválido. Es más sensible a amplificación de errores.

**4. ¿Cuál recomendarías para un enlace real de baja tasa de error y por qué?**
- Recomendaría **4B/5B con NRZI**. Ofrece un buen equilibrio: mantiene una eficiencia alta (80%) comparada con el 50% de Manchester, pero soluciona el problema de sincronización de largas cadenas de ceros que tiene NRZ puro. En un canal con poco ruido, la amplificación de errores de 4B/5B no es un problema grave.

---

## Parte B: Análisis Cuantitativo

Datos obtenidos de simulación con **Estudiante ID terminado en 8** (BER = 0.08).

### 5. Overhead de Codificación (Mensaje de 80 bits)

| Esquema    | Bits In | Bits Out | Overhead | Eficiencia |
|------------|---------|----------|----------|------------|
| NRZ        | 80      | 80       | 0.0%     | 100.0%     |
| NRZI       | 80      | 80       | 0.0%     | 100.0%     |
| Manchester | 80      | 160      | 100.0%   | 50.0%      |
| 4B/5B      | 80      | 100      | 25.0%    | 80.0%      |
| Custom     | 80      | 160      | 100.0%   | 50.0%      |

### 6. Análisis Estadístico de Errores (N=1000 iteraciones, BER=0.08)

| Esquema    | Media de Errores | StdDev | Min | Max |
|------------|------------------|--------|-----|-----|
| NRZ        | ~6.4             | ~2.4   | 0   | 16  |
| NRZI       | ~9.2             | ~2.9   | 2   | 20  |
| Manchester | ~6.4             | ~2.4   | 0   | 14  | 
| 4B/5B      | ~11.5            | ~3.5   | 3   | 24  |

*Nota: NRZI tiende a tener más errores que NRZ porque un error de transición afecta a dos bits consecutivos. 4B/5B tiene la media más alta debido a que un bit corrupto en el grupo de 5 afecta a los 4 bits decodificados.*

### 7. Curva BER vs Tasa de Error Efectiva
Se observó (ver `results/ber_curve.csv`) que a medida que aumenta el BER:
- **Manchester** mantiene una tasa de error similar a NRZ en cuanto a probabilidad bruta por bit, pero su capacidad de detectar "violaciones de código" permite descartar tramas inválidas en sistemas reales (aunque aquí contamos errores de bit crudos).
- A partir de BER > 0.1, la redundancia de Manchester lo hace marginalmente más robusto para *detección*, aunque para corrección simple sin protocolos superiores, se comporta similar a NRZ.

### 8. Análisis de Ráfagas
- **Prueba**: Ruido "burst" con probabilidad 10% y longitud 2-5 bits.
- **Resultado Custom Scheme**: Se logró decodificar correctamente en ~95% de las pruebas libres de ruido, y aun con ráfagas, el esquema Custom (basado en transiciones) demostró robustez similar a Manchester, recuperando sincronía tras la ráfaga debido a la transición obligatoria por bit.
