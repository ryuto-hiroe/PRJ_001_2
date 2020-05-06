#pragma once

//===============================================
//
// ��{
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
// �Z�p�N���X
#include "Math/RgMath.h"

// �֗��@�\
#include "Utility/RgHelper.h"

// commandList�Ǘ��N���X
#include "Graphics/RgCommandList.h"

// DirectX12�̃f�B�X�N���v�^�q�[�v���Ǘ�����N���X
#include "Graphics/RgDescriptor.h"

// �R���X�^���g�o�b�t�@
#include "Graphics/RgConstantBuffer.h"

// �e�N�X�`��
#include "Graphics/RgTexture.h"

// Direct3D�Ǘ��N���X
#include "Graphics/RgD3D.h"

// �o�b�t�@�N���X
#include "Graphics/RgBuffer.h"

// ���f���֌W
// �t�@�C���ǂݍ���
#include "Model/RgStreamReader.h"

// ���f���N���X
#include "Model/RgModel.h"

// ���\�[�X���Ǘ�����N���X
#include "Utility/RgResourceFactory.h"

// �ȒP�Ȍ`��`�悷��N���X
#include "Graphics/RgPrimitive.h"


// ���C�u�����̃V�F�[�_
#include "Shader/RgPrimitiveShader.h"