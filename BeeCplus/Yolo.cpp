#include "Yolo.h"
#define PYBIND11_NO_ASSERT_GIL_HELD_INCREF_DECREF
using namespace BeeCplus;

	
struct gil_scoped_acquire_local {
	gil_scoped_acquire_local() : state(PyGILState_Ensure()) {}
	gil_scoped_acquire_local(const gil_scoped_acquire_local&) = delete;
	gil_scoped_acquire_local& operator=(const gil_scoped_acquire_local&) = delete;
	~gil_scoped_acquire_local() { PyGILState_Release(state); }
	const PyGILState_STATE state;
};

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
	matRaw= imread("C:\\test.png");
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
std::tuple<py::list, py::list> GIL(float Score)
{
	//std::lock_guard<std::mutex>lock(gilmutex);
	IsCheking = true;
	std::string status = "";
	try
	{
		if (!Py_Initialize || !_yolo)
		{
			IsCheking = false;
			return std::tuple<py::list, py::list>();
		}
		
	

		
	//	py::gil_scoped_release release;
	//	py::gil_scoped_acquire acquire;
	

	// Chuyển đổi OpenCV image sang numpy array để gửi tới Python
	py::array_t<uint8_t> image_array = mat_to_numpy1(matProcess);
	// Gọi hàm YOLO trong Python để dự đoán
	
	
	auto result = _yolo.attr("predict")(image_array, Score);



	// Kiểm tra kết quả trả về từ Python
	if (py::isinstance<py::tuple>(result)) {
		std::tuple<py::list, py::list> lisRS;
		auto result_tuple = result.cast<py::tuple>();
		py::list boxes = result_tuple[0].cast<py::list>();
		py::list scores = result_tuple[1].cast<py::list>();
		lisRS = std::make_tuple(boxes, scores); IsCheking = false;
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

	return std::tuple<py::list, py::list>();
}
System::String^ Yolo::CheckYolo(float Score) {
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

	// Lấy GIL để chạy mã Python
	py::gil_scoped_acquire acquire;
//std::unique_lock<std::mutex> lock(gilmutex);
//	std::lock_guard<std::mutex>lock(gilmutex);
	std::tuple<py::list, py::list> result=GIL(Score);
	
//	std::cout << "Unlocked by thread: " << std::this_thread::get_id() << std::endl;
	
	//lock.unlock();
	matResult = matProcess.clone();
	//
	
	int numDetected = 0;
	float pixelCable = 0, sumOfAll = 0;
	int distanceV = 0, cycleTime = 0;
	std::ostringstream resultStream;
	std::string status="";
	
	// Kiểm tra xem hình ảnh có trống không

	// Nếu ảnh là ảnh xám, chuyển sang ảnh màu BGR
	/*if (matRaw.type() == CV_8UC1) {
		cv::cvtColor(matRaw, img, CV_GRAY2BGR);
	}
	else {
		img = matRaw.clone();
	}*/

	// Bảo vệ tài nguyên bằng mutex để đảm bảo thread safety
	//std::lock_guard<std::mutex> lock(gilmutex);

	try {
		//matResult = img.clone();
		///*py::gil_scoped_release release;
		//pybind11::gil_scoped_acquire acquire;*/
		////py::gil_scoped_acquire acquire;
		////std::lock_guard<std::mutex> lock(gilmutex);  // Bảo vệ GIL
		//if (PyGILState_Check() == 0)
		//{
		//	//py::gil_scoped_release release;
		//	return "Fail lib";
		//	//py::gil_scoped_acquire acquire;
		//}

		//else
		//{
		//	py::gil_scoped_release release;
		//	py::gil_scoped_acquire acquire;
		//}
		//// Chuyển đổi OpenCV image sang numpy array để gửi tới Python
		//py::array_t<uint8_t> image_array = mat_to_numpy1(img);

		//// Gọi hàm YOLO trong Python để dự đoán
		//auto result = _yolo.attr("predict")(image_array, Score);

		//py::gil_scoped_release release;

		//// Kiểm tra kết quả trả về từ Python
		//if (py::isinstance<py::tuple>(result)) {
		//	auto result_tuple = result.cast<py::tuple>();
		//	py::list boxes = result_tuple[0].cast<py::list>();
		//	py::list scores = result_tuple[1].cast<py::list>();
		py::list Boxes = std::get<0>(result);
		py::list Scores = std::get<1>(result);
			numDetected = Boxes.size();

			// Duyệt qua các box dự đoán
			for (size_t i = 0; i < Boxes.size(); ++i) {
				auto box = Boxes[i].cast<py::tuple>();
				int x1 = box[0].cast<int>(), y1 = box[1].cast<int>();
				int x2 = box[2].cast<int>(), y2 = box[3].cast<int>();
				float score = Scores[i].cast<float>();

				sumOfAll += sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

				// Vẽ hình chữ nhật và hiển thị điểm số
				cv::Scalar color = (score >= 0.8) ? COLOR_EXCELLENT : (score >= 0.7) ? COLOR_GOOD : COLOR_AVERAGE;
				cv::rectangle(matResult, { x1, y1 }, { x2, y2 }, color, 2);
				cv::putText(matResult, std::to_string(score).substr(0, 3), { x1, y1 - 5 }, cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
			}
//			py::gil_scoped_release release;

			// Kiểm tra nếu không phát hiện vật thể
			if (numDetected == 0) {
			/*	pixelCable = sumOfAll / numDetected;
				int checking = distanceV / pixelCable;

				status = (checking == numDetected) ? "OK"
					: (checking < numDetected) ? "ERROR"
					: "DETECTED AGAIN";
			}
			else {*/
			//	status = "NULL CABLE DETECTED.";
			}
		
	}
	
	catch (...) {
		status = "UNKNOWN EXCEPTION.";
	}
	if(status!="")
		return gcnew System::String(status.c_str());
	int cycle = int(clock() - startTime);
	return gcnew System::String("0," +numDetected + "," + cycle + ",0");
	
	///gil_acquire1.~gil_scoped_acquire_local();
//	gil_acquire.~gil_scoped_acquire_local();
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
