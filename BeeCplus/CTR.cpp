#include "CTR.h"

using namespace BeeCplus;

public enum paraVision {
	SCOREYOLO,
	CYCLETIME,
	PIXELCABLE ,
	NUMCABLE,
	DISTANCECABLE,
	PATHYOLO,
	PATHMODEL,
	NAMEYOLO,
	INVALID_PARAM
};

paraVision getParaVisionEnum(const string& name) {
	static unordered_map<string, paraVision> paramMap = {
		{"ScoreYolo", SCOREYOLO},
		{"CycleTime", CYCLETIME},
		{"PixelCable", PIXELCABLE},
		{"NumCable", NUMCABLE},
		{"Distance", DISTANCECABLE},
		{"PathYolo", PATHYOLO},
		{"PathModel", PATHMODEL},
		{"NameYolo", NAMEYOLO},
	};
	auto it = paramMap.find(name);
	if (it != paramMap.end()) {
		return it->second;
	}
	else {
		return INVALID_PARAM;
	}
}

System::String^ CTR::SetVision(System::String^ name, System::String^ value)
{
	return gcnew System::String("UNKNOWN ERROR.");
	//try
	//{
	//	int maxVal, minVal;
	//	paraVision param = getParaVisionEnum(String2std(name));
	//	switch (param)
	//	{
	//	case SCOREYOLO:

	//		break;
	//	case CYCLETIME:
	//		return gcnew System::String("DON'T SET VALUE.");
	//		break;
	//	case COUNTER:

	//		break;

	//	case PATHYOLO:

	//		break;
	//	case PATHMODEL:

	//		break;
	//	default:
	//		return gcnew System::String("INVALID PARAMETER NAME.");
	//	}
	//	return gcnew System::String(SUCCESS);
	//}
	//catch (const GenericException& e)
	//{
	//	return gcnew System::String("ERROR:  ") + gcnew System::String(e.GetDescription());
	//}
	//catch (...)
	//{
	//	return gcnew System::String("UNKNOWN ERROR.");
	//}
}

System::String^ CTR::GetVision(System::String^ name)
{
	return gcnew System::String("UNKNOWN ERROR.");
	/*
	try
	{
		paraVision param = getParaVisionEnum(String2std(name));
		switch (param)
		{
		case CYCLETIME:
			return gcnew System::String(std::to_string(camGigE.ExposureTimeRaw.GetValue()).c_str());
		case COUNTER:
			return gcnew System::String(std::to_string(camGigE.GainRaw.GetValue()).c_str());
		case SCOREYOLO:
			return gcnew System::String(std::to_string(camGigE.OffsetX.GetValue()).c_str());
		case PATHYOLO:
			return gcnew System::String(std::to_string(camGigE.OffsetY.GetValue()).c_str());
		case PATHMODEL:
			return gcnew System::String(std::to_string(camGigE.Width.GetValue()).c_str());
		default:
			return gcnew System::String("INVALID PARAMETER NAME.");
		}
	}
	catch (const GenericException& e)
	{
		return gcnew System::String("Error: ") + gcnew System::String(e.GetDescription());
	}
	catch (...)
	{
		return gcnew System::String("UNKNOWN ERROR.");
	}*/
}

Rect getROI(const Mat& img) {
	int roi_x = img.cols / 5;
	int roi_y = img.rows / 4;
	int roi_width = min(static_cast<int>(img.cols / 1.2), img.cols - roi_x);
	int roi_height = min(static_cast<int>(img.rows / 1.2), img.rows - roi_y);
	return Rect(roi_x, roi_y, 800, 50);
}

//Mat preImage(const Mat& img) {
//	Mat mBlurImage;
//	medianBlur(img, mBlurImage, 3);
//	Mat blurImage;
//	blur(mBlurImage, blurImage, cv::Size(3, 3));
//	Mat thresholdImage;
//	//threshold(blurredImage, thresholdImage, 120, 255, THRESH_BINARY_INV);
//	threshold(blurImage, thresholdImage, 80, 255, THRESH_BINARY);
//	bitwise_not(thresholdImage, thresholdImage);
//	Mat mono8Image = Mat(thresholdImage.size(), CV_8UC1, Scalar(0));
//	rectangle(mono8Image, Rect(0, 0, img.cols, img.rows), Scalar(255), -1);
//	Mat result;
//	bitwise_and(mono8Image, thresholdImage, result);
//	return result;
//}
//
//float meaSure(const Mat& mat1, const Rect& roi, cv::Point& midpoint1, cv::Point& midpoint2) {
//	int xmin_top = mat1.cols - 1, xmax_top = 0;
//	int xmin_bottom = mat1.cols - 1, xmax_bottom = 0;
//
//	for (int x = 0; x < mat1.cols; ++x) {
//		if (mat1.at<uchar>(0, x) == 255) {
//			xmin_top = min(xmin_top, x);
//			xmax_top = max(xmax_top, x);
//		}
//		if (mat1.at<uchar>(mat1.rows - 1, x) == 255) {
//			xmin_bottom = min(xmin_bottom, x);
//			xmax_bottom = max(xmax_bottom, x);
//		}
//	}
//
//	if (xmin_top == mat1.cols - 1 || xmax_top == 0 ||
//		xmin_bottom == mat1.cols - 1 || xmax_bottom == 0) {
//		return -1;
//	}
//
//	cv::Point pt1(xmin_top + roi.x, roi.y);
//	cv::Point pt2(xmax_top + roi.x, roi.y);
//	cv::Point pt3(xmin_bottom + roi.x, roi.y + roi.height - 1);
//	cv::Point pt4(xmax_bottom + roi.x, roi.y + roi.height - 1);
//
//	midpoint1 = cv::Point((pt1.x + pt3.x) / 2, (pt1.y + pt3.y) / 2);
//	midpoint2 = cv::Point((pt2.x + pt4.x) / 2, (pt2.y + pt4.y) / 2);
//
//	return sqrt(pow(midpoint2.x - midpoint1.x, 2) + pow(midpoint2.y - midpoint1.y, 2));
//}
//
//void showResult(Mat& img, int distance, int cable) {
//	string cableText = "Cable: " + to_string(cable);
//	string distanceText = "Width: " + to_string(distance);
//	putText(img, distanceText, cv::Point(10, img.rows - 40), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 255, 0), 2);
//	putText(img, cableText, cv::Point(10, img.rows - 10), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 255, 0), 2);
//}
//
//int calib(Mat& img, int cable) {
//	Rect roi = getROI(img);
//	//Mat roi_img(img(roi));
//	//imshow("", roi_img);
//
//	Mat newImage = preImage(img);
//	//imshow("calib", newImage);
//	cv::Point midpoint1, midpoint2;
//	int distance = meaSure(newImage, roi, midpoint1, midpoint2);
//
//	if (distance == -1) {
//		return -1;
//	}
//
//	int result = distance / cable;
//	return result;
//}

cv::Mat numpy_array_to_cv_mat(const py::array_t<unsigned char>& arr) {
	py::buffer_info buf_info = arr.request();
	int height = buf_info.shape[0];
	int width = buf_info.shape[1];
	int channels = buf_info.shape[2];
	return cv::Mat(height, width, CV_8UC3, (unsigned char*)buf_info.ptr);
}

py::array_t<unsigned char> mat_to_numpy(const cv::Mat& mat) {
	if (mat.empty()) {
		throw std::runtime_error("cv::Mat is empty");
	}
	return py::array_t<unsigned char>({ mat.rows, mat.cols, mat.channels() },mat.data);
}

extern "C" __declspec(dllexport) const char* IniPython()
{
	try
	{
		InitializeYolo();
		LoadModel();
		//PylonInitialize();
		return SUCCESS;
	}
	catch (const GenericException& e)
	{
		std::string s = ("ERROR :");
		s.append(e.GetDescription());
		return s.c_str();
	}
	catch (const std::exception& e)
	{
		std::string s = ("ERROR :");
		s.append(e.what());
		return s.c_str();
	}
	catch (...)
	{
		return "UNKNOWN ERROR.";
	}
}

extern "C" __declspec(dllexport) const char* DestroyPython()
{
	py::gil_scoped_release release;
	try
	{
		if (Py_IsInitialized())
		{
			//py::gil_scoped_acquire acquire;
			Py_Finalize();
		}
		
		return SUCCESS;
	}
	catch (const py::error_already_set& e)
	{
		std::string err = ("ERROR :");
		err.append(e.what());
		return err.c_str();
	}
	catch (const std::exception& e)
	{
		std::string err = ("ERROR :");
		err.append(e.what());
		return err.c_str();
	}
	catch (...)
	{
		return "UNKNOWN ERROR.";
	}
}

extern "C" __declspec(dllexport) void TestYolo()
{
	double startTime = clock();
	int numDetected = 0;
	float pixelCable = 0, sumOfAll = 0;
	int distanceV = 0, cycleTime = 0;

	std::ostringstream resultStream;
	std::string status;

	Mat img = imread("C:\\test.png");

	try {
	//	std::lock_guard<std::mutex>lock(gilmutex);
		 py::gil_scoped_release release;
		 py::gil_scoped_acquire acquire;


		py::array_t<uint8_t> image_array = mat_to_numpy(img);
		auto result = _yolo.attr("predict")(image_array,0.6);

		//Rect roi = getROI(matProc);
		//Mat preimg = preImage(matProc);
		//cv::Point midpoint1, midpoint2;
		//distanceV = meaSure(preimg, roi, midpoint1, midpoint2);

		if (py::isinstance<py::tuple>(result)) {
			auto result_tuple = result.cast<py::tuple>();
			py::list boxes = result_tuple[0].cast<py::list>();
			py::list scores = result_tuple[1].cast<py::list>();

			numDetected = boxes.size();

			for (size_t i = 0; i < boxes.size(); ++i) {
				auto box = boxes[i].cast<py::tuple>();
				int x1 = box[0].cast<int>(), y1 = box[1].cast<int>();
				int x2 = box[2].cast<int>(), y2 = box[3].cast<int>();
				float score = scores[i].cast<float>();

				sumOfAll += sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

				cv::Scalar color = (score >= 0.8) ? COLOR_EXCELLENT : (score >= 0.7) ? COLOR_GOOD : COLOR_AVERAGE;
				cv::rectangle(img, { x1, y1 }, { x2, y2 }, color, 2);
				cv::putText(img, std::to_string(score).substr(0, 3), { x1, y1 - 5 }, cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1);

			}
			if (numDetected == 0) {
				pixelCable = sumOfAll / numDetected;
				int checking = distanceV / pixelCable;

				status = (checking == numDetected) ? "OK"
					: (checking < numDetected) ? "ERROR"
					: "DETECTED AGAIN";
			}
			else status = "NULL CABLE DETECTED.";
		}
		else {
			status = "UNKNOWN ERROR.";
		}
	}
	catch (const py::error_already_set& e) {
		status = std::string("PYTHON ERROR: ") + e.what();
	}
	catch (...) {
		status = "UNKNOWN EXCEPTION.";
	}
	cycleTime = int(clock() - startTime);
	resultStream << status
		<< " numDetected=" << numDetected
		<< ", pixelCable=" << std::fixed << std::setprecision(1) << pixelCable
		<< ", distance=" << distanceV
		<< ", cycleTime=" << cycleTime;
	//return gcnew System::String(resultStream.str().c_str());
	cout << resultStream.str().c_str() << endl;
	Py_Finalize();
	py::gil_scoped_release release;
}

//extern "C" __declspec(dllexport) const char* Check(float Score)
//{
//	ScoreYolo = Score;
//	double startTime = clock();
//	int numDetected = 0;
//	float pixelCable = 0, sumOfAll = 0;
//	int distanceV = 0, cycleTime = 0;
//	std::ostringstream resultStream;
//	std::string status;
//
//	Mat img = Mat();
//
//	if (matRaw.empty()) return "NONE IMAGE.";
//	if (matRaw.type() == CV_8UC1) {
//		cv::cvtColor(matRaw, img, CV_GRAY2BGR);
//	}
//	else {
//		img = matRaw.clone();
//	}
//	
//	std::lock_guard<std::mutex>lock(gilmutex);
//	py::gil_scoped_release release;
//	try {
//		py::gil_scoped_acquire acquire;
//
//		py::array_t<uint8_t> image_array = mat_to_numpy(img);
//		auto result = _yolo.attr("predict")(image_array, ScoreYolo);
//
//		//Rect roi = getROI(matProc);
//		//Mat preimg = preImage(matProc);
//		//cv::Point midpoint1, midpoint2;
//		//distanceV = meaSure(preimg, roi, midpoint1, midpoint2);
//
//		matResult = img.clone();
//		if (py::isinstance<py::tuple>(result)) {
//			auto result_tuple = result.cast<py::tuple>();
//			py::list boxes = result_tuple[0].cast<py::list>();
//			py::list scores = result_tuple[1].cast<py::list>();
//
//			numDetected = boxes.size();
//
//			for (size_t i = 0; i < boxes.size(); ++i) {
//				auto box = boxes[i].cast<py::tuple>();
//				int x1 = box[0].cast<int>(), y1 = box[1].cast<int>();
//				int x2 = box[2].cast<int>(), y2 = box[3].cast<int>();
//				float score = scores[i].cast<float>();
//
//				sumOfAll += sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
//
//				cv::Scalar color = (score >= 0.8) ? COLOR_EXCELLENT : (score >= 0.7) ? COLOR_GOOD : COLOR_AVERAGE;
//				cv::rectangle(matResult, { x1, y1 }, { x2, y2 }, color, 2);
//				cv::putText(matResult, std::to_string(score).substr(0, 3), { x1, y1 - 5 }, cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
//
//			}
//			if (numDetected == 0) {
//				pixelCable = sumOfAll / numDetected;
//				int checking = distanceV / pixelCable;
//
//				status = (checking == numDetected) ? "OK"
//					: (checking < numDetected) ? "ERROR"
//					: "DETECTED AGAIN";
//			}
//			else status = "NULL CABLE DETECTED.";
//		}
//		else {
//			status = "UNKNOWN ERROR.";
//		}
//	}
//	catch (const py::error_already_set& e) {
//		status = std::string("PYTHON ERROR: ") + e.what();
//	}
//	catch (...) {
//		status = "UNKNOWN EXCEPTION.";
//	}
//	cycleTime = int(clock() - startTime);
//	resultStream << status
//		<< " numDetected=" << numDetected
//		<< ", pixelCable=" << std::fixed << std::setprecision(1) << pixelCable
//		<< ", distance=" << distanceV
//		<< ", cycleTime=" << cycleTime;
//	//py::gil_scoped_release release;
//	//std::lock_guard<std::mutex>unlock(gilmutex);
//	return resultStream.str().c_str();
//}

extern "C" __declspec(dllexport) void initialize_python() {
//	std::lock_guard<std::mutex> lock(gilmutex);
	if (!python_initialized) {
		py::initialize_interpreter();  // Khởi tạo Python
		python_initialized = true;
		std::cout << "Python initialized.\n";
	}
	nt.notify_all();
}



extern "C" __declspec(dllexport) const char* Check(float Score)
{
	//std::lock_guard<std::mutex> lock(gilmutex);
	py::gil_scoped_acquire acquire;  // Giành GIL
	py::gil_scoped_release release;      // Giải phóng GIL sau khi thực hiện tác vụ Python

	ScoreYolo = Score;
	double startTime = clock();
	int numDetected = 0;
	float pixelCable = 0, sumOfAll = 0;
	int distanceV = 0, cycleTime = 0;
	std::ostringstream resultStream;
	std::string status;

	Mat img = Mat();

	// Kiểm tra xem hình ảnh có trống không
	if (matRaw.empty()) return "NONE IMAGE.";

	// Nếu ảnh là ảnh xám, chuyển sang ảnh màu BGR
	if (matRaw.type() == CV_8UC1) {
		cv::cvtColor(matRaw, img, CV_GRAY2BGR);
	}
	else {
		img = matRaw.clone();
	}

	// Bảo vệ tài nguyên bằng mutex để đảm bảo thread safety
	//std::lock_guard<std::mutex> lock(gilmutex);

	try {
		// Giành quyền GIL để gọi Python code
		//py::gil_scoped_acquire acquire;

		// Chuyển đổi OpenCV image sang numpy array để gửi tới Python
		py::array_t<uint8_t> image_array = mat_to_numpy(img);

		// Gọi hàm YOLO trong Python để dự đoán
		auto result = _yolo.attr("predict")(image_array, ScoreYolo);

		matResult = img.clone();

		// Kiểm tra kết quả trả về từ Python
		if (py::isinstance<py::tuple>(result)) {
			auto result_tuple = result.cast<py::tuple>();
			py::list boxes = result_tuple[0].cast<py::list>();
			py::list scores = result_tuple[1].cast<py::list>();

			numDetected = boxes.size();

			// Duyệt qua các box dự đoán
			for (size_t i = 0; i < boxes.size(); ++i) {
				auto box = boxes[i].cast<py::tuple>();
				int x1 = box[0].cast<int>(), y1 = box[1].cast<int>();
				int x2 = box[2].cast<int>(), y2 = box[3].cast<int>();
				float score = scores[i].cast<float>();

				sumOfAll += sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

				// Vẽ hình chữ nhật và hiển thị điểm số
				cv::Scalar color = (score >= 0.8) ? COLOR_EXCELLENT : (score >= 0.7) ? COLOR_GOOD : COLOR_AVERAGE;
				cv::rectangle(matResult, { x1, y1 }, { x2, y2 }, color, 2);
				cv::putText(matResult, std::to_string(score).substr(0, 3), { x1, y1 - 5 }, cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
			}

			// Kiểm tra nếu không phát hiện vật thể
			if (numDetected == 0) {
				pixelCable = sumOfAll / numDetected;
				int checking = distanceV / pixelCable;

				status = (checking == numDetected) ? "OK"
					: (checking < numDetected) ? "ERROR"
					: "DETECTED AGAIN";
			}
			else {
				status = "NULL CABLE DETECTED.";
			}
		}
		else {
			status = "UNKNOWN ERROR.";
		}
	}
	catch (const py::error_already_set& e) {
		status = std::string("PYTHON ERROR: ") + e.what();
	}
	catch (...) {
		status = "UNKNOWN EXCEPTION.";
	}

	// Tính thời gian chu kỳ
	cycleTime = int(clock() - startTime);

	// Ghi kết quả vào stream
	resultStream << status
		<< " numDetected=" << numDetected
		<< ", pixelCable=" << std::fixed << std::setprecision(1) << pixelCable
		<< ", distance=" << distanceV
		<< ", cycleTime=" << cycleTime;

	// Trả về kết quả
	return resultStream.str().c_str();
}

PYBIND11_MODULE(example, m) {
	m.def("numpy_array_to_cv_mat", &numpy_array_to_cv_mat, "Gui hinh Len Python");
}

