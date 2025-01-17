﻿#include "GG.h"
#include "GGFramework.h"
#include "pch.h"
#include "SAMMITypes.hpp"
#include "WebSockets.hpp"
#include <signal.h>

GGFramework* GGFramework::instance_ = nullptr;
std::mutex GGFramework::mtx_{};
using json = nlohmann::json;

HMODULE base;
unsigned long int frameCounter = 0;
std::thread messageHandler;
worker_t worker{};
bool bRoundStarted = false;
bool bRoundEnded = false;
bool bInputHookLockout = false;
const CHARACTER_WORK* p1 = nullptr;
const CHARACTER_WORK* p2 = nullptr;
const char* p1DInputs = nullptr;
const char* p1PInputs = nullptr;
const char* p2DInputs = nullptr;
const char* p2PInputs = nullptr;
const int* camXCenter = nullptr;
const int* camBottomEdge = nullptr;
const int* camLeftEdge = nullptr;
const int* camWidth = nullptr;
const int* camHeight = nullptr;
const float* camZoom = nullptr;
int p1PrevActNo = 0;
int p2PrevActNo = 0;

void initalizeWSServer() {
	worker.thread = std::make_shared<std::thread>([]() {
		/* create uWebSocket worker and capture uWS::Loop, uWS::App objects. */
		worker.work();
		});
	std::cout << "ACPRSAMMI: WS: server started" << std::endl;
}

void sendEvent(std::string eventName, std::string customData) {
	std::string body = std::format("{{\"event\": \"{}\", \"eventInfo\": {}}}", eventName, customData);
	//send to all opened clients
	worker.loop->defer([body]() {
		worker.app->publish("broadcast", body, uWS::TEXT);
		});
}

void SignalHandler(int signal) {
	std::cout << "Signal: " << signal << std::endl;
	throw "!Access Vioation!";
}

void MessageHandler()
{
	std::cout << "Message Handler running" << std::endl;
	signal(SIGSEGV, SignalHandler);
	while (true)
	{
		if (bRoundStarted && p1 && p2) {
			StateUpdate newState{};
			//maybe add camera struct later
			try {
				newState.frameCount = frameCounter;
				newState.cam.camHeight = *camHeight;
				newState.cam.camLeftEdge = *camLeftEdge;
				newState.cam.camWidth = *camWidth;
				newState.cam.camBottomEdge = *camBottomEdge;
				newState.cam.camXCenter = *camXCenter;
				newState.cam.camZoom = *camZoom;

				newState.p1.health = p1->HitPoint;
				newState.p1.CharID = p1->idno;
				newState.p1.direction = p1->dirflag;
				newState.p1.negativeVal = p1->ply->NegativeVal;
				newState.p1.stun1 = p1->ply->FaintTime;
				newState.p1.stun2 = p1->ply->FaintPoint;
				newState.p1.tension = p1->ply->Tension;
				newState.p1.tensionBalance = p1->ply->TensionBalance;
				newState.p1.commandFlag = p1->actno;
				newState.p1.RISC = p1->ply->GuardPoint;
				newState.p1.hitLevel = p1->ActHeader.lvflag;
				newState.p1.posx = p1->posx;
				newState.p1.posy = p1->posy;
				newState.p1.inputs = inputFiller(*p1PInputs, *p1DInputs);
				newState.p1.seals = sealFiller(p1->ply);
				newState.p1.guard.FaultlessDefenceDisableTime = p1->ply->FaultlessDefenceDisableTime;
				newState.p1.guard.guardSt = p1->GuardSt;
				newState.p1.guard.JGWhiteTime = p1->ply->JGWhiteTime;
				newState.p1.guard.JustFD_ColorCounter = p1->ply->JustFD_ColorCounter;
				newState.p1.guard.JustFD_EasyCounter = p1->ply->JustFD_EasyCounter;
				newState.p1.guard.JustFD_EnableCounter = p1->ply->JustFD_EnableCounter;
				newState.p1.guard.JustFD_Flag = p1->ply->JustFD_Flag;
				newState.p1.guard.JustFD_GuardDisableCounter = p1->ply->JustFD_GuardDisableCounter;
				newState.p1.guard.JustFD_ReversalIgnoreTimer = p1->ply->JustFD_ReversalIgnoreTimer;
				newState.p1.guard.JustGuardIgnoreTime = p1->ply->JustGuardIgnoreTime;
				newState.p1.guard.JustGuardTime = p1->ply->JustGuardTime;
				newState.p1.guard.notThrowTime = p1->ply->notThrowTime;
				newState.p1.invincibleTime = p1->ply->MutekiTime;
				newState.p1.tensionPenaltyTime = p1->ply->RomanCancelTime;

				//p2
				newState.p2.health = p2->HitPoint;
				newState.p2.CharID = p2->idno;
				newState.p2.direction = p2->dirflag;
				newState.p2.negativeVal = p2->ply->NegativeVal;
				newState.p2.stun1 = p2->ply->FaintTime;
				newState.p2.stun2 = p2->ply->FaintPoint;
				newState.p2.tension = p2->ply->Tension;
				newState.p2.tensionBalance = p2->ply->TensionBalance;
				newState.p2.commandFlag = p2->actno;
				newState.p2.RISC = p2->ply->GuardPoint;
				newState.p2.hitLevel = p2->ActHeader.lvflag;
				newState.p2.posx = p2->posx;
				newState.p2.posy = p2->posy;
				newState.p2.inputs = inputFiller(*p2PInputs, *p2DInputs);
				newState.p2.seals = sealFiller(p2->ply);
				newState.p2.guard.FaultlessDefenceDisableTime = p2->ply->FaultlessDefenceDisableTime;
				newState.p2.guard.guardSt = p2->GuardSt;
				newState.p2.guard.JGWhiteTime = p2->ply->JGWhiteTime;
				newState.p2.guard.JustFD_ColorCounter = p2->ply->JustFD_ColorCounter;
				newState.p2.guard.JustFD_EasyCounter = p2->ply->JustFD_EasyCounter;
				newState.p2.guard.JustFD_EnableCounter = p2->ply->JustFD_EnableCounter;
				newState.p2.guard.JustFD_Flag = p2->ply->JustFD_Flag;
				newState.p2.guard.JustFD_GuardDisableCounter = p2->ply->JustFD_GuardDisableCounter;
				newState.p2.guard.JustFD_ReversalIgnoreTimer = p2->ply->JustFD_ReversalIgnoreTimer;
				newState.p2.guard.JustGuardIgnoreTime = p2->ply->JustGuardIgnoreTime;
				newState.p2.guard.JustGuardTime = p2->ply->JustGuardTime;
				newState.p2.guard.notThrowTime = p2->ply->notThrowTime;
				newState.p2.invincibleTime = p2->ply->MutekiTime;
				newState.p2.tensionPenaltyTime = p2->ply->RomanCancelTime;

				json j = newState;
				std::thread(sendEvent, "ggxx_stateUpdate", j.dump()).detach();
				frameCounter++;
				p1PrevActNo = p1->actno;
				p2PrevActNo = p2->actno;
			}
			catch (...) {
				std::cerr << "Message Handler caught error" << std::endl;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
	}
}

void hook_InputCheck(SafetyHookContext& ctx) {
	if (!bInputHookLockout) {
		//ctx.esi is ptr to one of the cw structs
		const CHARACTER_WORK* offset = reinterpret_cast<CHARACTER_WORK*>(ctx.esi);
		const CHARACTER_WORK* other;
		if (offset->padid == 0) {
			other = offset + 1;
			p1 = offset;
			p2 = other;
		}
		else if (offset->padid == 1) {
			other = offset - 1;
			p1 = other;
			p2 = offset;
		}
		bRoundStarted = true;
	}
}

void hook_RoundStart(SafetyHookContext& ctx) {
	bRoundStarted = true;
	bRoundEnded = false;
	bInputHookLockout = false;
	frameCounter = 0;
	p1DInputs = reinterpret_cast<char*>(base) + 0x6D0E80; // now functional
	p1PInputs = reinterpret_cast<char*>(base) + 0x6D0E81;
	p2DInputs = reinterpret_cast<char*>(base) + 0x6D0F18;
	p2PInputs = reinterpret_cast<char*>(base) + 0x6D0F19;

	camXCenter = (int*)(reinterpret_cast<char*>(base) + 0x6D5CE4);
	camBottomEdge = (int*)(reinterpret_cast<char*>(base) + 0x6D5CE8);
	camLeftEdge = (int*)(reinterpret_cast<char*>(base) + 0x6D5CF4);
	camWidth = (int*)(reinterpret_cast<char*>(base) + 0x6D5CFC);
	camHeight = (int*)(reinterpret_cast<char*>(base) + 0x6D5D00);
	camZoom = (float*)(reinterpret_cast<char*>(base) + 0x6D5D18);
	std::thread(sendEvent, "ggxx_roundStartEvent", "{}").detach();
}

void hook_RoundEndSkip(SafetyHookContext& ctx) { //might not get called on time up
	bRoundStarted = false;
	bInputHookLockout = true;
	if (!bRoundEnded) {
		//eax at this point is ptr to one of the player entries, seemingly the loser
		const auto loser = reinterpret_cast<CHARACTER_WORK*>(ctx.eax); 
		RoundEndEvent ree{};
		if (loser->padid == 0) {
			ree.whoWon = 2; //2 meaning player 2 (ofc)
		}
		else if (loser->padid == 1) {
			ree.whoWon = 1;
		}
		else {
			ree.whoWon = 0;
		}
		ree.frameCount = frameCounter;
		json j = ree;
		std::thread(sendEvent, "ggxx_roundEndEvent", j.dump()).detach();
	}
	bRoundEnded = true;
}

void hook_GameModeChange(SafetyHookContext& ctx) { // kind of a baid-aid for the crashed after properly leaving training mode bug
	bRoundStarted = false;
	bRoundEnded = true;
	bInputHookLockout = true;
}

void hook_CharaSelect(SafetyHookContext& ctx) { // executes a bunch of times on any charaselect
	bRoundStarted = false;
	bRoundEnded = false;
	bInputHookLockout = false;
}

void hook_NmlAtk(SafetyHookContext& ctx) { //TODO: few of these values are set properly when the hook is called, replace with other one or later addr in same func?
	// at this point esi points to the character who pressed's CW, and eax to their PE
	const CHARACTER_WORK* offset = reinterpret_cast<CHARACTER_WORK*>(ctx.esi);
	HitEvent he{};
	he.frameCount = frameCounter;
	if (offset->padid == 0) {
		// then p1 got hit
		he.attackerActNo = p2->actno;
		he.CleanHitCount = p2->ply->CleanHit_count;
		he.damage = p2->ActHeader.damage;
		he.defenderActNo = p1->actno;
		he.defenderPrevActNo = p1PrevActNo;
		he.hitCount = p2->ply->HitCount;
		he.initialProration = p1->ply->BaseComboDamage;
		he.idno = p2->idno;
	}
	else if (offset->padid == 1) {
		// then p2 got hit
		he.attackerActNo = p1->actno;
		he.CleanHitCount = p1->ply->CleanHit_count;
		he.damage = p1->ActHeader.damage;
		he.defenderActNo = p2->actno;
		he.defenderPrevActNo = p2PrevActNo;
		he.hitCount = p1->ply->HitCount;
		he.initialProration = p2->ply->BaseComboDamage;
		he.idno = p1->idno;
	}
	json j = he;
	std::thread(sendEvent, "ggxx_hitEvent", j.dump()).detach();
	
}

void hook_KD(SafetyHookContext& ctx) { //TODO change to be later in same func as downtimer hasn't been initalized yet
	// at this point esi and edi are CW pointer to the character getting knocked down
	// also eax is ptr to that chars PE too
	const CHARACTER_WORK* offset = reinterpret_cast<CHARACTER_WORK*>(ctx.esi);
	KnockDownEvent kde{};
	kde.frameCount = frameCounter;
	if (offset->padid == 0) {
		// then p1 got hit
		kde.attackerActNo = p2->actno;
		kde.defenderActNo = p1->actno;
		kde.defenderPrevActNo = p1PrevActNo;
		kde.idno = p2->idno;
		kde.downTimer = p1->ply->DownTimer;
		kde.downFlag = p1->ply->DownFlag;
	}
	else if (offset->padid == 1) {
		// then p2 got hit
		kde.attackerActNo = p1->actno;
		kde.defenderActNo = p2->actno;
		kde.defenderPrevActNo = p2PrevActNo;
		kde.idno = p1->idno;
		kde.downTimer = p2->ply->DownTimer;
		kde.downFlag = p2->ply->DownFlag;
	}
	json j = kde;
	std::thread(sendEvent, "ggxx_KnockDownEvent", j.dump()).detach();
}

auto GGFramework::initialize() -> void
{
	base = GetModuleHandle(NULL);
	input_check_hook_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x38F6DF, hook_InputCheck);
	RoundInitHook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x1D1360, hook_RoundStart);
	RoundEndHook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x1D4AE2, hook_RoundEndSkip);
	GameModeHook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x1c2be9, hook_GameModeChange);
	chara_select_hook_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x1fe02d, hook_CharaSelect);
	chara_select_hook_2_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x1F97AE, hook_CharaSelect);
	chara_select_hook_3_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x1f98bd, hook_CharaSelect);
	NormalAttackHook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x2AD598, hook_NmlAtk); //could also be 2b2728 (eax&bx ptr attacker)
	KnockDownHook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x1f4658, hook_KD);
}


auto GGFramework::get_instance() -> GGFramework*
{
	if (instance_ == nullptr)
	{
		std::lock_guard lock(mtx_);
		if (instance_ == nullptr)
		{
			instance_ = new GGFramework();
			initalizeWSServer();
			std::cout << "Starting message handler" << std::endl;
			std::thread(MessageHandler).detach();
		}
	}
	return instance_;
}
