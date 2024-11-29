#include "G.h"
#include <Windows.h>
#include <dshow.h>
#pragma comment(lib, "strmiids")
#include <map>
#include <unordered_map>
#include <string>

namespace BeeCplus {
	struct Device {



		int id;
		std::string devicePath;
		std::string deviceName;
	};

	class DeviceEnumerator {

	public:

		DeviceEnumerator() = default;

		//std::map<int, Device> getDevicesMap(const GUID deviceClass);
		//std::map<int, Device> getVideoDevicesMap();
		//std::map<int, Device> getAudioDevicesMap();


	private:

		//std::string ConvertBSTRToMBS(BSTR bstr);
		//std::string ConvertWCSToMBS(const wchar_t* pstr, long wslen);

	};

	public ref class CCD
	{
	List<System::String^>^ listNameCCD;
	public:float cycle = 0;
	public:int numERR = 0;
	public:bool IsErrCCD = false;
	public:int  colCCD = 1000, rowCCD = 200;
	public:int colCrop, rowCrop;

	public:void ReadRaw(bool IsHist);
	//public:List<System::String^>^ In4Basler();
	//public:System::String^ IniBasler();
	public:List<System::String^>^ ScanBasler();
	public:System::String^ ConnectBasler(System::String^ device);
	public:System::String^ DisconnectBasler();
	//public:void GrabBasler();
	public:void CalHist();
	//public:System::String^ SetPara(System::String^ name, float value);
	//public:System::String^ GetPara(System::String^ name);
	};
}
