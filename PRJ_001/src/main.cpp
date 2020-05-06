

#include "main.h"


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// メモリリークを知らせる
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// COM初期化
	auto hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	// mbstowcs_s関数で日本語対応にするために呼ぶ
	setlocale(LC_ALL, "japanese");

	//===================================================================
	// ウィンドウ作成、Direct3D初期化など
	//===================================================================
	if (APP.Init(hInstance, WIN_W, WIN_H) == false) {
		APP.Release();
		return 0;
	}

	//===================================================================
	// ループ
	//===================================================================
	APP.Loop();

	//===================================================================
	// 解放
	//===================================================================
	APP.Release();

	// COM解放
	CoUninitialize();

	return 0;
}

// アプリケーション初期化
bool Applicatoin::Init(HINSTANCE hInst, int w, int h)
{
	// インスタンスハンドルを記憶
	APP.m_hInst = hInst;
	// ウィンドウサイズを記憶
	m_winH = h;
	m_winW = w;

	//===================================================================
	// ウィンドウ作成
	//===================================================================
	if (m_Window.Create(hInst, w, h, "Window") == false) {
		MessageBox(nullptr, "ウィンドウ作成に失敗", "エラー", MB_OK);
		return false;
	}

	//===================================================================
	// フルスクリーン確認
	//===================================================================
	bool bFullScreen = false;
	if (MessageBox(m_Window.GetHWnd(), "フルスクリーンにしますか？", "確認", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES) {
		bFullScreen = true;
	}

	//===================================================================
	// Direct3D12 初期化
	//===================================================================
	if (RGD3D.Init(m_Window.GetHWnd(), m_winW, m_winH) == false) {
		MessageBox(nullptr, "Direct3D初期化に失敗", "エラー", MB_OK);
		return false;
	}

	// コマンドリストの作成
	if (COMLIST.Init() == false) {
		MessageBox(nullptr, "CommandList作成失敗", "エラー", MB_OK);
		return false;
	}

	// SRV、CBV、UAVのDescriptorHeapの作成
	if (RGHEAPMGR.CreateDescriptorHeap() == false) {
		MessageBox(nullptr, "DescriptorHeap作成失敗", "エラー", MB_OK);
		return false;
	}

	// コンスタントバッファの作成
	if (UCB.CreateConstantBuffer() == false) {
		MessageBox(nullptr, "ConstantBuffer作成失敗", "エラー", MB_OK);
		return false;
	}

	// シェーダマネージャの初期化
	if (SHMGR.Init() == false) {
		MessageBox(nullptr, "Shaderの初期化失敗", "エラー", MB_OK);
		return false;
	}

	// グラフィックパイプラインの作成
	if (GPL.Init() == false) {
		MessageBox(nullptr, "GraphicPipeline作成失敗", "エラー", MB_OK);
		return false;
	}

	//===================================================================
	// ImGui初期化
	//===================================================================
	// ImGui用DescriptorHeap Handleを取得
	RgCpuAndGpuHandle handle = RGHEAPMGR.GetHeapHandle();
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	//ImGui::DockSpace();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	
	// Setup Platform/Renderer bindings 
	if (ImGui_ImplWin32_Init(m_Window.GetHWnd()) == false) {
		MessageBox(nullptr, "ImGui初期化失敗", "エラー", MB_OK);
		return false;
	}
	auto ptr = RGHEAPMGR.GetDescriptorHeap();

	if (ImGui_ImplDX12_Init(
		RGD3D.GetDevice().Get(),
		RGD3D.FrameBufferCount,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		ptr.Get(),
		handle.cpuhandle,
		handle.gpuhandle
	) == false)
	{
		MessageBox(nullptr, "ImGui初期化失敗", "エラー", MB_OK);
		return false;
	}

	return true;
}

void Applicatoin::Loop()
{
	DWORD baseTime = timeGetTime();
	int count = 0;

	// ゲームマネージャの初期化
	GAMEMGR.Init();

	auto file = GetFileName_Dialog(
		FD_FILTER_IMAGE,
		"D:\\myProgram\\projects\\PRJ_001_2\\PRJ_001\\data\\Models\\chara");

	// ゲームループ
	while (1)
	{
		// 処理開始時間Get
		DWORD st = timeGetTime();

		// ゲーム終了指定があるとき
		if (m_EndFlag) {
			break;
		}

		//=========================================
		//
		// ウィンドウ関係の処理
		//
		//=========================================

		// ウィンドウのメッセージを処理する
		m_Window.ProcessMessage();

		// ウィンドウが破棄されてるなら、ループ終了
		if (m_Window.IsCreated() == false)break;


		//=========================================
		//
		// ゲーム処理
		//
		//=========================================
		GAMEMGR.Run();

		//=========================================
		// 60 Fps制御
		//=========================================
		// 処理終了時間Get
		DWORD et = timeGetTime();
		// Fps制御
		DWORD ms = 1000 / m_maxFps;
		if (et - st < ms) {
			Sleep(ms - (et - st));	// 速すぎたら待つ
		}

		// FPS計測
		count++;
		if (st - baseTime >= 1000) {
			m_Fps = (count * 1000) / (st - baseTime);
			baseTime = st;
			count = 0;
		}
	}
}

void Applicatoin::Release()
{
	RGD3D.WaitPreviousFrame();

	GAMEMGR.Release();

	// ImGui
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// Direct3Dの解放
	RGD3D.Release();
	// ウィンドウ削除
	m_Window.Release();
}