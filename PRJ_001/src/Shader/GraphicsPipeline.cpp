
#include "main.h"
#include "GraphicsPipeline.h"

bool GraphicsPipeline::Init()
{
	// CPU�� �擾
	SYSTEM_INFO sys;
	GetSystemInfo(&sys);
	int cpuCnt = (int)sys.dwNumberOfProcessors;

	// ���f���`��̃R�}���h���X�g���쐬
	m_threadNum = cpuCnt;
	m_ModelDrawCommandList.resize(m_threadNum);
	for (auto&& comList : m_ModelDrawCommandList)
	{
		comList = std::make_shared<RgCommandList>();
		comList->Create();
	}

	// GUI�p�R�}���h���X�g�̍쐬
	m_guiCommandList = std::make_shared<RgCommandList>();
	m_guiCommandList->Create();

	// �f�o�b�O�p�R�}���h���X�g�̍쐬
	m_debugCommandList = std::make_shared<RgCommandList>();
	m_debugCommandList->Create();

	// �`��R�}���h���Z�b�g
	m_modelDrawCalls.clear();

	//===============================
	// �萔�o�b�t�@�̏�����
	//===============================
	if (m_cb10_Camera.Create() == false) {
		return false;
	}

	return true;
}

void GraphicsPipeline::Draw()
{
	//===============
	// ���f���`��J�n
	//===============
	m_ModelDrawCommandList[0]->Begin();
	{
		// �����_�[�^�[�Q�b�g��ݒ肷��
		RGD3D.SetBackBuffer();
		RGD3D.ClearBackBuffer();

		// SRV�ACBV�p�̃f�B�X�N���v�^�q�[�v��ݒ�
		RGHEAPMGR.MakeCommand();

		// ���f���V�F�[�_��ݒ�
		SHMGR.m_ModelSh.SetUp();

		// �J������ݒ�
		CreateSetCameraCommand(2);

		// �쐬�����`��֐������s����
		for (auto&& call : m_modelDrawCalls)
		{
			call();
		}

		// �����_�[�^�[�Q�b�g��\���\�ɂ���
		RGD3D.ResetBackBuffer();
	}
	m_ModelDrawCommandList[0]->End();

	//===============
	// �f�o�b�O�`��
	//===============
	m_debugCommandList->Begin();
	{
		// �����_�[�^�[�Q�b�g��ݒ肷��
		RGD3D.SetBackBuffer();

		// SRV�ACBV�p�̃f�B�X�N���v�^�q�[�v��ݒ�
		RGHEAPMGR.MakeCommand();

		// �V�F�[�_��ݒ�
		SHMGR.m_PrimSh.SetUp();

		// �J������ݒ�
		CreateSetCameraCommand(1);

		// �`��֐����s
		m_debugDrawCall();

		// �����_�[�^�[�Q�b�g��\���\�ɂ���
		RGD3D.ResetBackBuffer();
	}
	m_debugCommandList->End();

	//===============
	// GUI�`��J�n
	//===============
	m_guiCommandList->Begin();
	{
		// �����_�[�^�[�Q�b�g��ݒ肷��
		RGD3D.SetBackBuffer();
	
		// SRV�ACBV�p�̃f�B�X�N���v�^�q�[�v��ݒ�
		RGHEAPMGR.MakeCommand();
	
		// GUI�`��֐�
		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(
			ImGui::GetDrawData(),
			m_guiCommandList->GetCommandList().Get()
		);
	
		// �����_�[�^�[�Q�b�g��\���\�ɂ���
		RGD3D.ResetBackBuffer();
	}
	m_guiCommandList->End();

	//===============
	// �R�}���h�̎��s
	//===============
	std::vector<ID3D12CommandList*> comLists;
	// ���f���`��R�}���h���X�g��o�^
	comLists.push_back(m_ModelDrawCommandList[0]->GetCommandList().Get());
	// �f�o�b�O�`��R�}���h���X�g��o�^
	comLists.push_back(m_debugCommandList->GetCommandList().Get());
	// GUI�`��R�}���h���X�g��o�^
	comLists.push_back(m_guiCommandList->GetCommandList().Get());
	// �R�}���h�̎��s
	RGD3D.GetCommandQueue()->ExecuteCommandLists((UINT)comLists.size(), &comLists[0]);

	//===============
	// �`��֐����N���A
	//===============
	m_modelDrawCalls.clear();

	// Update and Render additional Platform Windows
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault(NULL, (void*)m_guiCommandList->GetCommandList().Get());
	}
}