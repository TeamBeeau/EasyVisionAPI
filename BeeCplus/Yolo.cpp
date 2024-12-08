#include "Yolo.h"
#define PYBIND11_NO_ASSERT_GIL_HELD_INCREF_DECREF
using namespace BeeCplus;
using namespace cv;


struct gil_scoped_acquire_local {
	gil_scoped_acquire_local() : state(PyGILState_Ensure()) {}
	gil_scoped_acquire_local(const gil_scoped_acquire_local&) = delete;
	gil_scoped_acquire_local& operator=(const gil_scoped_acquire_local&) = delete;
	~gil_scoped_acquire_local() { PyGILState_Release(state); }
	const PyGILState_STATE state;
};
cv::Mat addWhitePadding(const cv::Mat& image, int padding_top, int padding_bottom) {
	cv::Mat padded_image;
	cv::copyMakeBorder(
		image,
		padded_image,
		padding_top,
		padding_bottom,
		0,
		0,
		cv::BORDER_CONSTANT,
		cv::Scalar(255, 255, 255)
	);

	return padded_image;
}
void recFilled(cv::Mat& image, cv::Point top_left, cv::Point bottom_right, cv::Scalar color, double alpha) {
	cv::Mat overlay = image.clone();
	cv::rectangle(overlay, { top_left.x , top_left.y }, { bottom_right.x , bottom_right.y }, color, cv::FILLED);
	cv::addWeighted(overlay, alpha, image, 1 - alpha, 0, image);
}
Rect getROI(Mat& img) {
	int roi_x = img.cols / 7;
	int roi_y = img.rows / 4;
	int roi_width = img.cols * 0.8;
	int roi_height = img.rows * 0.5;
	return Rect(roi_x, roi_y, roi_width, roi_height);
}

Mat preImage(const Mat& img) {
	Mat grayImage, mBlurImage, blurImage, thresholdImage;
	if (img.channels() == 1 && img.depth() == CV_8UC1) {
		grayImage = img;
	}
	else {
		cvtColor(img, grayImage, COLOR_BGR2GRAY);
	}
	medianBlur(grayImage, mBlurImage, 3);
	blur(mBlurImage, blurImage, cv::Size(3, 3));
	threshold(blurImage, thresholdImage, 150, 255, THRESH_BINARY);
	bitwise_not(thresholdImage, thresholdImage);

	Mat mono8Image = Mat(thresholdImage.size(), CV_8UC1, Scalar(255));
	Mat result;
	bitwise_and(mono8Image, thresholdImage, result);
	return result;
}


float meaSure(const Mat& mat1, const Rect& roi, cv::Point& midpoint1, cv::Point& midpoint2) {
	int xmin_top = mat1.cols - 1, xmax_top = 0;
	int xmin_bottom = mat1.cols - 1, xmax_bottom = 0;

	for (int x = 0; x < mat1.cols; ++x) {
		if (mat1.at<uchar>(0, x) == 255) {
			xmin_top = min(xmin_top, x);
			xmax_top = max(xmax_top, x);
		}
		if (mat1.at<uchar>(mat1.rows - 1, x) == 255) {
			xmin_bottom = min(xmin_bottom, x);
			xmax_bottom = max(xmax_bottom, x);
		}
	}

	if (xmin_top == mat1.cols - 1 || xmax_top == 0 ||
		xmin_bottom == mat1.cols - 1 || xmax_bottom == 0) {
		cout << "No white pixels found in the extremes." << endl;
		return -1;
	}

	cv::Point pt1(xmin_top + roi.x, roi.y);
	cv::Point pt2(xmax_top + roi.x, roi.y);
	cv::Point pt3(xmin_bottom + roi.x, roi.y + roi.height - 1);
	cv::Point pt4(xmax_bottom + roi.x, roi.y + roi.height - 1);

	midpoint1 = cv::Point((pt1.x + pt3.x) / 2, (pt1.y + pt3.y) / 2);
	midpoint2 = cv::Point((pt2.x + pt4.x) / 2, (pt2.y + pt4.y) / 2);

	//line(matResult, midpoint1, midpoint2, Scalar(255, 0, 0), 1);
	// Return the distance between the midpoints
	return sqrt(pow(midpoint2.x - midpoint1.x, 2) + pow(midpoint2.y - midpoint1.y, 2));
}

py::array_t<unsigned char> mat_to_numpy1(const cv::Mat& mat) {
	if (mat.empty()) {
		throw std::runtime_error("cv::Mat is empty");
	}
	return py::array_t<unsigned char>({ mat.rows, mat.cols, mat.channels() }, mat.data);
}
System::String^ Yolo::FinalizeGIL() {
	if (!Py_IsInitialized()) {
		std::cerr << "Python initialization failed!" << std::endl;
		return "Python initialization failed!";
	}
	_yolo.attr("close")();
	Py_FinalizeEx();

	//python_terminated = true;
	nt.notify_all();

	//py::finalize_interpreter();
	return SUCCESS;
}
System::String^ Yolo::IniStart()
{

	return SUCCESS;
}
int indexImage = 0;
System::String^ Yolo::IniGIL() {
	try
	{
		std::lock_guard<std::mutex> lock(gilmutex);
		Py_Initialize();
		gil_scoped_acquire_local gil_acquire;

		//	std::lock_guard<std::mutex> lock(gilmutex);
		//std::lock_guard<std::mutex>lock(gilmutex);

			//std::unique_lock<std::mutex> lock(gilmutex);
			//py::gil_scoped_acquire acquire;
			//gil_scoped_acquire_local gil_acquire;
			//	py::gil_scoped_acquire acquire;
			//	py::gil_scoped_acquire acquire;  // Giành GIL
			//pybind11::gil_scoped_acquire acquire; // Tự động đăng ký và giữ GIL
		py::module processor_module = py::module::import("yolo");
		_yolo = processor_module.attr("ObjectDetector")();
		_yolo.attr("load_model")(pathModel);
		auto ptr = std::make_unique<int[]>(10);
		//lock.unlock();
		//py::gil_scoped_release release;
		//PyEval_SaveThread
		//PylonInitialize();
		return SUCCESS;
	}
	catch (const GenericException& e)
	{
		std::string err = ("ERROR :");
		err.append(e.GetDescription());
		return  gcnew System::String(err.c_str());
	}
	catch (const std::exception& e)
	{
		std::string err = ("ERROR :");
		err.append(e.what());
		return  gcnew System::String(err.c_str());
	}
	catch (...)
	{
		return "UNKNOWN ERROR.";
	}



	//gil_acquire.~gil_scoped_acquire_local();
//	
}


System::String^ Yolo::ImportRaw()
{
	System::String^ managedString = "test.png";
	msclr::interop::marshal_context context;
	std::string path = context.marshal_as<std::string>(managedString);

	matRaw = imread(path);
	//indexImage++;
	if (matRaw.type() == CV_8UC1) {
		cv::cvtColor(matRaw, matProcess, CV_GRAY2BGR);
	}
	else {
		matProcess = matRaw.clone();
	}
	matResult = matProcess.clone();
	return "Check again Inintial Lib";
}
bool IsCheking = false;
std::tuple<py::list, py::list, float> GIL(float Score)
{
	//std::lock_guard<std::mutex>lock(gilmutex);
	IsCheking = true;
	std::string status = "";
	try
	{
		if (!Py_Initialize || !_yolo)
		{
			IsCheking = false;
			return std::tuple<py::list, py::list, float>();
		}




		//	py::gil_scoped_release release;
		//	py::gil_scoped_acquire acquire;


		// Chuyển đổi OpenCV image sang numpy array để gửi tới Python
		py::array_t<uint8_t> image_array = mat_to_numpy1(matProcess);
		// Gọi hàm YOLO trong Python để dự đoán


		auto result = _yolo.attr("predict")(image_array, Score);



		// Kiểm tra kết quả trả về từ Python
		if (py::isinstance<py::tuple>(result)) {
			std::tuple<py::list, py::list, float> lisRS;
			auto result_tuple = result.cast<py::tuple>();
			py::list boxes = result_tuple[0].cast<py::list>();
			py::list scores = result_tuple[1].cast<py::list>();
			float avg_width = result_tuple[2].cast<float>();    // Average width
			lisRS = std::make_tuple(boxes, scores, avg_width); IsCheking = false;
			//py::gil_scoped_release release;
			//auto ptr = std::make_unique<int[]>(10);
			return lisRS;
		}

	}
	catch (const py::error_already_set& e) {
		status = std::string("PYTHON ERROR: ") + e.what();
	}
	catch (...) {
		status = "UNKNOWN EXCEPTION.";
	}
	IsCheking = false;

	return std::tuple<py::list, py::list, float>();
}

struct BoundingBox {
	int x1, y1, x2, y2;
};
cv::Point midpoint1, midpoint2;
int reChecking(Mat& image, const std::vector<BoundingBox>& bounding_boxes, int numwire, float clength) {
	if (bounding_boxes.size() == 0)return 0;
	std::vector<std::pair<cv::Point, cv::Point>> segments;
	cv::Point STARTPoint, ENDPoint;

	/*STARTPoint = cv::Point(bounding_boxes[0].x1, image.rows / 2);*/
	//ENDPoint = cv::Point(bounding_boxes[bounding_boxes.size() - 1].x2, image.rows / 2);
	STARTPoint = midpoint1;
	ENDPoint = midpoint2;
	LineIterator it(image, STARTPoint, ENDPoint, 8);
	bool insideBox = false;
	cv::Point segmentStart = STARTPoint;



	for (int i = 0; i < it.count; ++i, ++it) {
		cv::Point pt(it.pos());
		insideBox = false;

		for (const auto& box : bounding_boxes) {
			if (pt.x >= box.x1 && pt.x <= box.x2 && pt.y >= box.y1 && pt.y <= box.y2) {
				insideBox = true;
				break;
			}
		}

		if (insideBox) {
			if (segmentStart != pt) {
				segments.push_back({ segmentStart, pt });
			}
			while (i < it.count && insideBox) {
				++i;
				++it;
				pt = it.pos();
				insideBox = false;
				for (const auto& box : bounding_boxes) {
					if (pt.x >= box.x1 && pt.x <= box.x2 && pt.y >= box.y1 && pt.y <= box.y2) {
						insideBox = true;
						break;
					}
				}
			}
			segmentStart = pt;
		}
	}
	//cv::rectangle(image, ENDPoint, midpoint2, COLOR_POOR, 1);
	//cv::putText(image, "EN", { ENDPoint.x, 0 + 30 }, cv::FONT_HERSHEY_SIMPLEX, 0.5, COLOR_POOR, 1);
	//numwire += 1;

	if (segmentStart != ENDPoint) {
		segments.push_back({ segmentStart, ENDPoint });
	}
	int index = 0;
	for (const auto& segment : segments) {
		double length = sqrt(pow(segment.second.x - segment.first.x, 2) + pow(segment.second.y - segment.first.y, 2));

		if (length >= clength * 0.4 && length < clength * 1.4) {
			numwire++;

			recFilled(matResult, { segment.first.x, 30 }, { segment.second.x, 130 }, COLOR_POOR, 0.2);
			cv::putText(matResult, "+1", {segment.first.x, +15}, cv::FONT_HERSHEY_SIMPLEX, 0.5, COLOR_POOR, 1);
			//cv::putText(matResult, std::to_string(i).substr(0, 3), { x1, 145 }, cv::FONT_HERSHEY_SIMPLEX, 0.3, color, 1);

		}
		else if (segment == segments[segments.size() - 1] && length <  clength * 0.4 ) //
	   {
			numwire++;
			recFilled(matResult, { segment.first.x, 30 }, { segment.second.x, 130 }, COLOR_POOR, 0.2);
			cv::putText(matResult, "", { segment.first.x, +15 }, cv::FONT_HERSHEY_SIMPLEX, 0.5, COLOR_POOR, 1);

		}
		else if (length >= clength * 1.4 && length < clength * 2.4)
		{
			numwire = numwire + 2;
			recFilled(matResult, { segment.first.x, 30 }, { segment.second.x, 130 }, COLOR_POOR, 0.2);
			cv::putText(matResult, "+2", { segment.first.x, +15 }, cv::FONT_HERSHEY_SIMPLEX, 0.5, COLOR_POOR, 1);
		}
		else if (length >= clength * 2.4 && length < clength * 3.4)
		{
			numwire = numwire + 3;
			recFilled(matResult, { segment.first.x, 30 }, { segment.second.x, 130 }, COLOR_POOR, 0.2);
			cv::putText(matResult, "+3", { segment.first.x, +15 }, cv::FONT_HERSHEY_SIMPLEX, 0.5, COLOR_POOR, 1);
		}
		else
		{
			numwire = numwire + 0;
		}
	}


	return numwire;
}

int xLeft = 0;
bool IsReseted = false;


int numWire = 0;
System::String^ Yolo::Reset()
{
	/*boundingBoxes.clear();
	IsReseted = true;
	indexImage = 0;
	numWire = 0;*/
	//boundingBoxes = {};
	return "";// boundingBoxes.size().ToString();
}
// Hàm so sánh để sắp xếp theo tọa độ X
bool compareByX(const BoundingBox& a, const BoundingBox& b) {
	return a.x1 < b.x1; // Sắp xếp tăng dần theo X
}

int xEndYolo = 0;
int xEndYoloOld = 0;
System::String^ Yolo::CheckYolo(float Score) {
	numCable = 0;
	if (IsCheking)
		return "WAIT";
	if (PyGILState_Check() == 0)
	{//_yolo.attr("load_model")(pathModel);
	//	PyGILState_Ensure();
	//	FinalizeGIL();
		//IniGIL();
		return FALSE;
	}


	if (!Py_Initialize || !_yolo)
	{

		return FALSE;
	}
	double startTime = clock();


	// Khóa mutex để đảm bảo không có luồng khác truy cập GIL đồng thời
	std::lock_guard<std::mutex> lock(gilmutex);

	py::gil_scoped_acquire acquire;
	std::tuple<py::list, py::list, float> result = GIL(Score);


	float pixelCable = 0, sumOfAll = 0;
	float avg_width = 0;
	int distanceV = 0, finalCable, cycleTime = 0, qty = 0;
	std::ostringstream resultStream;
	std::string status = "";


	matResult = matProcess.clone();
	std::vector<BoundingBox> boundingBoxes = {};


	Rect roi = getROI(matResult);
	Mat img_roi(matResult(roi));
	Mat preimg = preImage(img_roi);


	try {
		py::list Boxes = std::get<0>(result);
		py::list Scores = std::get<1>(result);
		avg_width = std::get<2>(result);


		int	numDetectYolo = Boxes.size();
		/*if (IsReseted && boundingBoxes.size() > 0)
		{
			IsReseted = false;
			xLeft = midpoint1.x;
		}*/

		//std::vector<BoundingBox> boundingNews = {};
		int width = 0;
		matResult = addWhitePadding(matResult, 30, 30);

		for (size_t i = 0; i < Boxes.size(); ++i) {
			auto box = Boxes[i].cast<py::tuple>();
			int x1 = box[0].cast<int>() + 30, y1 = box[1].cast<int>() + 30;
			int x2 = box[2].cast<int>() + 30, y2 = box[3].cast<int>() + 30;
			float score = Scores[i].cast<float>();
			boundingBoxes.push_back({ x1, y1, x2, y2 });
			//cv::Scalar color = (score >= 0.8) ? COLOR_EXCELLENT : (score >= 0.7) ? COLOR_GOOD : COLOR_AVERAGE;


			//cv::rectangle(matResult, { x1, y1 }, { x2,y2 }, color, 2);
			
			if (i % 2 == 0) {
				recFilled(matResult, { x1, y1 }, { x2,y2 }, COLOR_EXCELLENT, 0.2);
				cv::putText(matResult, std::to_string(score).substr(0, 3), { x1, y1 - 15 }, cv::FONT_HERSHEY_SIMPLEX, 0.5, COLOR_EXCELLENT, 1);
				cv::putText(matResult, std::to_string(i + 1).substr(0, 3), { x1, 145 }, cv::FONT_HERSHEY_SIMPLEX, 0.5, COLOR_EXCELLENT, 1);
			}
			else
			{
				recFilled(matResult, { x1, y1 }, { x2,y2 }, COLOR_2EXCELLENT, 0.2);
				cv::putText(matResult, std::to_string(score).substr(0, 3), { x1, y1 - 15 }, cv::FONT_HERSHEY_SIMPLEX, 0.5, COLOR_2EXCELLENT, 1);
				cv::putText(matResult, std::to_string(i + 1).substr(0, 3), { x1, 145 }, cv::FONT_HERSHEY_SIMPLEX, 0.5, COLOR_2EXCELLENT, 1);
			}

			//BoundingBox boundTemp = { x1, y1, x2, y2 };
			//if (boundingBoxes.size() == 0)
			//{

			//	boundingNews.push_back({ x1, y1, x2, y2 });

			//}
			//else if (boundTemp.x2 <= boundingBoxes[0].x1 + 2)
			//{
			//	boundingNews.push_back({ x1, y1, x2, y2 });
			//	width += Math::Abs(x2 - x1);//A
			//}

		}
		distanceV = meaSure(preimg, roi, midpoint1, midpoint2);
	/*	if (boundingBoxes.size() > 0 && xEndYolo != 0)
		{
			int delta = xEndYoloOld - xEndYolo;
			for each (BoundingBox box in boundingBoxes)
			{
				box.x1 -= delta;
				box.x2 -= delta;
			}
			if (xEndYolo != xEndYoloOld)
				xEndYoloOld = xEndYolo;
		}*/

		/*for each (BoundingBox box in boundingBoxes)
		{
			box.x1 += width;
			box.x2 += width;
		}*/
		// Gộp vector2 vào vector1
	//	boundingBoxes.insert(boundingBoxes.end(), boundingNews.begin(), boundingNews.end());
	//	std::sort(boundingBoxes.begin(), boundingBoxes.end(), compareByX);
	//	xEndYolo = boundingBoxes[boundingBoxes.size() - 1].x2;

		///boundingBoxes.push_back(boundingNews);
		//for each (BoundingBox box in boundingBoxes)
		//{
		//	cv::Scalar color = COLOR_GOOD;// (score >= 0.8) ? COLOR_EXCELLENT : (score >= 0.7) ? COLOR_GOOD : COLOR_AVERAGE;
		//	cv::rectangle(matResult, { box.x1, box.y1 }, { box.x2, box.y2 }, color, 2);
		//	//	cv::putText(matResult, std::to_string(score).substr(0, 3), { x1, y1 + 15 }, cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1);

		//}


		//check again
		//if (boundingBoxes.size() > 1)
		numCable = reChecking(matResult, boundingBoxes, boundingBoxes.size(), avg_width);



		//else
		//	qty = boundingBoxes.size();
		//if (qty > numCable)
		//	numCable = qty;

		//System::String^ managedString = "Test" + numCable + ".png";
		//msclr::interop::marshal_context context;
		//std::string path = context.marshal_as<std::string>(managedString);

		//cv::imwrite(path, matResult);
		//indexImage++;
		//			py::gil_scoped_release release;

		//// Kiểm tra nếu không phát hiện vật thể
		//if (numDetected > 0) {
		//	/*qty = reChecking(matResult, midpoint1, midpoint2, boundingBoxes, numDetected, avg_width);*/
		//	//pixelCable = sumOfAll / numDetected;
		//	//finalCable = distanceV / pixelCable;

		//	//if (finalCable > numDetected) {
		//	//	finalCable = finalCable + recounter(matResult,midpoint1,midpoint2,Boxes);
		//	//}
		//}
		//
	//	cv::imshow("RS", matResult);
	}

	catch (...) {
		status = "UNKNOWN EXCEPTION.";
	}

	if (status != "")
		return gcnew System::String(status.c_str());
	int cycle = int(clock() - startTime);
	return gcnew System::String("0" + "," + numCable + "," + cycle);
}

//System::String^ Yolo::CheckYolo(float Score) {
//
//	if (IsCheking)
//		return FALSE;
//	if (PyGILState_Check() == 0)
//	{
//		return FALSE;
//	}
//	if (!Py_Initialize || !_yolo)
//	{
//
//		return FALSE;
//	}
//	
//	double startTime = clock();
//	std::tuple<py::list, py::list> result = GIL(Score);
//	int numDetected = 0;
//	float pixelCable = 0, sumOfAll = 0;
//	int distanceV = 0, cycleTime = 0;
//	std::ostringstream resultStream;
//	std::string status = "";
//
//	
//	// Kiểm tra xem hình ảnh có trống không
//
//	// Nếu ảnh là ảnh xám, chuyển sang ảnh màu BGR
//	
//
//	// Bảo vệ tài nguyên bằng mutex để đảm bảo thread safety
//	//std::lock_guard<std::mutex> lock(gilmutex);
//
//	try {
//		//std::lock_guard<std::mutex>lock(gilmutex);
//		py::gil_scoped_release release;
//		py::gil_scoped_acquire acquire;
//
//		// Giành quyền GIL để gọi Python code
//		//py::gil_scoped_acquire acquire;
//
//		// Chuyển đổi OpenCV image sang numpy array để gửi tới Python
//		py::array_t<uint8_t> image_array = mat_to_numpy1(matProcess);
//
//		// Gọi hàm YOLO trong Python để dự đoán
//		auto result = _yolo.attr("predict")(image_array, Score);
//
//		matResult = matProcess.clone();
//
//		// Kiểm tra kết quả trả về từ Python
//		if (py::isinstance<py::tuple>(result)) {
//			auto result_tuple = result.cast<py::tuple>();
//			py::list boxes = result_tuple[0].cast<py::list>();
//			py::list scores = result_tuple[1].cast<py::list>();
//
//			numDetected = boxes.size();
//
//			// Duyệt qua các box dự đoán
//			for (size_t i = 0; i < boxes.size(); ++i) {
//				auto box = boxes[i].cast<py::tuple>();
//				int x1 = box[0].cast<int>(), y1 = box[1].cast<int>();
//				int x2 = box[2].cast<int>(), y2 = box[3].cast<int>();
//				float score = scores[i].cast<float>();
//
//				sumOfAll += sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
//
//				// Vẽ hình chữ nhật và hiển thị điểm số
//				cv::Scalar color = (score >= 0.8) ? COLOR_EXCELLENT : (score >= 0.7) ? COLOR_GOOD : COLOR_AVERAGE;
//				cv::rectangle(matResult, { x1, y1 }, { x2, y2 }, color, 2);
//				cv::putText(matResult, std::to_string(score).substr(0, 3), { x1, y1 - 5 }, cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
//			}
//
//			// Kiểm tra nếu không phát hiện vật thể
//			if (numDetected == 0) {
//				/*	pixelCable = sumOfAll / numDetected;
//					int checking = distanceV / pixelCable;
//
//					status = (checking == numDetected) ? "OK"
//						: (checking < numDetected) ? "ERROR"
//						: "DETECTED AGAIN";
//				}
//				else {*/
//				status = "NULL CABLE DETECTED.";
//			}
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
//	if (status != "")
//		return gcnew System::String(status.c_str());
//	int cycle = int(clock() - startTime);
//	return gcnew System::String(numDetected + "," + cycle + "ms");
//	///gil_acquire1.~gil_scoped_acquire_local();
////	gil_acquire.~gil_scoped_acquire_local();
//}
