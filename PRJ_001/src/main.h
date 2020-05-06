#pragma once

//===============================================
// ���C�u����
//===============================================
// Rg Library
#include "RgLibrary.h"
//
// ImGui
#include <imgui.h>
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"

#include "ImGuizmo.h"


//===============================================
// ���̑�
//===============================================
#include "Shader/ShaderManager.h"
#include "Shader/GraphicsPipeline.h"
#include "System/GameWindow.h"
#include "System/ImGuiHelper.h"
#include "System/EditorCamera.h"
#include "System/EditorStage.h"
#include "System/GameManager.h"

//===============================================
// �萔
//===============================================
// �E�B���h�E�T�C�Y
#define WIN_W 1280
#define WIN_H 780
#define DH_SRV_NUM 1



class Applicatoin
{
public:

	// �A�v���P�[�V����������
	bool Init(HINSTANCE hInst, int w, int h);

	// �Q�[�����[�v
	void Loop();

	// �A�v���P�[�V�������
	void Release();



	//=================================================
	// ���J�f�[�^
	//=================================================

	// �Q�[���E�B���h�E�N���X
	GameWindow	m_Window;


	//=====================================================
	// �O���[�o���f�[�^
	//=====================================================

	HINSTANCE	m_hInst = nullptr;		// �C���X�^���X�n���h��

	int			m_Fps = 0;					// ���݂�FPS�l
	int			m_maxFps = 60;				// �ő�FPS

	int			m_winH = WIN_H;
	int			m_winW = WIN_W;


private:

	// �Q�[���I���t���O true�ŏI������
	bool	m_EndFlag = false;



//=====================================================
// �V���O���g���p�^�[��
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
// �ȒP�ɃA�N�Z�X�ł���悤�ɂ��邽�߂̃}�N��
#define APP Applicatoin::GetInstance()