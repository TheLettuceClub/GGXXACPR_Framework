#pragma once
#include <cstdint>

enum class CharacterID : uint16_t
{
	Dummy_Random = 0x0,
	Sol = 0x1,
	Ky = 0x2,
	May = 0x3,
	Millia = 0x4,
	Axl = 0x5,
	Potemkin = 0x6,
	Chipp = 0x7,
	Eddie = 0x8,
	Baiken = 0x9,
	Faust = 0xa,
	Testament = 0xb,
	Jam = 0xc,
	Anji = 0xd,
	Johnny = 0xe,
	Venom = 0xf,
	Dizzy = 0x10,
	Slayer = 0x11,
	INo = 0x12,
	Zappa = 0x13,
	Bridget = 0x14,
	RoboKy = 0x15,
	ABA = 0x16,
	OrderSol = 0x17,
	Kliff = 0x18,
	Justice = 0x19
};

#pragma pack(push, 1)

union PERSONALWORK
{
	int32_t l;
	int16_t w[0x2];
	char b[0x4];
	uint32_t ul;
	uint16_t uw[0x2];
	uint8_t ub[0x4];
	float f;
};

struct PLAYER_ENTRY
{
	uint16_t Tension;
	char ShakeTime;
	char BackStepInvalid;
	uint16_t BaseComboDamage;
	int16_t NegativeVal;
	char counterredtime;
	char pad_h;
	int16_t pactst;
	int32_t homingtime;
	int32_t inertia;
	int32_t offx;
	uint16_t notThrowTime; //throw invuln time
	int16_t GuardPoint; //RISC bar value
	int16_t GuardTimer; //likely used for IB/SB lockout timers
	uint16_t DownTimer; //amount of frames on ground
	uint8_t HomingJumpDir;
	uint8_t pad_hd;
	uint8_t reqhit;
	uint8_t hitDispTime;
	float hitDispIn;
	uint8_t inertiaflag;
	uint8_t whiteouttime;
	char MutekiTime; //invuln time after air tech
	uint8_t maintain; //unknown, always 0
	uint8_t SkyDashCorrect;
	uint8_t inertiatime;
	uint8_t DownBoundDamage;
	uint8_t TensionPenaltyTime;
	uint16_t RomanCancelTime;
	uint8_t RomanCancelIgnoreTime;
	uint8_t bIgnoreBomberMuteki;
	char TensionHandicap;
	uint8_t NormalAttackDisableFlag;
	uint8_t field33_0x36;
	uint8_t field34_0x37;
	uint8_t field35_0x38;
	uint8_t field36_0x39;
	uint8_t field37_0x3a;
	uint8_t field38_0x3b;
	char TensionMovePenaltyTime;
	char JustGuardTime;
	char JustGuardIgnoreTime;
	char JGWhiteTime;
	void* GetChainDataTableProc;
	void* Enemy;
	uint16_t PunchKey; //these are set to their internal, constant values only when a player has bound that button, zero otherwise
	uint16_t KickKey; // they don't change at all during gameplay, so they're really not useful to me
	uint16_t SlashKey;
	uint16_t HeavySlashKey;
	uint16_t DustKey;
	uint16_t FuckinKey;
	uint16_t PosResetKey;
	uint16_t PauseKey;
	uint16_t RecPlyerKey;
	uint16_t RecEnemyKey;
	uint16_t PlayMemoryKey;
	uint16_t SwitchKey;
	uint16_t EnemyWalkKey;
	uint16_t EnemyJumpKey;
	uint16_t PandKKey;
	uint16_t PandDKey;
	uint16_t PKSKey;
	uint16_t PKSHSKey;
	int16_t lifelength;
	int16_t lifelengthdamage;
	char JumpCount;
	char SkyDashCount;
	uint16_t JumpFlag;
	uint32_t chainflag;
	uint32_t rendaflag;
	uint16_t FaintPoint;
	uint16_t FaintTime;
	void* ExCommandFunc;
	uint8_t JumpDir;
	uint8_t CPU;
	char sousaitime; //unknown, always 0
	uint8_t FaintMax;
	union PERSONALWORK PersonalWork[0x4];
	uint64_t padd;
	uint64_t paddd;
	uint32_t padddd;
	int32_t DownFlag; //either 0 or 16384
	void* Child;
	void* AtkHitInterrupt;
	void* AtkGuardInterrupt;
	void* DamageInterrupt;
	void* LandInterrupt;
	void* ActChangeInterrupt;
	void* GuardFunc;
	void* GetUpInterrupt;
	void* GuardCheckInterrupt;
	void* PosByEnemyAddr;
	uint8_t OverKillFlag;
	uint8_t ObjFlag;
	uint8_t DustTime;
	uint8_t AfterImageIndex;
	void* KeyHook;
	int16_t lifelengthdamagelock;
	int16_t TensionBalance;
	int16_t TensionSideTime;
	int16_t TensionNoTouchTime;
	int32_t lastposx;
	int16_t TensionBarLength;
	uint8_t TShand;
	uint8_t EnemyTouch; //amount of time both players are physically touching on screen
	uint8_t AttackDown;
	uint8_t DefenceDown;
	uint8_t JumpSeal;
	uint8_t ArtsSeal;
	uint8_t Poison;
	uint8_t MistTime;
	int16_t DamageTime; //amount of frames after tech window??
	uint8_t field112_0xf8;
	uint8_t field113_0xf9;
	uint8_t field114_0xfa;
	uint8_t bBomberEnable;
	uint8_t ucBomberIgnoreTime;
	uint8_t bDamageBomber;
	uint16_t HitCount;
	uint16_t DispHitCount;
	uint8_t DashSeal;
	uint8_t AllCounter; //unknown, always 0
	uint8_t FirstHitGuardOff;
	uint8_t PunchSeal;
	uint8_t KickSeal;
	uint8_t SlashSeal;
	uint8_t HeavySlashSeal;
	uint8_t DustSeal;
	uint8_t FaultlessDefenceDisableTime;
	int16_t JustFD_EnableCounter;
	uint8_t JustFD_GuardDisableCounter;
	uint8_t JustFD_ColorCounter;
	uint8_t JustFD_ReversalIgnoreTimer;
	int16_t JustFD_EasyCounter;
	uint8_t JustFD_Flag;
	uint8_t Nagenuke_Flag;
	uint8_t Nagenuke_EnableCounter;
	uint8_t Nagenuke_PrevState;
	int32_t Nagenuke_ColorCounter;
	int32_t shake_y_offset;
	int32_t CleanHit_count;
	int32_t ExSkyControlCount;
	uint8_t field142_0x126;
	uint8_t field143_0x127;
	uint8_t field144_0x128;
	uint8_t field145_0x129;
	uint8_t field146_0x12a;
	uint8_t field147_0x12b;
	uint8_t field148_0x12c;
	uint8_t field149_0x12d;
	uint8_t field150_0x12e;
	uint8_t field151_0x12f;
	uint8_t field152_0x130;
	uint8_t field153_0x131;
	uint8_t field154_0x132;
	uint8_t field155_0x133;
	uint8_t field156_0x134;
	uint8_t field157_0x135;
	uint8_t field158_0x136;
	uint8_t field159_0x137;
	uint8_t field160_0x138;
	uint8_t field161_0x139;
	uint8_t field162_0x13a;
	uint8_t field163_0x13b;
	uint8_t field164_0x13c;
	uint8_t field165_0x13d;
	uint8_t field166_0x13e;
	uint8_t field167_0x13f;
	uint8_t field168_0x140;
	uint8_t field169_0x141;
};

struct COLLISION
{
	int16_t x;
	int16_t y;
	uint16_t w;
	uint16_t h;
	uint32_t attr;
	uint32_t field5_0xc;
};

struct COLLIPOS
{
	int32_t x;
	int32_t y;
};

struct TACTHEADER
{
	uint32_t flag;
	uint16_t lvflag;
	uint8_t damage;
	uint8_t flag2;
};

union ACTWORK
{
	int32_t l;
	int16_t w[0x2];
	char b[0x4];
	uint32_t ul;
	uint16_t uw[0x2];
	uint8_t ub[0x4];
	float f;
	void* p;
};

struct HITSEPARAM
{
	unsigned char no[0x2];
	unsigned char flag[0x2];
};

union HITSEUNION
{
	unsigned int l;
	struct HITSEPARAM param;
};

struct DAMAGEPARAM
{
	unsigned short DownX;
	unsigned short DownY;
	unsigned short DownGrav;
	unsigned short DownUkemiTime;
	unsigned short FaintPoint;
	unsigned short field5_0xa;
	unsigned int DownFlag;
	unsigned short Kezuri;
	unsigned short DamageHosei;
	union HITSEUNION HitSE;
	union HITSEUNION GuardSE;
	unsigned char dno;
	unsigned char dprob;
	unsigned char gno;
	unsigned char gprob;
	unsigned short field15_0x20; //these two could be the void* DamInt
	unsigned short field16_0x22;
	char field17_0x24;
	char field18_0x25;
	char field19_0x26;
	char field20_0x27;
	char field21_0x28;
	char field22_0x29;
	char field23_0x2a;
	char field24_0x2b;
};

struct TACTNORMAL
{
	unsigned char id;
	unsigned char arg1;
	unsigned short arg2;
	unsigned short arg3;
	unsigned short arg4;
	unsigned char arg5;
	unsigned char arg6;
	unsigned char arg7;
};

struct CHARACTER_WORK
{
	enum CharacterID idno;
	uint8_t dirflag;
	uint8_t posdirflag;
	uint32_t back;
	struct CHARACTER_WORK* next;
	uint32_t actst;
	uint32_t actnonext;
	uint32_t actnonexttemp;
	int16_t actno;
	uint16_t Localid; //unknown
	uint16_t ActTimer;
	uint16_t HitPoint;
	struct CHARACTER_WORK* parentWork;
	int16_t actnoSv;
	uint8_t _pad;
	uint8_t padid;
	int16_t parentFlag;
	int16_t GuardSt;
	struct PLAYER_ENTRY* ply;
	void* graphicfunc;
	uint32_t attackst;
	uint32_t CommandFlag;
	uint32_t* CellTop;
	uint16_t CellNo;
	int16_t LastCellNo;
	uint16_t ImageNo;
	int16_t LastImageNo;
	uint32_t** GclTop;
	uint16_t corex;
	uint16_t corey;
	uint16_t scale;
	uint16_t scaleY;
	struct COLLISION* ColliAddr;
	struct COLLISION* ExColliAddr;
	char ColliFlag; //somtimes changes, unclear when or why
	char HitColliFlag; //unknown, always 0
	char DamColliFlag; //unknown, always 0
	uint8_t field37_0x5f;
	uint16_t TensionAddbyHit;
	void* HitColliAddr;
	void* DamColliAddr;
	struct COLLIPOS DamColliPos;
	struct TACTHEADER DamActHeader;
	int16_t Imagew;
	int16_t Imageh;
	char ColliCnt;
	char ColliNo;
	int16_t priority2; //does change when things happen, unclear why
	uint8_t field48_0x82;
	uint8_t field49_0x83;
	uint8_t field50_0x84;
	uint8_t field51_0x85;
	uint8_t field52_0x86;
	uint8_t field53_0x87;
	struct DAMAGEPARAM* HitParam;
	struct DAMAGEPARAM* DamParam;
	void* ColliHitInterrupt;
	void* PaletteProc;
	char PalId;
	char LocalPalette;
	uint8_t PaletteFlag;
	char FadeVal;
	int16_t SpriteIdTop;
	int16_t FBSpriteIdTop;
	int16_t SpriteIdMax;
	int16_t SpriteId;
	int32_t* SphAddr;
	char transmode;
	char transmode2;
	char SpriteIdIdx;
	uint8_t TexSizeMax;
	uint32_t SpriteFlag;
	int32_t posx;
	int32_t posy;
	int32_t xspeed;
	int32_t yspeed;
	int32_t speedsvx;
	int32_t speedsvy;
	int32_t HitBackx;
	int32_t RHitBackx;
	int32_t AdBackx;
	int32_t gravity;
	uint16_t parentx;
	uint16_t parenty;
	uint16_t angle;
	uint16_t priority; //does change, unclear why
	uint32_t* InstTb;
	struct TACTHEADER ActHeader;
	struct TACTHEADER* ActHeaderp;
	struct TACTNORMAL* InstAddr;
	int16_t InstSt;
	int16_t InstVal;
	int16_t AnimeNo;
	uint16_t AnimeTime;
	uint8_t AnimeRate;
	uint8_t HitStopTime;
	uint8_t AnimeTimeMax;
	uint8_t Mark;
	uint8_t ActHitCount;
	uint8_t MaxHitCount;
	uint8_t trans;
	uint8_t trans2;
	uint32_t LandBreakActno;
	union ACTWORK actwork;
	uint16_t transSpeed;
	uint16_t angleSpeed;
	uint32_t scaleSpeed;
	uint32_t scaleYSpeed;
	uint32_t field108_0x118;
	uint32_t field109_0x11c;
	uint32_t BoostArts_Flag;
	int32_t BoostArts_InheritanceActno;
	char var;
	uint8_t field113_0x129;
	uint8_t field114_0x12a;
	uint8_t field115_0x12b;
	uint8_t field116_0x12c;
	uint8_t field117_0x12d;
	uint8_t field118_0x12e;
	uint8_t field119_0x12f;
};

#pragma pack(pop)