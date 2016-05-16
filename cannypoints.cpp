#include <iostream>
#include "opencv2/opencv.hpp"
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

#define JITTER 3

int init;
int it_max = 40;
int STEP;
float RAIO;
int value;

char TrackbarName[50];

Mat image, border;


int main(int argc, char**argv){
  Mat frame, points;
  int width, height, gray;
  int x, y;

  image= imread("lena.png",CV_LOAD_IMAGE_GRAYSCALE);
  
  width=image.size().width;
  height=image.size().height;

  init = height/100;

  //declara imagem final
  points = image;

  //fazer canny para diferentes valores
  for (int i=1; i<it_max; i++) {
	value = 2*i+init;
  	Canny(image, border, value, 3*value);

	RAIO = 1+(it_max-i+10)/20;
	STEP = RAIO+2;


  	//gera circulos
  	for(int i=RAIO; i<height; i=i+STEP){
    		for(int j=RAIO; j<width; j=j+STEP){
      			gray = image.at<uchar>(i,j);
			if (border.at<uchar>(i,j) > 200) {
      				circle(points,
             				cv::Point(j,i),
             				RAIO,
             				CV_RGB(gray,gray,gray),
             				-1,
             				CV_AA);
			}
    		}
  	}
  }

  imwrite("pontos.png", points);
  imshow("pontos", points);
  return 0;

}
