#pragma once

//===============================================
//
// 基本
//
//===============================================
#pragma comment(lib,"winmm.lib")

#define NOMINMAX
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <commctrl.h>
#include <ctime>

#include <Msxml6.h>
#pragma comment(lib, "msxml6.lib")

//===============================================
//
// STL
//
//===============================================
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <list>
#include <iterator>
#include <queue>
#include <algorithm>
#include <memory>
#include <random>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <filesystem>
#include <bitset>

#include <cassert>
#include <cstdlib>


//===============================================
//
// Direct3D 12
//
//===============================================
#include <d3d12.h>
#include <dxgi1_6.h>

#include "d3dx12.h"
#include <wrl.h>

#include <dxcapi.h>
#include <D3Dcompiler.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxcompiler.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;

//===============================================
//
// DirectX12 TK
//
//===============================================
#pragma comment(lib, "DirectXTex.lib")

#include <SimpleMath.h>

//===============================================
//
// DirectX Tex
//
//===============================================
#pragma comment(lib, "DirectXTK12.lib")

#include <DirectXTex.h>

//===============================================
//
// GLTF
//
//===============================================
//#pragma comment(lib, "GLTFSDK.lib")

#include <GLTFSDK/GLTF.h>
//#include <GLTFSDK/GLTFResourceReader.h>
#include <GLTFSDK/GLBResourceReader.h>
#include <GLTFSDK/Deserialize.h>

//===============================================
//
// Rg Library
//
//===============================================
// 算術クラス
#include "Math/RgMath.h"

// 便利機能
#include "Utility/RgHelper.h"

// commandList管理クラス
#include "Graphics/RgCommandList.h"

// DirectX12のディスクリプタヒープを管理するクラス
#include "Graphics/RgDescriptor.h"

// コンスタントバッファ
#include "Graphics/RgConstantBuffer.h"

// テクスチャ
#include "Graphics/RgTexture.h"

// Direct3D管理クラス
#include "Graphics/RgD3D.h"

// バッファクラス
#include "Graphics/RgBuffer.h"

// モデル関係
// ファイル読み込み
#include "Model/RgStreamReader.h"

// モデルクラス
#include "Model/RgModel.h"

// リソースを管理するクラス
#include "Utility/RgResourceFactory.h"

// 簡単な形を描画するクラス
#include "Graphics/RgPrimitive.h"


// ライブラリのシェーダ
#include "Shader/RgPrimitiveShader.h"