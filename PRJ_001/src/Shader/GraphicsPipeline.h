#pragma once


class GraphicsPipeline
{
public:

	// ����������
	bool Init();

	// �`�揈��
	void Draw();

	//==============================
	//
	// �ݒ�֐�
	//
	//==============================
	// ���f���`��֐���o�^
	void SetModelDrawCall(std::function<void()>& fnc)
	{
		m_modelDrawCalls.push_back(fnc);
	}
	// GUI�`��֐���o�^
	void SetGUIDrawCall(std::function<void()>& fnc)
	{
		m_guiDrawCall = fnc;
	}
	// �f�o�b�O�`��֐���o�^
	void SetDebugDrawCall(std::function<void()>& fnc)
	{
		m_debugDrawCall = fnc;
	}

	//==============================
	//
	// �ݒ�֐�
	//
	//==============================
	// GUI�R�}���h���X�g�擾
	//RgCommandList& GetGuiCommandList() { return m_guiCommandList; }


private:

	//===================
	// ���f���`��֌W
	//===================
	// �`��֐������߂Ă����z��
	std::vector<std::function<void()>> m_modelDrawCalls;

	//===================
	// GUI�`��֌W
	//===================
	// GUI�`��֐�
	std::function<void()> m_guiDrawCall;

	//===================
	// �f�o�b�O�`��֌W
	//===================
	// �f�o�b�O�`��֐�
	std::function<void()> m_debugDrawCall;
	// �f�o�b�O�`��p
	//RgCommandList m_debugCommandList;

	//===================
	// �R�}���h���X�g
	//===================
	// ���f���`��p
	std::vector<std::shared_ptr<RgCommandList>> m_ModelDrawCommandList;
	// GUI�p
	std::shared_ptr<RgCommandList> m_guiCommandList = nullptr;
	// �f�o�b�O�`��p
	std::shared_ptr<RgCommandList> m_debugCommandList = nullptr;

	//===================
	// �}���`�X���b�h�֌W
	//===================
	// �}���`�X���b�h�L���E�����t���O
	bool m_UseMultiThreadFlag = true;
	// �X���b�h��
	UINT m_threadNum = 1;

	//===================
	// Render Targets
	//===================

	//===================
	// �J�����֌W
	//===================
	// �J����
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

	// �J�����̍X�V
	void UpdateCamera()
	{
		m_cb10_Camera.GetWork().mView = m_mView;
		m_cb10_Camera.GetWork().mProj = m_mProj;
		m_cb10_Camera.WriteData();
	}
	// �J������ݒ肷��R�}���h���쐬
	void CreateSetCameraCommand(const UINT num)
	{
		m_cb10_Camera.CreateCommandSetCBV(num);
	}

	//====================
	// �V���O���g���p�^�[��
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