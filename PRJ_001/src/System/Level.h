#pragma once


class Level
{
public:

	// 
	void Load();

	//
	void Save();

	//
	void Update();

	// �`��
	void Draw();

	// �G�f�B�^����
	void ImGuiUpdate();

	// 
	void Run();
	

	//=============================
	//
	// �擾�֌W
	//
	//=============================
	// root�I�u�W�F�N�g�̎擾
	const std::shared_ptr<Object>& GetRoot() { return m_root; }


private:

	// root�I�u�W�F�N�g
	std::shared_ptr<Object> m_root;

	// (��)
	RgRenderTargets m_renderTarget;

	//
	//DrawThreadManager m_threadMgr;
};