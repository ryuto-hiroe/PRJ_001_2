#pragma once


//============================
// std::stringバージョンInputText
//============================
inline bool ImGuiInputString(
	const char* label, std::string& str, 
	ImGuiInputTextFlags flags = 0)
{
	static char tmp[1024];
	strcpy_s(tmp, sizeof(str), str.c_str());
	//strcpy(tmp, str.c_str());
	if (ImGui::InputText(label, tmp, sizeof(tmp), flags)) {
		str = tmp;
		return true;
	}
	return false;
}

//===========================================
// ImGuzmo用 マニピュレータ表示と操作パネル表示
// ・mat : 操作する行列
// ・mView : ビュー行列
// ・mProj : 射影行列
// ・bb : 拡大操作時のサイズ枠用バウンディングボックスを指定(AABB) nullptrだと-0,5～0.5のサイズ枠が表示される
//===========================================
void ImGuizmoEditTransform(RgMatrix& mat, const RgMatrix& mView, const RgMatrix& mProj, const
	DirectX::BoundingBox* bb);


//===========================================
// リソースファイル選択ポップアップ
// ・id … ID
// ・inoutFilename … 選択されたファイルが入る
// ・extFilter … 拡張子フィルタ ここで指定された拡張子のみ表示される
// ・folderSelectMode … フォルダ選択モード
// ・戻り値 … true:選択された false:何もしていない
//===========================================
bool ImGuiBeginPopup_ResourceBrowser(
	const std::string& id, 
	std::string& inoutFilename, 
	const std::unordered_set<std::string> extFilter, 
	bool folderSelectMode = false);

//===========================================
// リソース選択ボタン(ImGuiBeginPopup_ResourceBrowser関数使用)
// ボタンを左クリックすると、ファイルブライザが開く簡易版
//===========================================
bool ImGuiResourceButton(
	const std::string& label, std::string& inoutFilename, 
	const std::unordered_set<std::string> extFilter, 
	bool folderSelectMode = false);