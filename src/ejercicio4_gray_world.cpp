#include "ejercicio4_gray_world.h"

// ============================================================
// EJERCICIO 4: Constancia de Color - Algoritmo Gray World
// ============================================================
// Objetivo: Corregir el balance de blancos de una imagen
//           asumiendo que el promedio de la escena debería ser gris
//
// Fórmulas Gray World:
//   1. avg_R, avg_G, avg_B = promedio de cada canal
//   2. gray_avg = (avg_R + avg_G + avg_B) / 3
//   3. Factores de escala: scale_X = gray_avg / avg_X
//   4. Píxel nuevo = min(píxel_viejo × scale, 255)
//
// Intuición: Si una foto tiene un tinte azulado (promedio B alto),
//            el factor para B será menor que 1, reduciendo el azul.
//            Los otros canales se aumentan para compensar.
// ============================================================
void ejercicio4_gray_world() {
    Mat img_bgr = cargar_imagen("imagen.jpg");
    if (img_bgr.empty()) return;

    int rows = img_bgr.rows;
    int cols = img_bgr.cols;
    int total_pixels = rows * cols;

    // ---- PASO 1: Calcular suma de cada canal ----
    double suma_b = 0.0, suma_g = 0.0, suma_r = 0.0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Vec3b pixel = img_bgr.at<Vec3b>(i, j);
            suma_b += pixel[0];
            suma_g += pixel[1];
            suma_r += pixel[2];
        }
    }

    // ---- PASO 2: Calcular promedios ----
    double avg_b = suma_b / total_pixels;
    double avg_g = suma_g / total_pixels;
    double avg_r = suma_r / total_pixels;

    cout << "Promedios originales -> R: " << avg_r
         << "  G: " << avg_g << "  B: " << avg_b << endl;

    // ---- PASO 3: Calcular promedio gris ----
    double gray_avg = (avg_r + avg_g + avg_b) / 3.0;
    cout << "Promedio gris objetivo: " << gray_avg << endl;

    // ---- PASO 4: Calcular factores de escala ----
    double scale_r = gray_avg / avg_r;
    double scale_g = gray_avg / avg_g;
    double scale_b = gray_avg / avg_b;

    cout << "Factores de escala -> R: " << scale_r
         << "  G: " << scale_g << "  B: " << scale_b << endl;

    // ---- PASO 5: Crear imagen corregida ----
    Mat img_resultado(rows, cols, CV_8UC3);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Vec3b pixel = img_bgr.at<Vec3b>(i, j);

            // Aplicar factores de escala con clamp a [0, 255]
            int nuevo_b = (int)min(pixel[0] * scale_b, 255.0);
            int nuevo_g = (int)min(pixel[1] * scale_g, 255.0);
            int nuevo_r = (int)min(pixel[2] * scale_r, 255.0);

            img_resultado.at<Vec3b>(i, j) = Vec3b((uchar)nuevo_b,
                                                    (uchar)nuevo_g,
                                                    (uchar)nuevo_r);
        }
    }

    // Mostrar resultados
    imshow("Original", img_bgr);
    imshow("Gray World", img_resultado);
    cout << "Presione cualquier tecla para cerrar las ventanas..." << endl;
    waitKey(0);
    destroyAllWindows();
}
