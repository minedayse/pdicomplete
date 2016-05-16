#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv){
  Mat image, mask;
  int width, height;
  int cont=0; 
  int wobjects=0; // Objeto sem bolha
  int bobjects=0; // Objeto com bolha (pontos pretos)
 
  CvPoint p;
  image = imread("imagem2.png",CV_LOAD_IMAGE_GRAYSCALE);
  
  if(!image.data){
    std::cout << "Erro: Nao foi possivel carregar a imagem.\n";
    return(-1);
  }
  width=image.size().width;
  height=image.size().height;

  p.x=0;
  p.y=0;

// Pintar todos os pixels da borda de branco
  for(int i=0; i<height; i++){
    image.at<uchar>(i,0) = 255; // Borda lateral esquerda
    image.at<uchar>(i,width-1) = 255; // Borda lateral direita
  }
  for(int j=0; j<width; j++){
    image.at<uchar>(0,j) = 255; // Borda superior
    image.at<uchar>(height-1,j) = 255; // Borda inferior
  }

// Pintar a borda e todos os elementos que estao ligados a ela de preto
  floodFill(image,p,0);
// Pintar o fundo de cinza
  floodFill(image,p,15);

// Procurar objetos
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){ // Encontrou um objeto
	wobjects++; // Por enquanto definir todos os objetos como "sem buracos"
	p.x=j;
	p.y=i;
	floodFill(image,p,100); 
	  }
	}
  }
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
//
      if (image.at<uchar>(i,j) == 100 || image.at<uchar>(i,j) == 200) {
	p.x=j;
	p.y=i;
      }
//
      else if(image.at<uchar>(i,j) == 0){ // Encontrou um furo
        if (image.at<uchar>(p.y, p.x) == 100) { // Caso ele esteja rotulado como um objeto sem buraco, agora ele deve passar a ser com buraco
          floodFill(image, p, 200);
          wobjects--;
          bobjects++;
        }        
        p.x = j;
        p.y = i;
        floodFill(image, p, 0); // Manter o furo preto = 0, pintar da cor da bolha = 200
      }
    }
  }
/*
// Pintar as bolhas em tons de cinza
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 100 || image.at<uchar>(i,j) == 200 ){ // Encontrou um objeto
	cont++; // Contador de tons de cinza
	p.x=j;
	p.y=i;
	floodFill(image,p,cont);
        if(cont==254){ cont = 0;} 
	  }
	}
  }	
*/  
// Pintar o fundo de preto
  p.x=0;
  p.y=0;
  floodFill(image,p,0);

  std::cout << "Objetos sem furos: " << wobjects << "\n";
  std::cout << "Objetos com furos: " << bobjects << "\n";

  imshow("image", image);
  imwrite("labeling.png", image);

  waitKey();
  return 0;
}
