#include "SAMMITypes.hpp"

Inputs inputFiller(char inputFlag, char dirFlag) { //TODO: fix, completely nonworking
	Inputs newIn{};
	if (inputFlag & 0x40) {
		newIn.p = true;
	}
	if (inputFlag & 0x80) {
		newIn.k = true;
	}
	if (inputFlag & 0x10) {
		newIn.s = true;
	}
	if (inputFlag & 0x08) {
		newIn.h = true;
	}
	if (inputFlag & 0x02) {
		newIn.d = true;
	}
	if (inputFlag & 0x20) {
		newIn.respect = true;
	}
	if (inputFlag & 0xC4) {
		newIn.pk = true;
	}
	if (inputFlag & 0x43) {
		newIn.pd = true;
	}
	if (inputFlag & 0xF0) {
		newIn.pks = true;
	}
	if (inputFlag & 0xD8) {
		newIn.pksh = true;
	}
	if (dirFlag & 0x80) {
		newIn.away = true;
	}
	if (dirFlag & 0x40) {
		newIn.down = true;
	}
	if (dirFlag & 0x20) {
		newIn.towards = true;
	}
	if (dirFlag & 0x10) {
		newIn.up = true;
	}
	return newIn;
}

Seals sealFiller(PLAYER_ENTRY* ply) {
	Seals newSe{};
	newSe.arts = ply->ArtsSeal;
	newSe.dash = ply->DashSeal;
	newSe.dust = ply->DustSeal;
	newSe.hslash = ply->HeavySlashSeal;
	newSe.kick = ply->KickSeal;
	newSe.jump = ply->JumpSeal;
	newSe.punch = ply->PunchSeal;
	newSe.slash = ply->SlashSeal;
	return newSe;
}