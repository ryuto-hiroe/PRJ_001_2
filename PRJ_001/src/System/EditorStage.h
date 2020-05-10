#pragma once


class EditorStage
{
public:

	// ‰Šú‰»
	bool Create();

	// •`‰æ
	void Draw();

private:

	UINT m_gridSize = 21;

	// üƒ‚ƒfƒ‹
	std::vector<RgLineModel> m_LineX;
	std::vector<RgLineModel> m_LineZ;

	RgLineModel m_LineR;
	RgLineModel m_LineG;
	RgLineModel m_LineB;
};