#pragma once





//===============================================================
//
// コンスタントバッファを管理するクラス
//	・一つ大きなコンスタントバッファを作成する
//
//===============================================================
class RgUniqueConstantBuffer
{
public:

	// コンスタントバッファ作成
	bool CreateConstantBuffer();

	// データ書き込み
	// 定数バッファ構造体アドレス
	// 定数バッファ構造体サイズ
	RgHeapHandle& WiteData(void* src, const UINT size);

	// 前描画が終わったらアクセッサーをはじめに戻す
	void ResetAccesser() {
		m_numAssigndPos = 0;
	}


private:

	// コンスタントバッファの個数
	const UINT m_bufferCount = 1000;

	// 一つのコンスタントバッファの最大サイズ
	const UINT m_maxConstantBufferBlocks = 0x100;

	// コンスタントバッファのサイズ
	UINT m_cbSize = 0;

	// 現在アクセスできる場所
	UINT m_numAssigndPos = 0;

	// 唯一のコンスタントバッファ
	ComPtr<ID3D12Resource1> m_cb = nullptr;
	// 書き込み先のアドレス
	struct {
		char buf[0x100];
	}
	*mappedConstantBuffer = nullptr;

	// 


	//====================================
	// シングルトンパターン
	//====================================
private:
	//
	RgUniqueConstantBuffer(){}

public:
	//
	static RgUniqueConstantBuffer& GetInstance()
	{
		static RgUniqueConstantBuffer instance;
		return instance;
	}
};
#define UCB RgUniqueConstantBuffer::GetInstance()