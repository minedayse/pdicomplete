#include <iostream>
#include "opencv2/opencv.hpp"
#define it_max 40
#define A 15
#define B 20
#define THR 250

using namespace std;
using namespace cv;

int main(int argc, char**argv){

  Mat image, aux, points;
  int passo, v, width, height, gray, x, y;
  float r;

  //le imagem e armazena seu tamanho em variaveis mais "faceis" de acessar
  image= imread("lena.png",CV_LOAD_IMAGE_GRAYSCALE);
  width=image.size().width;
  height=image.size().height;


  //declara imagem final
  points = image;

  //realiza a detecção de bordas de canny
  for (int i=1; i<it_max; i++) {
	v = 6*i+0.05*height;
  	Canny(image, aux, v, 3*v);

	r = 1+(it_max-i+A)/B;
	passo = r+2;


  	//aplica o efeito de pontilhismo
  	for(int i=r; i<height; i=i+passo){
    		for(int j=r; j<width; j=j+passo){
      			gray = image.at<uchar>(i,j);
			//verifica se o ponto atual está acima do limiar
			if (aux.at<uchar>(i,j) > THR) {
			//desenha os circulos propriamente ditos      			
				circle(points,
             				cv::Point(j,i),
             				r,
             				CV_RGB(gray,gray,gray),
             				-1,
             				CV_AA);
			}
    		}
  	}
  }

  //exibição da imagem final
  imwrite("output.png", points);
  return 0;
}
