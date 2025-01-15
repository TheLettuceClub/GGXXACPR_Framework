#pragma once
#include <mutex>

#include "safetyhook.hpp"

class GGFramework
{
private:
	GGFramework()
	{
		initialize();
	}

	static GGFramework* instance_;
	static std::mutex mtx_;

	SafetyHookMid input_check_hook_;
	SafetyHookMid RoundInitHook;
	SafetyHookMid RoundEndHook;
	SafetyHookMid GameModeHook;
	auto initialize() -> void;

public:
	GGFramework(const GGFramework&) = delete;

	static auto get_instance() -> GGFramework*;
};
