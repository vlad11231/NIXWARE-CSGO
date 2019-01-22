#include "csgostructs.hpp"
#include "../Helpers/Math.hpp"
#include "../Helpers/Utils.hpp"

bool C_BaseEntity::IsPlayer()
{
	return CallVFunction<bool(__thiscall*)(C_BaseEntity*)>(this, 153)(this);
}

bool C_BaseEntity::IsLoot() {
	return (GetClientClass()->m_ClassID == CPhysPropAmmoBox ||
		GetClientClass()->m_ClassID == CPhysPropLootCrate ||
		GetClientClass()->m_ClassID == CPhysPropRadarJammer ||
		GetClientClass()->m_ClassID == CPhysPropWeaponUpgrade ||
		GetClientClass()->m_ClassID == CDrone ||
		GetClientClass()->m_ClassID == CDronegun ||
		GetClientClass()->m_ClassID == CItem_Healthshot ||
		GetClientClass()->m_ClassID == CItemCash);
}

bool C_BaseEntity::IsWeapon()
{
	return CallVFunction<bool(__thiscall*)(C_BaseEntity*)>(this, 161)(this);
}

bool C_BaseEntity::IsPlantedC4()
{
	return GetClientClass()->m_ClassID == CPlantedC4;
}

bool C_BaseEntity::IsDefuseKit()
{
	return GetClientClass()->m_ClassID == CBaseAnimating;
}

CCSWeaponInfo* C_BaseCombatWeapon::GetCSWeaponData()
{
	return CallVFunction<CCSWeaponInfo*(__thiscall*)(void*)>(this, 448)(this);
}

bool C_BaseCombatWeapon::HasBullets()
{
	return !IsReloading() && m_iClip1() > 0;
}

bool C_BaseCombatWeapon::CanFire()
{
	static decltype(this) stored_weapon = nullptr;
	static auto stored_tick = 0;
	if (stored_weapon != this || stored_tick >= Globals::LocalPlayer->m_nTickBase()) {
		stored_weapon = this;
		stored_tick = Globals::LocalPlayer->m_nTickBase();
		return false; //cannot shoot first tick after switch
	}

	if (IsReloading() || m_iClip1() <= 0 || !Globals::LocalPlayer)
		return false;

	auto flServerTime = Globals::LocalPlayer->m_nTickBase() * Interfaces::GlobalVars->interval_per_tick;

	return m_flNextPrimaryAttack() <= flServerTime;
}

bool C_BaseCombatWeapon::IsZeus()
{
	if (this->m_Item().m_iItemDefinitionIndex() == WEAPON_ZEUSX27)
	{
		return true;
	}
	return false;
}

bool C_BaseCombatWeapon::IsGrenade()
{
	return GetCSWeaponData()->iWeaponType == WEAPONTYPE_GRENADE;
}

bool C_BaseCombatWeapon::IsGun()
{
	if (!this)
		return false;

	short id = this->m_iItemDefinitionIndex();

	switch (id)
	{
	case WEAPON_DEAGLE:
	case WEAPON_DUALBERETTA:
	case WEAPON_FIVESEVEN:
	case WEAPON_GLOCK:
	case WEAPON_AK47:
	case WEAPON_AUG:
	case WEAPON_AWP:
	case WEAPON_FAMAS:
	case WEAPON_G3SG1:
	case WEAPON_GALIL:
	case WEAPON_M249:
	case WEAPON_M4A4:
	case WEAPON_MAC10:
	case WEAPON_P90:
	case WEAPON_MP5SD:
	case WEAPON_UMP45:
	case WEAPON_XM1014:
	case WEAPON_BIZON:
	case WEAPON_MAG7:
	case WEAPON_NEGEV:
	case WEAPON_SAWEDOFF:
	case WEAPON_TEC9:
	case WEAPON_P2000:
	case WEAPON_MP7:
	case WEAPON_MP9:
	case WEAPON_NOVA:
	case WEAPON_P250:
	case WEAPON_SCAR20:
	case WEAPON_SG553:
	case WEAPON_SSG08:
		return true;
	case WEAPON_ZEUSX27:
	case WEAPON_KNIFE:
	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
	case WEAPON_SMOKEGRENADE:
	case WEAPON_MOLOTOV:
	case WEAPON_DECOY:
	case WEAPON_INC:
	case WEAPON_C4:
	case WEAPON_KNIFE_T:
		return false;
	case WEAPON_M4A1S:
	case WEAPON_USPS:
	case WEAPON_CZ75:
	case WEAPON_REVOLVER:
		return true;
	default:
		return false;
	}
}

bool C_BaseCombatWeapon::IsKnife()
{
	if (this->m_Item().m_iItemDefinitionIndex() == WEAPON_ZEUSX27) return false;
	return GetCSWeaponData()->iWeaponType == WEAPONTYPE_KNIFE;
}

bool C_BaseCombatWeapon::IsAutomaticGun()
{
	switch (GetCSWeaponData()->iWeaponType)
	{
	case WEAPONTYPE_RIFLE:
		return true;
	case WEAPONTYPE_SUBMACHINEGUN:
		return true;
	case WEAPONTYPE_SHOTGUN:
		return true;
	case WEAPONTYPE_MACHINEGUN:
		return true;
	default:
		return false;
	}
}

bool C_BaseCombatWeapon::IsMashineGun()
{
	return GetCSWeaponData()->iWeaponType == WEAPONTYPE_MACHINEGUN;
}

bool C_BaseCombatWeapon::IsRifle()
{
	return GetCSWeaponData()->iWeaponType == WEAPONTYPE_RIFLE;
}

bool C_BaseCombatWeapon::IsSMG()
{
	switch (GetCSWeaponData()->iWeaponType)
	{
	case WEAPONTYPE_SUBMACHINEGUN:
		return true;
	default:
		return false;
	}
}

bool C_BaseCombatWeapon::IsPistol()
{
	switch (GetCSWeaponData()->iWeaponType)
	{
	case WEAPONTYPE_PISTOL:
		return true;
	default:
		return false;
	}
}

bool C_BaseCombatWeapon::IsAuto()
{
	auto id = this->m_iItemDefinitionIndex();
	if (id == WEAPON_G3SG1 || id == WEAPON_SCAR20)
		return true;
	return false;
}

bool C_BaseCombatWeapon::IsShotgun()
{
	auto id = this->m_iItemDefinitionIndex();
	if (id == WEAPON_MAG7 || id == WEAPON_SAWEDOFF || id == WEAPON_XM1014 || id == WEAPON_NOVA)
		return true;
	return false;
}

bool C_BaseCombatWeapon::IsBallistic()
{
	auto id = this->m_iItemDefinitionIndex();
	return (id == WEAPON_AUG || id == WEAPON_SG553);
}

bool C_BaseCombatWeapon::IsSniper()
{
	switch (GetCSWeaponData()->iWeaponType)
	{
	case WEAPONTYPE_SNIPER_RIFLE:
		return true;
	default:
		return false;
	}
}

bool C_BaseCombatWeapon::IsReloading()
{
	static auto inReload = *(uint32_t*)(Utils::PatternScan(GetModuleHandleA(XorStr("client_panorama.dll")), "C6 87 ? ? ? ? ? 8B 06 8B CE FF 90") + 2);
	return *(bool*)((uintptr_t)this + inReload);
}

float C_BaseCombatWeapon::GetInaccuracy()
{
	return CallVFunction<float(__thiscall*)(void*)>(this, 471)(this);
}

float C_BaseCombatWeapon::GetSpread()
{
	return CallVFunction<float(__thiscall*)(void*)>(this, 440)(this);
}

void C_BaseCombatWeapon::UpdateAccuracyPenalty()
{
	CallVFunction<void(__thiscall*)(void*)>(this, 472)(this);
}

CUtlVector<IRefCounted*>& C_BaseCombatWeapon::m_CustomMaterials()
{	static auto inReload = *(uint32_t*)(Utils::PatternScan(GetModuleHandleA(XorStr("client_panorama.dll")), "83 BE ? ? ? ? ? 7F 67") + 2) - 12;
	return *(CUtlVector<IRefCounted*>*)((uintptr_t)this + inReload);
}

bool* C_BaseCombatWeapon::m_bCustomMaterialInitialized()
{
	static auto currentCommand = *(uint32_t*)(Utils::PatternScan(GetModuleHandleA(XorStr("client_panorama.dll")), "C6 86 ? ? ? ? ? FF 50 04") + 2);
	return (bool*)((uintptr_t)this + currentCommand);
}

CUserCmd*& C_BasePlayer::m_pCurrentCommand()
{
	static auto currentCommand = *(uint32_t*)(Utils::PatternScan(GetModuleHandleA(XorStr("client_panorama.dll")), "89 BE ? ? ? ? E8 ? ? ? ? 85 FF") + 2);
	return *(CUserCmd**)((uintptr_t)this + currentCommand);
}

int C_BasePlayer::GetNumAnimOverlays()
{
	return *(int*)((DWORD)this + 0x297C);
}

AnimationLayer *C_BasePlayer::GetAnimOverlays()
{
	// to find offset: use 9/12/17 dll
	// sig: 55 8B EC 51 53 8B 5D 08 33 C0
	return *(AnimationLayer**)((DWORD)this + 10608);
}

AnimationLayer *C_BasePlayer::GetAnimOverlay(int i)
{
	if (i < 15)
		return &GetAnimOverlays()[i];
	return nullptr;
}

int C_BasePlayer::GetSequenceActivity(int sequence)
{
	auto hdr = Interfaces::ModelInfo->GetStudiomodel(this->GetModel());

	if (!hdr)
		return -1;

	// sig for stuidohdr_t version: 53 56 8B F1 8B DA 85 F6 74 55
	// sig for C_BaseAnimating version: 55 8B EC 83 7D 08 FF 56 8B F1 74 3D
	// c_csplayer vfunc 242, follow calls to find the function.

	static auto get_sequence_activity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(Utils::PatternScan(GetModuleHandleA("client_panorama.dll"), "55 8B EC 83 7D 08 FF 56 8B F1 74 3D"));

	return get_sequence_activity(this, hdr, sequence);
}

CBasePlayerAnimState *C_BasePlayer::GetBasePlayerAnimState()
{
	static int basePlayerAnimStateOffset = 0x3870;
	return *(CBasePlayerAnimState**)((DWORD)this + basePlayerAnimStateOffset);
}

bool C_BasePlayer::IsBehindSmoke()
{
	return false;
}

bool C_BasePlayer::IsTeammate()
{
	return !Utils::IsInDangerZone() && this != Globals::LocalPlayer && this->m_iTeamNum() == Globals::LocalPlayer->m_iTeamNum();
}

bool C_BasePlayer::IsVisible()
{
	return Globals::LocalPlayer->CanSeePlayer(this, 3) || Globals::LocalPlayer->CanSeePlayer(this, 0) || Globals::LocalPlayer->CanSeePlayer(this, 4);
}

CCSPlayerAnimState *C_BasePlayer::GetPlayerAnimState()
{
	return *(CCSPlayerAnimState**)((DWORD)this + 0x3900);
}

void C_BasePlayer::UpdateAnimationState(CCSGOPlayerAnimState *state, QAngle angle)
{
	static auto UpdateAnimState = Utils::PatternScan(
		GetModuleHandleA(XorStr("client_panorama.dll")), XorStr("55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24"));
	/*static auto UpdateAnimState = Utils::PatternScan(
	GetModuleHandle(L"client_panorama.dll"), "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24");
	*/
	if (!UpdateAnimState)
		return;

	__asm {
		push 0
	}

	__asm
	{
		mov ecx, state

		movss xmm1, dword ptr[angle + 4]
		movss xmm2, dword ptr[angle]

		call UpdateAnimState
	}
}

void C_BasePlayer::ResetAnimationState(CCSGOPlayerAnimState *state)
{
	using ResetAnimState_t = void(__thiscall*)(CCSGOPlayerAnimState*);
	static auto ResetAnimState = (ResetAnimState_t)Utils::PatternScan(GetModuleHandleA(XorStr("client_panorama.dll")), XorStr("56 6A 01 68 ? ? ? ? 8B F1"));
	if (!ResetAnimState)
		return;

	ResetAnimState(state);
}

void C_BasePlayer::CreateAnimationState(CCSGOPlayerAnimState *state)
{
	using CreateAnimState_t = void(__thiscall*)(CCSGOPlayerAnimState*, C_BasePlayer*);
	static auto CreateAnimState = (CreateAnimState_t)Utils::PatternScan(GetModuleHandleA(XorStr("client_panorama.dll")), XorStr("55 8B EC 56 8B F1 B9 ? ? ? ? C7 46"));
	if (!CreateAnimState)
		return;

	CreateAnimState(state, this);
}

bool C_BasePlayer::IsGhost()
{
	return *reinterpret_cast<bool*>(uintptr_t(this) + 0x3A11);
}

bool C_BasePlayer::SetupBones2(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
{
	auto backupval = *reinterpret_cast<uint8_t*>((uintptr_t)this + 0x274);

	*reinterpret_cast<uint8_t*>((uintptr_t)this + 0x274) = 0;
	bool setuped_bones = this->SetupBones(pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	*reinterpret_cast<uint8_t*>((uintptr_t)this + 0x274) = backupval;

	return setuped_bones;
}

Vector C_BasePlayer::GetEyePos()
{
	Vector v_origin = *(Vector*)((DWORD)this + 0x138);
	Vector v_view = *(Vector*)((DWORD)this + 0x108);
	return(v_origin + v_view);
	//return m_vecOrigin() + m_vecViewOffset();
}

player_info_t C_BasePlayer::GetPlayerInfo()
{
	player_info_t info;
	Interfaces::Engine->GetPlayerInfo(EntIndex(), &info);
	return info;
}

bool C_BasePlayer::IsAlive()
{
	if (!this)
		return false;
	//if (this->IsGhost())
	//	return false;
	return this->m_iHealth() > 0;
}

//bool C_BasePlayer::IsAlive()
//{
//	return (m_lifeState() == LIFE_ALIVE);
//}

bool C_BasePlayer::IsFlashed()
{
	if (m_flFlashDuration() > 0.f) return true;
	return false;
}

bool C_BasePlayer::HasC4()
{
	static auto fnHasC4
		= reinterpret_cast<bool(__thiscall*)(void*)>(
			Utils::PatternScan(GetModuleHandleA(XorStr("client_panorama.dll")), XorStr("56 8B F1 85 F6 74 31"))
			);

	return fnHasC4(this);
}

Vector C_BasePlayer::GetHitboxPos(int hitbox_id)
{
	matrix3x4_t boneMatrix[MAXSTUDIOBONES];

	if (SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
		auto studio_model = Interfaces::ModelInfo->GetStudiomodel(GetModel());
		if (studio_model) {
			auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(hitbox_id);
			if (hitbox) {
				auto
					min = Vector{},
					max = Vector{};

				Math::VectorTransform(hitbox->bbmin, boneMatrix[hitbox->bone], min);
				Math::VectorTransform(hitbox->bbmax, boneMatrix[hitbox->bone], max);

				return (min + max) / 2.0f;
			}
		}
	}
	return Vector{};
}

mstudiobbox_t* C_BasePlayer::GetHitbox(int hitbox_id)
{
	matrix3x4_t boneMatrix[MAXSTUDIOBONES];

	if (SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
		auto studio_model = Interfaces::ModelInfo->GetStudiomodel(GetModel());
		if (studio_model) {
			auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(hitbox_id);
			if (hitbox) {
				return hitbox;
				//auto
				//	min = Vector{},
				//	max = Vector{};

				//Math::VectorTransform(hitbox->bbmin, boneMatrix[hitbox->bone], min);
				//Math::VectorTransform(hitbox->bbmax, boneMatrix[hitbox->bone], max);

				//return (min + max) / 2.0f;
			}
		}
	}
	return nullptr;
}

bool C_BasePlayer::IsNotTarget()
{
	return !this || !IsPlayer() || this == Globals::LocalPlayer || IsDormant() || !IsAlive() || m_bGunGameImmunity() || (m_fFlags() & FL_FROZEN) || GetClientClass()->m_ClassID != CCSPlayer;
}

bool C_BasePlayer::GetHitboxPos(int hitbox, Vector &output)
{
	if (hitbox >= HITBOX_MAX)
		return false;

	const model_t *model = this->GetModel();
	if (!model)
		return false;

	studiohdr_t *studioHdr = Interfaces::ModelInfo->GetStudiomodel(model);
	if (!studioHdr)
		return false;

	matrix3x4_t matrix[MAXSTUDIOBONES];
	if (!this->SetupBones(matrix, MAXSTUDIOBONES, 0x100, 0))
		return false;

	mstudiobbox_t *studioBox = studioHdr->GetHitboxSet(0)->GetHitbox(hitbox);
	if (!studioBox)
		return false;

	Vector min, max;

	Math::VectorTransform(studioBox->bbmin, matrix[studioBox->bone], min);
	Math::VectorTransform(studioBox->bbmax, matrix[studioBox->bone], max);

	output = (min + max) * 0.5f;

	return true;
}

Vector C_BasePlayer::GetBonePos(int bone)
{
	matrix3x4_t boneMatrix[MAXSTUDIOBONES];
	if (SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, 0.0f)) {
		return boneMatrix[bone].at(3);
	}
	return Vector{};
}

bool C_BasePlayer::CanSeePlayer(C_BasePlayer* player, int hitbox)
{
	CGameTrace tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = this;

	auto endpos = player->GetHitboxPos(hitbox);

	ray.Init(GetEyePos(), endpos);
	Interfaces::EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

	return tr.hit_entity == player || tr.fraction > 0.97f;
}

bool C_BasePlayer::CanSeePlayer(C_BasePlayer* player, const Vector& pos)
{
	CGameTrace tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = this;

	//auto start = GetEyePos();
	//auto dir = (pos - start).Normalized();

	ray.Init(GetEyePos(), pos);
	Interfaces::EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

	return tr.hit_entity == player || tr.fraction > 0.97f;
}

void C_BasePlayer::UpdateClientSideAnimation()
{
	return CallVFunction<void(__thiscall*)(void*)>(this, 218)(this);
}

void C_BasePlayer::SetAngle2(QAngle wantedang)
{
	typedef void(__thiscall* SetAngleFn)(void*, const QAngle &);
	static SetAngleFn SetAngle2 = reinterpret_cast<SetAngleFn>(Utils::PatternScan(GetModuleHandleA("client_panorama.dll"), "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));
	SetAngle2(this, wantedang);
}

void C_BasePlayer::InvalidateBoneCache()
{
	static auto InvalidateBoneCacheFn = Utils::PatternScan(GetModuleHandleA(XorStr("client_panorama.dll")), XorStr("80 3D ?? ?? ?? ?? ?? 74 16 A1 ?? ?? ?? ?? 48 C7 81"));
	reinterpret_cast<void(__fastcall*)(void*)>(InvalidateBoneCacheFn)(this);
}

int C_BasePlayer::m_nMoveType()
{
	return *(int*)((uintptr_t)this + 0x25C);
}

void C_BasePlayer::SetVAngles(QAngle angles)
{
	static auto deadflag = NetvarSys::Get().GetOffset("DT_BasePlayer", "deadflag");
	*(QAngle*)((DWORD)this + deadflag + 0x4) = angles;
}

QAngle* C_BasePlayer::GetVAngles()
{
	static auto deadflag = NetvarSys::Get().GetOffset("DT_BasePlayer", "deadflag");
	return (QAngle*)((uintptr_t)this + deadflag + 0x4);
}

void C_BaseAttributableItem::SetGloveModelIndex(int modelIndex)
{
	return CallVFunction<void(__thiscall*)(void*, int)>(this, 75)(this, modelIndex);
}

float C_BasePlayer::GetFlashBangTime()
{

	static uint32_t m_flFlashBangTime = *(uint32_t*)((uint32_t)Utils::PatternScan(GetModuleHandleA(XorStr("client_panorama.dll")),
		XorStr("F3 0F 10 86 ?? ?? ?? ?? 0F 2F 40 10 76 30")) + 4);
	return *(float*)(this + m_flFlashBangTime);
	//return *(float*)((uintptr_t)this + 0xa308);
}

void C_BaseViewModel::SendViewModelMatchingSequence(int sequence)
{
	return CallVFunction<void(__thiscall*)(void*, int)>(this, 241)(this, sequence);
}

CUtlVector<IRefCounted*>& C_EconItemView::m_CustomMaterials()
{
	return *(CUtlVector<IRefCounted*>*)((uintptr_t)this + 0x14);
}

CUtlVector<IRefCounted*>& C_EconItemView::m_VisualsDataProcessors()
{
	static auto inReload = *(uint32_t*)(Utils::PatternScan(GetModuleHandleA(XorStr("client_panorama.dll")), XorStr("81 C7 ? ? ? ? 8B 4F 0C 8B 57 04 89 4C 24 0C")) + 2);
	return *(CUtlVector<IRefCounted*>*)((uintptr_t)this + inReload);
}

float_t C_BasePlayer::m_flSpawnTime()
{
	return *(float_t*)((uintptr_t)this + 0xA290);
}