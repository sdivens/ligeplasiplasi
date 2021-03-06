#include "PluginSDK.h"
#include <map>
#include <string>

//TO DO:
// Need R safty Check?
// Chase Combo, save R and E, use GTargetSelector?
// Auto E with X Enemys?

//3/28:
//add ordwalk to flee and chasecombo and misaya
//fix kill steal
//test gap close

//3/28: fixed chasecombo
//flee need to fix with QR
char* Author = "Xu211";
char* Champion = "Diana";
char* Plugin = "Xu Diana";
int Version = 1.0;

PluginSetup("Xu Diana");

IMenu* MainMenu;
IMenu* ComboMenu;
IMenu* ChaseComboMenu;
IMenu* HarassMenu;
IMenu* FarmMenu;
IMenu* JungleMenu;
IMenu* MiscMenu;
IMenu* FleeMenu;
IMenu* Drawings;
IMenu* ItemsMenu;
IMenu* PotionMenu;
IMenu* GapClose;
IMenu* TrinketMenu;

IMenuOption* ComboQ;
IMenuOption* ComboW;
IMenuOption* ComboE;
IMenuOption* ComboR;
IMenuOption* ComboRB;
IMenuOption* UseMisaCombo;
IMenuOption* ComboMisa;
IMenuOption* UseIgnitecombo;

IMenuOption* ChaseKey;
IMenuOption* ChaseQ;
IMenuOption* ERange;
IMenuOption* RRange;
IMenuOption* Note;
IMenuOption* Note2;
IMenuOption* NoteER;

IMenuOption* HarassQ;
IMenuOption* HarassW;
IMenuOption* HarassE;
//IMenuOption* HarassR;
IMenuOption* HarassManaPercent;

IMenuOption* FarmQ;
IMenuOption* FarmW;
IMenuOption* FarmManaPercent;
IMenuOption* lasthitQ;
IMenuOption* JungleQ;
IMenuOption* JungleW;
IMenuOption* JungleE;
IMenuOption* JungleR;
IMenuOption* JungleManaPercent;

IMenuOption* KillstealQ;
IMenuOption* KillstealR;
IMenuOption* AutoQSS;

/*IMenuOption* FleeKey;
IMenuOption* FleeR;
IMenuOption* FleeQR;*/

IMenuOption* usepotion;
IMenuOption* usepotionhpper;

IMenuOption* GapW;
IMenuOption* GapE;

IMenuOption* DrawReady;
IMenuOption* DrawQ;
IMenuOption* DrawW;
IMenuOption* DrawE;
IMenuOption* DrawChaseE;
IMenuOption* DrawR;
IMenuOption* DrawChaseR;
IMenuOption* UseTrinket;

//Items
IInventoryItem* blade;
IInventoryItem* Cutlass;
IInventoryItem* HealthPot;
IInventoryItem* CorruptPot;
IInventoryItem* Biscuit;
IInventoryItem* RefillPot;
IInventoryItem* hunter;
IInventoryItem* Mersci;
IInventoryItem* QuickSiv;
IInventoryItem* Trinket;
IInventoryItem* Yellow;

IUnit* Player;

ISpell2* Q;
ISpell2* W;
ISpell2* E;
ISpell2* R;

ISpell* Ignite;
ISpell* Heal;

std::map<int, IMenuOption*> ChampionuseEGap;
std::map<int, IMenuOption*> ChampionuseWGap;
std::map<int, IMenuOption*> ChampionuseRQ;
std::map<int, IMenuOption*> ChampionuseTrinket;
std::map<int, IMenuOption*> ChampionuseChase;

static bool IsKeyDown(IMenuOption* menuOption)
{
	return (GetAsyncKeyState(menuOption->GetInteger()) & 0x8000) != 0;
}

void  Menu()
{
	//Main
	MainMenu = GPluginSDK->AddMenu("Xu Diana");

	//Combo
	ComboMenu = MainMenu->AddMenu("Combo Settings");
	UseIgnitecombo = ComboMenu->CheckBox("Use Ignite", true);
	ComboQ = ComboMenu->CheckBox("Use Q", true);
	ComboW = ComboMenu->CheckBox("Use W", true);
	ComboE = ComboMenu->CheckBox("Use E", true);
	ComboR = ComboMenu->CheckBox("Use R", true);
	ComboRB = ComboMenu->CheckBox("Only R Moonlight Target", true);
	Note2 = ComboMenu->CheckBox("^ Low ping, Untick will be misaya", true);
	/*UseMisaCombo = ComboMenu->CheckBox("Enable Misaya Combo", true);
	ComboMisa = ComboMenu->AddKey("Misaya Combo Key", 84);
	for (auto Enemy : GEntityList->GetAllHeros(false, true))
	{
		std::string szMenuName = "Use Misaya Combo On - " + std::string(Enemy->ChampionName());
		ChampionuseRQ[Enemy->GetNetworkId()] = ComboMenu->CheckBox(szMenuName.c_str(), false);
	}*/

	//Chase Combo
	ChaseComboMenu = MainMenu->AddMenu("Chase Combo");
	ChaseKey = ChaseComboMenu->AddKey("Chase Key", 74);
	ChaseQ = ChaseComboMenu->CheckBox("Use Q", true);
	ERange = ChaseComboMenu->AddInteger("Minium Range To E", 10, 450, 420);
	RRange = ChaseComboMenu->AddInteger("Minium Range To R", 10, 825, 800);
	Note = ChaseComboMenu->CheckBox("Note:Q Passive Only Stay 3 Sec", true);
	DrawChaseE = ChaseComboMenu->CheckBox("Draw Chase E Range", false);
	DrawChaseR = ChaseComboMenu->CheckBox("Draw Chase R Range", false);
	NoteER = ChaseComboMenu->CheckBox("Max ERange:450 Max RRange:825", true);

	//Harass
	HarassMenu = MainMenu->AddMenu("Harass Setting");
	HarassQ = HarassMenu->CheckBox("Use Q", true);
	HarassW = HarassMenu->CheckBox("Use W", true);
	HarassE = HarassMenu->CheckBox("Use E", true);
	//HarassR = HarassMenu->CheckBox("Use R", false);
	HarassManaPercent = HarassMenu->AddInteger("Mana Percent for harass", 10, 100, 45);

	//Farm
	FarmMenu = MainMenu->AddMenu("LaneClear Setting");
	FarmQ = FarmMenu->CheckBox("Use Q Farm", true);
	FarmW = FarmMenu->CheckBox("Use W Farm", false);
	FarmManaPercent = FarmMenu->AddInteger("Mana Percent for Farm", 10, 100, 45);
	lasthitQ = FarmMenu->CheckBox("Out Range Q Lasthit", true);

	//Jungle
	JungleMenu = MainMenu->AddMenu("Jungle Setting");
	JungleQ = JungleMenu->CheckBox("Use Q Jungle", true);
	JungleW = JungleMenu->CheckBox("Use W Jungle", false);
	JungleE = JungleMenu->CheckBox("Use E Jungle", true);
	JungleR = JungleMenu->CheckBox("Use R Jungle", false);
	JungleManaPercent = JungleMenu->AddInteger("Mana Percent for Farm", 10, 100, 45);

	//Misc
	MiscMenu = MainMenu->AddMenu("Misc Setting");
	KillstealQ = MiscMenu->CheckBox("Use Q to killsteal", true);
	KillstealR = MiscMenu->CheckBox("Use R to killsteal", true);
	AutoQSS = MiscMenu->CheckBox("Auto QSS", true);

	//Flee
	/*FleeMenu = MainMenu->AddMenu("Flee Setting");
	FleeR = FleeMenu->CheckBox("Use R To Flee", true);
	FleeQR = FleeMenu->CheckBox("Use QR To Flee", false);
	FleeKey = FleeMenu->AddKey("Flee Key", 74);*/

	//Potions
	PotionMenu = MainMenu->AddMenu("Potion Setting");
	usepotion = PotionMenu->CheckBox("Use Potions", true);
	usepotionhpper = PotionMenu->AddInteger("Use Potions if HP <", 10, 100, 75);

	//GapClose
	GapClose = MainMenu->AddMenu("GapClose Setting");
	GapW = GapClose->CheckBox("GapClose W", true);
	for (auto Enemy : GEntityList->GetAllHeros(false, true))
	{
		std::string szMenuName = "Auto W on GapClose - " + std::string(Enemy->ChampionName());
		ChampionuseWGap[Enemy->GetNetworkId()] = GapClose->CheckBox(szMenuName.c_str(), false);
	}

	GapE = GapClose->CheckBox("GapClose E", true);
	for (auto Enemy : GEntityList->GetAllHeros(false, true))
	{
		std::string szMenuName = "GapClose E - " + std::string(Enemy->ChampionName());
		ChampionuseEGap[Enemy->GetNetworkId()] = GapClose->CheckBox(szMenuName.c_str(), false);
	}



	//Drawings
	Drawings = MainMenu->AddMenu("Drawings");
	DrawReady = Drawings->CheckBox("Draw Only Ready Spells", true);
	DrawQ = Drawings->CheckBox("Draw Q", true);
	DrawW = Drawings->CheckBox("Draw W", false);
	DrawE = Drawings->CheckBox("Draw E", false);
	DrawR = Drawings->CheckBox("Draw R", true);

	//Trinkets
	TrinketMenu = MainMenu->AddMenu("Auto Trinket");
	UseTrinket = TrinketMenu->CheckBox("Enable Combo Trinket", true);
	for (auto Enemys : GEntityList->GetAllHeros(false, true))
	{
		std::string szMenuName = "Use Trinket on - " + std::string(Enemys->ChampionName());
		ChampionuseTrinket[Enemys->GetNetworkId()] = TrinketMenu->CheckBox(szMenuName.c_str(), false);
	}
}

	//Declear Spells && Items
	void LoadSpells()
	{
		Q = GPluginSDK->CreateSpell2(kSlotQ, kCircleCast, false, true, static_cast<eCollisionFlags>(kCollidesWithYasuoWall));
		W = GPluginSDK->CreateSpell2(kSlotW, kCircleCast, false, true, static_cast<eCollisionFlags>(kCollidesWithYasuoWall));
		E = GPluginSDK->CreateSpell2(kSlotE, kCircleCast, false, true, static_cast<eCollisionFlags>(kCollidesWithNothing));
		R = GPluginSDK->CreateSpell2(kSlotR, kTargetCast, false, false, static_cast<eCollisionFlags>(kCollidesWithNothing));

		Q->SetOverrideRange(830);
		W->SetOverrideRange(250);
		E->SetOverrideRange(450);
		R->SetOverrideRange(825);

		Q->SetOverrideDelay(0.25f);

		Q->SetOverrideRadius(185);

		Q->SetOverrideSpeed(1620);

		//SS
		auto slot1 = GPluginSDK->GetEntityList()->Player()->GetSpellName(kSummonerSlot1);
		auto slot2 = GPluginSDK->GetEntityList()->Player()->GetSpellName(kSummonerSlot2);

		if (strcmp(slot1, "SummonerDot") == 0) { Ignite = GPluginSDK->CreateSpell(kSummonerSlot1, 600); }
		if (strcmp(slot2, "SummonerDot") == 0) { Ignite = GPluginSDK->CreateSpell(kSummonerSlot2, 600); }
		if (strcmp(slot1, "SummonerHeal") == 0) { Heal = GPluginSDK->CreateSpell(kSummonerSlot1, 850); }
		if (strcmp(slot2, "SummonerHeal") == 0) { Heal = GPluginSDK->CreateSpell(kSummonerSlot2, 850); }

		//Items
		Trinket = GPluginSDK->CreateItemForId(3363, 4000);
		Yellow = GPluginSDK->CreateItemForId(3340, 600);
	}

	//Fountain Buff
	static bool InFountain(IUnit *unit)
	{
		return unit->HasBuff("kappachino");
	}

	//Count Enemys In Range
	int EnemiesInRange(IUnit* Source, float range)
	{
		auto Targets = GEntityList->GetAllHeros(false, true);
		auto enemiesInRange = 0;

		for (auto target : Targets)
		{
			if (target != nullptr)
			{
				auto flDistance = (target->GetPosition() - Source->GetPosition()).Length();
				if (flDistance < range)
				{
					enemiesInRange++;
				}
			}
		}
		return enemiesInRange;
	}


	//Calculate Distence Between Player and Target
	float GetDistance(IUnit* Player, IUnit* target)
	{
		return (Player->GetPosition() - target->GetPosition()).Length2D();
	}

	//Count Not ally heros in range
	int CountEnemiesInRange(float range)
	{
		int enemies = 0;
		for (auto enemy : GEntityList->GetAllHeros(false, true))
		{
			if (enemy != nullptr && GetDistance(GEntityList->Player(), enemy) <= range)
			{
				enemies++;
			}
		}
		return enemies;
	}

	//Excute Combo Logic
	void Combo()
	{
		if (Ignite != nullptr)
		{
			auto Enemy = GTargetSelector->FindTarget(QuickestKill, SpellDamage, Q->Range());
			if (UseIgnitecombo->Enabled() && Ignite->IsReady())
			{
				if (Enemy != nullptr && Enemy->IsValidTarget(Player, 570))
				{
					if (Enemy->HealthPercent() <= 30)
					{
						Ignite->CastOnUnit(Enemy);
					}
				}
			}
		}

		if (ComboQ->Enabled())
		{
			if (Q->IsReady())
			{
				auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, Q->Range());

				AdvPredictionOutput prediction_output;
				Q->RunPrediction(target, true, kCollidesWithYasuoWall, &prediction_output);
				if (Player->IsValidTarget(target, Q->Range()) && prediction_output.HitChance >= kHitChanceVeryHigh)
				{
					Q->CastOnTarget(target);
					//GGame->PrintChat("cast q");
				}
			}
		}

		if (ComboW->Enabled())
		{
			if (W->IsReady())
			{
				auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, W->Range());
				if (Player->IsValidTarget(target, W->Range()))
				{
					W->CastOnPlayer();
					//GGame->PrintChat("cast w");
				}
			}
		}

		if (ComboE->Enabled())
		{
			auto QWMANA = Q->ManaCost() + W->ManaCost();
			auto QRMANA = Q->ManaCost() + R->ManaCost();

			if (E->IsReady() && E->ManaCost() > QWMANA || E->ManaCost() > QRMANA || E->ManaCost() > Q->ManaCost() || E->ManaCost() > W->ManaCost())
			{
				auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, E->Range());
				if (Player->IsValidTarget(target, E->Range()))
				{
					E->CastOnPlayer();
					//GGame->PrintChat("cast e");
				}
			}
		}

		if (ComboR->Enabled())
		{
			if (R->IsReady())
			{
				auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, R->Range());
				if (target != nullptr && ComboRB->Enabled() && target->HasBuff("dianamoonlight") && Player->IsValidTarget(target, R->Range()))
				{
					R->CastOnUnit(target);
					//GGame->PrintChat("cast r only buff");
				}
				else
				{
					if (!ComboRB->Enabled() && Player->IsValidTarget(target, R->Range()))
					{
						R->CastOnUnit(target);
						//GGame->PrintChat("cast r");
					}
				}
			}
		}
	}

	//Misaya Combo
	/*void MisayaCombo()
	{
		if (IsKeyDown(ComboMisa))
		{
			GGame->IssueOrder(Player, kMoveTo, GGame->CursorPosition());
			//GGame->PrintChat("misaya move");
			if (UseMisaCombo->Enabled());
			{
				if (GEntityList->Player()->GetSpellState(kSlotR) == Ready &&  GEntityList->Player()->GetSpellState(kSlotQ) == Ready)
				{
					auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, R->Range());

					R->CastOnUnit(target);
					//GGame->PrintChat("cast r misaya");

					AdvPredictionOutput prediction_output;
					Q->RunPrediction(target, true, kCollidesWithYasuoWall, &prediction_output);
					if (ChampionuseRQ[target->GetNetworkId()]->Enabled() && Player->IsValidTarget(target, Q->Range()) && prediction_output.HitChance >= kHitChanceVeryHigh)
					{
						Q->CastOnTarget(target);
						//GGame->PrintChat("cast q misaya");
					}
				}
			}
		}
	}*/
	//---------------------------------------------COMBO END-------------------------------------------------------------------------------------------------

	//Harass
	void Harass()
	{
		if (Player->ManaPercent() < HarassManaPercent->GetInteger())
			return;
		if (HarassQ->Enabled())
		{
			if (Q->IsReady())
			{
				auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());
				if (Player->IsValidTarget(target, Q->Range()))
					Q->CastOnTarget(target, kHitChanceHigh);
			}
		}

		if (HarassW->Enabled())
		{
			if (W->IsReady())
			{
				auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, W->Range());
				if (Player->IsValidTarget(target, W->Range()))
					W->CastOnPlayer();
			}
		}

		if (HarassE->Enabled())
		{
			if (E->IsReady())
			{
				auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, E->Range());
				if (Player->IsValidTarget(target, E->Range()))
					E->CastOnPlayer();
			}
		}
	}
	//---------------------------------------------HARASS END-------------------------------------------------------------------------------------------------

	//LastHitQ
	void LastHitQ()
	{
		int MinionDie = 0;
		for (auto minions : GEntityList->GetAllMinions(false, true, false))
		{
			if (minions != nullptr && Player->IsValidTarget(minions, Q->Range()) && !Player->IsValidTarget(minions, 150))
			{
				auto dmg = GDamage->GetSpellDamage(Player, minions, kSlotQ);
				auto dmg1 = GDamage->GetAutoAttackDamage(Player, minions, true);
				if (minions->GetHealth() <= dmg || minions->GetHealth() <= dmg1 || minions->GetHealth() <= dmg1 + dmg)
					MinionDie++;

				if (Player->ManaPercent() < FarmManaPercent->GetInteger())
					return;
				if (Q->IsReady() && lasthitQ->Enabled())
				{
					if (MinionDie > 1)
						Q->CastOnUnit(minions);
					//GGame->PrintChat("last hit q");
				}
			}
		}
	}
	//---------------------------------------------LASTHITQ END-------------------------------------------------------------------------------------------------


	void LaneClear()
	{
		int MinionAround = 0;
		for (auto minions : GEntityList->GetAllMinions(false, true, false))
		{
			if (minions->IsValidTarget(Player, Q->Range()))
				MinionAround++;
			if (Player->ManaPercent() < FarmManaPercent->GetInteger())
				return;
			if (minions != nullptr && FarmQ->Enabled() && Q->IsReady())
			{
				if (MinionAround > 3)
					Q->CastOnUnit(minions);
				//GGame->PrintChat("lc q 3");
			}
			if (minions != nullptr && FarmW->Enabled() && W->IsReady())
			{
				if (MinionAround > 3 && minions->IsValidTarget(Player, W->Range()))
					W->CastOnUnit(minions);
				//GGame->PrintChat("w 3");
			}
		}
	}
	//---------------------------------------------LANECLEAR END-------------------------------------------------------------------------------------------------

	//JungleClear
	void JungleClear()
	{
		if (JungleQ->Enabled() && Q->IsReady())
		{
			for (auto jMinion : GEntityList->GetAllMinions(false, false, true))
			{
				if (jMinion != nullptr && !jMinion->IsDead() && Player->IsValidTarget(jMinion, Q->Range()))
				{
					if (Player->ManaPercent() < JungleManaPercent->GetInteger())
						return;
					Q->CastOnUnit(jMinion);
					//GGame->PrintChat("jg q");
				}
			}
		}

		if (JungleW->Enabled() && W->IsReady())
		{
			for (auto jMinion : GEntityList->GetAllMinions(false, false, true))
			{
				if (jMinion != nullptr && !jMinion->IsDead() && Player->IsValidTarget(jMinion, W->Range()))
				{
					if (Player->ManaPercent() < JungleManaPercent->GetInteger())
						return;
					W->CastOnPlayer();
					//GGame->PrintChat("jg  w");
				}
			}
		}

		if (JungleE->Enabled() && E->IsReady())
		{
			for (auto jMinion : GEntityList->GetAllMinions(false, false, true))
			{
				if (jMinion != nullptr && !jMinion->IsDead() && Player->IsValidTarget(jMinion, E->Range()))
				{
					if (Player->ManaPercent() < JungleManaPercent->GetInteger())
						return;
					E->CastOnPlayer();
					//GGame->PrintChat("jg  e");
				}
			}
		}
		if (JungleR->Enabled() && R->IsReady())
		{
			for (auto jMinion : GEntityList->GetAllMinions(false, false, true))
			{
				if (jMinion != nullptr  && jMinion->HasBuff("dianamoonlight") && !jMinion->IsDead() && Player->IsValidTarget(jMinion, R->Range()))
				{
					if (Player->ManaPercent() < JungleManaPercent->GetInteger())
						return;
					R->CastOnTarget(jMinion);
					//GGame->PrintChat("jg  r");
				}
			}
		}
	}
	//---------------------------------------------JUNGLECLEAR END-------------------------------------------------------------------------------------------------

	//KS
	void KillSteal()
	{
		for (auto Enemy : GEntityList->GetAllHeros(false, true))
		{
			auto QDamage = GDamage->GetSpellDamage(GEntityList->Player(), Enemy, kSlotQ);
			if (Enemy != nullptr && !Enemy->IsDead())
			{
				if (KillstealQ->Enabled() && Q->IsReady() && Enemy->IsValidTarget(GEntityList->Player(), Q->Range()) && QDamage >= Enemy->GetHealth())
				{
					Q->CastOnTarget(Enemy);
					//GGame->PrintChat("ks  q");
				}
			}

			auto RDamage = GDamage->GetSpellDamage(GEntityList->Player(), Enemy, kSlotR);
			if (Enemy != nullptr && !Enemy->IsDead())
			{
				if (KillstealR->Enabled() && R->IsReady() && Enemy->IsValidTarget(GEntityList->Player(), R->Range()) && RDamage >= Enemy->GetHealth())
				{
					R->CastOnTarget(Enemy);
					//GGame->PrintChat("ks  r");
				}

			}
		}
		/*
		for (auto Enemy : GEntityList->GetAllHeros(false, true))
		{
			auto QDamage = GDamage->GetSpellDamage(GEntityList->Player(), Enemy, kSlotQ);
			auto RDamage = GDamage->GetSpellDamage(GEntityList->Player(), Enemy, kSlotR);
			auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, Q->Range());
			AdvPredictionOutput prediction_output;
			Q->RunPrediction(target, true, kCollidesWithYasuoWall, &prediction_output);
			if (Enemy != nullptr && !Enemy->IsDead())
			{
				if (KillstealQ->Enabled() && Q->IsReady() && target->IsValidTarget(GEntityList->Player(), Q->Range()) && QDamage >= target->GetHealth() && prediction_output.HitChance >= kHitChanceHigh)
				{
					Q->CastOnTarget(target);
					//GGame->PrintChat("ks  q");
				}

			}
			
			auto rtarget = GTargetSelector->FindTarget(QuickestKill, SpellDamage, R->Range());
			if (Enemy != nullptr && !Enemy->IsDead())
			{
				if (KillstealR->Enabled() && R->IsReady() && rtarget->IsValidTarget(GEntityList->Player(), R->Range()) && RDamage >= rtarget->GetHealth())
				{
					R->CastOnTarget(rtarget);
					//GGame->PrintChat("ks  r");
				}

			}
		}*/
	}
	//---------------------------------------------KILLSTEAL END-------------------------------------------------------------------------------------------------

	//Flee
	/*void FleeMode()
	{
		if (IsKeyDown(FleeKey))
		{
			GGame->IssueOrder(GEntityList->Player(), kMoveTo, GGame->CursorPosition());
			if (IsKeyDown(FleeKey))
			{
				auto Enemy = GTargetSelector->FindTarget(QuickestKill, SpellDamage, Q->Range());
				for (auto Objects : GEntityList->GetAllMinions(false, true, true))
				{
					if (Q->IsReady() && Objects != Enemy && FleeQR->Enabled() && Player->IsValidTarget(Objects, Q->Range()) && Objects->IsValidTarget() && !Objects->IsDead())
					{
						Q->CastOnTarget(Objects);
						GGame->PrintChat("flee q");
					}
					if (R->IsReady() && FleeQR->Enabled() && Objects->HasBuff("dianamoonlight") && Player->IsValidTarget(Objects, R->Range() && Objects->IsValidTarget() && !Objects->IsDead()))
					{
						R->CastOnTarget(Objects);
						GGame->PrintChat("flee r");
					}
				}
			}
		}

		/*if (IsKeyDown(FleeKey))
		{
			GGame->IssueOrder(GEntityList->Player(), kMoveTo, GGame->CursorPosition());
			auto QRMANA = R->ManaCost() + Q->ManaCost();
			if (IsKeyDown(FleeKey) && R->IsReady() &&  Q->IsReady())
			{
				//if (Player->ManaPercent() > QRMANA)
					for (auto Objects : GEntityList->GetAllMinions(false, true, true))
					{
						if (Objects != nullptr && Player->IsValidTarget(Objects, Q->Range()) && Objects->IsValidTarget() && !Objects->IsDead())
						{
							if (FleeQR->Enabled())
							{
								Q->CastOnTarget(Objects);
								GGame->PrintChat("flee q");

								if (FleeQR->Enabled() && Objects->HasBuff("dianamoonlight"))
								{
									R->CastOnTarget(Objects);
									GGame->PrintChat("flee r");
								}

							}
						}
					}
			}
		}

		/*{
		GGame->IssueOrder(GEntityList->Player(), kMoveTo, GGame->CursorPosition());
		for (auto Objects : GEntityList->GetAllMinions(false, true, true))
		{
		if (Objects != nullptr && GEntityList->Player()->GetSpellState(kSlotR) == Ready && Player->IsValidTarget(Objects, R->Range()) && !Objects->IsDead)
		{
		if (FleeR->Enabled())
		{
		R->CastOnUnit(Objects);
		}
		}
		}
		}
	}*/
	//---------------------------------------------FLEE END-------------------------------------------------------------------------------------------------

	//Potions
	void Usepotion()
	{
		if (usepotion->Enabled() && !Player->IsRecalling() && !Player->IsDead())
		{
			if (CountEnemiesInRange(2000) > 0)
			{
				bool usepotions = Player->GetHealth() < Player->GetMaxHealth()* usepotionhpper->GetInteger() / 100;
				if (usepotions)
				{
					if (Player->GetBuffDataByName("ItemDarkCrystalFlask") || Player->GetBuffDataByName("ItemMiniRegenPotion")
						|| Player->GetBuffDataByName("ItemCrystalFlask") || Player->GetBuffDataByName("RegenerationPotion") || Player->HasBuff("ItemCrystalFlaskJungle"))
						return;

					if (Biscuit->IsOwned() && !InFountain(Player) && Biscuit->IsReady())
					{
						Biscuit->CastOnPlayer();
					}
					else if (HealthPot->IsOwned() && !InFountain(Player) && HealthPot->IsReady())
					{
						HealthPot->CastOnPlayer();
					}
					else if (CorruptPot->IsOwned() && CorruptPot->IsReady())
					{
						CorruptPot->CastOnPlayer();
					}
					else if (RefillPot->IsOwned() && RefillPot->IsReady())
					{
						RefillPot->CastOnPlayer();
					}
					else if (hunter->IsOwned() && hunter->IsReady())
					{
						hunter->CastOnPlayer();
					}
				}
			}
		}
	}
	//---------------------------------------------POTIONS END-------------------------------------------------------------------------------------------------

	//AutoQss
	void AutoQss()
	{
		auto Enemy = GTargetSelector->FindTarget(QuickestKill, SpellDamage, Q->Range());
		if (AutoQSS->Enabled() && Player->IsValidTarget(Enemy, 650))
		{
			if (Player->HasBuffOfType(BUFF_Snare) || Player->HasBuffOfType(BUFF_Taunt)
				|| Player->HasBuffOfType(BUFF_Suppression) || Player->HasBuffOfType(BUFF_Stun) || Player->HasBuffOfType(BUFF_Knockup)
				|| Player->HasBuffOfType(BUFF_Suppression) || Player->HasBuffOfType(BUFF_Charm) || Player->HasBuffOfType(BUFF_Polymorph)
				|| Player->HasBuffOfType(BUFF_Blind) || Player->HasBuffOfType(BUFF_Fear))
			{
				if (Mersci->IsOwned() && Mersci->IsReady())
					Mersci->CastOnPlayer();
				if (QuickSiv->IsOwned() && QuickSiv->IsReady())
					QuickSiv->CastOnPlayer();
			}
		}
	}
	//---------------------------------------------QSS END-------------------------------------------------------------------------------------------------

	//Trinket
	void ComboTrinket()
	{
		if (UseTrinket->Enabled() && (Trinket->IsOwned() && Trinket->IsReady() || Yellow->IsOwned() && Yellow->IsReady()))
		{
			auto Enemys = GEntityList->GetAllHeros(false, true);

			for (auto target : Enemys)
			{
				auto lastPos = target->GetPosition();
				if (!target->IsVisible())
				{
					if (ChampionuseTrinket[target->GetNetworkId()]->Enabled() && (GEntityList->Player()->GetPosition() - lastPos).Length2D() <= R->Range())
					{
						Trinket->CastOnPosition(lastPos) || Yellow->CastOnPosition(lastPos);
					}
				}
			}
		}
	}
	//---------------------------------------------TRINKET END-------------------------------------------------------------------------------------------------

	//Chase Combo
	void ChaseCombo()
	{
		if (IsKeyDown(ChaseKey))
		{
			if (!Player->IsWindingUp())
			{
				GGame->IssueOrder(Player, kMoveTo, GGame->CursorPosition());
			}
			//do attack issue order
			auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, Q->Range());
			GGame->IssueOrder(Player, kAttackUnit, target);

			//GGame->PrintChat("chase move");
			if (ComboQ->Enabled())
			{
				if (Q->IsReady())
				{
					auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, Q->Range());

					AdvPredictionOutput prediction_output;
					Q->RunPrediction(target, true, kCollidesWithYasuoWall, &prediction_output);
					if (Player->IsValidTarget(target, Q->Range()) && prediction_output.HitChance >= kHitChanceVeryHigh)
					{
						Q->CastOnTarget(target);
						//GGame->PrintChat("cast q");
					}
				}
			}

			auto Enemys = GEntityList->GetAllHeros(false, true);
			for (auto target : Enemys)
			{
				auto CurrentPos = target->GetPosition();
				if (target != nullptr && Player->IsValidTarget(target, 150))
				{
					GGame->IssueOrder(Player, kAutoAttack, target);
				}

				if (target != nullptr && Player->IsValidTarget(target, W->Range()))
				{
					W->CastOnPlayer();
				}

				if (target != nullptr && Player->IsValidTarget(target, Q->Range()))
				{
					if ((GEntityList->Player()->GetPosition() - CurrentPos).Length2D() <= ERange->GetInteger())
					{
						E->CastOnPlayer();
					}
				}
				if ((GEntityList->Player()->GetPosition() - CurrentPos).Length2D() <= RRange->GetInteger())
				{
					R->CastOnTarget(target);
				}
			}
		}

	}
	//---------------------------------------------CHASECOMBO END-------------------------------------------------------------------------------------------------

	//Drawing
	PLUGIN_EVENT(void) OnRender()
	{
		if (DrawReady->Enabled())
		{
			if (Q->IsReady() && DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }

			if (W->IsReady() && DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), W->Range()); }

			if (E->IsReady() && DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), E->Range()); }

			if (R->IsReady() && DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), R->Range()); }

		}
		else
		{
			if (DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }

			if (DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), W->Range()); }

			if (DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), E->Range()); }

			if (DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), R->Range()); }
		}

		if (DrawChaseE->Enabled())
		{
			if (E->IsReady() && DrawChaseE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 64, 0, 255), ERange->GetInteger()); }
		}
		if (DrawChaseR->Enabled())
		{
			if (R->IsReady() && DrawChaseR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(0, 139, 0, 255), RRange->GetInteger()); }
		}
	}
	//---------------------------------------------DRAWING END-------------------------------------------------------------------------------------------------

	//Gap Close
	PLUGIN_EVENT(void) OnGapcloser(GapCloserSpell const& args)
	{
		//auto target = GEntityList->GetAllHeros(false, true);
		auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, Q->Range());
		if (args.Sender != nullptr && args.Sender != Player && Player->IsValidTarget(args.Sender, E->Range())
			&& E->IsReady() && args.Sender->IsEnemy(Player) && GapE->Enabled()
			&& (Player->GetPosition() - args.EndPosition).Length() < 250 && ChampionuseEGap[target->GetNetworkId()]->Enabled())
		{
			E->CastOnPlayer();
			//GGame->PrintChat("Gap E");
		}

		if (args.Sender != nullptr && args.Sender != Player && Player->IsValidTarget(args.Sender, W->Range())
			&& W->IsReady() && args.Sender->IsEnemy(Player) && GapW->Enabled()
			&& (Player->GetPosition() - args.EndPosition).Length() < 250 && ChampionuseWGap[target->GetNetworkId()]->Enabled())
		{
			W->CastOnPlayer();
			//GGame->PrintChat("Gap W");
		}
	}
	//---------------------------------------------GAPCLOSE END-------------------------------------------------------------------------------------------------

	//OnUpdate
	PLUGIN_EVENT(void) OnGameUpdate()
	{
		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			Combo();
			ComboTrinket();
		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeLastHit)
		{
			LastHitQ();
		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
		{
			Harass();
			LastHitQ();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			LaneClear();
			JungleClear();
			LastHitQ();
		}
		//FleeMode();
		ChaseCombo();
		//MisayaCombo();
		/*if (GOrbwalking->GetOrbwalkingMode() == kModeCustom)
		{
			
		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeCustom)
		{
			
		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeCustom)
		{
			
		}*/
	}

	//OnLoad
	PLUGIN_API void OnLoad(IPluginSDK* PluginSDK)
	{
		PluginSDKSetup(PluginSDK);

		//GPlugin = new IPlugin(Author, Plugin, Version);
		Menu();
		LoadSpells();
		Player = GEntityList->Player();

		GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);
		GEventManager->AddEventHandler(kEventOnRender, OnRender);
		GEventManager->AddEventHandler(kEventOnGapCloser, OnGapcloser);
		//GExtension->CheckVersion(GPlugin->GetName(), GPlugin->GetVersion());
		if (strcmp(GEntityList->Player()->ChampionName(), "Diana") == 0)
		{
			GGame->PrintChat("<b><font color = \"#F535AA\">Xu Diana</font><font color=\"#4EE2EC\"> - Loaded</font></b>");
		}
		else
		{
			GGame->PrintChat("This plugin is for Diana");
		}

	}
	
	//OnUnload
	PLUGIN_API void OnUnload()
	{
		MainMenu->Remove();

		GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
		GEventManager->RemoveEventHandler(kEventOnRender, OnRender);
		GEventManager->RemoveEventHandler(kEventOnGapCloser, OnGapcloser);
	}