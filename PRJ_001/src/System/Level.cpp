

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
	// モデル描画の関数を作成
	m_root->Draw();
}

void Level::ImGuiUpdate()
{
	// ウィンドウ作成
	ImGui::Begin("Level Window", nullptr, ImGuiWindowFlags_MenuBar);
	{
		//=======================
		// メニューバー
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
		// オブジェクト追加
		//=======================
		if (ImGui::Button("AddObject"))
		{
			// 選択中のオブジェクトがあるとき
			if (GAMEMGR.m_Editor_Select_Object.expired() == false)
			{
				auto p = std::make_shared<Object>();
				// 選択中のオブジェクトのこともとして追加
				p->SetParent(GAMEMGR.m_Editor_Select_Object.lock());
				// 新しく作成したオブジェクトを選択中にする
				GAMEMGR.m_Editor_Select_Object = p;
			}
		}

		ImGui::Separator();

		//==================================
		// オブジェクトのツリーノード
		//==================================
		// 再帰的に繰り返す
		std::function<void(const std::shared_ptr<Object>&)> rec =
			[this, &rec](const std::shared_ptr<Object>& obj)
		{
			// アドレスの識別IDとしてセットする
			ImGui::PushID(obj.get());

			// ツリーの動作フラグ
			int treeFlags =
				ImGuiTreeNodeFlags_DefaultOpen |
				ImGuiTreeNodeFlags_OpenOnDoubleClick |
				ImGuiTreeNodeFlags_OpenOnArrow;

			// 選択されているときは、選択状態のフラグを付ける
			if (GAMEMGR.m_Editor_Select_Object.lock() == obj) {
				treeFlags |= ImGuiTreeNodeFlags_Selected;
			}
			// ツリーノード
			bool bTreeOpen = ImGui::TreeNodeEx(obj.get(), treeFlags, obj->IsName().c_str());

			// ツリーノードがクリックされたとき
			if (ImGui::IsItemClicked())
			{
				GAMEMGR.m_Editor_Select_Object = obj;
			}

			// ツリーノードが開かれている
			if (bTreeOpen)
			{
				//=====================
				// 子供の処理を行う
				//=====================
				// 子を再帰
				for (auto&& child : obj->GetChildren()) {
					rec(child);
				}

				// Tree Nodeの終わり
				ImGui::TreePop();
			}
			// 識別IDを解除
			ImGui::PopID();
		};

		//==========================
		// rootオブジェクトから再帰的に繰り返していく
		//==========================
		rec(m_root);
	}
	ImGui::End();
}