#pragma once


class DrawThreadManager
{
public:

	// マルチスレッドで描画コマンドを作成する
	void CreateDrawCommands();

	// 各コマンドリストを設定する
	void CommandSetUp();

	// オブジェクトリストのNullcheckをする
	void CheakObjectList();

	// コマンドリストを実行
	void ExecuteCommandLists();

	//=======================
	//
	// 設定関係
	//
	//=======================
	// スレッド数を設定
	void InitThread();

	// オブジェクトを追加
	void SetObject(const std::shared_ptr<Object>& p);

private:

	// thread num
	UINT m_threadNum = 1;

	// 
	std::vector<std::vector<std::weak_ptr<Object>>> m_objectList;
	// 描画コマンド作成用
	//	スレッド数分用いする
	std::vector<RgCommandList> m_commandList;
};