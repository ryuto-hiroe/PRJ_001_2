#pragma once

//=============================================
//
// 1つのコマンドリストクラス
//	・commandListとcommandAllocatorを持っている
//
//=============================================
class RgCommandList : public std::enable_shared_from_this<RgCommandList>
{
public:

	// 作成
	// ・num　・・・　アロケータの作成数。0を指定するとフレーム数分作成
	bool Create(const UINT num = 0);

	// コマンドの開始
	void Begin();
	// コマンドの終了
	void End();

	//========================
	//
	// 取得関係
	//
	//========================
	// コマンドアロケータの取得
	const std::vector<ComPtr<ID3D12CommandAllocator>>& GetAllocators(){ 
		return m_commandAllocators; 
	}
	// コマンドリストの取得
	ComPtr<ID3D12GraphicsCommandList>& GetCommandList() {
		return m_commandList;
	}


private:
	// 描画コマンドを蓄えるクラス
	//	・これはフレーム数分用いする
	std::vector<ComPtr<ID3D12CommandAllocator>> m_commandAllocators;
	// 描画コマンドを作成するクラス
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
};


//=============================================
//
// コマンドリストを管理するクラス
//	・RgCommandList	
//
//=============================================
class RgCommandListManager
{
public:

	// 初期化
	bool Init();


	//========================
	//
	// 取得関係
	//
	//========================
	// モデル描画用コマンドリスト
	std::vector<RgCommandList>& GetModelCommandList() {
		return m_drawModelCommandList;
	}

	// ワーク用コマンドリスト
	RgCommandList& GetWorkCommandList() {
		return m_workCommandList;
	}


	ComPtr<ID3D12GraphicsCommandList> m_drawCommandList;

private:

	// ワーク用コマンドリスト
	RgCommandList m_workCommandList;

	// モデル描画コマンド生成用
	std::vector<RgCommandList> m_drawModelCommandList;


	//========================
	// シングルトンパターン
	//========================
private:
	//
	RgCommandListManager() {}

public:

	static RgCommandListManager& GetInstance() {
		static RgCommandListManager inc;
		return inc;
	}
};
#define COMLIST RgCommandListManager::GetInstance()