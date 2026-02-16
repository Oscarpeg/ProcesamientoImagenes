#include "ejercicio2_saturacion.h"

// ============================================================
// EJERCICIO 2: Modificar saturación manualmente
// ============================================================
// Objetivo: Aumentar la saturación de una imagen trabajando
//           directamente con los valores numéricos HSV
//
// Proceso:
//   1. Convertir BGR → HSV (usando fórmulas del ejercicio 1)
//   2. Para cada píxel, multiplicar S por un factor (1.5)
//   3. Asegurar que S no exceda 255 (clamp)
//   4. Convertir HSV → BGR (fórmulas inversas)
//   5. Mostrar imagen original vs imagen con saturación aumentada
// ============================================================
void ejercicio2_modificar_saturacion() {
    Mat img_bgr = cargar_imagen("imagen.jpg");
    if (img_bgr.empty()) return;

    int rows = img_bgr.rows;
    int cols = img_bgr.cols;

    // Factor de saturación (el taller pide 1.5)
    double factor_saturacion = 1.5;

    cout << "Factor de saturacion aplicado: " << factor_saturacion << endl;

    // PASO 1: Convertir BGR → HSV usando la función del ejercicio 1
    Mat img_hsv = convertir_bgr_a_hsv(img_bgr);

    // PASO 2: Modificar la saturación de cada píxel
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Vec3b pixel_hsv = img_hsv.at<Vec3b>(i, j);

            // Obtener valores H, S, V
            int h = pixel_hsv[0];
            int s = pixel_hsv[1];
            int v = pixel_hsv[2];

            // Multiplicar S por el factor (sin exceder 255)
            int s_nuevo = (int)(s * factor_saturacion);
            s_nuevo = min(s_nuevo, 255);

            // Asignar nuevos valores (H y V no cambian)
            img_hsv.at<Vec3b>(i, j) = Vec3b((uchar)h,
                                              (uchar)s_nuevo,
                                              (uchar)v);
        }
    }

    // PASO 3: Convertir HSV → BGR usando las fórmulas inversas
    Mat img_resultado = convertir_hsv_a_bgr(img_hsv);

    // Mostrar resultados
    imshow("Original", img_bgr);
    imshow("Saturacion Aumentada (x1.5)", img_resultado);
    cout << "Presione cualquier tecla para cerrar las ventanas..." << endl;
    waitKey(0);
    destroyAllWindows();
}
