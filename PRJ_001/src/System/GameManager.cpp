
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
	// コンポーネントの更新処理
	for (auto&& comp : m_components)
	{
		comp->Update();
	}

	// 子供の更新処理
	for (auto&& child : m_children)
	{
		child->Update();
	}
}

void Object::Draw()
{
	// コンポーネントの描画処理
	for (auto&& comp : m_components)
	{
		comp->Draw();
	}

	// 子供の描画処理
	for (auto&& child : m_children)
	{
		child->Draw();
	}
}

void Object::ImGuiUpdate()
{
	// 名前
	ImGuiInputString("Name", m_name);

	ImGui::Separator();

	//=======================
	// コンポーネント追加
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

	// 位置
	// ImGuiGizumo
	RgMatrix m = m_mWorld;
	ImGuizmoEditTransform(m, GPL.m_mView, GPL.m_mProj, nullptr);
	SetMatrix(m, true);

	ImGui::Separator();
	ImGui::Separator();


	// 各コンポーネントのGUI処理
	for (auto&& comp : m_components)
	{
		// アドレスの識別IDとしてセットする
		ImGui::PushID(comp.get());

		bool bOpen = ImGui::CollapsingHeader(comp->ClassName().c_str()
			, ImGuiTreeNodeFlags_DefaultOpen);

		if (bOpen) {
			comp->ImGuiUpdate();
		}

		ImGui::PopID();
	}

}

// 指定した子を削除(即座に消すため、呼び出し時は注意)
void Object::RemoveChild(const std::shared_ptr<Object>& obj)
{
	// 検索
	auto it = std::find(m_children.begin(), m_children.end(), obj);
	// 発見
	if (it != m_children.end()) {
		// 削除
		m_children.erase(it);
	}
}

// 親を設定
void Object::SetParent(const std::shared_ptr<Object>& parent)
{
	// 親から脱退する前に、自分のshared_ptrを記憶
	std::shared_ptr<Object> spThis = shared_from_this();

	// 現在に親GameObjectから脱退する
	auto nowParent = GetParent();
	if (nowParent) {
		nowParent->RemoveChild(spThis);
	}

	// 親ポインタを変更
	m_parent = parent;

	// 新しい親の子として自分を登録
	if (parent) {
		parent->m_children.push_back(spThis);
	}
}

void Object::SetMatrix(const RgMatrix& m, bool moveChild)
{
	// 変化前の行列をバックアップ
	m_mPrevWorld = m_mWorld;

	// 行列をセット
	m_mWorld = m;

	// 変化前と今回の行列から、１フレームでの変化量算出
	RgMatrix mInvPrev;
	m_mPrevWorld.Inverse(mInvPrev);
	RgMatrix mDelta = mInvPrev * m_mWorld;

	// 子も連動させる
	if (moveChild) {
		// 子以下のすべてのGameObjectのTransformにmDeltaを合成していく関数
		std::function<void(const std::shared_ptr<Object>&)>
			proc = [this, &proc, &mDelta](const std::shared_ptr<Object>& obj)
		{
			// 変化前の行列をバックアップ
			obj->m_mPrevWorld *= mDelta;
			// 適用
			obj->m_mWorld *= mDelta;

			// 子再帰
			for (auto&& child : obj->GetChildren()) {
				proc(child);
			}
		};

		// 子を処理する
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
	// クラス作成関数登録
	REGISTER_COMP_CLASS(ModelComponent);

	//====================================
	// (仮)モデルデータ読み込み
	//====================================
	//m_model.LoadModel("data/Models/vrm/Miraikomachi.vrm");
	//m_model2.LoadModel("data/Models/test/alicia-solid.vrm");

	//m_model	= RGRESFACT.GetModel("data/Models/chara/2b/2b.glb");
	//m_model2 = RGRESFACT.GetModel("data/Models/chara/mirai/Miraikomachi.vrm");

	// カメラ情報の作成
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



	// (仮)
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
	// 更新
	//====================
	m_level->Update();

	// エディタカメラの更新
	m_EditorCamera.Update();

	//====================
	// ImGui処理
	//====================
	ImGuiUpdate();

	//====================
	// 描画処理
	//====================
	GPL.UpdateCamera();

	//====================
	// 描画
	//====================
	// 描画関数をためる
	m_level->Draw();
	
	// デバッグ描画
	std::function<void()> debuhDraw = [this]()
	{
		m_EditorStage.Draw();
	};
	GPL.SetDebugDrawCall(debuhDraw);

	// 登録した描画関数の実行
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