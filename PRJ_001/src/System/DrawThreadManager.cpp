
#include "main.h"

#include "DrawThreadManager.h"


void DrawThreadManager::InitThread()
{
	// �`��R�}���h���N���A
	m_objectList.clear();

	// CPU�� �擾
	SYSTEM_INFO sys;
	GetSystemInfo(&sys);
	int cpuCnt = (int)sys.dwNumberOfProcessors;

	// �`��R�}���h���X�g�ƃI�u�W�F�N�g���X�g���X���b�h�����p������
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
			// ���g���Ȃ�������폜
			auto it = node.begin();
			while (it != node.end()) {
				if ((*it).expired() == true)
				{
					it = node.erase(it);
					continue;
				}
				// ���̂��
				++it;
			}
		}
	}
}

void DrawThreadManager::SetObject(const std::shared_ptr<Object>& p)
{
	// 
	UINT saveNum = (UINT)m_objectList[0].size();
	
	// �������Ȃ��X���b�h�ɒǉ�
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
		// �g�p����R�}���h���X�g��o�^
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