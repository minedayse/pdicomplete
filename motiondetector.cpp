

#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  double k;
  Mat image;
  int width, height;
  VideoCapture cap;
  vector<Mat> planes;
  Mat histR, histG, histB, histComp;
  int nbins = 64;
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;
  bool enable = false;
  bool stop = false;
  float ac = 0;
  cap.open(0);

  if(!cap.isOpened()){
    cout << "cameras indisponiveis";
    return -1;
  }

  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  int histw = nbins, histh = nbins/2;
  Mat histImgR(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgG(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgB(histh, histw, CV_8UC3, Scalar(0,0,0));

  while(1){
    cap >> image;
    split (image, planes);
    calcHist(&planes[0], 1, 0, Mat(), histR, 1,
             &nbins, &histrange,
             uniform, acummulate);
    normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
//alto descida subida descida

	
    if (!enable) histComp = histR;

/*    for (int i=0; i<height; i++) {
	for (int j=0; j<width; j++) {
	     ac = histComp.at<uchar>(i,j) - histR.at<uchar>(i,j);
	     //cout << "ac = " << ac << endl;
             if (ac >= 20) {
		cout << "Alarme ativado";
	    }
	}
    }     
*/
  //  ac = ac/(height * width);


    k = compareHist(histComp, histR, CV_COMP_CHISQR);
    cout << k;
    sleep(0.5);
    //if (k > 5) cout << "alarme!!!!";
    enable = true;

    histImgR.setTo(Scalar(0));
	
    for(int i=0; i<nbins; i++){
      line(histImgR, Point(i, histh),
           Point(i, cvRound(histR.at<float>(i))),
           Scalar(0, 0, 255), 1, 8, 0);
    }
    histImgR.copyTo(image(Rect(0, 0       ,nbins, histh)));
//    histImgG.copyTo(image(Rect(0, histh   ,nbins, histh)));
//    histImgB.copyTo(image(Rect(0, 2*histh ,nbins, histh)));

    imshow("image", image);
    if(waitKey(30) >= 0) break;
  }
  return 0;
}


