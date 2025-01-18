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
	SafetyHookMid chara_select_hook_;
	SafetyHookMid chara_select_hook_2_;
	SafetyHookMid chara_select_hook_3_;
	SafetyHookMid NormalAttackHook;
	SafetyHookMid KnockDownHook;
	auto initialize() -> void;

public:
	GGFramework(const GGFramework&) = delete;

	static auto get_instance() -> GGFramework*;
};
