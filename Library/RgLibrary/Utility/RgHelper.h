#pragma once

//==========================================================
//
// ���S�ɉ������n�̃}�N��
//
//==========================================================

// ���S��Release�֐������s����
template<class T>
void RgSafeRelease(T*& p) {
	if (p) {
		p->Release();
		p = nullptr;
	}
}

// ���S��Delete�֐������s����
template<class T>
void RgSafeDelete(T*& p) {
	if (p) {
		delete p;
		p = nullptr;
	}
}

// �z���
template<class T>
void RgSafeDeleteArray(T*& p) {
	if (p) {
		delete[] p;
		p = nullptr;
	}
}


//==========================================================
//
// ������֌W
//
//==========================================================

// std::string����std::wstring�ɕϊ�����
inline std::wstring RgConvertStringToWstring(const std::string& str)
{	
	size_t len = str.length();
	std::wstring wStr;
	wStr.resize(len);
	size_t wLen;
	mbstowcs_s(&wLen, &wStr[0], len + 1, str.c_str(), _TRUNCATE);
	return wStr;
}

//==========================================================
//
// �L�[�֌W
//
//==========================================================
// �w��L�[��������Ă��邩����
inline bool RgIsKeyDown(int keyCode, bool repert = true)
{
	// �t���O
	static char sFlag[256] = {};

	if (GetAsyncKeyState(keyCode) & 0x8000) {
		// ���񂾂�true��Ԃ�
		if (sFlag[keyCode] == 0) {
			sFlag[keyCode] = 1;
			return true;
		}

		// repert�w�莞�́A�����Ă���Ԃ�����true��Ԃ�
		if (repert)return true;
	}
	else {
		sFlag[keyCode] = 0;
	}
	return false;
}

//======================================
//
// �t�@�C������֌W
//
//======================================
// �w��t�@�C���̓��e��S�ēǂݍ���
inline bool RgLoadFile(const std::string& filename, std::vector<char>& outBuffer)
{
	std::ifstream ifs(filename, std::ios::binary);
	if (!ifs) return false;

	// �t�@�C���T�C�Y
	ifs.seekg(0, std::ios::end);
	int len = (int)ifs.tellg();
	ifs.seekg(0, std::ios::beg);

	// �o�b�t�@�T�C�Y�m��
	outBuffer.resize(len);

	// �ǂݍ���
	ifs.read(&outBuffer[0], len);

	return true;
}

// �w��t�@�C���𕶎���Ƃ��ēǂݍ���
inline std::string RgLoadStringFile(const std::string& filename) {
	std::ifstream ifs(filename);
	if (!ifs)return "";

	std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	return std::move(str);
}

// �w��t�@�C���ɕ��������������(�t�@�C�������݂���ꍇ�͏㏑��)
inline bool RgSaveStringFile(const std::string& filename, const std::string& srcString)
{
	std::ofstream ifs(filename);
	if (!ifs)return false;

	ifs.write(srcString.c_str(), srcString.size());

	return true;
}

// �w�肵���t�H���_���ɓ����t�@�C���������邩���m�F�B
// �������O������������ɐ��������Ă܂��m�F����
inline std::string CheckRepetedName(
	const std::string& folderName,
	const std::string& fileName,
	const std::string& extName)
{
	std::filesystem::path path(folderName);
	std::string outString = fileName;
	static int cnt = 0;
	if (cnt > 0) {
		outString = fileName + std::to_string(cnt);
	}
	cnt++;

	std::for_each(
		std::filesystem::recursive_directory_iterator(path),
		std::filesystem::recursive_directory_iterator(),
		[&outString, folderName, fileName, extName](const std::filesystem::path& p) {
			if (std::filesystem::is_regular_file(p))
			{
				if (p.filename().string() == (outString + extName)) {
					// �������O�̃t�@�C������������	
					outString = CheckRepetedName(folderName, fileName, extName);
				}
			}
		}
	);

	cnt = 0;

	return (outString);
}

// �w��t�H���_�ȉ��̃t�@�C��/�t�H���_����S���擾
// �EbaseDirectory		�c ���̃t�H���_���猟������
// �EisFile				�c �t�@�C�������܂�
// �EisDirectory		�c �t�H���_�����܂�
inline std::vector<std::filesystem::path> RgEnumFiles(const std::string& baseDirectory, bool isFile = true, bool isDirectory = false)
{
	std::vector<std::filesystem::path> items;
	items.push_back(baseDirectory);

	std::filesystem::path basePath(baseDirectory);

	std::for_each(std::filesystem::recursive_directory_iterator(basePath), std::filesystem::recursive_directory_iterator(),
		[&items, isFile, isDirectory](const std::filesystem::path& p) {
			if (std::filesystem::is_regular_file(p)) { // �t�@�C���̏ꍇ
				if (isFile)items.push_back(p);
			}
			else if (std::filesystem::is_directory(p)) { // �f�B���N�g���̏ꍇ
				if (isDirectory)items.push_back(p);
			}
		}
	);

	return items;
}