

#include "main.h"

#include "Level.h"


void Level::Load()
{
	m_root = std::make_shared<Object>();
	m_root->SetName("root");
}

void Level::Save()
{

}

void Level::Update()
{
	m_root->Update();
}

void Level::Draw()
{
	// ���f���`��̊֐����쐬
	m_root->Draw();
}

void Level::ImGuiUpdate()
{
	// �E�B���h�E�쐬
	ImGui::Begin("Level Window", nullptr, ImGuiWindowFlags_MenuBar);
	{
		//=======================
		// ���j���[�o�[
		//=======================
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		//=======================
		// �I�u�W�F�N�g�ǉ�
		//=======================
		if (ImGui::Button("AddObject"))
		{
			// �I�𒆂̃I�u�W�F�N�g������Ƃ�
			if (GAMEMGR.m_Editor_Select_Object.expired() == false)
			{
				auto p = std::make_shared<Object>();
				// �I�𒆂̃I�u�W�F�N�g�̂��Ƃ��Ƃ��Ēǉ�
				p->SetParent(GAMEMGR.m_Editor_Select_Object.lock());
				// �V�����쐬�����I�u�W�F�N�g��I�𒆂ɂ���
				GAMEMGR.m_Editor_Select_Object = p;
			}
		}

		ImGui::Separator();

		//==================================
		// �I�u�W�F�N�g�̃c���[�m�[�h
		//==================================
		// �ċA�I�ɌJ��Ԃ�
		std::function<void(const std::shared_ptr<Object>&)> rec =
			[this, &rec](const std::shared_ptr<Object>& obj)
		{
			// �A�h���X�̎���ID�Ƃ��ăZ�b�g����
			ImGui::PushID(obj.get());

			// �c���[�̓���t���O
			int treeFlags =
				ImGuiTreeNodeFlags_DefaultOpen |
				ImGuiTreeNodeFlags_OpenOnDoubleClick |
				ImGuiTreeNodeFlags_OpenOnArrow;

			// �I������Ă���Ƃ��́A�I����Ԃ̃t���O��t����
			if (GAMEMGR.m_Editor_Select_Object.lock() == obj) {
				treeFlags |= ImGuiTreeNodeFlags_Selected;
			}
			// �c���[�m�[�h
			bool bTreeOpen = ImGui::TreeNodeEx(obj.get(), treeFlags, obj->IsName().c_str());

			// �c���[�m�[�h���N���b�N���ꂽ�Ƃ�
			if (ImGui::IsItemClicked())
			{
				GAMEMGR.m_Editor_Select_Object = obj;
			}

			// �c���[�m�[�h���J����Ă���
			if (bTreeOpen)
			{
				//=====================
				// �q���̏������s��
				//=====================
				// �q���ċA
				for (auto&& child : obj->GetChildren()) {
					rec(child);
				}

				// Tree Node�̏I���
				ImGui::TreePop();
			}
			// ����ID������
			ImGui::PopID();
		};

		//==========================
		// root�I�u�W�F�N�g����ċA�I�ɌJ��Ԃ��Ă���
		//==========================
		rec(m_root);
	}
	ImGui::End();
}