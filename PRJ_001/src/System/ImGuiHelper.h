#pragma once


//============================
// std::string�o�[�W����InputText
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
// ImGuzmo�p �}�j�s�����[�^�\���Ƒ���p�l���\��
// �Emat : ���삷��s��
// �EmView : �r���[�s��
// �EmProj : �ˉe�s��
// �Ebb : �g�呀�쎞�̃T�C�Y�g�p�o�E���f�B���O�{�b�N�X���w��(AABB) nullptr����-0,5�`0.5�̃T�C�Y�g���\�������
//===========================================
void ImGuizmoEditTransform(RgMatrix& mat, const RgMatrix& mView, const RgMatrix& mProj, const
	DirectX::BoundingBox* bb);


//===========================================
// ���\�[�X�t�@�C���I���|�b�v�A�b�v
// �Eid �c ID
// �EinoutFilename �c �I�����ꂽ�t�@�C��������
// �EextFilter �c �g���q�t�B���^ �����Ŏw�肳�ꂽ�g���q�̂ݕ\�������
// �EfolderSelectMode �c �t�H���_�I�����[�h
// �E�߂�l �c true:�I�����ꂽ false:�������Ă��Ȃ�
//===========================================
bool ImGuiBeginPopup_ResourceBrowser(
	const std::string& id, 
	std::string& inoutFilename, 
	const std::unordered_set<std::string> extFilter, 
	bool folderSelectMode = false);

//===========================================
// ���\�[�X�I���{�^��(ImGuiBeginPopup_ResourceBrowser�֐��g�p)
// �{�^�������N���b�N����ƁA�t�@�C���u���C�U���J���ȈՔ�
//===========================================
bool ImGuiResourceButton(
	const std::string& label, std::string& inoutFilename, 
	const std::unordered_set<std::string> extFilter, 
	bool folderSelectMode = false);