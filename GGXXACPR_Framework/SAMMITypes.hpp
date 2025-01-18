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

Seals sealFiller(PLAYER_ENTRY*);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Seals, jump, arts, dash, punch, kick, slash, hslash, dust)

struct GuardStuff {
	int guardSt{};
	int FaultlessDefenceDisableTime{};
	int JustFD_EnableCounter{};
	int JustFD_GuardDisableCounter{};
	int JustFD_ColorCounter{};
	int JustFD_ReversalIgnoreTimer{};
	int JustFD_EasyCounter{};
	int JustFD_Flag{};
	int notThrowTime{}; //throw invuln time
	int JustGuardTime{};
	int JustGuardIgnoreTime{};
	int JGWhiteTime{};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GuardStuff, guardSt, FaultlessDefenceDisableTime, JustFD_EnableCounter, JustFD_GuardDisableCounter, JustFD_ColorCounter, JustFD_ReversalIgnoreTimer, JustFD_EasyCounter, JustFD_Flag, notThrowTime, JustGuardTime, JustGuardIgnoreTime, JGWhiteTime)

struct PlayerState {
	CharacterID CharID{};
	int direction{};
	int health{};
	int tension{};
	int negativeVal{};
	int commandFlag{};
	int stun1{};
	int stun2{};
	int tensionBalance{};
	Seals seals{};
	int RISC{};
	int hitLevel{};
	int posx{};
	int posy{};
	Inputs inputs{};
	GuardStuff guard{};
	int invincibleTime{};
	int tensionPenaltyTime{};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PlayerState, invincibleTime, tensionPenaltyTime, CharID, direction, health, tension, negativeVal, commandFlag, stun1, stun2, tensionBalance, RISC, posx, posy, inputs, seals, guard)

struct Camera {
	int camXCenter{};
	int camLeftEdge{};
	int camBottomEdge{};
	int camWidth{};
	int camHeight{};
	float camZoom{};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Camera, camXCenter, camLeftEdge, camWidth, camHeight, camZoom, camBottomEdge)

struct StateUpdate {
	PlayerState p1{};
	PlayerState p2{};
	Camera cam{};
	unsigned long int frameCount{};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(StateUpdate, p1, p2, cam, frameCount)

struct RoundEndEvent {
	int whoWon{};
	unsigned long int frameCount{};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RoundEndEvent, whoWon, frameCount)

struct HitEvent {
	int damage{};
	unsigned long int frameCount{};
	int attackerActNo{};
	int defenderActNo{};
	int defenderPrevActNo{};
	int hitCount{};
	int initialProration{};
	int CleanHitCount{};
	CharacterID idno{};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(HitEvent, idno, damage, frameCount, attackerActNo, defenderPrevActNo, defenderActNo, hitCount, initialProration, CleanHitCount)

struct KnockDownEvent {
	CharacterID idno{};
	int attackerActNo{};
	int defenderActNo{};
	int defenderPrevActNo{};
	int downTimer{};
	int downFlag{};
	unsigned long int frameCount{};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(KnockDownEvent, downFlag, idno, attackerActNo, defenderActNo, defenderPrevActNo, downTimer, frameCount)

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