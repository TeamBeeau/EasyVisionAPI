#pragma once
#include "G.h"

namespace BeeCplus {
	
	public ref class Yolo
	{

	public:	System::String^ ImportRaw();
	public:System::String^ FinalizeGIL();
	public:System::String^ IniGIL();
	public:System::String^ IniStart();
	
	//public:System::String^ TestYolo(float Score);
	public:System::String^ CheckYolo(float Score);

	};
	
	
	
}

