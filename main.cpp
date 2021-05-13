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

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("Usage : %s <path of image>\n", argv[0]);
		return -1;
	}
	mask = cv::imread("rsc/mask.png");
	cv::Mat frame;
	
	std::string path = "rsc/haarcascade_frontalface_default.xml";
	cv::CascadeClassifier classifier;
	classifier.load(path);
	
	std::vector<cv::Rect> faces;
	std::string img_path = argv[1];
	frame = cv::imread(img_path);
	classifier.detectMultiScale(frame, faces);
	for(int i = 0; i < faces.size(); i++)
		addMask(faces[i], frame);	
	cv::imshow("Output", frame);	
	cv::waitKey(0);			
	
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
	
	int h1 = (y2-y1); //height
	int w1 = (x2-x1); //width
	int xt = x1; //x coordinate of top left corner
	int yt = y1; //y coordinate of top left corner
	cv::Rect roi(xt, yt, w1, h1);
	cv::Mat crop = img(roi);
	
		
	//Adjust brightness of mask relative to the region
	cv::Mat hsv;
	cv::cvtColor(crop, hsv, cv::COLOR_BGR2HSV);
	cv::Scalar values = cv::mean(hsv);
	cv::Mat dB; //decreased brightness
			
	//Resize mask to fit in the region	
	cv::Mat resized_mask;
	cv::Size size(w1, h1);
	cv::resize(mask, resized_mask, size);
	
	resized_mask.convertTo(dB, -1, 1, -(values[2]/4));
	dB.copyTo(resized_mask);
		
	//Removes background from the mask so as to make the face visible
	cv::Mat transparent;
	cv::inRange(resized_mask, cv::Scalar(0, 0, 0), cv::Scalar(0, 0, 0), transparent);
	resized_mask.copyTo(img(roi), 255-transparent);
}
