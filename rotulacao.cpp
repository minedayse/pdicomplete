#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv){
  Mat image, mask;
  int width, height;
  int nobjects, cont;
  
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

  nobjects=0;
  cont = 0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){ // Encontrou um objeto
		nobjects++;
		cont++;
		p.x=j;
		p.y=i;
		floodFill(image,p,cont);
		if(cont==254){ cont = 0;} // Estourou o limite do contador, zerar
	  }
	}
  } 
  std::cout << "Numero de objetos: " << nobjects << "\n"; // Total de objetos encontrados

  imshow("image", image);
  imwrite("labeling.png", image);
  waitKey();
  return 0;
}
