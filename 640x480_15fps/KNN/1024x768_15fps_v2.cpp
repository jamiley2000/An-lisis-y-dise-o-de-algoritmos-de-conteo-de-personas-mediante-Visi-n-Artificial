/*--------------------------- Comentarios y cambios ---------------------------

	Cambios con respecto a la versión anterior:
		- Se ha añadido el límite de area máximo

--------------------------- Comentarios y cambios ---------------------------*/

/*--------------------------- Librerias ---------------------------*/
#include <opencv2/opencv.hpp>
#include <chrono>
#include <fstream>
/*--------------------------- Librerias ---------------------------*/

using namespace cv;
using namespace std;
using namespace chrono;
/*--------------------------- Inicialización de la clase ---------------------------*/
class Persona {

	vector<Vec2i> Tracks;
	int id;
	int xpos;
	int ypos;
	bool Finalizado;
	int max_frame;
	bool estado;
	bool cruce;
	int frame;


public:
	Persona(int id, int xpos, int ypos, int max_frame) {

		this->id = id;
		this->xpos = xpos;
		this->ypos = ypos;
		this->Tracks = {};
		this->Finalizado = false;
		this->estado = false;
		this->frame = 0;
		this->max_frame = max_frame;
		this->cruce = false;
	}

	int getId() {
		return this->id;
	}

	int getXpos() {
		return this->xpos;
	}

	int getYpos() {
		return this->ypos;
	}

	int getFrame() {
		return this->frame;
	}

	int getMax_frame() {
		return this->max_frame;
	}

	vector<Vec2i> getTracks() {
		return this->Tracks;
	}

	bool getEstado() {
		return this->estado;
	}

	bool getcruce() {
		return this->cruce;
	}

	void NuevaCoordenada(int xpos, int ypos) {
		this->frame = 0;
		this->Tracks.push_back(Vec2i{ this->xpos , this->ypos });
		this->xpos = xpos;
		this->ypos = ypos;
		this->estado = false;
	}

	void setFinalizado() {
		this->Finalizado = true;
	}

	bool getFinalizado() {
		return this->Finalizado;
	}

	bool Arriba(int limite) {
		if (this->Tracks.size() >= 2) {
				if ((this->Tracks[this->Tracks.size() - 1][1] < limite) && (this->Tracks[this->Tracks.size() - 2][1] >= limite)) {
					this->cruce = true;
					return true;
				}
			    else return false;
		}
		else return false;
	}

	bool Abajo(int limite) {
		if (this->Tracks.size() >= 2) {
				if ((this->Tracks[this->Tracks.size() - 1][1] > limite) && (this->Tracks[this->Tracks.size() - 2][1] <= limite)) {
					this->cruce = true;
					return true;
				}
				else return false;
		}
		else return false;
	}

	void añadirframe() {
		this->frame += 1;
		this->estado = true;
		if (this->frame > this->max_frame) {
			this->Finalizado = true;
		}
	}

};
/*--------------------------- Inicialización de la clase ---------------------------*/
int main() {
	/*--------------------------- Inicialización de variables ---------------------------*/
	auto start = high_resolution_clock::now();
	auto start_inicializacion = high_resolution_clock::now();
	/*--- Inicialización de variables ---*/
	VideoCapture Cap("Videos/TestVideo/TestVideo_15fps.avi");
	VideoWriter writer = VideoWriter("Video2_TestVideo.avi/640x480_15fps/KNN/640x480_15fps.avi", VideoWriter::fourcc('X', 'V', 'I', 'D'), 15, Size(Cap.get(cv::CAP_PROP_FRAME_WIDTH), Cap.get(cv::CAP_PROP_FRAME_HEIGHT)));
	VideoWriter writer2 = VideoWriter("Video2_TestVideo.avi/640x480_15fps/KNN/640x480_15fps_SDF.avi", VideoWriter::fourcc('X', 'V', 'I', 'D'), 15, Size(Cap.get(cv::CAP_PROP_FRAME_WIDTH), Cap.get(cv::CAP_PROP_FRAME_HEIGHT)), false);
	/*--- Inicialización de variables ---*/
	ofstream archivo("Video2_TestVideo.avi/640x480_15fps/KNN/TiemposProcesamiento.txt", fstream::out);
	ofstream Archivo2("Video2_TestVideo.avi/640x480_15fps/KNN/PersonasContadas.txt", fstream::out);
	/*--- Inicialización de variables ---*/
	Mat Frame1, Frame2;
	/*--- Inicialización de variables ---*/
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	/*--- Inicialización de variables ---*/
	Rect objectBoundingRectangle = Rect(0, 0, 0, 0);
	/*--- Inicialización de variables ---*/
	vector<Persona> Personas = {};
	/*--- Inicialización de variables ---*/
	bool nuevo;
	int ID = 1;
	int max_frame = 5;
	int he = 480;
	int wi = 640;
	int frameArea = he * wi;
	float areaTH = frameArea / static_cast<float>(250);
	float areaMAX = frameArea / static_cast<float>(2);
	int medio_arriba = int(2 * (he / 4));
	int medio_abajo = int(2 * (he / 4));
	int limite_arriba = int(1 * (he / static_cast<double>(5)));
	int limite_abajo = int(4 * (he / static_cast<double>(5)));
	int cont_arriba = 0;
	int cont_abajo = 0;
	float count_up = 0;
	float count_down = 0;
	int frame = 1;
	/*--- Inicialización de variables ---*/
	Ptr<BackgroundSubtractorKNN> subtractor = createBackgroundSubtractorKNN(500, 400, true);
	/*--- Inicialización de variables ---*/
	int duracion_total = 0;
	int media_inicializacion = 0;
	int media_frames = 0;
	float media_preprocesamiento = 0;
	int media_sustraccion = 0;
	int media_transmorfo = 0;
	int media_contornos = 0;
	float media_conteo = 0;
	int media_posprocesamiento = 0;
	/*--- Inicialización de variables ---*/
	int personas_totales = 8;
	int personas_arriba = 3;
	int personas_abajo = 5;

	auto stop_inicializacion = high_resolution_clock::now();
	/*--------------------------- Inicialización de variables ---------------------------*/
	while (1) {
		auto start_frames = high_resolution_clock::now();
		/*--------------------------- Preprocesamiento ---------------------------*/
		auto start_preprocesamiento = high_resolution_clock::now();

		Cap.read(Frame1);
		if (Frame1.empty())
			break;

		for (int i = 0; i < Personas.size(); i++) {
			Personas[i].añadirframe();
		}

		auto stop_preprocesamiento = high_resolution_clock::now();
		/*--------------------------- Preprocesamiento ---------------------------*/

		/*--------------------------- Sustraccion del fondo ---------------------------*/
		auto start_SustraccionFondo = high_resolution_clock::now();

		subtractor->apply(Frame1, Frame2);

		auto stop_SustraccionFondo = high_resolution_clock::now();
		/*--------------------------- Sustraccion del fondo ---------------------------*/

		/*--------------------------- Transformaciones morfológicas ---------------------------*/
		auto start_TransMorfologicas = high_resolution_clock::now();

		threshold(Frame2, Frame2, 200, 255, THRESH_BINARY);
		morphologyEx(Frame2, Frame2, MORPH_OPEN, (getStructuringElement(MORPH_RECT, Size(3, 3))), Point(-1, -1), 1);
		morphologyEx(Frame2, Frame2, MORPH_CLOSE, (getStructuringElement(MORPH_RECT, Size(11, 11))), Point(-1, -1), 1);

		auto stop_TransMorfologicas = high_resolution_clock::now();
		/*--------------------------- Transformaciones morfológicas ---------------------------*/

		/*--------------------------- Obtencion de contornos ---------------------------*/
		auto start_Contornos = high_resolution_clock::now();

		findContours(Frame2, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		/*drawContours(Frame1, contours, -1, Scalar(0, 0, 255), 2, LINE_8, hierarchy, 0);*/
		auto stop_Contornos = high_resolution_clock::now();
		/*--------------------------- Obtencion de contornos ---------------------------*/

		/*--------------------------- Deteccion y conteo de personas ---------------------------*/
		auto start_Conteo = high_resolution_clock::now();

		for (int i = 0; i < contours.size(); i++) {
			/*--------------------------- Diferenciacion de personas por el area ---------------------------*/
			int area = contourArea(contours[i]);
			if (area > areaTH && area < areaMAX) {
				/*--------------------------- Calculo de la posicion ---------------------------*/
				objectBoundingRectangle = boundingRect(contours[i]);
				int x = objectBoundingRectangle.x;
				int y = objectBoundingRectangle.y;
				int w = objectBoundingRectangle.width;
				int h = objectBoundingRectangle.height;
				int xpos = x + w / 2;
				int ypos = y + h / 2;
				/*--------------------------- Tracking y Conteo de personas ---------------------------*/
				nuevo = true;
				/*--------------------------- Analizar personas dentro de un rango ---------------------------*/
				for (int p = 0; p < Personas.size(); p++) {
					/*--------------------------- Enlazar y contar personas entre frames ---------------------------*/
					if ((abs(xpos - Personas[p].getXpos())*1.5 <= w) && (abs(ypos - Personas[p].getYpos())*1.5 <= h) && Personas[p].getEstado()) {
						nuevo = false;
						Personas[p].NuevaCoordenada(xpos, ypos);
						/*--------------------------- Contar personas ---------------------------*/
						if (!Personas[p].getcruce()) {
							if (Personas[p].Arriba(medio_arriba)) {
								cont_arriba += 1;
								auto stop_arriba = high_resolution_clock::now();
								auto duration_arriba = duration_cast<seconds>(stop_arriba - start);
								Archivo2 << "La persona " << Personas[p].getId() << " entra a los " << duration_arriba.count() << " segundos" << endl;
							}
							else if (Personas[p].Abajo(medio_abajo)) {
								cont_abajo += 1;
								auto stop_abajo = high_resolution_clock::now();
								auto duration_abajo = duration_cast<seconds>(stop_abajo - start);
								Archivo2 << "La persona " << Personas[p].getId() << " sale a los " << duration_abajo.count() << " segundos" << endl;
							}
							break;
						}
						/*--------------------------- Contar personas ---------------------------*/
					}
					/*--------------------------- Eliminar personas ya contadas ---------------------------*/					
					/*--------------------------- Eliminar personas ya contadas ---------------------------*/
				}
				/*--------------------------- Enlazar personas entre frames ---------------------------*/

				/*--------------------------- Añadir persona si no existe ---------------------------*/
				if (nuevo) {
					Persona per = Persona(ID, xpos, ypos, max_frame);
					Personas.push_back(per);
					ID += 1;
				}
				/*--------------------------- Añadir persona si no existe ---------------------------*/
			/*--------------------------- Tracking y Conteo de personas ---------------------------*/

				/*--------------------------- Dibujar Contorno y centroide ---------------------------*/
				rectangle(Frame1, Point(x, y), Point(w + x, h + y), Scalar(0, 255, 0), 2);
				circle(Frame1, Point(xpos, ypos), 5, Scalar(0, 0, 255), 2);
				/*--------------------------- Dibujar Contorno y centroide ---------------------------*/
			}
			for (int i = 0; i < Personas.size(); i++) {
				putText(Frame1, to_string(Personas[i].getId()), Point(Personas[i].getXpos(), Personas[i].getYpos()), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(0, 255, 0), 1, false);
			}
		}
		for (int i = 0; i < Personas.size(); i++) {
			if (Personas[i].getFinalizado()) {
				Personas.erase(Personas.begin() + i);
			}
		}
		auto stop_Conteo = high_resolution_clock::now();
		/*--------------------------- Deteccion y conteo de personas ---------------------------*/

		/*--------------------------- Posprocesamiento y dibujado de lineas ---------------------------*/

		/*--------------------------- Dibujado de lineas ---------------------------*/
		auto start_posprocesamiento = high_resolution_clock::now();

		line(Frame1, Point(0, limite_abajo), Point(wi, limite_abajo), Scalar(255, 0, 255), 2);
		line(Frame1, Point(0, limite_arriba), Point(wi, limite_arriba), Scalar(255, 0, 255), 2);
		line(Frame1, Point(0, medio_abajo), Point(wi, medio_abajo), Scalar(255, 0, 0), 2);
		line(Frame1, Point(0, medio_arriba), Point(wi, medio_arriba), Scalar(255, 0, 0), 2);

		string str_up = "UP: " + to_string(cont_arriba);
		string str_down = "DOWN: " + to_string(cont_abajo);

		putText(Frame1, str_up, Point(10, 40), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 2, false);
		putText(Frame1, str_up, Point(10, 40), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1, false);
		putText(Frame1, str_down, Point(10, 90), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 2, false);
		putText(Frame1, str_down, Point(10, 90), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 0), 1, false);

		/*--------------------------- Dibujado de lineas ---------------------------*/

		/*--------------------------- Mostrar imagenes por pantalla y guardado de pruebas ---------------------------*/

		imshow("Frame1", Frame1);
		imshow("Frame2", Frame2);
		writer.write(Frame1);
		writer2.write(Frame2);

		char c = (char)waitKey(1);
		if (c == 27)
			break;

		auto stop_posprocesamiento = high_resolution_clock::now();
		auto stop = high_resolution_clock::now();
		/*--------------------------- Mostrar imagenes por pantalla y guardado de pruebas ---------------------------*/

		/*--------------------------- Posprocesamiento y dibujado de lineas ---------------------------*/

		/*--------------------------- Calculo de tiempos ---------------------------*/
		auto duration = duration_cast<milliseconds>(stop - start);
		auto duration_frames = duration_cast<milliseconds>(stop - start_frames);
		auto duration_inicializacion = duration_cast<milliseconds>(stop_inicializacion - start_inicializacion);
		auto duration_preprocesamiento = duration_cast<microseconds>(stop_preprocesamiento - start_preprocesamiento);
		auto duration_SustraccionFondo = duration_cast<milliseconds>(stop_SustraccionFondo - start_SustraccionFondo);
		auto duration_TransMorfologicas = duration_cast<milliseconds>(stop_TransMorfologicas - start_TransMorfologicas);
		auto duration_Contornos = duration_cast<milliseconds>(stop_Contornos - start_Contornos);
		auto duration_Conteo = duration_cast<microseconds>(stop_Conteo - start_Conteo);
		auto duration_posprocesamiento = duration_cast<milliseconds>(stop_posprocesamiento - start_posprocesamiento);


		/*--------------------------- Calculo de tiempos ---------------------------*/

		/*--------------------------- Mostrar tiempos ---------------------------*/
		archivo << setprecision(3) << "Frame numero: " << frame << " | Tiempo total desde el inicio: " << duration.count() << " milisegundos |" << " Tiempo de procesamiento por cada frame: " << duration_frames.count() << " milisegundos |" << " Inicialización: " << duration_inicializacion.count() << " milisegundos |" << " Preprocesamiento: " << float(duration_preprocesamiento.count()) / 1000 << " milisegundos |" << " Sustraccion de fondo: " << duration_SustraccionFondo.count() << " milisegundos |" << " Transformaciones Morfologicas: " << duration_TransMorfologicas.count() << " milisegundos |" << " Calculo de contornos: " << duration_Contornos.count() << " milisegundos |" << " Algoritmo de conteo: " << float(duration_Conteo.count()) / 1000 << " milisegundos |" << " Posprocesamiento: " << duration_posprocesamiento.count() << " milisegundos |" << endl;

		frame += 1;

		duracion_total = duration.count();
		media_frames += duration_frames.count();
		media_inicializacion += duration_inicializacion.count();
		media_preprocesamiento += float(duration_preprocesamiento.count()) / 1000;
		media_sustraccion += duration_SustraccionFondo.count();
		media_transmorfo += duration_TransMorfologicas.count();
		media_contornos += duration_Contornos.count();
		media_conteo += float(duration_Conteo.count()) / 1000;
		media_posprocesamiento += duration_posprocesamiento.count();

		/*--------------------------- Mostrar tiempos ---------------------------*/
	}

	archivo << setprecision(3) << "Frames totales: " << frame - 1 << " | Tiempo total desde el inicio: " << duracion_total << " milisegundos |" << " Tiempo medio de procesamiento por cada frame: " << media_frames / frame << " milisegundos |" << " Inicialización media: " << media_inicializacion / frame << " milisegundos |" << " Preprocesamiento medio: " << media_preprocesamiento / frame << " milisegundos |" << " Sustraccion de fondo medio: " << media_sustraccion / frame << " milisegundos |" << " Transformaciones Morfologicas media: " << media_transmorfo / frame << " milisegundos |" << " Calculo de contornos medio: " << media_contornos / frame << " milisegundos |" << " Algoritmo de conteo medio: " << media_conteo / frame << " milisegundos |" << " Posprocesamiento medio: " << media_posprocesamiento / frame << " milisegundos |" << endl;
	/*Suponemos que entrar es ir hacia arriba y salir es ir hacia abajo*/
	Archivo2 << setprecision(3) << "Numero real de personas que han pasado por pantalla = " << personas_totales << " | Personas reales que han entrado = " << personas_arriba << " | Personas reales que han salido = " << personas_abajo << endl << "Numero total de personas que han pasado por pantalla = " << cont_abajo + cont_arriba << " | Personas contadas que entrado = " << cont_arriba << " | Personas contadas que han salido = " << cont_abajo << endl << "Eficacia total = " << float((cont_abajo + cont_arriba) * 100 / personas_totales) << " % | Eficacia entrando = " << float(cont_arriba * 100 / personas_arriba) << " % | Eficacia saliendo = " << float(cont_abajo * 100 / personas_abajo) << " % |" << endl;

	Cap.release();
	writer.release();
	writer2.release();
	archivo.close();
	destroyAllWindows();

	return 0;

}