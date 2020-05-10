#pragma once


class GraphicsPipeline
{
public:

	// 初期化処理
	bool Init();

	// 描画処理
	void Draw();

	//==============================
	//
	// 設定関数
	//
	//==============================
	// モデル描画関数を登録
	void SetModelDrawCall(std::function<void()>& fnc)
	{
		m_modelDrawCalls.push_back(fnc);
	}
	// GUI描画関数を登録
	void SetGUIDrawCall(std::function<void()>& fnc)
	{
		m_guiDrawCall = fnc;
	}
	// デバッグ描画関数を登録
	void SetDebugDrawCall(std::function<void()>& fnc)
	{
		m_debugDrawCall = fnc;
	}

	//==============================
	//
	// 設定関数
	//
	//==============================
	// GUIコマンドリスト取得
	//RgCommandList& GetGuiCommandList() { return m_guiCommandList; }


private:

	//===================
	// モデル描画関係
	//===================
	// 描画関数をためておく配列
	std::vector<std::function<void()>> m_modelDrawCalls;

	//===================
	// GUI描画関係
	//===================
	// GUI描画関数
	std::function<void()> m_guiDrawCall;

	//===================
	// デバッグ描画関係
	//===================
	// デバッグ描画関数
	std::function<void()> m_debugDrawCall;
	// デバッグ描画用
	//RgCommandList m_debugCommandList;

	//===================
	// コマンドリスト
	//===================
	// モデル描画用
	std::vector<std::shared_ptr<RgCommandList>> m_ModelDrawCommandList;
	// GUI用
	std::shared_ptr<RgCommandList> m_guiCommandList = nullptr;
	// デバッグ描画用
	std::shared_ptr<RgCommandList> m_debugCommandList = nullptr;

	//===================
	// マルチスレッド関係
	//===================
	// マルチスレッド有効・無効フラグ
	bool m_UseMultiThreadFlag = true;
	// スレッド数
	UINT m_threadNum = 1;

	//===================
	// Render Targets
	//===================

	//===================
	// カメラ関係
	//===================
	// カメラ
	struct cbCamera
	{
		RgMatrix mView;
		RgMatrix mProj;
		RgVec3 camPos;
		float tmp = 0;
	};
	RgStaticConstantBuffer<cbCamera> m_cb10_Camera;

public:
	RgMatrix m_mView;
	RgMatrix m_mProj;

	// カメラの更新
	void UpdateCamera()
	{
		m_cb10_Camera.GetWork().mView = m_mView;
		m_cb10_Camera.GetWork().mProj = m_mProj;
		m_cb10_Camera.WriteData();
	}
	// カメラを設定するコマンドを作成
	void CreateSetCameraCommand(const UINT num)
	{
		m_cb10_Camera.CreateCommandSetCBV(num);
	}

	//====================
	// シングルトンパターン
	//====================
private:
	//
	GraphicsPipeline(){}

public:

	static GraphicsPipeline& GetInstance() {
		static GraphicsPipeline ins;
		return ins;
	}
};
#define GPL GraphicsPipeline::GetInstance()