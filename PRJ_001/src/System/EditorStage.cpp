
#include "main.h"
#include "EditorStage.h"

bool EditorStage::Create()
{
	// ƒ‚ƒfƒ‹ì¬
	m_LineX.resize(m_gridSize);
	for (int i = 0; i < m_gridSize; i++)
	{
		float offset = (float)(1 * i) - (m_gridSize / 2);
		m_LineX[i].Create({ offset, 0, -10 }, { offset, 0, 10 });
	}

	m_LineZ.resize(m_gridSize);
	for (int i = 0; i < m_gridSize; i++)
	{
		float offset = (float)(1 * i) - (m_gridSize / 2);
		m_LineZ[i].Create({ -10, 0, offset }, { 10, 0, offset });
	}

	m_LineR.Create({ 0, 0, 0 }, { 1, 0, 0 });
	m_LineG.Create({ 0, 0, 0 }, { 0, 1, 0 });
	m_LineB.Create({ 0, 0, 0 }, { 0, 0, 1 });

	return true;
}

void EditorStage::Draw(RgCommandList& comList)
{
	SHMGR.m_PrimSh.DrawLine(m_LineR, { 1, 0, 0, 1 }, comList.GetCommandList());
	SHMGR.m_PrimSh.DrawLine(m_LineG, { 0, 1, 0, 1 }, comList.GetCommandList());
	SHMGR.m_PrimSh.DrawLine(m_LineB, { 0, 0, 1, 1 }, comList.GetCommandList());
	
	for (auto&& model : m_LineX)
	{
		SHMGR.m_PrimSh.DrawLine(model, { 1, 1, 1, 0.7 }, comList.GetCommandList());
	}
	for (auto&& model : m_LineZ)
	{
		SHMGR.m_PrimSh.DrawLine(model, { 1, 1, 1, 0.7 }, comList.GetCommandList());
	}
}