
#include "main.h"
#include "System/GameManager.h"
#include "ModelComponent.h"

void ModelComponent::Draw()
{
	if (m_enable == false)return;
	if (m_model == nullptr)return;

	std::function<void()> 
		drawCall = [this]()
	{
		SHMGR.m_ModelSh.SetWorld(m_pOwner.lock()->GetMatrix());
		// 描画
		SHMGR.m_ModelSh.DrawModel(*m_model);
	};
	// 描画関数を登録
	GPL.SetModelDrawCall(drawCall);
}

void ModelComponent::ImGuiUpdate()
{
	BaseComponent::ImGuiUpdate();

	// モデルファイル
	if (ImGuiResourceButton("Model File", m_filename, { ".vrm", ".glb" })) {
		SetModel(m_filename);
	}
}


void ModelComponent::SetModel(const std::string& file)
{
	m_filename = file;

	// 新しいモデルを読み込む
	m_model = RGRESFACT.GetModel(m_filename);
	//m_model = RGRESFACT.GetModel("data/Models/vrm/Miraikomachi.vrm");
}