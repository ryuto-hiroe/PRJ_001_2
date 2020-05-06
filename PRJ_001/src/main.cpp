

#include "main.h"


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// ���������[�N��m�点��
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// COM������
	auto hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	// mbstowcs_s�֐��œ��{��Ή��ɂ��邽�߂ɌĂ�
	setlocale(LC_ALL, "japanese");

	//===================================================================
	// �E�B���h�E�쐬�ADirect3D�������Ȃ�
	//===================================================================
	if (APP.Init(hInstance, WIN_W, WIN_H) == false) {
		APP.Release();
		return 0;
	}

	//===================================================================
	// ���[�v
	//===================================================================
	APP.Loop();

	//===================================================================
	// ���
	//===================================================================
	APP.Release();

	// COM���
	CoUninitialize();

	return 0;
}

// �A�v���P�[�V����������
bool Applicatoin::Init(HINSTANCE hInst, int w, int h)
{
	// �C���X�^���X�n���h�����L��
	APP.m_hInst = hInst;
	// �E�B���h�E�T�C�Y���L��
	m_winH = h;
	m_winW = w;

	//===================================================================
	// �E�B���h�E�쐬
	//===================================================================
	if (m_Window.Create(hInst, w, h, "Window") == false) {
		MessageBox(nullptr, "�E�B���h�E�쐬�Ɏ��s", "�G���[", MB_OK);
		return false;
	}

	//===================================================================
	// �t���X�N���[���m�F
	//===================================================================
	bool bFullScreen = false;
	if (MessageBox(m_Window.GetHWnd(), "�t���X�N���[���ɂ��܂����H", "�m�F", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES) {
		bFullScreen = true;
	}

	//===================================================================
	// Direct3D12 ������
	//===================================================================
	if (RGD3D.Init(m_Window.GetHWnd(), m_winW, m_winH) == false) {
		MessageBox(nullptr, "Direct3D�������Ɏ��s", "�G���[", MB_OK);
		return false;
	}

	// �R�}���h���X�g�̍쐬
	if (COMLIST.Init() == false) {
		MessageBox(nullptr, "CommandList�쐬���s", "�G���[", MB_OK);
		return false;
	}

	// SRV�ACBV�AUAV��DescriptorHeap�̍쐬
	if (RGHEAPMGR.CreateDescriptorHeap() == false) {
		MessageBox(nullptr, "DescriptorHeap�쐬���s", "�G���[", MB_OK);
		return false;
	}

	// �R���X�^���g�o�b�t�@�̍쐬
	if (UCB.CreateConstantBuffer() == false) {
		MessageBox(nullptr, "ConstantBuffer�쐬���s", "�G���[", MB_OK);
		return false;
	}

	// �V�F�[�_�}�l�[�W���̏�����
	if (SHMGR.Init() == false) {
		MessageBox(nullptr, "Shader�̏��������s", "�G���[", MB_OK);
		return false;
	}

	// �O���t�B�b�N�p�C�v���C���̍쐬
	if (GPL.Init() == false) {
		MessageBox(nullptr, "GraphicPipeline�쐬���s", "�G���[", MB_OK);
		return false;
	}

	//===================================================================
	// ImGui������
	//===================================================================
	if (m_Gui.Init(m_Window.GetHWnd()) == false)
	{
		MessageBox(nullptr, "GUI���������s", "�G���[", MB_OK);
		return false;
	}

	return true;
}

void Applicatoin::Loop()
{
	DWORD baseTime = timeGetTime();
	int count = 0;

	// �Q�[���}�l�[�W���̏�����
	GAMEMGR.Init();

	auto file = GetFileName_Dialog(
		FD_FILTER_IMAGE,
		"D:\\myProgram\\projects\\PRJ_001_2\\PRJ_001\\data\\Models\\chara");

	// �Q�[�����[�v
	while (1)
	{
		// �����J�n����Get
		DWORD st = timeGetTime();

		// �Q�[���I���w�肪����Ƃ�
		if (m_EndFlag) {
			break;
		}

		//=========================================
		//
		// �E�B���h�E�֌W�̏���
		//
		//=========================================

		// �E�B���h�E�̃��b�Z�[�W����������
		m_Window.ProcessMessage();

		// �E�B���h�E���j������Ă�Ȃ�A���[�v�I��
		if (m_Window.IsCreated() == false)break;


		//=========================================
		//
		// �Q�[������
		//
		//=========================================
		GAMEMGR.Run();

		//=========================================
		// 60 Fps����
		//=========================================
		// �����I������Get
		DWORD et = timeGetTime();
		// Fps����
		DWORD ms = 1000 / m_maxFps;
		if (et - st < ms) {
			Sleep(ms - (et - st));	// ����������҂�
		}

		// FPS�v��
		count++;
		if (st - baseTime >= 1000) {
			m_Fps = (count * 1000) / (st - baseTime);
			baseTime = st;
			count = 0;
		}
	}
}

void Applicatoin::Release()
{
	RGD3D.WaitPreviousFrame();

	GAMEMGR.Release();

	// ImGui
	m_Gui.Release();

	// Direct3D�̉��
	RGD3D.Release();
	// �E�B���h�E�폜
	m_Window.Release();
}