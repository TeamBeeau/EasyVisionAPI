#include "G.h"
#include <Windows.h>
#include <dshow.h>
#pragma comment(lib, "strmiids")
#include <map>
#include <unordered_map>
#include <string>
using namespace System;
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
	public:System::String^ GetPara(System::String^ name);
	public:System::String^ SetPara(System::String^ name, float value);
	public:cli::array<Byte>^ GetRaw();
	public:cli::array<Byte>^ GetResult();
	public:System::String^ GrabBasler();
	public: bool IsScan ;
	List<System::String^>^ listNameCCD;
	public:float cycle = 0;
	public:int numERR = 0;
	public:bool IsErrCCD = false;
	public:int  cols = 1000, rows = 100,typ=0, exposures = 600;
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
