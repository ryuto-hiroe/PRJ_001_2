#pragma once

// �e�V�F�[�_�N���X�̃C���N���[�h
#include "ModelShader/ModelShader.h"


class ShaderManager
{
public:

	//==========================
	// �V�F�[�_�N���X
	//==========================
	// ���C�u�����V�F�[�_
	RgPrimitiveShader m_PrimSh;
	// �I���W�i���V�F�[�_
	ModelShader m_ModelSh;	// ���f���V�F�[�_


	//==========================
	// �J����
	//==========================
	RgMatrix m_mView;
	RgMatrix m_mProj;


	//==========================
	// �萔�o�b�t�@
	//==========================
	// �J����
	struct cbCamera
	{
		RgMatrix mView;
		RgMatrix mProj;
		RgVec3 camPos;
		float tmp;
	};
	RgStaticConstantBuffer<cbCamera> m_cb10_Camera;

	// �J�����̍X�V
	void UpdateCamera()
	{
		m_cb10_Camera.GetWork().mView = m_mView;
		m_cb10_Camera.GetWork().mProj = m_mProj;
		m_cb10_Camera.WriteData();
		//m_cb10_Camera.CreateCommandSetCBV(RGD3D.m_commandList, 2);
		//m_cb10_Camera.CreateCommandSetCBV(
		//	RGD3D.m_setUpCommandList.GetCommandList(), 2);
	}
	// �J������ݒ肷��R�}���h���쐬
	void CreateSetCameraCommand(
		const UINT num)
	{
		m_cb10_Camera.CreateCommandSetCBV(num);
	}


	bool Init();

	void Release();


private:



	//===================================
	// �V���O���g���p�^�[��
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