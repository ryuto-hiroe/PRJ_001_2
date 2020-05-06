
#include "RgLibrary.h"
#include "RgGui.h"

bool RgGui::Init(void* hwnd)
{
	// ImGui用DescriptorHeap Handleを取得
	RgCpuAndGpuHandle handle = RGHEAPMGR.GetHeapHandle();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	// コンフィグ設定
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Gui style 
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// docking
	//ImGui::DockSpace();
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer bindings 
	if (ImGui_ImplWin32_Init(hwnd) == false) {
		MessageBox(nullptr, "ImGui初期化失敗", "エラー", MB_OK);
		return false;
	}

	//
	auto ptr = RGHEAPMGR.GetDescriptorHeap();
	if (ImGui_ImplDX12_Init(
		RGD3D.GetDevice().Get(),
		RGD3D.FrameBufferCount,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		ptr.Get(),
		handle.cpuhandle,
		handle.gpuhandle) == false)
	{
		MessageBox(nullptr, "ImGui初期化失敗", "エラー", MB_OK);
		return false;
	}

	return true;
}

void RgGui::Release()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}