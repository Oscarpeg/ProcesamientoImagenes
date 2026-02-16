#ifndef UTILS_H
#define UTILS_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace cv;
using namespace std;

// ============================================================
// Estructura para manejar píxeles con valores double (K-Means)
// ============================================================
struct Pixel {
    double r, g, b;
    Pixel() : r(0), g(0), b(0) {}
    Pixel(double r_, double g_, double b_) : r(r_), g(g_), b(b_) {}
};

// ============================================================
// Funciones de conversión reutilizables
// ============================================================

// Convierte un píxel BGR (Vec3b) a HSV (Vec3b) usando las fórmulas manuales
Vec3b bgr_a_hsv(const Vec3b& pixel_bgr);

// Convierte un píxel HSV (Vec3b) a BGR (Vec3b) usando las fórmulas inversas
Vec3b hsv_a_bgr(const Vec3b& pixel_hsv);

// Convierte una imagen completa BGR a HSV píxel por píxel
Mat convertir_bgr_a_hsv(const Mat& img_bgr);

// Convierte una imagen completa HSV a BGR píxel por píxel
Mat convertir_hsv_a_bgr(const Mat& img_hsv);

// Distancia euclidiana entre dos píxeles en espacio RGB
double distancia_euclidiana(const Pixel& p1, const Pixel& p2);

// Carga una imagen y verifica que no esté vacía
Mat cargar_imagen(const string& ruta);

#endif // UTILS_H
