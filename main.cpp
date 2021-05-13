#include <opencv2/opencv.hpp>

/*
*	
*	Masker - Program to add mask to a human face
*	Author : shn
*	License : MIT License
*
*/

cv::Mat mask;
void addMask(cv::Rect, cv::Mat);

int main()
{
	mask = cv::imread("rsc/mask.png");
	
	cv::VideoCapture vid(0);
	cv::Mat frame;
	
	int keyPress;
	
	std::string path = "rsc/haarcascade_frontalface_default.xml";
	cv::CascadeClassifier classifier;
	classifier.load(path);
	
	std::vector<cv::Rect> faces;
	while(true)
	{
		vid.read(frame);
		classifier.detectMultiScale(frame, faces);
		for(int i = 0; i < faces.size(); i++)
			addMask(faces[i], frame);	
		cv::imshow("Image", frame);
		
		keyPress = cv::waitKey(1);
		if(keyPress == 113) // 'q'
			break;				
	}
	return 0;
}

void addMask(cv::Rect face, cv::Mat img)
{
	int height = face.height;
	int x = face.x;
	int y = face.y;

	//For finding the lower part of the face	
	int x1 = x;
	int y1 = y+(height/2);
	int x2 = face.br().x;
	int y2 = face.br().y;
	
	//cv::Point p1(x1, y1);
	//cv::Point p2(x2, y2);
	//cv::Scalar color(0, 0, 255);
	//cv::rectangle(img, p1, p2, color, 2);

	int h1 = (y2-y1); //height
	int w1 = (x2-x1); //width
	int xt = x1; //x coordinate of top left corner
	int yt = y1; //y coordinate of top left corner
	cv::Rect roi(xt, yt, w1, h1);
	
	cv::Mat resized_mask;
	cv::Size size(w1, h1);
	cv::resize(mask, resized_mask, size);
	
	cv::Mat transparent;
	cv::inRange(resized_mask, cv::Scalar(0, 0, 0), cv::Scalar(0, 0, 0), transparent);
	resized_mask.copyTo(img(roi), 255-transparent);
}
