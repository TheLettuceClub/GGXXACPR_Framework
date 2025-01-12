#pragma once
#include <string>
#include "GG.h"
#include "json.hpp"

struct Inputs {
	//TODO fill in
	bool up = false;
	bool down = false;
	bool towards = false; //yes, acpr actually has side-relative inputs, trust me
	bool away = false;
	bool p = false;
	bool k = false;
	bool s = false;
	bool h = false;
	bool d = false;
	bool respect = false;
	bool pk = false;
	bool pd = false;
	bool pks = false;
	bool pksh = false;
};

Inputs inputFiller(char, char);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Inputs, up, down, towards, away, p, k, s, h, d, respect, pk, pd, pks, pksh)

struct Seals { //TODO: actually assign to these vars later
	int jump{};
	int arts{};
	int dash{};
	int punch{};
	int kick{};
	int slash{};
	int hslash{};
	int dust{};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Seals, jump, arts, dash, punch, kick, slash, hslash, dust)

struct PlayerState { //TODO: expand to include more relevant data
	CharacterID CharID{};
	int direction{};
	int health{};
	int tension{};
	int damage{};
	int negativeVal{};
	int commandFlag{};
	int stun1{};
	int stun2{};
	int tensionBalance{};
	//Seals seal{};
	int CleanHitCount{};
	int hitCount{};
	int RISC{};
	int hitLevel{};
	int posx{};
	int posy{};
	Inputs inputs{};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PlayerState, CharID, direction, health, tension, damage, negativeVal, commandFlag, stun1, stun2, tensionBalance, CleanHitCount, hitCount, RISC, hitLevel, posx, posy, inputs)

struct StateUpdate {
	PlayerState p1{};
	PlayerState p2{};
	unsigned long int frameCount{};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(StateUpdate, p1, p2, frameCount)

struct RoundEndEvent {
	int whoWon{};
	unsigned long int frameCount{};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RoundEndEvent, whoWon, frameCount)

NLOHMANN_JSON_SERIALIZE_ENUM(CharacterID, {
										   {CharacterID::Dummy_Random, "Dummy_Random"},
										   {CharacterID::Sol, "Sol"},
										   {CharacterID::Ky, "Ky"},
										   {CharacterID::May, "May"},
										   {CharacterID::Millia, "Millia"},
										   {CharacterID::Axl, "Axl"},
										   {CharacterID::Potemkin, "Potemkin"},
										   {CharacterID::Chipp, "Chipp"},
										   {CharacterID::Eddie, "Eddie"},
										   {CharacterID::Baiken, "Baiken"},
										   {CharacterID::Faust, "Faust"},
										   {CharacterID::Testament, "Testament"},
										   {CharacterID::Jam, "Jam"},
										   {CharacterID::Anji, "Anji"},
										   {CharacterID::Johnny, "Johnny"},
										   {CharacterID::Venom, "Venom"},
										   {CharacterID::Dizzy, "Dizzy"},
										   {CharacterID::Slayer, "Slayer"},
										   {CharacterID::INo, "I-No"},
										   {CharacterID::Zappa, "Zappa"},
										   {CharacterID::Bridget, "Bridget"},
										   {CharacterID::RoboKy, "Robo-Ky"},
										   {CharacterID::ABA, "A.B.A"},
										   {CharacterID::Kliff, "Kliff"},
										   {CharacterID::Justice, "Justice"},
	})