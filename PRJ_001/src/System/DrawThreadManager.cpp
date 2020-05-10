
#include "main.h"

#include "DrawThreadManager.h"


void DrawThreadManager::InitThread()
{
	// 描画コマンドをクリア
	m_objectList.clear();

	// CPU数 取得
	SYSTEM_INFO sys;
	GetSystemInfo(&sys);
	int cpuCnt = (int)sys.dwNumberOfProcessors;

	// 描画コマンドリストとオブジェクトリストをスレッド数分用いする
	m_objectList.resize(cpuCnt);
	m_commandList.resize(cpuCnt);

	for (auto comList : m_commandList)
	{
		comList.Create();
	}

	m_threadNum = cpuCnt;
}

void DrawThreadManager::CheakObjectList()
{
	for (auto&& node : m_objectList)
	{
		for (auto&& obj : node)
		{
			// 中身がなかったら削除
			auto it = node.begin();
			while (it != node.end()) {
				if ((*it).expired() == true)
				{
					it = node.erase(it);
					continue;
				}
				// 次のやつへ
				++it;
			}
		}
	}
}

void DrawThreadManager::SetObject(const std::shared_ptr<Object>& p)
{
	// 
	UINT saveNum = (UINT)m_objectList[0].size();
	
	// 数が少ないスレッドに追加
	bool addFlag = false;
	for (UINT i = 1; i < m_objectList.size(); i++)
	{
		if (saveNum > m_objectList[i].size())
		{
			m_objectList[i].push_back(p);
			addFlag = true;
			break;
		}
	}
	if (addFlag == false)
	{
		m_objectList[0].push_back(p);
	}
}

void DrawThreadManager::CommandSetUp()
{
	for (UINT i = 0; i < m_threadNum; i++)
	{
		//SHMGR.m_ModelSh.SetUp(m_commandList[i].GetCommandList());
	}
}

void DrawThreadManager:: CreateDrawCommands()
{
	for (UINT i = 0; i < m_objectList.size(); i++)
	{
		// 使用するコマンドリストを登録
		COMLIST.m_drawCommandList = m_commandList[i].GetCommandList();

		m_commandList[i].Begin();

		for (auto&& obj : m_objectList[i])
		{
			obj.lock()->Draw();
		}

		m_commandList[i].End();
	}
}

void DrawThreadManager::ExecuteCommandLists()
{
	const UINT listNUm = (UINT)m_commandList.size();
	ID3D12CommandList* lists[30];
	for (UINT i = 0; i < listNUm; i++)
	{
		lists[i] = m_commandList[i].GetCommandList().Get();
	}
	RGD3D.m_commandQueue->ExecuteCommandLists((UINT)m_commandList.size(), lists);
}