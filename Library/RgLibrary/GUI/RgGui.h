#pragma once

// Imgui
#include <imgui.h>
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include "ImGuizmo.h"

//
#include "GUI/RgGuiWindow_Impl.h"


//
class RgGui
{
public:
	bool Init(void* hwnd);	// ‰Šú‰»
	void Release();			// I—¹

};