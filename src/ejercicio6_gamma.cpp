#include "ejercicio6_gamma.h"

// ============================================================
// EJERCICIO 6: Corrección Gamma con tabla de lookup
// ============================================================
// Objetivo: Aplicar corrección gamma para ajustar brillo/contraste
//
// Fórmula: pixel_out = 255 × (pixel_in / 255) ^ gamma
//
//   gamma < 1 → Aclara la imagen (sombras se elevan)
//   gamma = 1 → Sin cambio
//   gamma > 1 → Oscurece la imagen (luces se comprimen)
//
// Optimización: Se usa una tabla de lookup (LUT) que pre-calcula
//   la transformación para todos los 256 valores posibles.
//   Así evitamos calcular pow() para cada píxel individualmente.
// ============================================================
void ejercicio6_gamma(double gamma) {
    Mat img_bgr = cargar_imagen("imagen.jpg");
    if (img_bgr.empty()) return;

    int rows = img_bgr.rows;
    int cols = img_bgr.cols;

    cout << "Aplicando correccion gamma = " << gamma << endl;
    if (gamma < 1.0) cout << "  (La imagen se aclarara)" << endl;
    else if (gamma > 1.0) cout << "  (La imagen se oscurecera)" << endl;
    else cout << "  (Sin cambio)" << endl;

    // ---- PASO 1: Crear tabla de lookup (para eficiencia) ----
    // Pre-calcular la transformación gamma para valores 0-255
    uchar tabla_gamma[256];
    for (int i = 0; i < 256; i++) {
        // Fórmula: pixel_out = 255 × (pixel_in / 255) ^ gamma
        double normalizado = i / 255.0;
        double transformado = pow(normalizado, gamma);
        int valor = (int)(transformado * 255.0);

        // Clamp al rango [0, 255]
        tabla_gamma[i] = (uchar)max(0, min(255, valor));
    }

    // ---- PASO 2: Aplicar transformación a cada píxel ----
    Mat img_resultado(rows, cols, CV_8UC3);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Vec3b pixel = img_bgr.at<Vec3b>(i, j);

            // Aplicar la tabla de lookup a cada canal B, G, R
            uchar nuevo_b = tabla_gamma[pixel[0]];
            uchar nuevo_g = tabla_gamma[pixel[1]];
            uchar nuevo_r = tabla_gamma[pixel[2]];

            img_resultado.at<Vec3b>(i, j) = Vec3b(nuevo_b, nuevo_g, nuevo_r);
        }
    }

    // Mostrar resultados
    imshow("Original", img_bgr);
    imshow("Gamma = " + to_string(gamma), img_resultado);
    cout << "Presione cualquier tecla para cerrar las ventanas..." << endl;
    waitKey(0);
    destroyAllWindows();
}
