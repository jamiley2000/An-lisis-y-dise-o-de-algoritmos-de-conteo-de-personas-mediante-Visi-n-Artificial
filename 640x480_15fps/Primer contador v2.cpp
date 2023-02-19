#include <opencv2/opencv.hpp>
#include <vector>
#include <chrono>


using namespace cv;
using namespace std;
using namespace chrono;

class Persona {

	vector<Vec2i> Tracks;
	int id;
	int xpos;
	int ypos;
	bool Finalizado;
	int max_frame;
	bool estado;
	string direccion;
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
		this->direccion = "Ninguna";
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

	string getDireccion() {
		return this->direccion;
	}

	void NuevaCoordenada(int xpos, int ypos) {
		this->frame = 0;
		this->Tracks.push_back(Vec2i{ this->xpos , this->ypos });
		this->xpos = xpos;
		this->ypos = ypos;
	}

	void setFinalizado() {
		this->Finalizado = true;
	}

	bool getFinalizado() {
		return this->Finalizado;
	}

	bool Arriba(int limite) {
		if (this->Tracks.size() >= 2) {
			if (!this->estado) {
				if ((this->Tracks[this->Tracks.size() - 1][1] < limite) && (this->Tracks[this->Tracks.size() - 2][1] > limite)) {
					this->estado = true;
					this->direccion = "arriba";
					return true;
				}
				else return false;
			}
			else return false;
		}
		else return false;
	}

	bool Abajo(int limite) {
		if (this->Tracks.size() >= 2) {
			if (!this->estado) {
				if ((this->Tracks[this->Tracks.size() - 1][1] > limite) && (this->Tracks[this->Tracks.size() - 2][1] < limite)) {
					this->estado = true;
					this->direccion = "abajo";
					return true;
				}
				else return false;
			}
			else return false;
		}
		else return false;
	}

	void añadirframe() {
		this->frame += 1;
		if (this->frame > this->max_frame) {
			this->Finalizado = true;
		}
	}

};

int main() {

	VideoCapture Cap("Videos/esp32.avi");
	Mat Frame1, Frame2, Frame3, Frame4;
	Mat grayImage1, grayImage2;
	Mat differenceImage;
	Mat thresholdImage, kernel;
	Mat cannyImage, cannyDilate;
	vector<vector<Point>> contours, contornos, prueba;
	vector<Vec4i> hierarchy;
	Rect objectBoundingRectangle = Rect(0, 0, 0, 0);
	vector<Persona> Personas = {};
	int ypos_AS = 0, ypos_AE = 0;
	int frames = 0;
	int contador = 0;
	int area;
	bool nuevo;
	int morph_size = 5;
	int morph_size2 = 1;
	int ID = 1;
	int max_frame = 5;
	int h = 480;
	int w = 640;
	int frameArea = h * w;
	float areaTH = frameArea / static_cast<float>(250);
	int medio_arriba = int(1 * (h / 5));
	int medio_abajo = int(4 * (h / 5));
	int limite_arriba = int(0.5 * (h / static_cast<double>(5)));
	int limite_abajo = int(4.5 * (h / static_cast<double>(5)));
	int cont_arriba = 0;
	int cont_abajo = 0;
	float count_up = 0;
	float count_down = 0;
	bool frame = false;
	int contframe = 0;
	///* -------------------------------------------------*/
	//vector<Persona> Tracks = {};
	//Persona per1 = Persona(1, 1, 2, max_frame);
	//Tracks.push_back(per1);
	//Persona per2 = Persona(2, 3, 4, max_frame);
	//Tracks.push_back(per2);
	//Persona per3 = Persona(3, 5, 6, max_frame);
	//Tracks.push_back(per3);

	//for (int i = 0; i < Tracks.size(); i++) {
	//	Tracks.erase(Tracks.begin() + i);
	//	for (int j = 0; j < Tracks.size(); j++) {
	//		cout << Tracks[j].getXpos() << " " << Tracks[j].getYpos() << " ";
	//	}
	//	cout << endl;
	//}
	///*----------------------------------------------------*/
	// Create structuring element
	Mat element = getStructuringElement(
		MORPH_RECT,
		Size(2 * morph_size + 1,
			2 * morph_size + 1),
		Point(morph_size, morph_size));

	Mat element2 = getStructuringElement(
		MORPH_RECT,
		Size(2 * morph_size2 + 1,
			2 * morph_size2 + 1),
		Point(morph_size2, morph_size2));

	frame = false;

	Ptr<BackgroundSubtractorKNN> subtractor = createBackgroundSubtractorKNN(true);
	while (1) {
		auto start = high_resolution_clock::now();

		//-----------------------------------------------



		Cap.read(Frame1);
		//resize(Frame1, Frame1, Size(), 0.5, 0.5);
		//GaussianBlur(Frame1, Frame1, Size(3, 3), 0);

		for (int i = 0; i < Personas.size(); i++) {
			Personas[i].añadirframe();
		}
		subtractor->apply(Frame1, Frame2);

		if (frame) {

			threshold(Frame2, Frame2, 200, 255, THRESH_BINARY);


			dilate(Frame2, Frame2, (getStructuringElement(MORPH_RECT, Size(11, 11))), Point(-1, -1), 2);
			erode(Frame2, Frame2, (getStructuringElement(MORPH_RECT, Size(11, 11))), Point(-1, -1), 2);
			//morphologyEx(Frame2, Frame2,MORPH_OPEN, (getStructuringElement(MORPH_RECT, Size(11,11))));

			//morphologyEx(Frame2, Frame2,MORPH_CLOSE, element);
			/*Canny(Frame2, cannyImage, 100, 200);
			dilate(cannyImage, cannyDilate, element);*/

			findContours(Frame2, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);


			for (int i = 0; i < contours.size(); i++) {

				int area = contourArea(contours[i]);
				if (area > areaTH) {

					objectBoundingRectangle = boundingRect(contours[i]);
					int x = objectBoundingRectangle.x;
					int y = objectBoundingRectangle.y;
					int w = objectBoundingRectangle.width;
					int h = objectBoundingRectangle.height;
					int xpos = objectBoundingRectangle.x + objectBoundingRectangle.width / 2;
					int ypos = objectBoundingRectangle.y + objectBoundingRectangle.height / 2;
					/*----------------------------------*/
					nuevo = true;
					if (ypos > limite_arriba && ypos < limite_abajo) {
						for (int p = 0; p < Personas.size(); p++) {
							
							if ((abs(xpos - Personas[p].getXpos()) <= w) && (abs(ypos - Personas[p].getYpos()) <= h)) {
								nuevo = false;
								Personas[p].NuevaCoordenada(xpos, ypos);
								if (Personas[p].Arriba(medio_arriba)) {
										cont_arriba += 1;
										cout << Personas[p].getId() << " Cruza yendo arriba" << endl;
								}
								else if (Personas[p].Abajo(medio_abajo)) {
										cont_abajo += 1;
										cout << Personas[p].getId() << " Cruza yendo abajo" << endl;
								}
								break;
							}
							if (Personas[p].getEstado()) {
								if (Personas[p].getDireccion() == "abajo" && Personas[p].getYpos() > limite_abajo) {
									Personas[p].setFinalizado();
								}
								else if (Personas[p].getDireccion() == "arriba" && Personas[p].getYpos() < limite_arriba) {
									Personas[p].setFinalizado();
								}
							}
							if (Personas[p].getFinalizado()) {
								Personas.erase(Personas.begin() + p);

							}
						}
						if (nuevo) {
							Persona per = Persona(ID, xpos, ypos, max_frame);
							Personas.push_back(per);
							ID += 1;
						}
					}


					/*----------------------------------*/
					/*cout << "xpos: " << xpos << " " << "ypos: " << ypos << " "
						<< "width: " << objectBoundingRectangle.width << " " << "height: " << objectBoundingRectangle.height << endl;*/
					rectangle(Frame1, Point(x, y), Point(w + x, h + y), Scalar(0, 255, 0), 2);
					circle(Frame1, Point(xpos, ypos), 5, Scalar(0, 0, 255), 2);
					//cout << Frame2.size().width << " " << Frame2.size().height << endl;

				}


			}


			for (int i = 0; i < Personas.size(); i++) {
				putText(Frame1, to_string(Personas[i].getId()), Point(Personas[i].getXpos(), Personas[i].getYpos()), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(0, 255, 0), 2, false);
			}
			line(Frame1, Point(0, limite_abajo), Point(w, limite_abajo), Scalar(255, 0, 0), 2);
			line(Frame1, Point(0, limite_arriba), Point(w, limite_arriba), Scalar(255, 0, 0), 2);
			line(Frame1, Point(0, medio_abajo), Point(w, medio_abajo), Scalar(255, 0, 0), 2);
			line(Frame1, Point(0, medio_arriba), Point(w, medio_arriba), Scalar(255, 0, 0), 2);

			string str_up = "UP: " + to_string(cont_arriba);
			string str_down = "DOWN: " + to_string(cont_abajo);

			putText(Frame1, str_up, Point(10, 40), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 2, false);
			putText(Frame1, str_up, Point(10, 40), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1, false);
			putText(Frame1, str_down, Point(10, 90), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 2, false);
			putText(Frame1, str_down, Point(10, 90), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 0), 1, false);

		}
		else {
			contframe += 1;
			if (contframe > 7) {
				frame = true;
			}
		}

		imshow("Frame1", Frame1);
		imshow("Frame2", Frame2);
		//imshow("canny", cannyImage);
		//imshow("cannydilate", cannyDilate);




		switch (waitKey(1)) {

		case 27: //'esc' key has been pressed, exit program.
			return 0;
		}
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);

		cout << "Time taken by function: "
			<< duration.count() << " milliseconds" << endl;
	}
	
	return 0;

}
