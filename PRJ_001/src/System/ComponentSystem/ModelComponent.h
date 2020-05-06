#pragma once

//=============================================
//
// ���f���������R���|�[�l���g
//
//=============================================
class ModelComponent : public BaseComponent
{
public:

	void Draw()override;
	void ImGuiUpdate()override;

	// ���f����ݒ�
	void SetModel(const std::string& file);

private:
	// ���f���t�@�C����
	std::string m_filename = "";
	// ���f���f�[�^
	std::shared_ptr<RgModel> m_model;

};