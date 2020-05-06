

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
	if (m_Gui.Init(m_Window.GetHWnd()) == false)
	{
		MessageBox(nullptr, "GUI初期化失敗", "エラー", MB_OK);
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
	m_Gui.Release();

	// Direct3Dの解放
	RGD3D.Release();
	// ウィンドウ削除
	m_Window.Release();
}