#ifndef _COMMON_HPP
#define _COMMON_HPP

#include <windows.h>
#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <Psapi.h>
#include <unordered_map>
#include <mutex>
#include <functional>
#include <stack>
#include <span>

#include <d3d11.h>
#include <d3dx11.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

inline std::atomic_bool g_watermark{ true };
inline std::atomic_bool g_partner{ false };
inline std::string Version = "V71";

inline std::atomic_bool token_while{ true };
inline std::atomic_bool token_true{ false };
inline int Secure1 = INT_MAX;
inline int Secure69420 = INT_MAX;

inline int key3 = INT_MAX;
inline int key4 = INT_MAX;
inline int key5 = INT_MAX;
inline int key6 = INT_MAX;

#endif