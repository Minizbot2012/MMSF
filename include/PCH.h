#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define UNICODE
#define _UNICODE
// clang-format off
#include <SKSE/SKSE.h>
#include <RE/Skyrim.h>
#include <windows.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/fmt/bin_to_hex.h>
#include <REX/REX.h>
// clang-format on
namespace logger = SKSE::log;
using namespace REX;
using namespace std::literals;
#define DLLEXPORT __declspec(dllexport)
#include <Hooking.h>
