#include "../main.h"
#include "GameWindow.h"

// imgui
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


bool GameWindow::Create(HINSTANCE hInst, int clientWidth, int clientHeight, const std::string& windowClassName)
{
	//===================================================================
	// メインウィンドウ作成
	//===================================================================

	//ウィンドウクラスの定義
	WNDCLASSEX wc;										// ウィンドウクラスの定義用
	wc.cbSize = sizeof(WNDCLASSEX);							// 構造体のサイズ
	wc.style = 0;											// スタイル
	wc.lpfnWndProc = &GameWindow::callWindowProc;			// ウインドウ関数
	wc.cbClsExtra = 0;										// エキストラクラス情報 
	wc.cbWndExtra = 0;										// エキストラウィンドウ情報
	wc.hInstance = hInst;									// インスタンスハンドル
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);			// ラージアイコン
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);		// スモールアイコン 
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);			// マウスカーソル
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// 背景色 
	wc.lpszMenuName = nullptr;								// メインメニュー名
	wc.lpszClassName = windowClassName.c_str();				// ウィンドウクラス名

	//ウィンドウクラスの登録
	if (!RegisterClassEx(&wc)) {
		return false;
	}

	//ウィンドウの作成
	m_hWnd = CreateWindow(
		windowClassName.c_str(),							// ウィンドウクラス名
		"3Dゲームプログラミング",							// ウィンドウのタイトル
		WS_OVERLAPPEDWINDOW - WS_THICKFRAME,				// ウィンドウタイプを標準タイプに	
		0,													// ウィンドウの位置（Ｘ座標）
		0,													// ウィンドウの位置（Ｙ座標）						
		clientWidth,										// ウィンドウの幅
		clientHeight,										// ウィンドウの高さ			
		nullptr,											// 親ウィンドウのハンドル
		nullptr,											// メニューのハンドル
		hInst,												// インスタンスハンドル 
		this);												// 追加情報 
	if (m_hWnd == nullptr) {
		return false;
	}

	// ウィンドウプロパティにこのクラスのインスタンスアドレスを記憶しておく
//	SetProp(m_hWnd, "GameWindowInstance", this);

	// クライアントのサイズを設定
	SetClientSize(clientWidth, clientHeight);

	//===================================================================
	//ウィンドウの表示
	//===================================================================
	ShowWindow(m_hWnd, SW_SHOW);
	//ウィンドウの更新
	UpdateWindow(m_hWnd);

	// timeGetTime関数の精度を1msに設定
	timeBeginPeriod(1);

	return true;
}

void GameWindow::Release()
{
	if (m_hWnd) {
		DestroyWindow(m_hWnd);
		m_hWnd = nullptr;
	}
}

bool GameWindow::ProcessMessage()
{
	m_MouseWheelVal = 0;

	// メッセージ取得
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// 終了メッセージがきた
		if (msg.message == WM_QUIT) {
			return false;
		}

		//メッセージ処理
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

// ウィンドウ関数(Static関数)
LRESULT CALLBACK GameWindow::callWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// ウィンドウプロパティから、GameWindowクラスのインスタンスを取得
	GameWindow* pThis = (GameWindow*)GetProp(hWnd, "GameWindowInstance");

	// nullptrの場合は、デフォルト処理を実行
	if (pThis == nullptr) {
		switch (message) {
			case WM_CREATE:
			{
				// CreateWindow()で渡したパラメータを取得
				CREATESTRUCT * createStruct = (CREATESTRUCT*)lParam;
				GameWindow* window = (GameWindow*)createStruct->lpCreateParams;

				// ウィンドウプロパティにこのクラスのインスタンスアドレスを埋め込んでおく
				// 次回から、pThis->WindowProcの方へ処理が流れていく
				SetProp(hWnd, "GameWindowInstance", window);

			}
			return 0;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	// インスタンス側のWindow関数を実行する
	return pThis->WindowProc(hWnd, message, wParam, lParam);
}

// ウィンドウ関数
LRESULT GameWindow::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// ImGuiにイベント通知
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	//===================================
	//メッセージによって処理を選択
	//===================================
	switch (message) {
	// ホイールスクロール時
	case WM_MOUSEWHEEL:
		{
			m_MouseWheelVal = (short)HIWORD(wParam);
		}
		break;
	// Xボタンが押された
	case WM_CLOSE:
		// 破棄
		Release();
		break;
	// ウィンドウ破棄直前
	case WM_DESTROY:
		RemoveProp(hWnd, "GameWindowInstance");
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			Release();
			break;
		}
		break;

	default:
		// メッセージのデフォルト処理
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ウィンドウのクライアントのサイズを指定サイズにする。
void GameWindow::SetClientSize(int w, int h)
{
	RECT rcWnd, rcCli;

	GetWindowRect(m_hWnd, &rcWnd); // ウィンドウのRECT取得
	GetClientRect(m_hWnd, &rcCli); // クライアント領域のRECT取得

	// ウィンドウの余白を考えて、クライアントのサイズを指定サイズにする。
	MoveWindow(m_hWnd,
		rcWnd.left,// X座標
		rcWnd.top,// Y座標
		w + (rcWnd.right - rcWnd.left) - (rcCli.right - rcCli.left),
		h + (rcWnd.bottom - rcWnd.top) - (rcCli.bottom - rcCli.top),
		TRUE);
}
