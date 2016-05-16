#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#define c 5;

using namespace cv;
using namespace std;

// troca os quadrantes da imagem da DFT
void deslocaDFT(Mat& image ){
  Mat filtro, A, B, C, D;

  // se a imagem tiver tamanho impar, recorta a regiao para
  // evitar cópias de tamanho desigual
  image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols/2;
  int cy = image.rows/2;
  
  // reorganiza os quadrantes da transformada
  // A B   ->  D C
  // C D       B A
  A = image(Rect(0, 0, cx, cy));
  B = image(Rect(cx, 0, cx, cy));
  C = image(Rect(0, cy, cx, cy));
  D = image(Rect(cx, cy, cx, cy));

  // A <-> D
  A.copyTo(filtro);  D.copyTo(A);  filtro.copyTo(D);

  // C <-> B
  C.copyTo(filtro);  B.copyTo(C);  filtro.copyTo(B);
}

int main(int , char**){
  float yh = 2.5;
  float yl = 0.25;
  float d;
  VideoCapture cap;   
  Mat imaginaryInput, complexImage, multsp;
  Mat padded, filter, mag;
  Mat image, imagegray, filtro; 
  Mat_<float> realInput, zeros;
  vector<Mat> planos;
  int dft_M, dft_N;


  // guarda tecla capturada
  char key;

  // abre o dispositivo de gravação zero
  cap.open(0);
  if(!cap.isOpened())
    return -1;

  // inicialização da captura
  cap >> image;

  // obtenção dos valores para realização da dft 
  dft_M = getOptimalDFTSize(image.rows);
  dft_N = getOptimalDFTSize(image.cols);

  // realiza o padding da imagem
  copyMakeBorder(image, padded, 0,
                 dft_M - image.rows, 0,
                 dft_N - image.cols,
                 BORDER_CONSTANT, Scalar::all(0));

  // declara a parte complexa da imagem
  zeros = Mat_<float>::zeros(padded.size());

  // prepara a matriz complexa para ser preenchida
  complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

  // clona a matriz complexa(o filtro deve ter a mesma dimensão dela)
  filter = complexImage.clone();

  // cria uma matriz temporária para criar as componentes real
  // e imaginaria do filtro ideal
  filtro = Mat(dft_M, dft_N, CV_32F);


  // matriz de filtragem homomorfica
  d = 0.7*dft_M/2;
  for(int i=0; i<dft_M; i++){
    for(int j=0; j<dft_N; j++){
        filtro.at<float>(i,j) = (yh - yl) * (1 - exp((-((i-dft_M/2)^2+(j-dft_N/2)^2))/pow(d,2))) + yl;
    }
  }

  // cria a matriz com as componentes do filtro e junta
  // ambas em uma matriz multicanal complexa
  Mat comps[] = {filtro, filtro};
  merge(comps, 2, filter);

  for(;;){
    // inicia a captura da imagem em tons de cinza e exibe na tela
    cap >> image;
    cvtColor(image, imagegray, CV_BGR2GRAY);
    imshow("original", imagegray);

    // realiza o padding da imagem
    copyMakeBorder(imagegray, padded, 0,
                   dft_M - image.rows, 0,
                   dft_N - image.cols,
                   BORDER_CONSTANT, Scalar::all(0));


    // limpa o array de matrizes que vao compor a
    // imagem complexa
    planos.clear();

    // cria a compoente real
    realInput = Mat_<float>(padded);

    // soma 1 a todos os elementos da imagem, a fim evitar o calculo do logaritmo de possiveis zeros na matriz
    for(int i=0; i<dft_M; i++){
      for(int j=0; j<dft_N; j++){
          realInput.at<float>(i,j) = 255*realInput.at<float>(i,j) + 1.0;
      }
    }

    //log da imagem 
    log(realInput, realInput);

    // insere as duas componentes no array de matrizes
    planos.push_back(realInput);
    planos.push_back(zeros);

    // combina o array de matrizes em uma unica
    // componente complexa
    merge(planos, complexImage);

    // calcula o dft
    dft(complexImage, complexImage);

    // realiza a troca de quadrantes
    deslocaDFT(complexImage);

    // aplica o filtro frequencial
    mulSpectrums(complexImage,filter,complexImage,0);

    // troca novamente os quadrantes
    deslocaDFT(complexImage);

    // calcula a DFT inversa
    idft(complexImage, complexImage);


    // limpa o array de planos
    planos.clear();

    // separa as partes real e imaginaria da
    // imagem filtrada
    split(complexImage, planos);

    // normaliza a parte real e o calculo da exponencial
    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
    exp(planos[0], planos[0]);


    // normaliza a parte real para exibicao
    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
    imshow("filtrada", planos[0]);
 
    key = (char) waitKey(10);
    if( key == 27 ) break; // esc pressed!

  }
  return 0;
}
