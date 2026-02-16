#include "ejercicio7_vignette.h"

// ============================================================
// EJERCICIO 7: Corrección de Viñeteo
// ============================================================
// Objetivo: Corregir el oscurecimiento que ocurre en las esquinas
//           de una imagen debido a limitaciones ópticas del lente
//
// Fórmulas:
//   1. Centro de imagen: cx = ancho/2, cy = alto/2
//   2. Distancia al centro: d = sqrt((x-cx)² + (y-cy)²)
//   3. Distancia normalizada: d_norm = d / d_max
//   4. Factor de corrección: f = 1 / (1 - k × d_norm²)
//   5. Píxel corregido = min(píxel_original × f, 255)
//
// El coeficiente k controla qué tan fuerte es la corrección:
//   k = 0.3 → corrección suave
//   k = 0.5 → corrección fuerte
//   k típico → 0.3 a 0.5
// ============================================================
void ejercicio7_vignette(double k) {
    Mat img_bgr = cargar_imagen("imagen.jpg");
    if (img_bgr.empty()) return;

    int rows = img_bgr.rows;
    int cols = img_bgr.cols;

    cout << "Correccion de vineteo con k = " << k << endl;

    // ---- PASO 1: Calcular centro de la imagen ----
    double cx = cols / 2.0;
    double cy = rows / 2.0;

    // ---- PASO 2: Calcular distancia máxima (centro a esquina) ----
    double d_max = sqrt(cx * cx + cy * cy);

    cout << "Centro de imagen: (" << cx << ", " << cy << ")" << endl;
    cout << "Distancia maxima (a esquina): " << d_max << endl;

    // ---- PASO 3: Aplicar corrección píxel por píxel ----
    Mat img_resultado(rows, cols, CV_8UC3);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Vec3b pixel = img_bgr.at<Vec3b>(i, j);

            // Calcular distancia del píxel (j, i) al centro
            double dx = j - cx;
            double dy = i - cy;
            double d = sqrt(dx * dx + dy * dy);

            // Normalizar la distancia
            double d_norm = d / d_max;

            // Calcular factor de corrección
            // f = 1 / (1 - k × d_norm²)
            // Cuando d_norm=0 (centro): f=1 (sin cambio)
            // Cuando d_norm→1 (esquinas): f > 1 (se aclara)
            double factor = 1.0 / (1.0 - k * d_norm * d_norm);

            // Aplicar corrección a cada canal
            int nuevo_b = (int)min(pixel[0] * factor, 255.0);
            int nuevo_g = (int)min(pixel[1] * factor, 255.0);
            int nuevo_r = (int)min(pixel[2] * factor, 255.0);

            img_resultado.at<Vec3b>(i, j) = Vec3b((uchar)nuevo_b,
                                                    (uchar)nuevo_g,
                                                    (uchar)nuevo_r);
        }
    }

    // Mostrar resultados
    imshow("Original", img_bgr);
    imshow("Vineteo Corregido k=" + to_string(k), img_resultado);
    cout << "Presione cualquier tecla para cerrar las ventanas..." << endl;
    waitKey(0);
    destroyAllWindows();
}
