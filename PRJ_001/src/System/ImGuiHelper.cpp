
#include "main.h"
#include "ImGuiHelper.h"


// ImGuizmo用マニピュレータ表示と操作パネル表示関数
void ImGuizmoEditTransform(RgMatrix& mat, const RgMatrix& mView, const RgMatrix& mProj, const DirectX::BoundingBox* bb)
{
	// 操作パネル
	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
	// 座標、回転、拡大の切り替えラジオボタン
	if (ImGui::RadioButton("Pos", mCurrentGizmoOperation == ImGuizmo::TRANSLATE)) {
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE)) {
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE)) {
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	}
	// 行列から、座標、回転、拡大へ変換
	float matrixTranslation[3], matrixRotation[3], matrixScale[3];
	ImGuizmo::DecomposeMatrixToComponents(mat, matrixTranslation, matrixRotation, matrixScale);
	// 操作
	ImGui::DragFloat3("Pos", matrixTranslation, 0.01f);
	ImGui::DragFloat3("Rotate", matrixRotation, 1);
	ImGui::DragFloat3("Scale", matrixScale, 0.001f);
	// 座標、回転、拡大から行列へ変換
	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, mat);
	// ローカル操作かワールド操作
	if (mCurrentGizmoOperation != ImGuizmo::SCALE)
	{
		if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL)) {
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD)) {
			mCurrentGizmoMode = ImGuizmo::WORLD;
		}
	}
	// スナップOn/Off
	static bool useSnap(false);
	ImGui::Checkbox("", &useSnap);
	ImGui::SameLine();
	// スナップ設定
	RgVec3 snap;
	static RgVec3 sPosSnap = { 1,1,1 };
	static float sRotateSnap = 10;
	static float sScaleSnap = 0.1f;
	switch (mCurrentGizmoOperation)
	{
	case ImGuizmo::TRANSLATE:
		snap = sPosSnap;
		ImGui::InputFloat3("Snap", &snap.x);
		break;
	case ImGuizmo::ROTATE:
		snap.x = sRotateSnap;
		ImGui::InputFloat("Angle Snap", &snap.x);
		break;
	case ImGuizmo::SCALE:
		snap.x = sScaleSnap;
		ImGui::InputFloat("Scale Snap", &snap.x);
		break;
	}
	// 境界サイズ変更枠
	static bool boundSizing = false;
	ImGui::Checkbox("Bound Sizing", &boundSizing);
	static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	if (bb) {
		bounds[0] = bb->Center.x - bb->Extents.x;
		bounds[1] = bb->Center.y - bb->Extents.y;
		bounds[2] = bb->Center.z - bb->Extents.z;
		bounds[3] = bb->Center.x + bb->Extents.x;
		bounds[4] = bb->Center.y + bb->Extents.y;
		bounds[5] = bb->Center.z + bb->Extents.z;
	}

	// ImGuizmo
	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	ImGuizmo::Manipulate(mView, mProj, mCurrentGizmoOperation, mCurrentGizmoMode, mat, NULL, useSnap ?
		&snap.x : NULL, boundSizing ? bounds : NULL);
}

bool ImGuiBeginPopup_ResourceBrowser(const std::string& id, std::string& inoutFilename, const
	std::unordered_set<std::string> extFilter, bool folderSelectMode)
{
	// １つのフォルダデータ
	struct FolderNode {
		// フォルダパス
		std::filesystem::path m_Path;
		// 配属されているファイル
		std::list<std::filesystem::path> m_Childs;
	};
	static std::vector<FolderNode> sFolderList;
	static int sSelectIndex = -1;
	//
	bool bSelected = false;
	ImGui::PushID(id.c_str());
	//----------------------------------------------
	// 直前のコントロールが左クリックされると、ファイルブラウザーを開く
	//----------------------------------------------
	if (ImGui::IsMouseReleased(0) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
	{
		// クリックされたら、フォルダとファイル一覧を作成
		sFolderList.clear();
		sSelectIndex = -1;
		std::string parentpath = std::filesystem::path(inoutFilename).parent_path().string();
		// dataフォルダ以下のファイル名を全取得
		std::vector<std::filesystem::path> fileList = RgEnumFiles("data", true, true);
		for (auto&& item : fileList) {
			// フォルダかどうか
			if (std::filesystem::is_directory(item)) {
				sFolderList.push_back(FolderNode());
				sFolderList.back().m_Path = item;
				// 現在のファイルと同じフォルダなら、選択indexを指定する
				if (parentpath == item) {
					sSelectIndex = sFolderList.size() - 1;
				}
			}
		}
		if (folderSelectMode == false) {
			// 全ファイル名を調べ、該当するフォルダへ登録
			for (auto&& item : fileList) {
				// ファイルかどうか
				if (std::filesystem::is_directory(item) == false) {
					// 指定拡張子のファイルのみ
					if (extFilter.count(item.extension().string()) == 0) continue;
					// 自分の所属するフォルダ
					auto parent = item.parent_path();
					// フォルダ一覧から探す
					for (auto&& n : sFolderList) {
						if (n.m_Path == parent) {
							// そのフォルダの子としてファイル名を登録
							n.m_Childs.push_back(item);
							break;
						}
					}
				}
			}
		}
	}
	//----------------------------------------------
	// ファイルブラウザーポップアップ
	//----------------------------------------------
	if (ImGui::BeginPopupContextItem("ResourceFileBrowserContextMenu", 0)) {
		// 解除
		if (ImGui::Button(u8"解除")) {
			// 選択
			inoutFilename = "";
			bSelected = true;
			// ポップアップを閉じる
			ImGui::CloseCurrentPopup();
		}
		ImGui::BeginChild("ResourceFileBrowserContextMenu Left", ImVec2(300, 400), true);
		ImGui::Text(u8"フォルダ");
		ImGui::Separator();
		// フォルダ一覧
		{
			int idx = 0;
			for (auto&& node : sFolderList) {
				// 選択されているフォルダ？
				bool selected = idx == sSelectIndex ? true : false;
				// ファイル名
				if (ImGui::Selectable(node.m_Path.string().c_str(), selected)) {
					sSelectIndex = idx;
					// フォルダ選択モード
					if (folderSelectMode) {
						// 選択
						inoutFilename = node.m_Path.string();
						bSelected = true;
						// ポップアップを閉じる
						ImGui::CloseCurrentPopup();
					}
				}
				idx++;
			}
		}
		ImGui::EndChild();
		if (folderSelectMode == false) {
			ImGui::SameLine();
			ImGui::BeginChild("ResourceFileBrowserContextMenu Right", ImVec2(300, 400), true);
			ImGui::Text(u8"ファイル");
			ImGui::Separator();
			// ファイル一覧
			if (sSelectIndex >= 0) {
				auto& currentFolder = sFolderList[sSelectIndex];
				for (auto&& item : currentFolder.m_Childs) {
					// ファイル名
					if (ImGui::Selectable(item.filename().string().c_str())) {
						// 選択
						inoutFilename = item.string();
						bSelected = true;
						// ポップアップを閉じる
						ImGui::CloseCurrentPopup();
					}
				}
			}
			ImGui::EndChild();
		}
		ImGui::EndPopup();
	}
	ImGui::PopID();
	return bSelected;
}


bool ImGuiResourceButton(const std::string& label, std::string& inoutFilename, const
	std::unordered_set<std::string> extFilter, bool folderSelectMode)
{
	//----------------------------------------------
	// Label
	//----------------------------------------------
	ImGui::Text(label.c_str());
	std::string str;
	if (inoutFilename.size() == 0)str = u8"no file";
	else str = inoutFilename;
	// ボタン
	ImGui::Button(str.c_str());
	// 上記のコントロールの左クリックを感知し、リソースブラウザの処理が実行される
	if (ImGuiBeginPopup_ResourceBrowser(label, str, extFilter, folderSelectMode)) {
		inoutFilename = str;
		return true;
	}
	return false;
}