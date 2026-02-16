#include "ejercicio1_rgb_hsv.h"

// ============================================================
// EJERCICIO 1: Conversión manual BGR → HSV píxel por píxel
// ============================================================
// Objetivo: Demostrar cómo se convierte cada píxel de BGR a HSV
//           aplicando las fórmulas matemáticas sin usar cvtColor()
//
// Proceso por cada píxel:
//   1. Extraer B, G, R y normalizar a [0,1]
//   2. Cmax = max(r,g,b), Cmin = min(r,g,b), Δ = Cmax - Cmin
//   3. H depende de cuál canal es el máximo
//   4. S = Δ/Cmax (0 si Cmax=0)
//   5. V = Cmax
//   6. Escalar a rangos OpenCV: H/2, S*255, V*255
// ============================================================
void ejercicio1_rgb_a_hsv() {
    Mat img_bgr = cargar_imagen("imagen.jpg");
    if (img_bgr.empty()) return;

    int rows = img_bgr.rows;
    int cols = img_bgr.cols;

    // Crear matriz de salida HSV con mismas dimensiones
    Mat img_hsv(rows, cols, CV_8UC3);

    // Recorrer cada píxel de la imagen
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Leer el píxel BGR original
            Vec3b pixel_bgr = img_bgr.at<Vec3b>(i, j);

            // PASO 1: Obtener valores BGR y normalizar a [0, 1]
            double b = pixel_bgr[0] / 255.0;
            double g = pixel_bgr[1] / 255.0;
            double r = pixel_bgr[2] / 255.0;

            // PASO 2: Calcular Cmax, Cmin y Delta
            double cmax = max({r, g, b});
            double cmin = min({r, g, b});
            double delta = cmax - cmin;

            // PASO 3: Calcular Hue (H) en grados [0, 360]
            double h = 0.0;
            if (delta == 0) {
                h = 0;  // Gris puro, sin tono definido
            } else if (cmax == r) {
                h = 60.0 * fmod(((g - b) / delta), 6.0);
                if (h < 0) h += 360.0;
            } else if (cmax == g) {
                h = 60.0 * (((b - r) / delta) + 2.0);
            } else if (cmax == b) {
                h = 60.0 * (((r - g) / delta) + 4.0);
            }

            // PASO 4: Calcular Saturation (S)
            double s = 0.0;
            if (cmax != 0) {
                s = delta / cmax;
            }

            // PASO 5: Calcular Value (V)
            double v = cmax;

            // PASO 6: Escalar a rangos de OpenCV y asignar
            int h_opencv = max(0, min(180, (int)(h / 2.0)));
            int s_opencv = max(0, min(255, (int)(s * 255.0)));
            int v_opencv = max(0, min(255, (int)(v * 255.0)));

            img_hsv.at<Vec3b>(i, j) = Vec3b((uchar)h_opencv,
                                              (uchar)s_opencv,
                                              (uchar)v_opencv);
        }
    }

    // Mostrar resultados
    imshow("Original BGR", img_bgr);
    imshow("HSV (Manual)", img_hsv);
    cout << "Presione cualquier tecla para cerrar las ventanas..." << endl;
    waitKey(0);
    destroyAllWindows();
}
