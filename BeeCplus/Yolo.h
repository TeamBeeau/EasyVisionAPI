#pragma once
#include "G.h"
 #define PYBIND11_NO_ASSERT_GIL_HELD_INCREF_DECREF 
namespace BeeCplus {
	
	public ref class Yolo
	{

	public:	System::String^ ImportRaw();
	public:System::String^ FinalizeGIL();
	public:System::String^ IniGIL();
	public:System::String^ IniStart();
	public:	System::String^ Reset();
	//public:System::String^ TestYolo(float Score);
	public:System::String^ CheckYolo(float Score);

	};
	
	
	
}

