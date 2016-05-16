#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

int main(int, char**){

srand(time(NULL));

  Mat image;
  int width, height;
  int t;// Troca (regioes)
  int aux1, aux2; // Auxiliar

  image= imread("imagem.png",CV_LOAD_IMAGE_GRAYSCALE);

  if(!image.data){
    cout << "Erro: Nao foi possivel carregar a imagem." << endl;
  }

  else{
  namedWindow("Janela",WINDOW_AUTOSIZE);

  width=image.size().width;
  height=image.size().height;
 
// Translacao
  t = rand() % 3; // Troca aleatoria
  if(t==0){ // 1-2, 3-4
    for(int i=0;i<height/2;i++){
      for(int j=0;j<width/2;j++){
        aux1 = image.at<uchar>(i,j);
        image.at<uchar>(i,j) = image.at<uchar>(i,j + width/2);
        image.at<uchar>(i,j + width/2) = aux1;
        aux2 = image.at<uchar>(i + height/2,j);
        image.at<uchar>(i + height/2,j) = image.at<uchar>(i + height/2,j + width/2);
        image.at<uchar>(i + height/2,j + width/2) = aux2;
      }
    }
  }
  else if(t==1){ // 1-3, 2-4
    for(int i=0;i<height/2;i++){
      for(int j=0;j<width/2;j++){
        aux1 = image.at<uchar>(i,j);
        image.at<uchar>(i,j) = image.at<uchar>(i + height/2,j);
        image.at<uchar>(i + height/2,j) = aux1;
        aux2 = image.at<uchar>(i,j + width/2);
        image.at<uchar>(i,j + width/2) = image.at<uchar>(i + height/2,j + width/2);
        image.at<uchar>(i + height/2,j + width/2) = aux2;
      }
     }
  } 
  else{ // 1-4, 2-3
    for(int i=0;i<height/2;i++){
      for(int j=0;j<width/2;j++){
        aux1 = image.at<uchar>(i,j);
        image.at<uchar>(i,j) = image.at<uchar>(i + height/2,j + width/2);
        image.at<uchar>(i + height/2,j + width/2) = aux1;
        aux2 = image.at<uchar>(i,j + width/2);
        image.at<uchar>(i,j + width/2) = image.at<uchar>(i + height/2,j);
        image.at<uchar>(i + height/2,j) = aux2;
      }
     }
  }
// FimTranslacao
  
  imshow("Janela", image);  
  imwrite("Exemplo2.png", image);
  waitKey();
  }

  return 0;
}
