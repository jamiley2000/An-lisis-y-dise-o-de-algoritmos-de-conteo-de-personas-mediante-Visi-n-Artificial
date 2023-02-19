#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>

using namespace cv;

int main()
{
    HOGDescriptor hog;
    std::vector<float> imageHOG;
    Mat Frame1, Frame2;
    VideoCapture Cap("Videos/esp32/esp32_v4_15fps.avi");
    // Calcular el histograma de gradiente orientado del fondo estimado
    Mat backgroundImage = imread("Videos/esp32/esp32.jpg"); // Cargar o calcular la imagen del fondo estimado
    std::vector<float> backgroundHOG;
    hog.compute(backgroundImage, backgroundHOG);

    while (1) {
        // Cargar la imagen
        Cap.read(Frame1);
        if (Frame1.empty())
            break;

        // Calcular el histograma de gradiente orientado de la imagen

        hog.compute(Frame1, imageHOG);



        // Comparar los histogramas de gradiente orientado y eliminar el fondo
        Mat foregroundMask = Mat::zeros(Frame1.size(), CV_8UC1);
        for (int y = 0; y < Frame1.rows; y++)
        {
            for (int x = 0; x < Frame1.cols; x++)
            {
                // Comparar el histograma de gradiente orientado del píxel con el del fondo
                // Si la diferencia es mayor que cierto umbral, considerar el píxel un elemento en primer plano
                if (compareHOG(imageHOG[y * Frame1.cols + x], backgroundHOG[y * Frame1.cols + x]) > threshold)
                {
                    foregroundMask.at<uchar>(y, x) = 255;
                }
            }
        }

        // Aplicar la máscara de primer plano a la imagen para eliminar el fondo
        Mat foregroundImage;
        Frame1.copyTo(foregroundImage, foregroundMask);
    }
    return 0;
}