#include "GG.h"
#include "GGFramework.h"
#include "pch.h"
#include "SAMMITypes.hpp"
#include "WebSockets.hpp"
#include <iostream>

GGFramework* GGFramework::instance_ = nullptr;
std::mutex GGFramework::mtx_{};
using json = nlohmann::json;

HMODULE base;
unsigned long int frameCounter = 0;
json updateMessage{};
std::thread messageHandler;
worker_t worker{};
bool bRoundStarted = false;
bool bRoundEnded = false;

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

void MessageHandler()
{
	while (true)
	{
		if (bRoundStarted) {
			json j = updateMessage;
			sendEvent("ggxx_stateUpdate", j.dump());
			frameCounter++;
			std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
		}
	}
}

void hook_InputCheck(SafetyHookContext& ctx) {
	//printToConsole(L"hook_InputCheck called\r\n");
	const CHARACTER_WORK* offset = reinterpret_cast<CHARACTER_WORK*>(ctx.esi);
	const CHARACTER_WORK* otherPlayer;
	//aquire relevant data, package, send
	StateUpdate newState{};
	newState.frameCount = frameCounter;
	if (offset->padid == 0) {
		// then we've been given player 1, need to get p2
		otherPlayer = offset + 1;
		// now assemble the object and send
		newState.p1.health = offset->HitPoint;
		newState.p1.CharID = offset->idno;
		newState.p1.CleanHitCount = offset->ply->CleanHit_count;
		newState.p1.damage = offset->ActHeader.damage; //according to ggxp-kor, true damage is on ActHeader, not DamActheader confirmed
		newState.p1.direction = offset->dirflag;
		newState.p1.hitCount = offset->ply->HitCount;
		newState.p1.negativeVal = offset->ply->NegativeVal;
		newState.p1.stun1 = offset->ply->FaintTime;
		newState.p1.stun2 = offset->ply->FaintPoint;
		newState.p1.tension = offset->ply->Tension;
		newState.p1.tensionBalance = offset->ply->TensionBalance;
		newState.p1.commandFlag = offset->actno;
		newState.p1.RISC = offset->ply->GuardPoint;
		newState.p1.hitLevel = offset->ActHeader.lvflag;

		//p2
		newState.p2.health = otherPlayer->HitPoint;
		newState.p2.CharID = otherPlayer->idno;
		newState.p2.CleanHitCount = otherPlayer->ply->CleanHit_count;
		newState.p2.damage = otherPlayer->ActHeader.damage;
		newState.p2.direction = otherPlayer->dirflag;
		newState.p2.hitCount = otherPlayer->ply->HitCount;
		newState.p2.negativeVal = otherPlayer->ply->NegativeVal;
		newState.p2.stun1 = otherPlayer->ply->FaintTime;
		newState.p2.stun2 = otherPlayer->ply->FaintPoint;
		newState.p2.tension = otherPlayer->ply->Tension;
		newState.p2.tensionBalance = otherPlayer->ply->TensionBalance;
		newState.p2.commandFlag = otherPlayer->actno;
		newState.p2.RISC = otherPlayer->ply->GuardPoint;
		newState.p2.hitLevel = otherPlayer->ActHeader.lvflag;
	}
	else if (offset->padid == 1) {
		// we've been given p2, find p1
		otherPlayer = offset - 1;
		// now assemble the object and send
		newState.p1.health = otherPlayer->HitPoint;
		newState.p1.CharID = otherPlayer->idno;
		newState.p1.CleanHitCount = otherPlayer->ply->CleanHit_count;
		newState.p1.damage = otherPlayer->ActHeader.damage;
		newState.p1.direction = otherPlayer->dirflag;
		newState.p1.hitCount = otherPlayer->ply->HitCount;
		newState.p1.negativeVal = otherPlayer->ply->NegativeVal;
		newState.p1.stun1 = otherPlayer->ply->FaintTime;
		newState.p1.stun2 = otherPlayer->ply->FaintPoint;
		newState.p1.tension = otherPlayer->ply->Tension;
		newState.p1.tensionBalance = otherPlayer->ply->TensionBalance;
		newState.p1.commandFlag = otherPlayer->actno;
		newState.p1.RISC = otherPlayer->ply->GuardPoint;
		newState.p1.hitLevel = otherPlayer->ActHeader.lvflag;

		//p2
		newState.p2.health = offset->HitPoint;
		newState.p2.CharID = offset->idno;
		newState.p2.CleanHitCount = offset->ply->CleanHit_count;
		newState.p2.damage = offset->ActHeader.damage;
		newState.p2.direction = offset->dirflag;
		newState.p2.hitCount = offset->ply->HitCount;
		newState.p2.negativeVal = offset->ply->NegativeVal;
		newState.p2.stun1 = offset->ply->FaintTime;
		newState.p2.stun2 = offset->ply->FaintPoint;
		newState.p2.tension = offset->ply->Tension;
		newState.p2.tensionBalance = offset->ply->TensionBalance;
		newState.p2.commandFlag = offset->actno;
		newState.p2.RISC = offset->ply->GuardPoint;
		newState.p2.hitLevel = offset->ActHeader.lvflag;
	}
	else {
		// if something bad happens, do nothing
		return;
	}

	//then dump the json into the message sender
	updateMessage = newState;
	bRoundStarted = true;
}

void hook_RoundStart(SafetyHookContext& ctx) {
	bRoundStarted = true;
	bRoundEnded = false;
	frameCounter = 0;
	std::thread(sendEvent, "ggxx_roundStartEvent", "{}").detach();
}

void hook_RoundEndSkip(SafetyHookContext& ctx) { //called multiple times at least when not skipped, doesn't get called on time up
	bRoundStarted = false;
	if (!bRoundEnded) {
		//eax at this point is ptr to one of the player entries, seemingly the loser
		const auto loser = reinterpret_cast<CHARACTER_WORK*>(ctx.eax); 
		RoundEndEvent ree{};
		if (loser->padid == 0) { //TODO: this isn't working, idk why, to debug
			ree.whoWon = 2; //2 meaning player 2 (ofc)
		}
		else if (loser->padid == 1) {
			ree.whoWon = 1;
		}
		else {
			ree.whoWon = 0;
		}
		ree.whoWon = 0;
		ree.frameCount = frameCounter;
		json j = ree;
		std::thread(sendEvent, "ggxx_roundEndEvent", j.dump()).detach();
	}
	bRoundEnded = true;
}

auto GGFramework::initialize() -> void
{
	base = GetModuleHandle(NULL);
	input_check_hook_ = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x38F6DF, hook_InputCheck);
	RoundInitHook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x1D1360, hook_RoundStart);
	RoundEndHook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x1D4AE2, hook_RoundEndSkip);
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
			std::thread(MessageHandler).detach();
		}
	}
	return instance_;
}
