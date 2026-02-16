#include "ejercicio1_rgb_hsv.h"
#include "ejercicio2_saturacion.h"
#include "ejercicio3_kmeans.h"
#include "ejercicio4_gray_world.h"
#include "ejercicio6_gamma.h"
#include "ejercicio7_vignette.h"

int main() {
    cout << "========================================================" << endl;
    cout << "   TALLER DE PROCESAMIENTO DE IMAGENES - COLORES" << endl;
    cout << "========================================================\n" << endl;

    // --- Parte I: Conversion Manual de Espacios de Color ---
    cout << "=== Ejercicio 1: Conversion BGR -> HSV ===" << endl;
    ejercicio1_rgb_a_hsv();

    cout << "\n=== Ejercicio 2: Modificar Saturacion ===" << endl;
    ejercicio2_modificar_saturacion();

    // --- Parte II: K-Means Desde Cero ---
    cout << "\n=== Ejercicio 3: K-Means Manual (K=5) ===" << endl;
    ejercicio3_kmeans_manual(5);

    // --- Parte III: Constancia de Color Manual ---
    cout << "\n=== Ejercicio 4: Gray World ===" << endl;
    ejercicio4_gray_world();

    // --- Parte IV: Calibracion Radiometrica Manual ---
    cout << "\n=== Ejercicio 6: Correccion Gamma (gamma=1.5) ===" << endl;
    ejercicio6_gamma(1.5);

    cout << "\n=== Ejercicio 7: Correccion de Vineteo (k=0.4) ===" << endl;
    ejercicio7_vignette(0.4);

    cout << "\n========================================================" << endl;
    cout << "   Todos los ejercicios completados." << endl;
    cout << "========================================================" << endl;

    return 0;
}
