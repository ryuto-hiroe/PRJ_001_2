
#include "main.h"

#include "GameManager.h"


void Object::Awake()
{

}

void Object::Start()
{

}

void Object::Update()
{
	// �R���|�[�l���g�̍X�V����
	for (auto&& comp : m_components)
	{
		comp->Update();
	}

	// �q���̍X�V����
	for (auto&& child : m_children)
	{
		child->Update();
	}
}

void Object::Draw()
{
	// �R���|�[�l���g�̕`�揈��
	for (auto&& comp : m_components)
	{
		comp->Draw();
	}

	// �q���̕`�揈��
	for (auto&& child : m_children)
	{
		child->Draw();
	}
}

void Object::ImGuiUpdate()
{
	// ���O
	ImGuiInputString("Name", m_name);

	ImGui::Separator();

	//=======================
	// �R���|�[�l���g�ǉ�
	//=======================
	ImGui::Button("Add Component");
	if (ImGui::BeginPopupContextItem("AddComp ID", 0))
	{
		for (auto&& node : GAMEMGR.m_componentClassMaker)
		{
			if (ImGui::Selectable(node.first.c_str()))
			{
				auto comp = node.second();
				AddComponent(comp);
			}
		}
		ImGui::EndPopup();
	}

	ImGui::Separator();

	// �ʒu
	// ImGuiGizumo
	RgMatrix m = m_mWorld;
	ImGuizmoEditTransform(m, GPL.m_mView, GPL.m_mProj, nullptr);
	SetMatrix(m, true);

	ImGui::Separator();
	ImGui::Separator();


	// �e�R���|�[�l���g��GUI����
	for (auto&& comp : m_components)
	{
		// �A�h���X�̎���ID�Ƃ��ăZ�b�g����
		ImGui::PushID(comp.get());

		bool bOpen = ImGui::CollapsingHeader(comp->ClassName().c_str()
			, ImGuiTreeNodeFlags_DefaultOpen);

		if (bOpen) {
			comp->ImGuiUpdate();
		}

		ImGui::PopID();
	}

}

// �w�肵���q���폜(�����ɏ������߁A�Ăяo�����͒���)
void Object::RemoveChild(const std::shared_ptr<Object>& obj)
{
	// ����
	auto it = std::find(m_children.begin(), m_children.end(), obj);
	// ����
	if (it != m_children.end()) {
		// �폜
		m_children.erase(it);
	}
}

// �e��ݒ�
void Object::SetParent(const std::shared_ptr<Object>& parent)
{
	// �e����E�ނ���O�ɁA������shared_ptr���L��
	std::shared_ptr<Object> spThis = shared_from_this();

	// ���݂ɐeGameObject����E�ނ���
	auto nowParent = GetParent();
	if (nowParent) {
		nowParent->RemoveChild(spThis);
	}

	// �e�|�C���^��ύX
	m_parent = parent;

	// �V�����e�̎q�Ƃ��Ď�����o�^
	if (parent) {
		parent->m_children.push_back(spThis);
	}
}

void Object::SetMatrix(const RgMatrix& m, bool moveChild)
{
	// �ω��O�̍s����o�b�N�A�b�v
	m_mPrevWorld = m_mWorld;

	// �s����Z�b�g
	m_mWorld = m;

	// �ω��O�ƍ���̍s�񂩂�A�P�t���[���ł̕ω��ʎZ�o
	RgMatrix mInvPrev;
	m_mPrevWorld.Inverse(mInvPrev);
	RgMatrix mDelta = mInvPrev * m_mWorld;

	// �q���A��������
	if (moveChild) {
		// �q�ȉ��̂��ׂĂ�GameObject��Transform��mDelta���������Ă����֐�
		std::function<void(const std::shared_ptr<Object>&)>
			proc = [this, &proc, &mDelta](const std::shared_ptr<Object>& obj)
		{
			// �ω��O�̍s����o�b�N�A�b�v
			obj->m_mPrevWorld *= mDelta;
			// �K�p
			obj->m_mWorld *= mDelta;

			// �q�ċA
			for (auto&& child : obj->GetChildren()) {
				proc(child);
			}
		};

		// �q����������
		for (auto&& child : GetChildren()) {
			proc(child);
		}
	}
}

void Object::AddComponent(const std::shared_ptr<BaseComponent>& comp)
{
	comp->SetOwner(shared_from_this());
	comp->Start();
	m_components.push_back(comp);
}


bool GameManager::Init()
{
	// �N���X�쐬�֐��o�^
	REGISTER_COMP_CLASS(ModelComponent);

	//====================================
	// (��)���f���f�[�^�ǂݍ���
	//====================================
	//m_model.LoadModel("data/Models/vrm/Miraikomachi.vrm");
	//m_model2.LoadModel("data/Models/test/alicia-solid.vrm");

	//m_model	= RGRESFACT.GetModel("data/Models/chara/2b/2b.glb");
	//m_model2 = RGRESFACT.GetModel("data/Models/chara/mirai/Miraikomachi.vrm");

	// �J�������̍쐬
	RgMatrix mV, mP;
	RgVec3 camPos = { 0, 1, -2.5 };
	mV.Move(camPos);
	mV.Inverse();
	mP.CreatePerspectiveFovLH(60, 1.77777779f, 0.01f, 1000);

	SHMGR.m_cb10_Camera.GetWork().mView = mV;
	SHMGR.m_cb10_Camera.GetWork().mProj = mP;
	SHMGR.m_cb10_Camera.GetWork().camPos = camPos;

	//
	m_lineModel1.Create({ 2, 1, -2 }, { 2, 1, 2 });
	m_lineModel2.Create({ -2, 1, -2 }, { -2, 1, 2 });

	m_EditorStage.Create();



	// (��)
	m_level = std::make_shared<Level>();
	m_level->Load();


	return true;
}

void GameManager::Release()
{
	
}

void GameManager::Draw()
{

}

void GameManager::Run()
{
	RGD3D.BeginFrame();

	//====================
	// �X�V
	//====================
	m_level->Update();

	// �G�f�B�^�J�����̍X�V
	m_EditorCamera.Update();

	//====================
	// ImGui����
	//====================
	ImGuiUpdate();

	//====================
	// �`�揈��
	//====================
	GPL.UpdateCamera();

	//====================
	// �`��
	//====================
	// �`��֐������߂�
	m_level->Draw();
	
	// �f�o�b�O�`��
	std::function<void()> debuhDraw = [this]()
	{
		m_EditorStage.Draw();
	};
	GPL.SetDebugDrawCall(debuhDraw);

	// �o�^�����`��֐��̎��s
	GPL.Draw();

	RGD3D.m_swapchain->Present(1, 0);

	RGD3D.WaitPreviousFrame();
}


void GameManager::ImGuiUpdate()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	// ImGuizmo
	ImGuizmo::BeginFrame();

	// demo
	ImGui::ShowDemoWindow(nullptr);

	// level  Window
	m_level->ImGuiUpdate();

	// Inspector
	ImGui::Begin("Inspector");
	{
		if (m_Editor_Select_Object.expired() == false)
		{
			m_Editor_Select_Object.lock()->ImGuiUpdate();
		}
	}
	ImGui::End();
}