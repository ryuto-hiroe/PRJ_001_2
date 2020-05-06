#pragma once

//===============================================
// ライブラリ
//===============================================
// Rg Library
#include "RgLibrary.h"

//===============================================
// その他
//===============================================
#include "Shader/ShaderManager.h"
#include "Shader/GraphicsPipeline.h"
#include "System/GameWindow.h"
#include "System/ImGuiHelper.h"
#include "System/EditorCamera.h"
#include "System/EditorStage.h"
#include "System/GameManager.h"

//===============================================
// 定数
//===============================================
// ウィンドウサイズ
#define WIN_W 1280
#define WIN_H 780
#define DH_SRV_NUM 1

//
#define FD_FILTER_IMAGE "Image file\0*.png;*.jpg\0\0"
#define FD_FILTER_MODEL "Model file\0*.glb\0\0"



class Applicatoin
{
public:

	// アプリケーション初期化
	bool Init(HINSTANCE hInst, int w, int h);

	// ゲームループ
	void Loop();

	// アプリケーション解放
	void Release();



	//=================================================
	// 公開データ
	//=================================================

	// ゲームウィンドウクラス
	GameWindow	m_Window;

	// Gui
	RgGui		m_Gui;

	//=====================================================
	// グローバルデータ
	//=====================================================

	HINSTANCE	m_hInst = nullptr;		// インスタンスハンドル

	int			m_Fps = 0;					// 現在のFPS値
	int			m_maxFps = 60;				// 最大FPS

	int			m_winH = WIN_H;
	int			m_winW = WIN_W;


private:

	// ゲーム終了フラグ trueで終了する
	bool	m_EndFlag = false;



//=====================================================
// シングルトンパターン
//=====================================================
private:
	// 
	Applicatoin() {

	}

public:
	static Applicatoin& GetInstance() {
		static Applicatoin Instance;
		return Instance;
	}
};
// 簡単にアクセスできるようにするためのマクロ
#define APP Applicatoin::GetInstance()