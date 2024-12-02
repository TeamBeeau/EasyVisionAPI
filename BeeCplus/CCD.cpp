#include "CCD.h"
#include <future>

bool IsShow = false;

using namespace BeeCplus;
//using namespace System::Runtime::InteropServices;

public enum class PreferencesEnum
{
	Unknown = 0,
	Exposure = 1,
	Cycle = 2,
	MinExposure = 3,
	MaxExposure = 4
};
public enum Parameter {
	EXPOSURE,
	GAIN,
	OFFSETX,
	OFFSETY,
	WIDTH,
	HEIGHT,
	GAMMA,
	CONTRAST,
	TIMEDELAY,
	BRIGHTNESS,
	INVALID_PARAM
};
Parameter getParamEnum(const string& name) {
	static unordered_map<string, Parameter> paramMap = {
		{"Exposure", EXPOSURE},
		{"Gain", GAIN},
		{"OffsetX", OFFSETX},
		{"OffsetY", OFFSETY},
		{"Width", WIDTH},
		{"Height", HEIGHT},
		{"Gamma", GAMMA},
		{"Contrast", CONTRAST},
		{"TimeDelay", TIMEDELAY},
		{"Brightness", BRIGHTNESS}
	};
	auto it = paramMap.find(name);
	if (it != paramMap.end()) {
		return it->second;
	}
	else {
		return INVALID_PARAM;
	}
}

void ReadImage()
{
	ucRaw = MatToBytes(matRaw);
}

void SetImage(int indexTool, uchar* uc, int image_rows, int image_cols, int image_type)
{
	m_matDst[indexTool] = Mat();
	m_matDst[indexTool] = BytesToMat(uc, image_rows, image_cols, image_type).clone();
	if (m_matDst[indexTool].type() == CV_8UC3)
		cvtColor(m_matDst[indexTool], m_matDst[indexTool], COLOR_BGR2GRAY);
}
cli::array<Byte>^ CCD::GetRaw()
{
	

	// Create a byte array in managed memory
	cli::array<Byte>^ byteArray = gcnew cli::array<Byte>(matRaw.total() * matRaw.elemSize());
	cols = matRaw.cols;
	rows = matRaw.rows;
	typ = matRaw.type();
	// Pin the array to copy data
	pin_ptr<Byte> pinnedArray = &byteArray[0];
	memcpy(pinnedArray, matRaw.data, matRaw.total() * matRaw.elemSize());

	return byteArray;
}
cli::array<Byte>^ CCD::GetResult()
{


	// Create a byte array in managed memory
	cli::array<Byte>^ byteArray = gcnew cli::array<Byte>(matResult.total() * matResult.elemSize());
	cols = matResult.cols;
	rows = matResult.rows;
	typ = matResult.type();
	// Pin the array to copy data
	pin_ptr<Byte> pinnedArray = &byteArray[0];
	memcpy(pinnedArray, matResult.data, matResult.total() * matResult.elemSize());

	return byteArray;
}
extern "C" __declspec(dllexport) uchar* GetImage(int* rows, int* cols, int* Type)
{
	int rows_ = matRaw.rows;
	int cols_ = matRaw.cols;
	int Type_ = matRaw.type();
	*rows = rows_;
	*cols = cols_;
	*Type = Type_;

	return  MatToBytes(matRaw);
}

extern "C" __declspec(dllexport) uchar* GetProcImage(int* rows, int* cols, int* Type)
{
	int rows_ = matProc.rows;
	int cols_ = matProc.cols;
	int Type_ = matProc.type();
	*rows = rows_;
	*cols = cols_;
	*Type = Type_;

	return  MatToBytes(matProc);
}

extern "C" __declspec(dllexport) uchar* GetResultImage(int* rows, int* cols, int* Type)
{
	int rows_ = matResult.rows;
	int cols_ = matResult.cols;
	int Type_ = matResult.type();
	*rows = rows_;
	*cols = cols_;
	*Type = Type_;

	return  MatToBytes(matResult);
}

extern "C" __declspec(dllexport) uchar* GetImageCrop(int* rows, int* cols, int* Type)
{

	int rows_ = matCrop.rows;
	int cols_ = matCrop.cols;
	int Type_ = matCrop.type();
	*rows = rows_;
	*cols = cols_;
	*Type = Type_;
	return  MatToBytes(matCrop);
}

extern "C" __declspec(dllexport) void SetDst(int indexTool, uchar* uc, int image_rows, int image_cols, int image_type)
{

	SetImage(indexTool, uc, image_rows, image_cols, image_type);

}

extern "C" __declspec(dllexport) void SetSrc(uchar* uc, int image_rows, int image_cols, int image_type)
{
	matRaw = BytesToMat(uc, image_rows, image_cols, image_type);

}

extern "C" __declspec(dllexport) const char* IniBasler()
{
	//std::lock_guard<std::mutex>lock(gilmutex);
	try
	{
		InitializeYolo();
		LoadModel();
		//PylonInitialize();
		return SUCCESS;
	}
	catch (const GenericException& e)
	{
		std::string err = ("ERROR :");
		err.append(e.GetDescription());
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
DeviceInfoList_t devices;

List<System::String^>^ CCD::ScanBasler() {
	IsScan = false;
	PylonInitialize();
	listNameCCD = gcnew List<System::String^>();
	//std::lock_guard<std::mutex> lock(gilmutex);
	static std::string response;

	try {

		CTlFactory& factory = CTlFactory::GetInstance();
		size_t deviceCount = factory.EnumerateDevices(devices);

		if (deviceCount == 0) {
			return  gcnew List<System::String^>();
		}
		else {

			for (size_t i = 0; i < devices.size(); ++i) {
				const CDeviceInfo& device = devices[i];
				if (device.IsUserDefinedNameAvailable()) {
					listNameCCD->Add(gcnew System::String(device.GetUserDefinedName().c_str()));
				}
				else {
					listNameCCD->Add(gcnew System::String(device.GetModelName().c_str()));
				}

			}
		}
	}
	catch (const GenericException& e) {
		return  gcnew List<System::String^>();
	}
	catch (const std::exception& e) {
		return  gcnew List<System::String^>();
	}
	IsScan = true;
	return listNameCCD; // Trả về con trỏ tới chuỗi C-style
}
System::String^ CCD:: ConnectBasler(System::String^ device) {
	if (device=="")return "";
	if(!IsScan)return "";
	try
	{
		int index = listNameCCD->IndexOf(device);

		if (index == -1)return "";
		CTlFactory& tlFactory = CTlFactory::GetInstance();
		if (devices[index].GetFullName() != "")// đã nhận dc tên ccd
		{
			try
			{
				camGigE.Attach(tlFactory.CreateDevice(devices[index]));//kt quyền điều khiển
				camGigE.Open();
				GenApi::CBooleanPtr ptrAutoPacketSize = camGigE.GetStreamGrabberNodeMap().GetNode("AutoPacketSize");
				if (GenApi::IsWritable(ptrAutoPacketSize))
				{
					ptrAutoPacketSize->SetValue(true);
				}
				camGigE.Width.SetValue(cols);
				camGigE.Height.SetValue(rows);
				int with = (int)camGigE.Width.GetMax();
				int height = (int)camGigE.Height.GetMax();
				camGigE.CenterX = true;
				camGigE.CenterX = true;


				camGigE.StartGrabbing();//Tao luong Doc anh
				fc.OutputPixelFormat = PixelType_Mono8;
				camGigE.RetrieveResult(-1, ptrGrabResult, TimeoutHandling_ThrowException);//Lay Data Camera SAU KHOẢNG THỜI GIAN SẼ THOÁT RA ,(NẾU GIÁ TRỊ BẰNG -1 KHÔNG THOÁT RA)
				if (ptrGrabResult->GrabSucceeded())
				{
					fc.Convert(image, ptrGrabResult);///Chuyen gia tri ma camera qua anh thu viện Pylon Balser
					matRaw = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC1, (uint8_t*)image.GetBuffer(), Mat::AUTO_STEP);///convert anh thu vien pylon thanh Mat			


					ptrGrabResult.Release();//Xoa data
					camGigE.StopGrabbing();
					return SUCCESS;
				}
				else
				{
					camGigE.StopGrabbing();
					camGigE.Close();
					camGigE.DetachDevice();
					return "";
				}
			}
			catch (GenICam::GenericException& e)
			{
				camGigE.StopGrabbing();
				camGigE.Close();
				camGigE.DetachDevice();
				PylonTerminate();
				return gcnew System::String(e.what());
			}
		}
	}
	catch (GenICam::GenericException& e)
	{
		
		camGigE.StopGrabbing();
		camGigE.Close();
		camGigE.DetachDevice();
		PylonTerminate();
		return gcnew System::String( e.what());
	}
}


Mat equalizeBGRA(const Mat& img)
{
	Mat res(img.size(), img.type());
	Mat imgB(img.size(), CV_8UC1);
	Mat imgG(img.size(), CV_8UC1);
	Mat imgR(img.size(), CV_8UC1);
	Vec3b pixel;



	for (int r = 0; r < img.rows; r++)
	{
		for (int c = 0; c < img.cols; c++)
		{
			pixel = img.at<Vec3b>(r, c);
			imgB.at<uchar>(r, c) = pixel[0];
			imgG.at<uchar>(r, c) = pixel[1];
			imgR.at<uchar>(r, c) = pixel[2];
		}
	}

	equalizeHist(imgB, imgB);
	equalizeHist(imgG, imgG);
	equalizeHist(imgR, imgR);

	for (int r = 0; r < img.rows; r++)
	{
		for (int c = 0; c < img.cols; c++)
		{
			pixel = Vec3b(imgB.at<uchar>(r, c), imgG.at<uchar>(r, c), imgR.at<uchar>(r, c));
			res.at<Vec3b>(r, c) = pixel;
		}
	}

	return res;
}bool IsCap = false;
System::String^ CCD::GrabBasler() {
	if (IsCap)
		return "";
	//std::lock_guard<std::mutex>lock(gilmutex);
	IsCap = true;
	//std::unique_lock<std::mutex> lock(gilmutex);
	matRaw.release();
	matProcess.release();
	matResult.release();
	camGigE.StartGrabbing();
	camGigE.RetrieveResult(-1, ptrGrabResult, TimeoutHandling_ThrowException);//Lay Data Camera SAU KHOẢNG THỜI GIAN SẼ THOÁT RA ,(NẾU GIÁ TRỊ BẰNG -1 KHÔNG THOÁT RA)
	if (ptrGrabResult->GrabSucceeded())
	{
		fc.Convert(image, ptrGrabResult);
		matRaw = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC1, (uint8_t*)image.GetBuffer(), Mat::AUTO_STEP);
		if (matRaw.type() == CV_8UC1) {
			cv::cvtColor(matRaw, matProcess, CV_GRAY2BGR);
		}
		else {
			matProcess = matRaw.clone();
		}
	}

	ptrGrabResult.Release();
	camGigE.StopGrabbing();
	//lock.unlock();
	IsCap = false;

	return SUCCESS;
}

void CCD::ReadRaw(bool IsHist)
{
	double d1 = clock();
	Mat raw = Mat();
	camUSB.read(raw);

	camUSB >> raw;
	if (IsHist)
		matRaw = equalizeBGRA(raw);
	else
		matRaw = raw;
	if (matRaw.empty() || matRaw.cols == 0 || matRaw.rows == 0)
		numERR++;

	if (numERR > 5)
	{
		numERR = 0;
		IsErrCCD = true;
	}
	cycle = int(clock() - d1);
}

void CCD::CalHist()
{
	Mat src = matRaw.clone();
	vector<Mat> bgr_planes;
	split(src, bgr_planes);
	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange[] = { range };
	bool uniform = true, accumulate = false;
	Mat b_hist, g_hist, r_hist;
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate);
	int hist_w = 512, hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			cv::Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			cv::Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			cv::Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}
	imshow("Source image", equalizeBGRA(matRaw.clone()));
	imshow("calcHist Demo", histImage);

}

System::String^ CCD:: DisconnectBasler()
{
	try
	{
		if (camGigE.IsOpen())
		{
			if (camGigE.IsGrabbing())
			{
				camGigE.StopGrabbing();
			}
			if (camGigE.IsOpen())
			{
				camGigE.Close();
			}
			if (camGigE.IsPylonDeviceAttached())
			{
				camGigE.DetachDevice();
			}
			PylonTerminate();

			cv::destroyAllWindows();
		}
		return SUCCESS;
	}
	catch (const GenericException& e)
	{
		std::string err = "ERROR :";
		err.append(e.GetDescription());
		return gcnew System::String( err.c_str());
	}
	catch (const std::exception& e)
	{
		std::string err = "ERROR :";
		err.append(e.what());
		return gcnew System::String(err.c_str());
	}
	catch (...)
	{
		return "UNKNOWN ERROR.";
	}
}

System::String^  CCD::SetPara(System::String^ name, float value)
{
	static std::string result;
	try
	{
		int maxVal, minVal;
		float minG, maxG;
		Parameter param = getParamEnum(_toString(name));
		switch (param)
		{
		case EXPOSURE:
			minVal = (int)camGigE.ExposureTimeRaw.GetMin();
			maxVal = (int)camGigE.ExposureTimeRaw.GetMax();
			if (value < minVal) value = minVal;
			if (value > maxVal) value = maxVal;
			camGigE.ExposureTimeRaw.SetValue(static_cast<int>(value));
			result = SUCCESS;
			break;
		case GAIN:
			minVal = (int)camGigE.GainRaw.GetMin();
			maxVal = (int)camGigE.GainRaw.GetMax();
			if (value < minVal) value = minVal;
			if (value > maxVal) value = maxVal;
			camGigE.GainRaw.SetValue(static_cast<int>(value));
			result = SUCCESS;
			break;
		case OFFSETX:
			minVal = 0;
			maxVal = (int)camGigE.Width.GetMax() - (int)camGigE.Width.GetInc();
			if (value < minVal) value = 0;
			if (value > maxVal) value = maxVal;
			camGigE.OffsetX.SetValue(static_cast<int>(value));
			result = SUCCESS;
			break;
		case OFFSETY:
			minVal = 0;
			maxVal = (int)camGigE.Height.GetMax() - (int)camGigE.Height.GetInc();
			if (value < minVal) value = 0;
			if (value > maxVal) value = maxVal;
			camGigE.OffsetY.SetValue(static_cast<int>(value));
			result = SUCCESS;
			break;
		case WIDTH:
			minVal = (int)camGigE.Width.GetMin();
			maxVal = (int)camGigE.Width.GetMax();
			if (value < minVal) value = minVal;
			if (value > maxVal) value = maxVal;
			camGigE.Width.SetValue(static_cast<int>(value));
			result = SUCCESS;
			break;
		case HEIGHT:
			minVal = (int)camGigE.Height.GetMin();
			maxVal = (int)camGigE.Height.GetMax();
			if (value < minVal) value = minVal;
			if (value > maxVal) value = maxVal;
			camGigE.Height.SetValue(static_cast<int>(value));
			result = SUCCESS;
			break;
		case GAMMA:
			minG = camGigE.Gamma.GetMin();
			maxG = camGigE.Gamma.GetMax();
			if (value < minG) value = minG;
			if (value > maxG) value = maxG;
			camGigE.Gamma.SetValue(value);
			result = SUCCESS;
			break;
		case CONTRAST:
			result = "NON SUPPORT.";
			break;
		case TIMEDELAY:
			minVal = camGigE.TimerDelayRaw.GetMin();
			maxVal = camGigE.TimerDelayRaw.GetMax();
			if (value < minVal) value = minVal;
			if (value > maxVal) value = maxVal;
			camGigE.TimerDelayRaw.SetValue(static_cast<int>(value));
			result = SUCCESS;
			break;
		case BRIGHTNESS:
			result = "NON SUPPORT.";
			break;
		default:
			result = "INVALID PARAMETER NAME.";
			break;
		}
		return  gcnew System::String(result.c_str());
	}
	catch (const GenericException& e)
	{
		std::string err = "ERROR :";
		err.append(e.GetDescription());
		return gcnew System::String(err.c_str());
	}
	catch (const std::exception& e)
	{
		std::string err = "ERROR :";
		err.append(e.what());
		return gcnew System::String(err.c_str());
	}
	catch (...)
	{
		return "UNKNOWN ERROR.";
	}
}

System::String^ CCD::GetPara(System::String^ name)
{
	static std::string result;
	try
	{
		Parameter param = getParamEnum(_toString( name));
		switch (param)
		{
		case EXPOSURE:
			result = std::to_string(camGigE.ExposureTimeRaw.GetValue());
			break;
		case GAIN:
			result = std::to_string(camGigE.GainRaw.GetValue());
			break;
		case OFFSETX:
			result = std::to_string(camGigE.OffsetX.GetValue());
			break;
		case OFFSETY:
			result = std::to_string(camGigE.OffsetY.GetValue());
			break;
		case WIDTH:
			result = std::to_string(camGigE.Width.GetValue());
			break;
		case HEIGHT:
			result = std::to_string(camGigE.Height.GetValue());
			break;
		case GAMMA:
			result = std::to_string(camGigE.Gamma.GetValue());
			break;
		case CONTRAST:
			result = "NON SUPPORT.";
			break;
		case TIMEDELAY:
			result = std::to_string(camGigE.TimerDelayRaw.GetValue());
			break;
		case BRIGHTNESS:
			result = "NON SUPPORT.";
			break;
		default:
			result = "INVALID PARAMETER NAME.";
			break;
		}
		return gcnew System::String(result.c_str());
	}
	catch (const GenericException& e)
	{
		std::string err = "ERROR :";
		err.append(e.GetDescription());
		return gcnew System::String(err.c_str());
	}
	catch (const std::exception& e)
	{
		std::string err = "ERROR :";
		err.append(e.what());
		return gcnew System::String(err.c_str());
	}
	catch (...)
	{
		return gcnew System::String("UNKNOWN ERROR.");
	}
}

extern "C" __declspec(dllexport) const char* In4Basler()
{
	static std::string result;
	try
	{
		if (camGigE.IsOpen())
		{
			std::stringstream ss;
			ss << "Camera Information:" << std::endl;
			ss << "Model: " << nameBasler.GetModelName() << std::endl;
			ss << "Serial Number: " << nameBasler.GetSerialNumber() << std::endl;
			ss << "Device Class: " << nameBasler.GetDeviceClass() << std::endl;
			ss << "Friendly Name: " << nameBasler.GetFriendlyName() << std::endl;
			ss << "Full Name: " << nameBasler.GetFullName() << std::endl;
			ss << "Device Factory: " << nameBasler.GetDeviceFactory() << std::endl;
			ss << "Device Version: " << nameBasler.GetDeviceVersion() << std::endl;
			ss << "User Defined Name: " << nameBasler.GetUserDefinedName() << std::endl;
			ss << "Vendor Name: " << nameBasler.GetVendorName() << std::endl;

			result = ss.str(); // Lưu chuỗi vào biến tĩnh
		}
		else
		{
			result = "NO CONNECTED.";
		}
		return result.c_str();
	}
	catch (const GenericException& e)
	{
		std::string err = "ERROR :";
		err.append(e.GetDescription());
		return err.c_str();
	}
	catch (const std::exception& e)
	{
		std::string err = "ERROR :";
		err.append(e.what());
		return err.c_str();
	}
	catch (...)
	{
		return "UNKNOWN ERROR.";
	}

	
}
