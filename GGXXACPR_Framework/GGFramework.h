#pragma once
#include <mutex>

#include "safetyhook.hpp"

struct PushColli
{
    uint16_t stand_width;
    uint16_t stand_height;
    uint16_t sit_width;
    uint16_t sit_height;
    uint16_t sky_width;
    uint16_t sky_height;
    uint16_t sky_base_height;
};

GGFRAMEWORK_API class GGFramework
{
private:
    GGFramework()
    {
        initialize();
    }

    static GGFramework* instance_;
    static std::mutex mtx_;

    SafetyHookMid player_main_hook_;
    SafetyHookMid load_obj_file_hook_;
    SafetyHookMid load_obj_file_hook_2_;
    SafetyHookMid load_obj_file_hook_3_;
    SafetyHookMid load_obj_file_hook_4_;
    SafetyHookMid load_obj_file_hook_5_;
    SafetyHookMid allocate_file_hook_;
    SafetyHookMid allocate_file_hook_2_;
    SafetyHookMid chara_select_hook_;
    SafetyHookMid chara_select_hook_2_;
    SafetyHookMid chara_select_hook_3_;
    SafetyHookMid set_game_version_hook_;
    SafetyHookMid load_dlc_hook_;
    SafetyHookMid obj_file_link_hook_;
    SafetyHookMid input_check_hook_;
    SafetyHookMid taunt_check_hook_;
    SafetyHookMid respect_check_hook_;
    SafetyHookMid special_attack_check_hook_;
    SafetyHookMid player_action_break_hook_;
    SafetyHookMid char_pos_collision_execute_hook_;
    SafetyHookMid char_pos_collision_execute_hook_2_;
    SafetyHookMid char_pos_collision_execute_hook_3_;
    SafetyHookMid char_pos_collision_execute_hook_4_;
    SafetyHookMid char_pos_collision_execute_hook_5_;
    SafetyHookMid char_pos_collision_execute_hook_6_;
    SafetyHookMid char_pos_collision_execute_hook_7_;
    SafetyHookMid char_pos_collision_execute_hook_8_;
    SafetyHookMid char_pos_collision_execute_hook_9_;
    SafetyHookMid char_pos_collision_execute_hook_10_;
    SafetyHookMid char_pos_collision_execute_hook_11_;
    SafetyHookMid char_pos_collision_execute_hook_12_;
    SafetyHookMid char_pos_collision_execute_hook_13_;
    SafetyHookMid char_pos_collision_execute_hook_14_;
    SafetyHookMid normal_attack_disable_table_hook_;
    SafetyHookMid normal_attack_disable_table_hook_2_;
    SafetyHookMid cmn_attack_check_hook_;
    
    static std::vector<void*> act_tbs;
    static std::vector<void*> obj_ids;
    static std::vector<std::string> chara_paths_;
    static std::vector<int32_t(*)(struct CHARACTER_WORK*)> input_check_funcs_;
    static std::vector<int32_t(*)(CHARACTER_WORK*)> taunt_check_funcs_;
    static std::vector<int32_t(*)(CHARACTER_WORK*)> respect_check_funcs_;
    static std::vector<int32_t(*)(CHARACTER_WORK*)> special_attack_check_funcs_;
    static std::vector<PushColli> push_collis_;
    static std::vector<uint32_t> normal_attack_disables_;
    static std::vector<uint16_t> near_slash_dists_;
    static int* game_version_;
    auto initialize() -> void;

public:
    GGFramework(const GGFramework&) = delete;

    static auto get_instance() -> GGFramework*;
    
    static auto register_act_tb(void* act_tb) -> void;
    static auto register_obj_id(void* obj_id) -> void;
    static auto register_chara_id(const std::string& id) -> void;
    static auto register_input_check_func(int32_t (*func)(CHARACTER_WORK*)) -> void;
    static auto register_taunt_check_func(int32_t (*func)(CHARACTER_WORK*)) -> void;
    static auto register_respect_check_func(int32_t (*func)(CHARACTER_WORK*)) -> void;
    static auto register_special_attack_check_func(int32_t (*func)(CHARACTER_WORK*)) -> void;
    static auto register_push_colli(const PushColli& push_colli) -> void;
    static auto register_normal_attack_disable(uint32_t disable) -> void;
    static auto register_near_slash_dist(uint16_t dist) -> void;
};
