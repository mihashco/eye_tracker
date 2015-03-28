#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(int, char**)
{
	VideoCapture cap(0); // open the default camera
	
	Mat frame; //original frame from camera.
	Mat gray; //gray image is used for all calculations.

	Mat faceRoi; //storage for detected face
	
	Mat leftEyeRoi; //storage for detected left eye
	Mat rightEyeRoi; //storage for detected right eye

	Mat resizedLeftEyeRoi;
	Mat resizedRightEyeRoi;

	if (!cap.isOpened())  // check if we succeeded
		return -1;
	
	//Loads data for cascade classifiers
	CascadeClassifier faceCascadeClassifier("E:\\Dev\\opencv2411\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_default.xml");
	CascadeClassifier eyeCascadeClassifier("E:\\Dev\\opencv2411\\opencv\\sources\\data\\haarcascades\\haarcascade_eye_tree_eyeglasses.xml");

	/*
	namedWindow("Frame", 1);
	namedWindow("FaceROI", 1);
	*/
	
	namedWindow("Eye1", 1);
	namedWindow("Eye2", 1);

	for (;;)
	{
		//Storage for detected faces.
		vector<Rect> faces;

		cap >> frame; // get a new frame from camera
		if (frame.empty())
			continue;

		cv::cvtColor(frame, gray, COLOR_BGR2GRAY);

		//Use haarcascadeclassifier
		faceCascadeClassifier.detectMultiScale(gray, faces, 1.1, 5, 0 | CASCADE_SCALE_IMAGE, Size(10, 10));
		
		//Detecting eyes
		for (int i = 0; i < faces.size(); i++)
		{
			//Storage for detected eyes.
			vector<Rect> eyes;
			faceRoi = gray(faces[i]); // crop the image to face size.+
			/*
			rectangle(gray, faces[i], Scalar(255.0, 150.0, 150.0), 3);
			*/
			eyeCascadeClassifier.detectMultiScale(faceRoi, eyes, 1.2, 2, 0 | CASCADE_SCALE_IMAGE, Size(10, 10));
			/* Drawing frame around detected object
			for (int j = 0; j < eyes.size(); j++)
			{
				rectangle(faceRoi, eyes[j], Scalar(255.0, 255.0, 255.0), 2);
			}
			*/

			if (eyes.size() == 2)
			{
				leftEyeRoi	= faceRoi(eyes[0]);
				rightEyeRoi = faceRoi(eyes[1]);
				
				if (!leftEyeRoi.empty() || !rightEyeRoi.empty())
				{
					resize(leftEyeRoi, resizedLeftEyeRoi, Size(100, 100));
					resize(rightEyeRoi, resizedRightEyeRoi, Size(100, 100));

					imshow("Eye1", resizedLeftEyeRoi);
					imshow("Eye2", resizedRightEyeRoi);
				}
			}
			/* Draw rectangle around detected face
			if (!faceRoi.empty()) imshow("FaceROI", faceRoi);
			*/
		}

		/*
		imshow("display", gray);
		*/
		

		if (waitKey(30) >= 0)
			break;
	}

	cap.release();
	return 0;
}