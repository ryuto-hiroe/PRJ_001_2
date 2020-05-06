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

	// 描画
	void Draw();

	// エディタ処理
	void ImGuiUpdate();

	// 
	void Run();
	

	//=============================
	//
	// 取得関係
	//
	//=============================
	// rootオブジェクトの取得
	const std::shared_ptr<Object>& GetRoot() { return m_root; }


private:

	// rootオブジェクト
	std::shared_ptr<Object> m_root;

	// (仮)
	RgRenderTargets m_renderTarget;

	//
	//DrawThreadManager m_threadMgr;
};