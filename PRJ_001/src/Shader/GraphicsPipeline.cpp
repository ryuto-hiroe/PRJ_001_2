
#include "main.h"
#include "GraphicsPipeline.h"

bool GraphicsPipeline::Init()
{
	// CPU数 取得
	SYSTEM_INFO sys;
	GetSystemInfo(&sys);
	int cpuCnt = (int)sys.dwNumberOfProcessors;

	// モデル描画のコマンドリストを作成
	m_threadNum = cpuCnt;
	m_ModelDrawCommandList.resize(m_threadNum);
	for (auto&& comList : m_ModelDrawCommandList)
	{
		comList = std::make_shared<RgCommandList>();
		comList->Create();
	}

	// GUI用コマンドリストの作成
	m_guiCommandList = std::make_shared<RgCommandList>();
	m_guiCommandList->Create();

	// デバッグ用コマンドリストの作成
	m_debugCommandList = std::make_shared<RgCommandList>();
	m_debugCommandList->Create();

	// 描画コマンドリセット
	m_modelDrawCalls.clear();

	//===============================
	// 定数バッファの初期化
	//===============================
	if (m_cb10_Camera.Create() == false) {
		return false;
	}

	return true;
}

void GraphicsPipeline::Draw()
{
	//===============
	// モデル描画開始
	//===============
	m_ModelDrawCommandList[0]->Begin();
	{
		// レンダーターゲットを設定する
		RGD3D.SetBackBuffer();
		RGD3D.ClearBackBuffer();

		// SRV、CBV用のディスクリプタヒープを設定
		RGHEAPMGR.MakeCommand();

		// モデルシェーダを設定
		SHMGR.m_ModelSh.SetUp();

		// カメラを設定
		CreateSetCameraCommand(2);

		// 作成した描画関数を実行する
		for (auto&& call : m_modelDrawCalls)
		{
			call();
		}

		// レンダーターゲットを表示可能にする
		RGD3D.ResetBackBuffer();
	}
	m_ModelDrawCommandList[0]->End();

	//===============
	// デバッグ描画
	//===============
	m_debugCommandList->Begin();
	{
		// レンダーターゲットを設定する
		RGD3D.SetBackBuffer();

		// SRV、CBV用のディスクリプタヒープを設定
		RGHEAPMGR.MakeCommand();

		// シェーダを設定
		SHMGR.m_PrimSh.SetUp();

		// カメラを設定
		CreateSetCameraCommand(1);

		// 描画関数実行
		m_debugDrawCall();

		// レンダーターゲットを表示可能にする
		RGD3D.ResetBackBuffer();
	}
	m_debugCommandList->End();

	//===============
	// GUI描画開始
	//===============
	m_guiCommandList->Begin();
	{
		// レンダーターゲットを設定する
		RGD3D.SetBackBuffer();
	
		// SRV、CBV用のディスクリプタヒープを設定
		RGHEAPMGR.MakeCommand();
	
		// GUI描画関数
		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(
			ImGui::GetDrawData(),
			m_guiCommandList->GetCommandList().Get()
		);
	
		// レンダーターゲットを表示可能にする
		RGD3D.ResetBackBuffer();
	}
	m_guiCommandList->End();

	//===============
	// コマンドの実行
	//===============
	std::vector<ID3D12CommandList*> comLists;
	// モデル描画コマンドリストを登録
	comLists.push_back(m_ModelDrawCommandList[0]->GetCommandList().Get());
	// デバッグ描画コマンドリストを登録
	comLists.push_back(m_debugCommandList->GetCommandList().Get());
	// GUI描画コマンドリストを登録
	comLists.push_back(m_guiCommandList->GetCommandList().Get());
	// コマンドの実行
	RGD3D.GetCommandQueue()->ExecuteCommandLists((UINT)comLists.size(), &comLists[0]);

	//===============
	// 描画関数をクリア
	//===============
	m_modelDrawCalls.clear();

	// Update and Render additional Platform Windows
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault(NULL, (void*)m_guiCommandList->GetCommandList().Get());
	}
}