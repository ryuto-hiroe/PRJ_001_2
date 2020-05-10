#pragma once


class EditorStage
{
public:

	// ������
	bool Create();

	// �`��
	void Draw();

private:

	UINT m_gridSize = 21;

	// �����f��
	std::vector<RgLineModel> m_LineX;
	std::vector<RgLineModel> m_LineZ;

	RgLineModel m_LineR;
	RgLineModel m_LineG;
	RgLineModel m_LineB;
};