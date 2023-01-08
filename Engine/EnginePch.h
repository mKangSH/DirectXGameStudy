#pragma once

// std::byte ������� ����
#define _HAS_STD_BYTE 0

// include (STL)
#include <windows.h>
#include <tchar.h>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>
using namespace std;

#include <filesystem>
namespace fs = std::filesystem;

// include (DirectX)
#include "d3dx12.h"
#include <d3d12.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

// ThirdPartyLib
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>
#include "SimpleMath.h"

// lib
#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex.lib")
#endif

// typedef
using int8		= __int8;
using int16		= __int16;
using int32		= __int32;
using int64		= __int64;
using uint8		= unsigned __int8;
using uint16	= unsigned __int16;
using uint32	= unsigned __int32;
using uint64	= unsigned __int64;
using Vec2		= DirectX::SimpleMath::Vector2;
using Vec3		= DirectX::SimpleMath::Vector3;
using Vec4		= DirectX::SimpleMath::Vector4;
using Matrix	= DirectX::SimpleMath::Matrix;

enum class CBV_REGISTER : uint8
{
	b0,
	b1,
	b2,
	b3,
	b4,

	END
};

enum class SRV_REGISTER : uint8
{
	t0 = static_cast<uint8>(CBV_REGISTER::END),
	t1,
	t2,
	t3,
	t4,

	END
};

enum
{
	SWAP_CHAIN_BUFFER_COUNT = 2,
	SWAP_CHAIN_REFRESH_RATE_COUNT = 60,
	CBV_REGISTER_COUNT = CBV_REGISTER::END,
	SRV_REGISTER_COUNT = static_cast<uint8>(SRV_REGISTER::END) - CBV_REGISTER_COUNT,
	REGISTER_COUNT = CBV_REGISTER_COUNT + SRV_REGISTER_COUNT,
};

struct WindowInfo
{
	HWND	hwnd;		// ��� ������ �ڵ�
	int32	width;		// ������ �ʺ�
	int32	height;		// ������ ����
	bool	windowed;	// â���, ��ü ȭ��
};

struct Vertex
{
	Vertex() {}

	Vertex(Vec3 p, Vec2 u, Vec3 n, Vec3 t)
		: pos(p), uv(u), normal(n), tangent(t)
	{
	}

	Vec3 pos;
	Vec2 uv;
	Vec3 normal;
	Vec3 tangent;
};

struct TransformParams
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProjection;
	Matrix matWV;
	Matrix matWVP;
};

#define DECLARE_SINGLE(type)					\
public:											\
	static type* GetInstance()					\
	{											\
		static type instance;					\
		return &instance;						\
	}											\
private:										\
	type() = default;							\
	~type() {}									\
	type(const type&) = delete;					\
	type& operator=(const type&) = delete;		\
	type(type&&) = delete;						\
	type& operator=(type&&) = delete;			\

#define GET_SINGLE(type)	type::GetInstance()

#define DEVICE				GEngine->GetDevice()->GetDevice()
#define CMD_LIST			GEngine->GetCommandQueue()->GetCommandList()
#define RESOURCE_CMD_LIST	GEngine->GetCommandQueue()->GetResourceCommandList()
#define ROOT_SIGNATURE		GEngine->GetRootSignature()->GetSignature()

#define INPUT				GET_SINGLE(Input)
#define TIMER				GET_SINGLE(Timer)
#define DELTA_TIME			TIMER->GetDeltaTime()

#define CONST_BUFFER(type)	GEngine->GetConstantBuffer(type)

extern unique_ptr<class Engine> GEngine;
void GlobalEngineInit();