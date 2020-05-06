#pragma once



//=====================================================
//
// ゲームウィンドウクラス
//
//  [主な機能]
//　・ウィンドウの作成
//
//=====================================================
class GameWindow {
public:

	// ウィンドウハンドル取得
	HWND GetHWnd() const { return m_hWnd; }
	// ウィンドウが存在する？
	bool IsCreated() const { return m_hWnd ? true : false; }
	// マウスホイールの変化量を取得
	int GetMouseWheelVal() const { return m_MouseWheelVal; }

	// ウィンドウ作成
	bool Create(HINSTANCE hInst, int clientWidth, int clientHeight, const std::string& windowClassName);

	// 解放
	void Release();

	// ウィンドウメッセージを処理する
	//  戻り値：終了メッセージが来たらfalseが返る
	bool ProcessMessage();

	// クライアントサイズの設定
	void SetClientSize(int w, int h);

	//
	~GameWindow() {
		Release();
	}

private:

	// ウィンドウハンドル
	HWND	m_hWnd = nullptr;

	// ウィンドウ関数
	static LRESULT CALLBACK callWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// マウスホイール値
	int		m_MouseWheelVal = 0;

};

