#include "utils.h"

// ============================================================
// Conversión de un píxel BGR → HSV (fórmulas manuales)
// ============================================================
// Pasos:
// 1. Normalizar B, G, R al rango [0, 1]
// 2. Calcular Cmax, Cmin y Delta
// 3. Calcular H (Hue) según qué canal es el máximo
// 4. Calcular S (Saturation) = Delta / Cmax
// 5. Calcular V (Value) = Cmax
// 6. Escalar a rangos de OpenCV: H=[0,180], S=[0,255], V=[0,255]
// ============================================================
Vec3b bgr_a_hsv(const Vec3b& pixel_bgr) {
    // Extraer canales BGR y normalizar a [0, 1]
    double b = pixel_bgr[0] / 255.0;
    double g = pixel_bgr[1] / 255.0;
    double r = pixel_bgr[2] / 255.0;

    // Calcular Cmax, Cmin y Delta
    double cmax = max({r, g, b});
    double cmin = min({r, g, b});
    double delta = cmax - cmin;

    // --- Calcular Hue (H) ---
    double h = 0.0;
    if (delta == 0) {
        h = 0;  // Sin color dominante, tono indefinido
    } else if (cmax == r) {
        // El rojo es dominante
        h = 60.0 * fmod(((g - b) / delta), 6.0);
        if (h < 0) h += 360.0;  // Asegurar que H sea positivo
    } else if (cmax == g) {
        // El verde es dominante
        h = 60.0 * (((b - r) / delta) + 2.0);
    } else if (cmax == b) {
        // El azul es dominante
        h = 60.0 * (((r - g) / delta) + 4.0);
    }

    // --- Calcular Saturation (S) ---
    double s = 0.0;
    if (cmax != 0) {
        s = delta / cmax;
    }

    // --- Calcular Value (V) ---
    double v = cmax;

    // --- Escalar a rangos de OpenCV ---
    // OpenCV usa H en [0, 180] (divide entre 2 porque uchar solo llega a 255)
    int h_opencv = (int)(h / 2.0);
    int s_opencv = (int)(s * 255.0);
    int v_opencv = (int)(v * 255.0);

    // Asegurar que los valores estén en rango válido
    h_opencv = max(0, min(180, h_opencv));
    s_opencv = max(0, min(255, s_opencv));
    v_opencv = max(0, min(255, v_opencv));

    return Vec3b((uchar)h_opencv, (uchar)s_opencv, (uchar)v_opencv);
}

// ============================================================
// Conversión de un píxel HSV → BGR (fórmulas inversas)
// ============================================================
// Pasos:
// 1. Desnormalizar H, S, V desde rangos de OpenCV
// 2. Calcular C = V × S, X, m
// 3. Según el sector de H (0-360°), asignar r', g', b'
// 4. Convertir a [0, 255] y ordenar como BGR
// ============================================================
Vec3b hsv_a_bgr(const Vec3b& pixel_hsv) {
    // Desnormalizar desde rangos de OpenCV
    double h = pixel_hsv[0] * 2.0;       // [0,180] → [0,360]
    double s = pixel_hsv[1] / 255.0;     // [0,255] → [0,1]
    double v = pixel_hsv[2] / 255.0;     // [0,255] → [0,1]

    // Calcular valores intermedios
    double c = v * s;                             // Chroma
    double x = c * (1.0 - fabs(fmod(h / 60.0, 2.0) - 1.0)); // Segundo componente
    double m = v - c;                             // Ajuste de brillo

    double r_prime = 0, g_prime = 0, b_prime = 0;

    // Según el sector del ángulo H, asignar componentes
    if (h >= 0 && h < 60) {
        r_prime = c;  g_prime = x;  b_prime = 0;
    } else if (h >= 60 && h < 120) {
        r_prime = x;  g_prime = c;  b_prime = 0;
    } else if (h >= 120 && h < 180) {
        r_prime = 0;  g_prime = c;  b_prime = x;
    } else if (h >= 180 && h < 240) {
        r_prime = 0;  g_prime = x;  b_prime = c;
    } else if (h >= 240 && h < 300) {
        r_prime = x;  g_prime = 0;  b_prime = c;
    } else if (h >= 300 && h < 360) {
        r_prime = c;  g_prime = 0;  b_prime = x;
    }

    // Convertir a [0, 255] - formato BGR para OpenCV
    int B = (int)((b_prime + m) * 255.0);
    int G = (int)((g_prime + m) * 255.0);
    int R = (int)((r_prime + m) * 255.0);

    // Clamp a rango válido
    B = max(0, min(255, B));
    G = max(0, min(255, G));
    R = max(0, min(255, R));

    return Vec3b((uchar)B, (uchar)G, (uchar)R);
}

// ============================================================
// Conversión de imagen completa BGR → HSV
// ============================================================
Mat convertir_bgr_a_hsv(const Mat& img_bgr) {
    int rows = img_bgr.rows;
    int cols = img_bgr.cols;
    Mat img_hsv(rows, cols, CV_8UC3);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Vec3b pixel_bgr = img_bgr.at<Vec3b>(i, j);
            img_hsv.at<Vec3b>(i, j) = bgr_a_hsv(pixel_bgr);
        }
    }

    return img_hsv;
}

// ============================================================
// Conversión de imagen completa HSV → BGR
// ============================================================
Mat convertir_hsv_a_bgr(const Mat& img_hsv) {
    int rows = img_hsv.rows;
    int cols = img_hsv.cols;
    Mat img_bgr(rows, cols, CV_8UC3);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Vec3b pixel_hsv = img_hsv.at<Vec3b>(i, j);
            img_bgr.at<Vec3b>(i, j) = hsv_a_bgr(pixel_hsv);
        }
    }

    return img_bgr;
}

// ============================================================
// Distancia euclidiana entre dos píxeles en espacio RGB
// ============================================================
// d = sqrt((R1-R2)^2 + (G1-G2)^2 + (B1-B2)^2)
// ============================================================
double distancia_euclidiana(const Pixel& p1, const Pixel& p2) {
    double dr = p1.r - p2.r;
    double dg = p1.g - p2.g;
    double db = p1.b - p2.b;
    return sqrt(dr * dr + dg * dg + db * db);
}



// ============================================================
// Carga una imagen y verifica que no esté vacía
// ============================================================
Mat cargar_imagen(const string& ruta) {
    Mat img = imread(ruta);
    if (img.empty()) {
        cout << "Error: No se pudo cargar la imagen '" << ruta << "'" << endl;
        cout << "Asegurese de que el archivo existe en el directorio de ejecucion." << endl;
    }
    return img;
}
