#pragma once

//==========================================================
//
// 安全に解放する系のマクロ
//
//==========================================================

// 安全にRelease関数を実行する
template<class T>
void RgSafeRelease(T*& p) {
	if (p) {
		p->Release();
		p = nullptr;
	}
}

// 安全にDelete関数を実行する
template<class T>
void RgSafeDelete(T*& p) {
	if (p) {
		delete p;
		p = nullptr;
	}
}

// 配列版
template<class T>
void RgSafeDeleteArray(T*& p) {
	if (p) {
		delete[] p;
		p = nullptr;
	}
}


//==========================================================
//
// 文字列関係
//
//==========================================================

// std::stringからstd::wstringに変換する
inline std::wstring RgConvertStringToWstring(const std::string& str)
{	
	size_t len = str.length();
	std::wstring wStr;
	wStr.resize(len);
	size_t wLen;
	mbstowcs_s(&wLen, &wStr[0], len + 1, str.c_str(), _TRUNCATE);
	return wStr;
}

// std::stringからchar*に変換
inline void RgConvertStringToChar(const std::string& str, char* out, DWORD size)
{
	UINT i = 0;
	for (; i < str.size(); i++)
	{
		if (i >= size - 1)break;
		out[i] = str[i];
	}
	out[i] = '\0';
}

//==========================================================
//
// キー関係
//
//==========================================================
// 指定キーが押されているか判定
inline bool RgIsKeyDown(int keyCode, bool repert = true)
{
	// フラグ
	static char sFlag[256] = {};

	if (GetAsyncKeyState(keyCode) & 0x8000) {
		// 初回だけtrueを返す
		if (sFlag[keyCode] == 0) {
			sFlag[keyCode] = 1;
			return true;
		}

		// repert指定時は、押している間ずっとtrueを返す
		if (repert)return true;
	}
	else {
		sFlag[keyCode] = 0;
	}
	return false;
}

//======================================
//
// ファイル操作関係
//
//======================================
// 指定ファイルの内容を全て読み込む
inline bool RgLoadFile(const std::string& filename, std::vector<char>& outBuffer)
{
	std::ifstream ifs(filename, std::ios::binary);
	if (!ifs) return false;

	// ファイルサイズ
	ifs.seekg(0, std::ios::end);
	int len = (int)ifs.tellg();
	ifs.seekg(0, std::ios::beg);

	// バッファサイズ確保
	outBuffer.resize(len);

	// 読み込み
	ifs.read(&outBuffer[0], len);

	return true;
}

// 指定ファイルを文字列として読み込む
inline std::string RgLoadStringFile(const std::string& filename) {
	std::ifstream ifs(filename);
	if (!ifs)return "";

	std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	return std::move(str);
}

// 指定ファイルに文字列を書き込む(ファイルが存在する場合は上書き)
inline bool RgSaveStringFile(const std::string& filename, const std::string& srcString)
{
	std::ofstream ifs(filename);
	if (!ifs)return false;

	ifs.write(srcString.c_str(), srcString.size());

	return true;
}

// 指定したフォルダ内に同じファイル名があるかを確認。
// 同じ名前があったら後ろに数字をつけてまた確認する
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
					// 同じ名前のファイルが見つかった	
					outString = CheckRepetedName(folderName, fileName, extName);
				}
			}
		}
	);

	cnt = 0;

	return (outString);
}

// 指定フォルダ以下のファイル/フォルダ名を全部取得
// ・baseDirectory		… このフォルダから検索する
// ・isFile				… ファイル名を含む
// ・isDirectory		… フォルダ名を含む
inline std::vector<std::filesystem::path> RgEnumFiles(const std::string& baseDirectory, bool isFile = true, bool isDirectory = false)
{
	std::vector<std::filesystem::path> items;
	items.push_back(baseDirectory);

	std::filesystem::path basePath(baseDirectory);

	std::for_each(std::filesystem::recursive_directory_iterator(basePath), std::filesystem::recursive_directory_iterator(),
		[&items, isFile, isDirectory](const std::filesystem::path& p) {
			if (std::filesystem::is_regular_file(p)) { // ファイルの場合
				if (isFile)items.push_back(p);
			}
			else if (std::filesystem::is_directory(p)) { // ディレクトリの場合
				if (isDirectory)items.push_back(p);
			}
		}
	);

	return items;
}


// ファイルダイアログを表示
inline std::string FileDialog(const char* extFilter, std::string selectFile_def = "")
{
	OPENFILENAME    ofn;
	std::string     filename;
	char			workStr[256];

	filename = selectFile_def;  //忘れるとデフォルトファイル名に変な文字列が表示される
	RgConvertStringToChar(filename, workStr, sizeof(workStr));
	memset(&ofn, 0, sizeof(OPENFILENAME));  //構造体を0でクリア
	ofn.lStructSize		= sizeof(OPENFILENAME);
	ofn.lpstrFilter		= extFilter;
	ofn.lpstrFile		= workStr;
	ofn.nMaxFile		= sizeof(workStr);
	ofn.Flags			= OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt		= "glb";

	GetOpenFileName(&ofn);
	filename = workStr;
	return filename;
}