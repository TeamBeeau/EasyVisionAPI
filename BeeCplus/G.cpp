#include "G.h"
#include <future>
//using namespace BeeCplus;
namespace BeeCplus
{

	std::condition_variable nt;
	bool python_initialized = false;
	bool python_terminated = false;
	std::mutex gilmutex;

	CDeviceInfo nameBasler;
	Camera_t camGigE;
	CGrabResultPtr ptrGrabResult;
	CPylonImage image;/////anh output
	CImageFormatConverter fc;///anh convert

	cv::Mat matCrop;
	cv::VideoCapture camUSB;
	cv::Mat matTemp, matRaw, matProcess, matResult, matProc, matRsTemp;
	cv::Mat m_matSrc;
	vector< cv::Mat> m_matDst;
	uchar* ucRaw; uchar* ucCrop;

	string _toString(System::String^ STR)
	{
		char cStr[1000] = { 0 };
		sprintf(cStr, "%s", STR);
		std::string s(cStr);
		return s;
	}
	
	py::object _yolo;

	//std::shared_ptr<py::object> _yolo;

	std::string listYolo;
	std::vector<float> scores;

	float ScoreYolo;
	int cycleTime;
	int pixelCable;
	int numCable;
	int distanceCable;
	std::string pathYolo;
	std::string pathModel = "C:\\model.pt";
	std::string nameYolo = "yolo";

	struct gil_scoped_acquire_local {
		gil_scoped_acquire_local() : state(PyGILState_Ensure()) {}
		gil_scoped_acquire_local(const gil_scoped_acquire_local&) = delete;
		gil_scoped_acquire_local& operator=(const gil_scoped_acquire_local&) = delete;
		~gil_scoped_acquire_local() { PyGILState_Release(state); }
		const PyGILState_STATE state;
	};



	System::String^ InitializeYolo()
	{
		try
		{
			Py_Initialize();
			gil_scoped_acquire_local gil_acquire;

			py::module processor_module = py::module::import(nameYolo.c_str());
			_yolo = processor_module.attr("ObjectDetector")();

			if (!py::hasattr(_yolo, "load_model")) {
				throw std::runtime_error("Python class does not have method 'load_model'");
			}
			if (!py::hasattr(_yolo, "predict")) {
				throw std::runtime_error("Python class does not have method 'predict'");
			}
			return gcnew System::String(SUCCESS);
		}
		catch (const py::error_already_set& ex)
		{
			return gcnew System::String("PYTHON ERROR: ") + gcnew System::String(ex.what());
		}
		catch (const std::runtime_error& ex)
		{
			return gcnew System::String("Runtime error: ") + gcnew System::String(ex.what());
		}
		catch (const std::exception& ex)
		{
			return gcnew System::String("C++ Standard Exception: ") + gcnew System::String(ex.what());
		}
		catch (...)
		{
			return gcnew System::String("UNKNOWN ERROR.");
		}
	}

	System::String^ ClosePython()
	{
		try
		{
			py::gil_scoped_release release;

			if (Py_IsInitialized())
			{
				//py::gil_scoped_acquire acquire;
				Py_Finalize();
			}
			return gcnew System::String(SUCCESS);
		}
		catch (const py::error_already_set& e)
		{
			return gcnew System::String("ERROR (PYBIND11): ") + gcnew System::String(e.what());
		}
		catch (const std::runtime_error& ex)
		{
			return gcnew System::String("RUNTIME ERROR: ") + gcnew System::String(ex.what());
		}
		catch (const std::exception& e)
		{
			return gcnew System::String("ERROR: ") + gcnew System::String(e.what());
		}
		catch (...)
		{
			return gcnew System::String("UNKNOWN ERROR.");
		}
	}

	System::String^ LoadModel()
	{
		try
		{
			_yolo.attr("load_model")(pathModel);
			return gcnew System::String(SUCCESS);
		}
		catch (const py::error_already_set& ex)
		{
			return gcnew System::String("ERROR (PYTHON): ") + gcnew System::String(ex.what());
		}
		catch (const std::exception& ex)
		{
			return gcnew System::String("C++ Standard Exception: ") + gcnew System::String(ex.what());
		}
		catch (...)
		{
			return gcnew System::String("UNKNOWN ERROR.");
		}
	}

	Mat CropImage(Mat matCrop, Rect rect)
	{
		return matCrop(rect);
	}

	uchar* MatToBytes(cv::Mat image)
	{
		int image_size = image.total() * image.elemSize(); 
		uchar* image_uchar = new uchar[image_size];
		//image_uchar is a class data member of uchar*
		std::memcpy(image_uchar, image.data, image_size * sizeof(uchar));
		return image_uchar;
	}

	cv::Mat BytesToMat(uchar* uc, int image_rows, int image_cols, int image_type)
	{
		cv::Mat img(image_rows, image_cols, image_type, uc, cv::Mat::AUTO_STEP);

		return img;
	}

	Mat RotateMat(Mat raw, RotatedRect rot)
	{
		Mat matRs, matR = getRotationMatrix2D(rot.center, rot.angle, 1);

		float fTranslationX = (rot.size.width - 1) / 2.0f - rot.center.x;
		float fTranslationY = (rot.size.height - 1) / 2.0f - rot.center.y;
		matR.at<double>(0, 2) += fTranslationX;
		matR.at<double>(1, 2) += fTranslationY;
		warpAffine(raw, matRs, matR, rot.size, INTER_LINEAR, BORDER_CONSTANT);
		return matRs;
	}

	int getMaxAreaContourId(vector <vector<cv::Point>> contours) {
		double maxArea = 0;
		int maxAreaContourId = -1;
		for (int j = 0; j < contours.size(); j++) {
			double newArea = cv::contourArea(contours.at(j));
			if (newArea > maxArea) {
				maxArea = newArea;
				maxAreaContourId = j;
			} // End if
		} // End for
		return maxAreaContourId;
	}

	void G::CropRotate(int x, int y, int w, int  h, float angle) {
		matCrop = RotateMat(matRaw.clone(), RotatedRect(cv::Point2f(x, y), cv::Size2f(w, h), angle));
	}
	
	
	//void G::LoadSrc(System::String^ path)
	//{
	//	matRaw = cv::imread(_toString(path), ImreadModes::IMREAD_COLOR);
	//}
	//void G::LoadDst(System::String^ path)
	//{
	//	matTemp = cv::imread(_toString(path), ImreadModes::IMREAD_GRAYSCALE);
	//	///int a = matTemp.cols;
	//	//cv::imshow("a", matTemp);
	//}


	//std::string String2std(System::String^ str)
	//{
	//	const wchar_t* wbuf = static_cast<const wchar_t*>(System::Runtime::InteropServices::Marshal::StringToHGlobalUni(str).ToPointer());
	//	char* buf = new char[wcslen(wbuf) + 1];
	//	sprintf(buf, "%ls", wbuf);
	//	std::string result(buf);
	//	delete[] buf;
	//	return result;
	//}
}
