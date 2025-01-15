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
//json updateMessage{};
std::thread messageHandler;
worker_t worker{};
bool bRoundStarted = false;
bool bRoundEnded = false;
const CHARACTER_WORK* p1 = nullptr;
const CHARACTER_WORK* p2 = nullptr;
const char* p1DInputs = nullptr;
const char* p1PInputs = nullptr;
const char* p2DInputs = nullptr;
const char* p2PInputs = nullptr;

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
	std::cout << "Message Handler running" << std::endl;
	while (true)
	{
		if (bRoundStarted && p1 && p2) {
			StateUpdate newState{};
			//maybe add camera struct later
			try {
				newState.frameCount = frameCounter;
				newState.p1.health = p1->HitPoint;
				newState.p1.CharID = p1->idno;
				newState.p1.CleanHitCount = p1->ply->CleanHit_count;
				newState.p1.damage = p1->ActHeader.damage; //according to ggxp-kor, true damage is on ActHeader, not DamActheader confirmed
				newState.p1.direction = p1->dirflag;
				newState.p1.hitCount = p2->ply->HitCount; //hit count is actually stored in this variable on the other player
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

				//p2
				newState.p2.health = p2->HitPoint;
				newState.p2.CharID = p2->idno;
				newState.p2.CleanHitCount = p2->ply->CleanHit_count;
				newState.p2.damage = p2->ActHeader.damage;
				newState.p2.direction = p2->dirflag;
				newState.p2.hitCount = p1->ply->HitCount;
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
			}
			catch (std::exception e) {
				std::cerr << "Message Handler caught error: " << e.what() << std::endl;
			}
			json j = newState;
			std::thread(sendEvent, "ggxx_stateUpdate", j.dump()).detach();
			frameCounter++;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
	}
}

void hook_InputCheck(SafetyHookContext& ctx) {
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

void hook_RoundStart(SafetyHookContext& ctx) {
	bRoundStarted = true;
	bRoundEnded = false;
	frameCounter = 0;
	p1DInputs = reinterpret_cast<char*>(base) + 0x6D0E80; // now functional
	p1PInputs = reinterpret_cast<char*>(base) + 0x6D0E81;
	p2DInputs = reinterpret_cast<char*>(base) + 0x6D0F18;
	p2PInputs = reinterpret_cast<char*>(base) + 0x6D0F19;
	std::thread(sendEvent, "ggxx_roundStartEvent", "{}").detach();
}

void hook_RoundEndSkip(SafetyHookContext& ctx) { //might not get called on time up
	bRoundStarted = false;
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
}

void hook_CharaSelect(SafetyHookContext& ctx) { // executes a bunch of times on any charaselect
	bRoundStarted = false;
	bRoundEnded = false;
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
