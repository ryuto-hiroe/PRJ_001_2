#pragma once


//=======================================================
//
// コンポーネントのベースクラス
//
//=======================================================
class BaseComponent : public std::enable_shared_from_this<BaseComponent>
{
public:

	virtual ~BaseComponent() {}

	//===============================
	//
	// 初期化関数
	//
	//===============================
	// オブジェクト生成時に呼ばれる
	virtual void Awake(){}

	// 
	virtual void Start(){}

	//===============================
	//
	// 更新関数
	//
	//===============================
	//
	virtual void Update() {}

	//===============================
	//
	// 描画関数
	//
	//===============================
	//
	virtual void Draw(){}

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
	virtual void ImGuiUpdate();

	//===============================
	//
	// 基本情報取得
	//
	//===============================
	// クラス名取得
	std::string ClassName() {
		return &typeid(*this).name()[6];
	}

	// 
	// 持ち主取得
	std::weak_ptr<Object> GetOwner()const { return m_pOwner; }
	// 持ち主設定
	void SetOwner(std::shared_ptr<Object> owner) { m_pOwner = owner; }

protected:
	// オーナーオブジェクトのポインタ
	std::weak_ptr<Object> m_pOwner;

	//
	bool m_isDelete = false;

	// 有効 / 無効フラグ
	bool m_enable = true;
};