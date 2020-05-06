#pragma once

namespace Rg
{
	// 簡単にバッファを作成する関数
	static ComPtr<ID3D12Resource1> CreateBuffer(UINT size, const void* src);
};


//====================================================
//
// 動的に頂点バッファを作成するクラス
//
//====================================================
template<class DataType>
class RgDynamicVertexBuffer
{
public:

	//===================================
	//
	// 取得関係
	//
	//===================================
	// バッファの構造体を取得(変更可能)
	std::vector<DataType>& GetWork() {
		//m_changeData = true;
		return m_data;
	}

	// バッファの構造体を取得(変更不可)
	//const DataType& GetWork_Read() const { return m_dataType; }

	//===================================
	//
	// 作成関係
	//
	//===================================
	// バッファ作成
	bool Create(const UINT dataNum);

	// CBVを設定するコマンドを作成
	void CreateCommandSetCBV(ComPtr<ID3D12GraphicsCommandList>& command)const;

	// データ書き込み
	void WriteData();

private:
	// 定数バッファのデータ
	std::vector<DataType> m_data;

	// 定数バッファ書き込み先
	DataType* m_mapped;

	// バッファのサイズ
	UINT m_bufferSize = 0;

	// 全体の大きさ
	UINT m_totalSize = 0;

	// バッファ
	ComPtr<ID3D12Resource> m_buffer;

	D3D12_VERTEX_BUFFER_VIEW m_bufferView;

	// 頂点バッファ用メモリ確保数
	const UINT m_MaxNum = 256;

	// 現在の使用番地
	UINT m_useIndex = 0;
};



//====================================================
//
// １フレームに１回以上定数バッファの内容を変更しないモノ用
//
//====================================================
template<class DataType>
class RgStaticConstantBuffer
{
public:

	//===================================
	//
	// 取得関係
	//
	//===================================
	// バッファの構造体を取得(変更可能)
	DataType& GetWork() {
		m_changeData = true;
		return m_dataType;
	}

	// バッファの構造体を取得(変更不可)
	const DataType& GetWork_Read() const { return m_dataType; }

	//===================================
	//
	// 作成関係
	//
	//===================================
	// バッファ作成
	bool Create();

	// CBVを設定するコマンドを作成
	void CreateCommandSetCBV(ComPtr<ID3D12GraphicsCommandList>& command, UINT paraNum)const;
	
	// データ書き込み
	void WriteData();

	// すべてのバッファに書き込む
	void WriteData_AllBuffer();

private:

	// 定数バッファのデータ
	DataType m_dataType;

	// 定数バッファ書き込み先
	//std::vector<DataType*> m_mapped;
	DataType* m_mapped;

	// バッファ
	//std::vector<ComPtr<ID3D12Resource>> m_buffer;
	ComPtr<ID3D12Resource> m_buffer;

	//CBV
	//std::vector<RgHeapHandle> m_cbv;
	RgHeapHandle m_cbv;

	// 定数バッファのサイズ
	UINT m_bufferSize = 0;

	// このフラグがtrueの時だけデータを書き込む
	bool m_changeData = false;
};

//====================================================
//
// 動的にCBVを作成する定数バッファクラス
//
//====================================================
template<class DataType>
class RgDynamicConstantBuffer
{
public:

	//===================================
	//
	// 取得関係
	//
	//===================================
	// バッファの構造体を取得(変更可能)
	DataType& GetWork() {
		m_changeData = true;
		return m_dataType;
	}

	// バッファの構造体を取得(変更不可)
	const DataType& GetWork_Read() const { return m_dataType; }


	//===================================
	//
	// 作成関係
	//
	//===================================
	// バッファ作成
	bool Create();

	// CBVを設定するコマンドを作成
	void CreateCommandSetCBV(ComPtr<ID3D12GraphicsCommandList>& command, UINT paraNum);



	// データ書き込み
	void WriteData();

private:
	// 定数バッファの作成数
	const UINT m_MaxCBNum = 256;

	// バッファ
	ComPtr<ID3D12Resource> m_buffer;

	//CBV
	RgHeapHandle m_cbv;
	// 定数バッファのデータ
	DataType m_dataType;
	// 定数バッファのサイズ
	UINT m_bufferSize = 0;

	// 書き込み先
	const UINT m_dataSize = 0x100; //256ppe
	struct { char buf[0x100]; }*mapped;

//	DataType* mapped;
	//
	UINT m_Sride = 0;
	// 現在使用できる場所
	UINT m_nextUsePos = 0;

	// 使用中のCBとDHのIndexを記憶
	std::unordered_map<UINT, UINT> m_saveIndicesInUse;

	// このフラグがtrueの時だけデータを書き込む
	bool m_changeData = false;
};
#include "RgBuffer.inl"