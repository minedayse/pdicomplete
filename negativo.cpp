#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image;

  image= imread("imagem.png",CV_LOAD_IMAGE_GRAYSCALE);

  if(!image.data){
    cout << "Erro: Nao foi possivel carregar a imagem." << endl;
  }

  else{
    namedWindow("Janela",WINDOW_AUTOSIZE);

    int x1, x2, y1, y2;

    cout << "Ponto 1" << endl;
    cout << "X: ";
    cin >> x1;
    cout << "Y: ";
    cin >> y1;
    cout << "Ponto 2" << endl;
    cout << "X: ";
    cin >> x2;
    cout << "Y: ";
    cin >> y2; 

// Negativar regiao entre os pontos 1 e 2
    for(int i=x1;i<x2;i++){
      for(int j=y1;j<y2;j++){
        image.at<uchar>(i,j)=255 - image.at<uchar>(i,j);
      }
    }
  
    imshow("Janela", image);  
    imwrite("imagemnegativa.png", image);
    waitKey();
    }

  return 0;
}
