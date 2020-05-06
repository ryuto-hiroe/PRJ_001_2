#pragma once

//=============================================
//
// モデルを扱うコンポーネント
//
//=============================================
class ModelComponent : public BaseComponent
{
public:

	void Draw()override;
	void ImGuiUpdate()override;

	// モデルを設定
	void SetModel(const std::string& file);

private:
	// モデルファイル名
	std::string m_filename = "";
	// モデルデータ
	std::shared_ptr<RgModel> m_model;

};