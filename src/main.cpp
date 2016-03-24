//*************************************************************/
//
// AUTHOR: Michal Bednarek
// 2016.02.10 Poznan, PL
// Simple Camscanner-like program to make scans from photos
//
//*************************************************************/

#include "myLibrary.h"

void erosion(Mat& input)
{
	int erosion_type = MORPH_ELLIPSE;
	Mat element1 = getStructuringElement(erosion_type, Size(3, 3));
	erode(input, input, element1);
}

void dilation(Mat& input)
{
	int dilation_type = MORPH_ELLIPSE;
	Mat element2 = getStructuringElement(dilation_type, Size(3, 3));
	dilate(input, input, element2);
}

void cornerPoints(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		if (cnt == 0)
		{
			p1.x = x;
			p1.y = y;
			circle(input, p1, 5, Scalar(0, 0, 255), 4);
			cnt = 1;
		}
		else if (cnt == 1)
		{
			p2.x = x;
			p2.y = y;
			circle(input, p2, 5, Scalar(0, 0, 255), 4);
			cnt = 2;
		}
		else if (cnt == 2)
		{
			p3.x = x;
			p3.y = y;
			circle(input, p3, 5, Scalar(0, 0, 255), 4);
			cnt = 3;
		}
		else if (cnt == 3)
		{
			p4.x = x;
			p4.y = y;
			circle(input, p4, 5, Scalar(0, 0, 255), 4);

			src[0] = p1;
			src[1] = p2;
			src[2] = p3;
			src[3] = p4;

			transformMatrix = getPerspectiveTransform(src, dst);
			warpPerspective(input, output, transformMatrix, output.size());
			
			//Gaussian blur
			GaussianBlur( output, output, Size( 3, 3 ), 0, 0 );
			
			//high-pass filter
			Mat kernel = (Mat_<double>(3,3) << -1, -1, -1, -1, 9, -1, -1, -1, -1) / (float)(3*3);
			filter2D(output, output, -1 , kernel, Point(-1,-1), 80, BORDER_DEFAULT );
			
			///CLAHE ALGORITHM
			cvtColor(output, output, CV_BGR2YUV);

			//Luminance channel
			vector<Mat> channels(4);
			split(output, channels);

			//apply clahe
			Ptr<cv::CLAHE> clahe = cv::createCLAHE();
			clahe->setClipLimit(2);
			Mat dst;
			clahe->apply(channels[0], dst);

			//merge
			dst.copyTo(channels[0]);
			merge(channels, output);

			//cvt back to BGR
			Mat image_clahe;
			cvtColor(output, output, CV_YUV2BGR);
			cvtColor(output, output, CV_BGR2GRAY);

			addWeighted(output, 2.1, output, -1.0, 0, output);

			//adaptive threshold
			adaptiveThreshold(output, output, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 101, 6);

			dilation(output);
			erosion(output);
			erosion(output);
			
			cnt = 0;
		}
	}
}

int main(int argc, char** argv)
{
	//arg count
	if (argc != 2)
	{
		cout << "Usage: noteScanner <photo_to_scan.extension>" << endl;
		return -1;
	}
	
	//input photo
	input = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	
	output = Mat::zeros(input.rows, input.cols, input.type());
	transformMatrix = Mat::zeros(input.rows, input.cols, input.type());
	
	if (!input.data)
	{
		cout << "Image does not exsist or cannot be loaded" << std::endl;
		return -1;
	}
	
	//point corners to perspective transform
	dst[0] = Point2f(0, 0);
	dst[1] = Point2f(output.cols - 1, 0);
	dst[2] = Point2f(0, output.rows - 1);
	dst[3] = Point2f(output.cols - 1, output.rows - 1);

	namedWindow("output", WINDOW_NORMAL);
	namedWindow("input", WINDOW_NORMAL);
	setMouseCallback("input", cornerPoints, NULL);

	char key;
	stringstream ss;
	string saveName;
	int copyNumber = 0;
	
	while (true)
	{
		ss << "copies/copy_" << copyNumber << "_" << argv[1];
		ss >> saveName;
		ss.str( string() );
		ss.clear();
		
		key = waitKey(30);
		imshow("input", input);
		imshow("output", output);
		
		if(key == 27 || key == 91 || key == 113) 	//ESC, q or Q
		  break;
		else if (key == 83 || key == 115)	//s or S
		{
		  imwrite(saveName, output);
		  cout << "Saved as: " << saveName;
		  copyNumber++;
		}
	}

	
	return 0;
}
