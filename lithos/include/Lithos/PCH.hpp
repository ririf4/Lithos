// lithos/include/Lithos/PCH.hpp
#pragma once
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

// Windows & DirectX
#include <windows.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#include <d2d1_3.h>
#include <d3d11.h>
#include <dxgi1_6.h>
#include <dwrite_3.h>
#include <wincodec.h>

// 標準ライブラリ
#include <string>
#include <vector>
#include <memory>
#include <type_traits>