
#include "main.h"
#include "EditorCamera.h"


void EditorCamera::Update()
{
	//============================
	// �ˉe�s��
	//============================
	GPL.m_mProj.CreatePerspectiveFovLH(60, 1.7777779f, 0.01f, 10000.0f);

	//============================
	// �r���[�s��
	//============================
	POINT nowPos;
	GetCursorPos(&nowPos);

	// Shift�ő傫��
	float ratio = 1.0f;
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
		ratio = 5;
	}
	// Control�ŏ�����
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
		ratio = 0.2f;
	}

	// �}�E�X�̉E�{�^��
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
		// �}�E�X�̑O�񂩂�̈ړ���
		float deltaX = (float)(nowPos.x - m_PrevMousePos.x) * 0.5f * ratio;
		float deltaY = (float)(nowPos.y - m_PrevMousePos.y) * 0.5f * ratio;

		// �J������] Y����]
		RgMatrix mR;
		mR.RotateAxis(RgVec3(0, 1, 0), deltaX);
		m_mCam *= mR;

		// �J������] X����]
		mR.CreateRotateAxis(m_mCam.GetXAxis(), deltaY);
		m_mCam *= mR;
	}

	// �z�C�[���h���b�O
	if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) {
		float deltaX = (float)(nowPos.x - m_PrevMousePos.x) * 0.05f * ratio;
		float deltaY = (float)(nowPos.y - m_PrevMousePos.y) * 0.05f * ratio;

		m_vCamBasePos -= m_mCam.GetXAxis() * deltaX;
		m_vCamBasePos += m_mCam.GetYAxis() * deltaY;
	}

	// �z�C�[��
	if (APP.m_Window.GetMouseWheelVal()) {
		RgMatrix mT;
		RgVec3 mVec = { 0, 0, (float)APP.m_Window.GetMouseWheelVal() * 0.01f * ratio };
		mVec.TransformNormal(m_mCam);
		m_vCamBasePos += mVec;
	}

	m_mCam.SetPos(m_vCamBasePos);
	m_PrevMousePos = nowPos;

	// �J�����s�񂩂�r���[�s��֕ϊ�
	//SHMGR.m_mView = m_mCam;
	//SHMGR.m_mView.Inverse();
	GPL.m_mView = m_mCam;
	GPL.m_mView.Inverse();
}

void EditorCamera::SetToShader()
{
	// �J�����s�񂩂�r���[�s��֕ϊ�
	SHMGR.m_mView = m_mCam;
	SHMGR.m_mView.Inverse();
}
