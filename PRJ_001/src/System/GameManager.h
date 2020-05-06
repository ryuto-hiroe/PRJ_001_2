#pragma once

class Object;

// コンポーネントクラス
#include "System/ComponentSystem/BaseComponent.h"
#include "System/ComponentSystem/ModelComponent.h"


//==========================================
//
// オブジェクトクラス
//
//==========================================
class Object : public std::enable_shared_from_this<Object>
{
public:

	//===============================
	//
	// 初期化関数
	//
	//===============================
	// オブジェクト生成時に呼ばれる
	void Awake();

	// 
	void Start();

	//===============================
	//
	// 更新関数
	//
	//===============================
	//
	void Update();

	//===============================
	//
	// 描画関数
	//
	//===============================
	//
	void Draw();

	//===============================
	//
	// Serialize / Deserialize
	//
	//===============================


	//===============================
	//
	// Editor用
	//
	//===============================
	// ImGuiの処理
	void ImGuiUpdate();

	//===============================
	//
	// 親子構造関係
	//
	//===============================
	// 子リスト取得
	const std::list<std::shared_ptr<Object>>& GetChildren() { return m_children; }

	// 親を取得
	std::shared_ptr<Object> GetParent() const { return m_parent.lock(); }

	// 指定した子を削除(即座に消すため、呼び出し時は注意)
	void RemoveChild(const std::shared_ptr<Object>& obj);

	// 親を設定
	void SetParent(const std::shared_ptr<Object>& parent);

	//===============================
	//
	// 行列操作関係
	//
	//===============================
	// 行列
	void SetMatrix(const RgMatrix& m, bool moveChild);
	// 行列の取得
	const RgMatrix& GetMatrix()const { return m_mWorld; }

	//===============================
	//
	// 基本情報の取得・設定
	//
	//===============================
	// 名前
	const std::string& IsName()const {
		return m_name;
	}
	void SetName(const std::string& name) { 
		m_name = name; 
	}

	//===============================
	//
	// コンポーネント関係
	//
	//===============================
	//
	void AddComponent(const std::shared_ptr<BaseComponent>& comp);

private:

	// 名前
	std::string m_name = "a";

	// 行列
	RgMatrix m_mWorld;		// ワールド行列
	RgMatrix m_mPrevWorld;	// 変更前の行列

	// 子リスト
	std::list<std::shared_ptr<Object>>	m_children;
	// 親ポインタ
	std::weak_ptr<Object>				m_parent;

	// コンポーネント
	std::list<std::shared_ptr<BaseComponent>> m_components;

	// 削除フラグ
	bool m_isDelete = false;
};

//
#include "System/DrawThreadManager.h"
//
#include "System/Level.h"


class GameManager
{
public:

	// 初期化
	bool Init();

	// 解放
	void Release();

	// 描画処理
	void Draw();

	// GUI処理
	void ImGuiUpdate();

	// ゲーム処理
	void Run();

	// 現在選択中のオブジェクト
	std::weak_ptr<Object> m_Editor_Select_Object;

	EditorCamera m_EditorCamera;
	EditorStage m_EditorStage;


	// Componentリスト & 生成
	std::map<std::string, std::function<std::shared_ptr<BaseComponent>()>>
		m_componentClassMaker;

private:

	// 
	std::shared_ptr<Level> m_level;

	//========================
	// デバッグ用
	//========================
	// モデル
	std::shared_ptr<RgModel> m_model;
	std::shared_ptr<RgModel> m_model2;

	// テスト用
	RgPrimitive m_primitive;

	RgLineModel m_lineModel1;
	RgLineModel m_lineModel2;


	//===============================
	// シングルトンパターン
	//===============================
private:

	//
	GameManager() {}

public:

	//
	static GameManager& GetInstance() {
		static GameManager instance;
		return instance;
	}
};
//
#define GAMEMGR GameManager::GetInstance()

// クラスメーカーの短縮
#define REGISTER_COMP_CLASS(className)\
GAMEMGR.m_componentClassMaker[#className] = []() {\
	return std::make_shared<className>();\
}