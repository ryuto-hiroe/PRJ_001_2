#pragma once


class EditorStage
{
public:

	// 初期化
	bool Create();

	// 描画
	void Draw(RgCommandList& comList);

private:

	UINT m_gridSize = 21;

	// 線モデル
	std::vector<RgLineModel> m_LineX;
	std::vector<RgLineModel> m_LineZ;

	RgLineModel m_LineR;
	RgLineModel m_LineG;
	RgLineModel m_LineB;
};