#pragma once


class EditorCamera
{
public:
	// 
	EditorCamera() {
		m_mCam.Move(0, 5, -10);
		//m_mCam.RotateX_Local(20);
	}

	// 処理
	void Update();

	// ビュー・射影行列をセット
	void SetToShader();


	RgVec3		m_vCamBasePos;
	RgMatrix	m_mCam;

private:

	POINT		m_PrevMousePos = { 0, 0 };
};