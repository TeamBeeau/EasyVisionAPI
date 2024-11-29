#include "pch.h"
// pylon 5 
#include <pylon/PylonIncludes.h>
#include <pylon/gige/BaslerGigEInstantCamera.h>
// opencv 4.5
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/types_c.h>
// system
#include <direct.h>
#include <iostream>
#include <fstream>
#include <process.h>   
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <ctime>
#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>

#using <System.Drawing.dll> 
#include <msclr/marshal_cppstd.h>

// #include "zbar.h" // QRCode
// 
// pybind
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

// DEFINED
//////////////////////////////
#define ENUM_TO_STR(ENUM) std::string(#ENUM)
#define TRUE     true
#define FALSE    "FALSE."	
#define SUCCESS  "SUCCESS."

#define COLOR_BAD       cv::Scalar(0, 0, 255)   // Đỏ
#define COLOR_POOR      cv::Scalar(0, 128, 255) // Cam
#define COLOR_AVERAGE   cv::Scalar(0, 255, 255) // Vàng
#define COLOR_GOOD      cv::Scalar(0, 255, 128) // Màu Lime
#define COLOR_EXCELLENT cv::Scalar(0, 255, 0)   // Xanh lá cây

typedef Pylon::CBaslerGigEInstantCamera Camera_t;
typedef Camera_t::GrabResultPtr_t GrabResultPtr_t;
using namespace GenApi;
using namespace Basler_GigECameraParams;
using namespace Pylon;

using namespace cv;
using namespace std;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Threading;
using namespace System::Drawing;

using namespace System::Runtime::InteropServices;

#pragma once
namespace BeeCplus {

	extern std::mutex gilmutex;
	extern py::object _yolo;
	/////////////////////////
	extern std::condition_variable nt;
	extern bool python_initialized;
	extern bool python_terminated;
	//extern std::mutex gilmutex;


	extern std::string listYolo;
	extern std::vector<float> scores;

	extern float ScoreYolo;
	extern int cycleTime;
	extern int pixelCable;
	extern int numCable;
	extern int distanceCable;
	extern std::string pathYolo;
	extern std::string pathModel;
	extern std::string nameYolo;

	extern System::String^ InitializeYolo();
	extern System::String^ ClosePython();
	extern System::String^ LoadModel();

	extern uchar* MatToBytes(cv::Mat image);
	extern std::string String2std(System::String^ str);
	extern cv::Mat BytesToMat(uchar* uc, int image_rows, int image_cols, int image_type);
	extern int getMaxAreaContourId(vector <vector<cv::Point>> contours);
	extern Mat RotateMat(Mat raw, RotatedRect rot);

	extern string _toString(System::String^ STR);
	extern cv::VideoCapture camUSB;

	extern CDeviceInfo nameBasler;
	extern Camera_t camGigE;
	extern CGrabResultPtr ptrGrabResult;
	extern CPylonImage image;/////anh output
	extern CImageFormatConverter fc;///anh convert


	extern uchar* ucRaw;
	extern cv::Mat matTemp, matRaw, matProcess, matResult, matProc, matRsTemp, matCrop;
	extern cv::Mat m_matSrc;
	extern vector<cv::Mat> m_matDst;
	Mat CropImage(Mat matCrop, Rect rect);

	public ref class G
	{
	public:void CropRotate(int x, int y, int w, int  h, float angle);
		  //public:Byte* ReturnRaw();

	//public:void LoadSrc(System::String^ path);
	//public:void LoadDst(System::String^ path);
		  //public:void Initial(const std::string& filename);
		  //public:int stringToMyEnum(const std::string& str);

	};
}




