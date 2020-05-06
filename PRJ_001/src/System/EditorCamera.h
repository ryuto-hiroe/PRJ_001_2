#pragma once


class EditorCamera
{
public:
	// 
	EditorCamera() {
		m_mCam.Move(0, 5, -10);
		//m_mCam.RotateX_Local(20);
	}

	// ����
	void Update();

	// �r���[�E�ˉe�s����Z�b�g
	void SetToShader();


	RgVec3		m_vCamBasePos;
	RgMatrix	m_mCam;

private:

	POINT		m_PrevMousePos = { 0, 0 };
};