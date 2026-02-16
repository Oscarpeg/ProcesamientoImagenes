#include "ejercicio3_kmeans.h"

// ============================================================
// EJERCICIO 3: K-Means desde cero (sin usar cv::kmeans())
// ============================================================
// Objetivo: Implementar el algoritmo K-Means completo para
//           cuantización de color en una imagen
//
// Algoritmo K-Means:
//   1. Seleccionar K centroides aleatorios del conjunto de píxeles
//   2. Asignar cada píxel al centroide más cercano (distancia euclidiana)
//   3. Recalcular centroides como promedio de los píxeles asignados
//   4. Repetir pasos 2-3 hasta convergencia o max iteraciones
//   5. Reemplazar cada píxel por el color de su centroide
//
// Distancia Euclidiana RGB:
//   d = sqrt((R1-R2)² + (G1-G2)² + (B1-B2)²)
// ============================================================
void ejercicio3_kmeans_manual(int K) {
    Mat img_bgr = cargar_imagen("imagen.jpg");
    if (img_bgr.empty()) return;

    // Redimensionar para acelerar el procesamiento
    Mat img_small;
    resize(img_bgr, img_small, Size(160, 120));

    int rows = img_small.rows;
    int cols = img_small.cols;
    int total_pixels = rows * cols;

    cout << "Procesando " << total_pixels << " pixeles con K=" << K << endl;

    // ---- PASO 1: Crear array de píxeles ----
    // Almacenar todos los píxeles de la imagen en un vector
    vector<Pixel> pixeles(total_pixels);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Vec3b p = img_small.at<Vec3b>(i, j);
            int idx = i * cols + j;
            // OpenCV usa BGR, guardamos como R, G, B
            pixeles[idx] = Pixel(p[2], p[1], p[0]);
        }
    }

    // ---- PASO 2: Inicializar K centroides aleatorios ----
    // Elegimos K píxeles aleatorios de la imagen como centroides iniciales
    srand(time(0));
    vector<Pixel> centroides(K);
    for (int k = 0; k < K; k++) {
        int idx_random = rand() % total_pixels;
        centroides[k] = pixeles[idx_random];
    }

    // ---- PASO 3: Array para almacenar asignaciones ----
    // Cada píxel se asigna a un cluster [0, K-1]
    vector<int> asignaciones(total_pixels, 0);

    // ---- PASO 4: Iterar K-Means ----
    int max_iteraciones = 20;

    for (int iter = 0; iter < max_iteraciones; iter++) {
        cout << "Iteracion " << (iter + 1) << "/" << max_iteraciones << endl;

        // PASO 4a: Asignar cada píxel al centroide más cercano
        for (int i = 0; i < total_pixels; i++) {
            double min_dist = distancia_euclidiana(pixeles[i], centroides[0]);
            int mejor_cluster = 0;

            for (int k = 1; k < K; k++) {
                double dist = distancia_euclidiana(pixeles[i], centroides[k]);
                if (dist < min_dist) {
                    min_dist = dist;
                    mejor_cluster = k;
                }
            }
            asignaciones[i] = mejor_cluster;
        }

        // PASO 4b: Recalcular centroides
        // Arrays para acumular suma RGB y contar píxeles por cluster
        vector<double> suma_r(K, 0.0);
        vector<double> suma_g(K, 0.0);
        vector<double> suma_b(K, 0.0);
        vector<int> conteo(K, 0);

        for (int i = 0; i < total_pixels; i++) {
            int cluster = asignaciones[i];
            suma_r[cluster] += pixeles[i].r;
            suma_g[cluster] += pixeles[i].g;
            suma_b[cluster] += pixeles[i].b;
            conteo[cluster]++;
        }

        // Nuevo centroide = promedio de todos los píxeles del cluster
        for (int k = 0; k < K; k++) {
            if (conteo[k] > 0) {
                centroides[k].r = suma_r[k] / conteo[k];
                centroides[k].g = suma_g[k] / conteo[k];
                centroides[k].b = suma_b[k] / conteo[k];
            }
        }
    }

    // ---- PASO 5: Crear imagen cuantizada ----
    // Reemplazar cada píxel por el color de su centroide
    Mat img_quantized(rows, cols, CV_8UC3);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int idx = i * cols + j;
            int cluster = asignaciones[idx];

            // Convertir centroide a BGR para OpenCV
            uchar B = (uchar)max(0.0, min(255.0, centroides[cluster].b));
            uchar G = (uchar)max(0.0, min(255.0, centroides[cluster].g));
            uchar R = (uchar)max(0.0, min(255.0, centroides[cluster].r));

            img_quantized.at<Vec3b>(i, j) = Vec3b(B, G, R);
        }
    }

    // ---- PASO 6: Crear paleta de colores ----
    // Mostrar los K colores resultantes como bloques
    int ancho_bloque = 60;
    int alto_paleta = 60;
    Mat paleta(alto_paleta, ancho_bloque * K, CV_8UC3);

    for (int k = 0; k < K; k++) {
        uchar B = (uchar)max(0.0, min(255.0, centroides[k].b));
        uchar G = (uchar)max(0.0, min(255.0, centroides[k].g));
        uchar R = (uchar)max(0.0, min(255.0, centroides[k].r));

        // Llenar el bloque del color k
        for (int i = 0; i < alto_paleta; i++) {
            for (int j = k * ancho_bloque; j < (k + 1) * ancho_bloque; j++) {
                paleta.at<Vec3b>(i, j) = Vec3b(B, G, R);
            }
        }

        cout << "Cluster " << k << ": R=" << (int)R
             << " G=" << (int)G << " B=" << (int)B << endl;
    }

    // Mostrar resultados
    imshow("Original", img_small);
    imshow("K-Means Manual K=" + to_string(K), img_quantized);
    imshow("Paleta de Colores", paleta);
    cout << "Presione cualquier tecla para cerrar las ventanas..." << endl;
    waitKey(0);
    destroyAllWindows();
}
