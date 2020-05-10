#pragma once

// 各シェーダクラスのインクルード
#include "ModelShader/ModelShader.h"


class ShaderManager
{
public:

	//==========================
	// シェーダクラス
	//==========================
	// ライブラリシェーダ
	RgPrimitiveShader m_PrimSh;
	// オリジナルシェーダ
	ModelShader m_ModelSh;	// モデルシェーダ


	//==========================
	// カメラ
	//==========================
	RgMatrix m_mView;
	RgMatrix m_mProj;


	//==========================
	// 定数バッファ
	//==========================
	// カメラ
	struct cbCamera
	{
		RgMatrix mView;
		RgMatrix mProj;
		RgVec3 camPos;
		float tmp;
	};
	RgStaticConstantBuffer<cbCamera> m_cb10_Camera;

	// カメラの更新
	void UpdateCamera()
	{
		m_cb10_Camera.GetWork().mView = m_mView;
		m_cb10_Camera.GetWork().mProj = m_mProj;
		m_cb10_Camera.WriteData();
		//m_cb10_Camera.CreateCommandSetCBV(RGD3D.m_commandList, 2);
		//m_cb10_Camera.CreateCommandSetCBV(
		//	RGD3D.m_setUpCommandList.GetCommandList(), 2);
	}
	// カメラを設定するコマンドを作成
	void CreateSetCameraCommand(
		const UINT num)
	{
		m_cb10_Camera.CreateCommandSetCBV(num);
	}


	bool Init();

	void Release();


private:



	//===================================
	// シングルトンパターン
	//===================================
private:

	//
	ShaderManager(){}


public:
	static ShaderManager& GetInstance() {
		static ShaderManager instance;
		return instance;
	}
};
//
#define SHMGR ShaderManager::GetInstance()