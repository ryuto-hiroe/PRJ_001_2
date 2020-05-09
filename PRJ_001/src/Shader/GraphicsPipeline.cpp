
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
		comList.Create();
	}

	// GUI�p�R�}���h���X�g�̍쐬
	m_guiCommandList.Create();

	// �f�o�b�O�p�R�}���h���X�g�̍쐬
	m_debugCommandList.Create();

	// �`��R�}���h���Z�b�g
	m_modelDrawCalls.clear();

	//===============================
	// RT�쐬
	//===============================
	//if (m_rts.CreateRT())

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
	m_ModelDrawCommandList[0].Begin();
	{
		// �����_�[�^�[�Q�b�g��ݒ肷��
		RGD3D.SetBackBuffer(m_ModelDrawCommandList[0]);
		RGD3D.ClearBackBuffer(m_ModelDrawCommandList[0]);

		// SRV�ACBV�p�̃f�B�X�N���v�^�q�[�v��ݒ�
		RGHEAPMGR.MakeCommand(m_ModelDrawCommandList[0].GetCommandList());

		// ���f���V�F�[�_��ݒ�
		SHMGR.m_ModelSh.SetUp(m_ModelDrawCommandList[0].GetCommandList());

		// �J������ݒ�
		CreateSetCameraCommand(2, m_ModelDrawCommandList[0].GetCommandList());

		// �쐬�����`��֐������s����
		for (auto&& call : m_modelDrawCalls)
		{
			call(m_ModelDrawCommandList[0]);
		}

		// �����_�[�^�[�Q�b�g��\���\�ɂ���
		RGD3D.ResetBackBuffer(m_ModelDrawCommandList[0]);
	}
	m_ModelDrawCommandList[0].End();

	//===============
	// �f�o�b�O�`��
	//===============
	m_debugCommandList.Begin();
	{
		// �����_�[�^�[�Q�b�g��ݒ肷��
		RGD3D.SetBackBuffer(m_debugCommandList);

		// SRV�ACBV�p�̃f�B�X�N���v�^�q�[�v��ݒ�
		RGHEAPMGR.MakeCommand(m_debugCommandList.GetCommandList());

		// �V�F�[�_��ݒ�
		SHMGR.m_PrimSh.SetUp(m_debugCommandList.GetCommandList());

		// �J������ݒ�
		CreateSetCameraCommand(1, m_debugCommandList.GetCommandList());

		// �`��֐����s
		m_debugDrawCall(m_debugCommandList);

		// �����_�[�^�[�Q�b�g��\���\�ɂ���
		RGD3D.ResetBackBuffer(m_debugCommandList);
	}
	m_debugCommandList.End();

	//===============
	// GUI�`��J�n
	//===============
	m_guiCommandList.Begin();
	{
		// �����_�[�^�[�Q�b�g��ݒ肷��
		RGD3D.SetBackBuffer(m_guiCommandList);
	
		// SRV�ACBV�p�̃f�B�X�N���v�^�q�[�v��ݒ�
		RGHEAPMGR.MakeCommand(m_guiCommandList.GetCommandList());
	
		// GUI�`��֐�
		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(
			ImGui::GetDrawData(),
			m_guiCommandList.GetCommandList().Get()
		);
	
		// �����_�[�^�[�Q�b�g��\���\�ɂ���
		RGD3D.ResetBackBuffer(m_guiCommandList);
	}
	m_guiCommandList.End();

	//===============
	// �R�}���h�̎��s
	//===============
	std::vector<ID3D12CommandList*> comLists;
	// ���f���`��R�}���h���X�g��o�^
	comLists.push_back(m_ModelDrawCommandList[0].GetCommandList().Get());
	// �f�o�b�O�`��R�}���h���X�g��o�^
	comLists.push_back(m_debugCommandList.GetCommandList().Get());
	// GUI�`��R�}���h���X�g��o�^
	comLists.push_back(m_guiCommandList.GetCommandList().Get());
	// �R�}���h�̎��s
	RGD3D.GetCommandQueue()->ExecuteCommandLists(comLists.size(), &comLists[0]);

	//===============
	// �`��֐����N���A
	//===============
	m_modelDrawCalls.clear();

	// Update and Render additional Platform Windows
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault(NULL, (void*)GPL.GetGuiCommandList().GetCommandList().Get());
	}
}