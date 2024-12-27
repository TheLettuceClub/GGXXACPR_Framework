#include "pch.h"
#include "GGFramework.h"
#include "GG.h"
#include <algorithm>
#include <iostream>

GGFramework* GGFramework::instance_ = nullptr;
std::mutex GGFramework::mtx_{};
std::vector<void*> GGFramework::act_tbs;
std::vector<void*> GGFramework::obj_ids;
std::vector<std::string> GGFramework::chara_paths_;
int* GGFramework::game_version_;
std::vector<int32_t(*)(CHARACTER_WORK*)> GGFramework::input_check_funcs_;
std::vector<int32_t(*)(CHARACTER_WORK*)> GGFramework::taunt_check_funcs_;
std::vector<int32_t(*)(CHARACTER_WORK*)> GGFramework::respect_check_funcs_;
std::vector<int32_t(*)(CHARACTER_WORK*)> GGFramework::special_attack_check_funcs_;
std::vector<PushColli> GGFramework::push_collis_;
std::vector<uint32_t> GGFramework::normal_attack_disables_;
std::vector<uint16_t> GGFramework::near_slash_dists_;

namespace
{
    HMODULE base;
    int sl_reload_obj_ids [] { 0x1a, 0x2c, 0x42 };
    int* obj_id_tb[] { sl_reload_obj_ids };
}

auto GGFramework::get_instance() -> GGFramework*
{
    if (instance_ == nullptr)
    {
        std::lock_guard lock(mtx_);
        if (instance_ == nullptr)
        {
            instance_ = new GGFramework();
        }
    }
    return instance_;
}

auto GGFramework::initialize() -> void
{
    base = GetModuleHandle(NULL);

    game_version_ = reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(base) + 0x6d0538);
    
    register_chara_id("sl_reload");
    register_act_tb(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(base) + 0x5f08b8));
    register_obj_id(&obj_id_tb);
    register_input_check_func(reinterpret_cast<int32_t(*)(CHARACTER_WORK*)>(reinterpret_cast<uintptr_t>(base) + 0x253530));
    register_taunt_check_func(reinterpret_cast<int32_t(*)(CHARACTER_WORK*)>(reinterpret_cast<uintptr_t>(base) + 0x252660));
    register_respect_check_func(reinterpret_cast<int32_t(*)(CHARACTER_WORK*)>(reinterpret_cast<uintptr_t>(base) + 0x252640));
    register_special_attack_check_func(reinterpret_cast<int32_t(*)(CHARACTER_WORK*)>(reinterpret_cast<uintptr_t>(base) + 0x253610));
    register_push_colli(PushColli {.stand_width= 0xBB8, .stand_height= 0x2710, .sit_width= 0xC80, .sit_height= 0x1B58,
        .sky_width= 0xBB8, .sky_height= 0x1B58, .sky_base_height= 0xFA0
    });
    register_normal_attack_disable(0xe025ffff);
    register_near_slash_dist(0x4268);

    player_main_hook_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x137b5e, [](SafetyHookContext& ctx)
    {
        if (static_cast<CharacterID>(ctx.eax) > CharacterID::Justice)
        {
            if (ctx.eax - static_cast<uint32_t>(CharacterID::Justice) > act_tbs.size()) return;
            auto result = act_tbs[ctx.eax - static_cast<uint32_t>(CharacterID::Justice) - 1];

            if (result == nullptr) return;

            ctx.ebx = reinterpret_cast<uintptr_t>(result);
        }
    });

    load_obj_file_hook_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x113d10, [](SafetyHookContext& ctx)
    {
        const auto current_characters = reinterpret_cast<CharacterID*>(reinterpret_cast<uintptr_t>(base) + 0x6d660c);
        if (current_characters[0] <= CharacterID::Justice) return;

        ctx.eax = reinterpret_cast<uintptr_t>(chara_paths_[static_cast<int32_t>(current_characters[0]) - static_cast<int32_t>(
            CharacterID::Justice) - 1].c_str());
    });

    load_obj_file_hook_2_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x113d80, [](SafetyHookContext& ctx)
    {
        const auto current_characters = reinterpret_cast<CharacterID*>(reinterpret_cast<uintptr_t>(base) + 0x6d660c);
        if (current_characters[1] <= CharacterID::Justice) return;

        ctx.eax = reinterpret_cast<uintptr_t>(chara_paths_[static_cast<int32_t>(current_characters[1]) - static_cast<int32_t>(
            CharacterID::Justice) - 1].c_str());
    });

    load_obj_file_hook_3_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x113de3, [](SafetyHookContext& ctx)
    {
        const auto current_characters = reinterpret_cast<CharacterID*>(reinterpret_cast<uintptr_t>(base) + 0x6d660c);
        if (current_characters[0] <= CharacterID::Justice) return;

        ctx.eax = reinterpret_cast<uintptr_t>(chara_paths_[static_cast<int32_t>(current_characters[0]) - static_cast<int32_t>(
            CharacterID::Justice) - 1].c_str());
    });

    load_obj_file_hook_4_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x113e47, [](SafetyHookContext& ctx)
    {
        const auto current_characters = reinterpret_cast<CharacterID*>(reinterpret_cast<uintptr_t>(base) + 0x6d660c);
        if (current_characters[0] <= CharacterID::Justice) return;

        ctx.eax = reinterpret_cast<uintptr_t>(chara_paths_[static_cast<int32_t>(current_characters[0]) - static_cast<int32_t>(
            CharacterID::Justice) - 1].c_str());
    });

    load_obj_file_hook_5_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x113e92, [](SafetyHookContext& ctx)
    {
        const auto current_characters = reinterpret_cast<CharacterID*>(reinterpret_cast<uintptr_t>(base) + 0x6d660c);
        if (current_characters[1] <= CharacterID::Justice) return;

        ctx.eax = reinterpret_cast<uintptr_t>(chara_paths_[static_cast<int32_t>(current_characters[1]) - static_cast<int32_t>(
            CharacterID::Justice) - 1].c_str());
    });

    allocate_file_hook_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x1139a2, [](SafetyHookContext& ctx)
    {
        if (ctx.esi > 0x8DB && ctx.esi < 0x8F3)
        {
            ctx.edx = (uintptr_t)chara_paths_[static_cast<int32_t>(ctx.esi - 0x8DC)].c_str();
        }
    });

    allocate_file_hook_2_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x1139ef, [](SafetyHookContext& ctx)
    {
        if (ctx.esi > 0x8DB && ctx.esi < 0x8F3)
        {
            ctx.eax = (uintptr_t)chara_paths_[static_cast<int32_t>(ctx.esi - 0x8DC)].c_str();
        }
    });
    
    chara_select_hook_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x1fe02d, [](SafetyHookContext& ctx)
    {
        if (ctx.eax >= static_cast<uintptr_t>(CharacterID::Justice)) ctx.eax = 0;
    });

    chara_select_hook_2_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x1F97AE, [](SafetyHookContext& ctx)
    {
        if (ctx.edi >= static_cast<uintptr_t>(CharacterID::Justice)) ctx.edi = 0;
    });

    chara_select_hook_3_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x1f98bd, [](SafetyHookContext& ctx)
    {
        if (ctx.eax >= static_cast<uintptr_t>(CharacterID::Justice)) ctx.eax = 0;
    });

    set_game_version_hook_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x117a08, [](SafetyHookContext& ctx)
    {
        if (ctx.eax == 1) ctx.eax = 2;
    });
    
    load_dlc_hook_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x36ecc3, [](SafetyHookContext& ctx)
    {
        if (*reinterpret_cast<int*>(ctx.ebp + 8) == 1) *reinterpret_cast<int*>(ctx.ebp + 8) = 2;
    });

    obj_file_link_hook_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x1bfbf9, [](SafetyHookContext& ctx)
    {
        if (ctx.eax >= static_cast<uintptr_t>(CharacterID::Justice))
        {
            if (ctx.eax - static_cast<uint32_t>(CharacterID::Justice) >= obj_ids.size()) return;
            auto result = obj_ids[ctx.eax - static_cast<uint32_t>(CharacterID::Justice)];

            if (result == nullptr) return;

            ctx.edx = *static_cast<uintptr_t*>(result);
        }
    });

    input_check_hook_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x38f6df, [](SafetyHookContext& ctx)
    {
        const auto offset = reinterpret_cast<CHARACTER_WORK*>(ctx.esi);
        if (offset->idno > CharacterID::Justice)
        {
            if (static_cast<uint32_t>(offset->idno) - static_cast<uint32_t>(CharacterID::Justice) > input_check_funcs_.size()) return;
            auto result = input_check_funcs_[static_cast<uint32_t>(offset->idno) - static_cast<uint32_t>(CharacterID::Justice) - 1];

            if (result == nullptr) return;

            ctx.eax = reinterpret_cast<uintptr_t>(result);
        }
    });
    
    taunt_check_hook_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x38e787, [](SafetyHookContext& ctx)
    {
        const auto offset = reinterpret_cast<CHARACTER_WORK*>(ctx.esi);
        if (offset->idno > CharacterID::Justice)
        {
            if (static_cast<uint32_t>(offset->idno) - static_cast<uint32_t>(CharacterID::Justice) > taunt_check_funcs_.size()) return;
            auto result = taunt_check_funcs_[static_cast<uint32_t>(offset->idno) - static_cast<uint32_t>(CharacterID::Justice) - 1];

            if (result == nullptr) return;

            ctx.ecx = reinterpret_cast<uintptr_t>(result);
        }
    });
    
    respect_check_hook_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x38f71d, [](SafetyHookContext& ctx)
    {
        const auto offset = reinterpret_cast<CHARACTER_WORK*>(ctx.esi);
        if (offset->idno > CharacterID::Justice)
        {
            if (static_cast<uint32_t>(offset->idno) - static_cast<uint32_t>(CharacterID::Justice) > respect_check_funcs_.size()) return;
            auto result = respect_check_funcs_[static_cast<uint32_t>(offset->idno) - static_cast<uint32_t>(CharacterID::Justice) - 1];

            if (result == nullptr) return;

            ctx.eax = reinterpret_cast<uintptr_t>(result);
        }
    });

    special_attack_check_hook_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x38f778, [](SafetyHookContext& ctx)
    {
        const auto offset = reinterpret_cast<CHARACTER_WORK*>(ctx.esi);
        if (offset->idno > CharacterID::Justice)
        {
            if (static_cast<uint32_t>(offset->idno) - static_cast<uint32_t>(CharacterID::Justice) > special_attack_check_funcs_.size()) return;
            auto result = special_attack_check_funcs_[static_cast<uint32_t>(offset->idno) - static_cast<uint32_t>(CharacterID::Justice) - 1];

            if (result == nullptr) return;

            ctx.eax = reinterpret_cast<uintptr_t>(result);
        }
    });

    player_action_break_hook_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x139444, [](SafetyHookContext& ctx)
    {
        if (ctx.eax > 0x2c) ctx.eip = reinterpret_cast<uintptr_t>(base) + 0x139576;
        else
        {
            if (ctx.eax > 0x18) ctx.eip = reinterpret_cast<uintptr_t>(base) + 0x139458;
            else ctx.eip = reinterpret_cast<uintptr_t>(base) + 0x13944a;
        }
    });

    char_pos_collision_execute_hook_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x14bcf1, [](SafetyHookContext& ctx)
    {
        if (static_cast<CharacterID>(ctx.eax) > CharacterID::Justice)
        {
            if (ctx.eax - static_cast<uint32_t>(CharacterID::Justice) > push_collis_.size()) return;
            ctx.ebx = push_collis_[ctx.eax - static_cast<uint32_t>(CharacterID::Justice) - 1].stand_width;
        }
    });

    char_pos_collision_execute_hook_2_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x14bdc5, [](SafetyHookContext& ctx)
    {
        if (static_cast<CharacterID>(ctx.eax) > CharacterID::Justice)
        {
            if (ctx.eax - static_cast<uint32_t>(CharacterID::Justice) > push_collis_.size()) return;
            ctx.ebx = push_collis_[ctx.eax - static_cast<uint32_t>(CharacterID::Justice) - 1].stand_width;
        }
    });

    char_pos_collision_execute_hook_3_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x14bd67, [](SafetyHookContext& ctx)
    {
        auto idno = ctx.eax / 2;
        if (static_cast<CharacterID>(idno) > CharacterID::Justice)
        {
            if (idno - static_cast<uint32_t>(CharacterID::Justice) > push_collis_.size()) return;
            ctx.eax = push_collis_[idno - static_cast<uint32_t>(CharacterID::Justice) - 1].sit_height;
            ctx.eip = reinterpret_cast<uintptr_t>(base) + 0x14bd6e;
        }
    });

    char_pos_collision_execute_hook_4_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x14be3a, [](SafetyHookContext& ctx)
    {
        auto idno = ctx.eax / 2;
        if (static_cast<CharacterID>(idno) > CharacterID::Justice)
        {
            if (idno - static_cast<uint32_t>(CharacterID::Justice) > push_collis_.size()) return;
            ctx.eax = push_collis_[idno - static_cast<uint32_t>(CharacterID::Justice) - 1].sit_height;
            ctx.eip = reinterpret_cast<uintptr_t>(base) + 0x14be41;
        }
    });

    char_pos_collision_execute_hook_5_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x14bd8f, [](SafetyHookContext& ctx)
    {
        auto idno = ctx.eax / 2;
        if (static_cast<CharacterID>(idno) > CharacterID::Justice)
        {
            if (idno - static_cast<uint32_t>(CharacterID::Justice) > push_collis_.size()) return;
            ctx.eax = push_collis_[idno - static_cast<uint32_t>(CharacterID::Justice) - 1].stand_height;
            ctx.eip = reinterpret_cast<uintptr_t>(base) + 0x14bd96; 
        }
    });

    char_pos_collision_execute_hook_6_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x14be62, [](SafetyHookContext& ctx)
    {
        auto idno = ctx.eax / 2;
        if (static_cast<CharacterID>(idno) > CharacterID::Justice)
        {
            if (idno - static_cast<uint32_t>(CharacterID::Justice) > push_collis_.size()) return;
            ctx.eax = push_collis_[idno - static_cast<uint32_t>(CharacterID::Justice) - 1].stand_height;
            ctx.eip = reinterpret_cast<uintptr_t>(base) + 0x14be69; 
        }
    });

    char_pos_collision_execute_hook_7_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x14bd09, [](SafetyHookContext& ctx)
    {
        auto idno = ctx.eax / 2;
        if (static_cast<CharacterID>(idno) > CharacterID::Justice)
        {
            if (idno - static_cast<uint32_t>(CharacterID::Justice) > push_collis_.size()) return;
            ctx.eax = push_collis_[idno - static_cast<uint32_t>(CharacterID::Justice) - 1].sky_base_height;
            ctx.eip = reinterpret_cast<uintptr_t>(base) + 0x14bd10;
        }
    });

    char_pos_collision_execute_hook_8_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x14bddd, [](SafetyHookContext& ctx)
    {
        auto idno = ctx.eax / 2;
        if (static_cast<CharacterID>(idno) > CharacterID::Justice)
        {
            if (idno - static_cast<uint32_t>(CharacterID::Justice) > push_collis_.size()) return;
            ctx.eax = push_collis_[idno - static_cast<uint32_t>(CharacterID::Justice) - 1].sky_base_height;
            ctx.eip = reinterpret_cast<uintptr_t>(base) + 0x14bde4; 
        }
    });

    char_pos_collision_execute_hook_9_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x14bd65, [](SafetyHookContext& ctx)
    {
        auto idno = ctx.eax;
        if (static_cast<CharacterID>(idno) > CharacterID::Justice)
        {
            if (idno - static_cast<uint32_t>(CharacterID::Justice) > push_collis_.size()) return;
            ctx.edx = push_collis_[idno - static_cast<uint32_t>(CharacterID::Justice) - 1].sit_width;
        }
    });

    char_pos_collision_execute_hook_10_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x14be38, [](SafetyHookContext& ctx)
    {
        auto idno = ctx.eax;
        if (static_cast<CharacterID>(idno) > CharacterID::Justice)
        {
            if (idno - static_cast<uint32_t>(CharacterID::Justice) > push_collis_.size()) return;
            ctx.edx = push_collis_[idno - static_cast<uint32_t>(CharacterID::Justice) - 1].sit_width;
        }
    });

    char_pos_collision_execute_hook_11_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x14bd81, [](SafetyHookContext& ctx)
    {
        auto idno = ctx.eax;
        if (static_cast<CharacterID>(idno) > CharacterID::Justice)
        {
            if (idno - static_cast<uint32_t>(CharacterID::Justice) > push_collis_.size()) return;
            ctx.edx = push_collis_[idno - static_cast<uint32_t>(CharacterID::Justice) - 1].sky_width;
        }
    });

    char_pos_collision_execute_hook_12_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x14be54, [](SafetyHookContext& ctx)
    {
        auto idno = ctx.eax;
        if (static_cast<CharacterID>(idno) > CharacterID::Justice)
        {
            if (idno - static_cast<uint32_t>(CharacterID::Justice) > push_collis_.size()) return;
            ctx.edx = push_collis_[idno - static_cast<uint32_t>(CharacterID::Justice) - 1].sky_width;
        }
    });

    char_pos_collision_execute_hook_13_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x14bd04, [](SafetyHookContext& ctx)
    {
        auto idno = ctx.eax / 2;
        if (static_cast<CharacterID>(idno) > CharacterID::Justice)
        {
            if (idno - static_cast<uint32_t>(CharacterID::Justice) > push_collis_.size()) return;
            ctx.ebx = push_collis_[idno - static_cast<uint32_t>(CharacterID::Justice) - 1].sky_height;
        }
    });

    char_pos_collision_execute_hook_14_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x14bdd8, [](SafetyHookContext& ctx)
    {
        auto idno = ctx.eax / 2;
        if (static_cast<CharacterID>(idno) > CharacterID::Justice)
        {
            if (idno - static_cast<uint32_t>(CharacterID::Justice) > push_collis_.size()) return;
            ctx.ebx = push_collis_[idno - static_cast<uint32_t>(CharacterID::Justice) - 1].sky_height;
        }
    });

    normal_attack_disable_table_hook_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x2a9ffe, [](SafetyHookContext& ctx)
    {
        if (static_cast<CharacterID>(ctx.edx) > CharacterID::Justice)
        {
            if (ctx.edx - static_cast<uint32_t>(CharacterID::Justice) > normal_attack_disables_.size()) return;
            ctx.edx = normal_attack_disables_[ctx.edx - static_cast<uint32_t>(CharacterID::Justice) - 1];
            ctx.eip = reinterpret_cast<uintptr_t>(base) + 0x2aa005;
        }
    });

    normal_attack_disable_table_hook_2_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x2aa081, [](SafetyHookContext& ctx)
    {
        if (static_cast<CharacterID>(ctx.edx) > CharacterID::Justice)
        {
            if (ctx.edx - static_cast<uint32_t>(CharacterID::Justice) > normal_attack_disables_.size()) return;
            ctx.ecx = normal_attack_disables_[ctx.edx - static_cast<uint32_t>(CharacterID::Justice) - 1];
        }
    });

    /*cmn_attack_check_hook_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x38fe8e, [](SafetyHookContext& ctx)
    {
        if (static_cast<CharacterID>(ctx.ecx) > CharacterID::Justice)
        {
            if (ctx.ecx - static_cast<uint32_t>(CharacterID::Justice) > near_slash_dists_.size()) return;
            ctx.edx = near_slash_dists_[ctx.ecx - static_cast<uint32_t>(CharacterID::Justice) - 1];
        }
    });*/
}

auto GGFramework::register_act_tb(void* act_tb) -> void
{
    act_tbs.push_back(act_tb);
}

auto GGFramework::register_obj_id(void* obj_id) -> void
{
    obj_ids.push_back(obj_id);
}

auto GGFramework::register_chara_id(const std::string& id) -> void
{
    chara_paths_.push_back(std::string("obj/") + id + ".bin");
}

auto GGFramework::register_input_check_func(int32_t(* func)(CHARACTER_WORK*)) -> void
{
    input_check_funcs_.push_back(func);
}

auto GGFramework::register_taunt_check_func(int32_t(* func)(CHARACTER_WORK*)) -> void
{
    taunt_check_funcs_.push_back(func);
}

auto GGFramework::register_respect_check_func(int32_t(* func)(CHARACTER_WORK*)) -> void
{
    respect_check_funcs_.push_back(func);
}

auto GGFramework::register_special_attack_check_func(int32_t(* func)(CHARACTER_WORK*)) -> void
{
    special_attack_check_funcs_.push_back(func);    
}

auto GGFramework::register_push_colli(const PushColli& push_colli) -> void
{
    push_collis_.push_back(push_colli);    
}

auto GGFramework::register_normal_attack_disable(uint32_t disable) -> void
{
    normal_attack_disables_.push_back(disable); 
}

auto GGFramework::register_near_slash_dist(uint16_t dist) -> void
{
    near_slash_dists_.push_back(dist); 
}
