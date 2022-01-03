#ifndef BASECARD_H_
#define BASECARD_H_

#include <cstring>

enum CARD_NAME
{
	CARDNAME_UNDEF,
	BETRAYAL_OF_FLESH,
	INFERNAL_TUTOR,
	COERCION,
	BRAINSPOIL,
	DEMONIC_COLLUSION,
	LEYLINE_OF_SANCTITY,
	SNEAK_ATTACK,
	SORINS_VENGEANCE,
	SNAP,
	ONE_WITH_NOTHING,
	CHIMNEY_IMP,
	PAST_IN_FLAMES,
	TENDRILS_OF_AGONY,
	SPITTING_IMAGE,
	GREAT_WHALE,
	HOODWINK,
	BASIC_LAND,
};

enum CARD_TYPE
{
	CARDTYPE_UNDEF,
	CREATURE,
	INSTANT,
	SORCERY,
	ENCHANTMENT,
	LAND,
};

enum ZONE_TYPE 
{
	ZONETYPE_UNDEF,
	BATTLEFIELD,
	HAND,
	GRAVEYARD,
	EXILE,
	LIBRARY,
};

class BaseCard
{
public:
	BaseCard();

	virtual int GetCost() = 0;
	virtual CARD_NAME GetName() = 0;
	virtual CARD_TYPE GetType() = 0;
	virtual bool CanCast() = 0;
	virtual void Cast() = 0;
	virtual char* GetLabel() = 0;

	bool haste;
	bool isToken;
	bool hasFlashback;
	ZONE_TYPE zone;
};

class BetrayalOfFlesh : public BaseCard
{
public:
	int GetCost() { return 6; }
	CARD_NAME GetName() { return BETRAYAL_OF_FLESH; }
	CARD_TYPE GetType() { return INSTANT; }
	char* GetLabel() { return "Betrayal of Flesh"; }
	bool CanCast();

	void Cast();

	void Kill();
	void Revive();
};

class InfernalTutor : public BaseCard
{
public:
	int GetCost() { return 2; }
	CARD_NAME GetName() { return INFERNAL_TUTOR; }
	CARD_TYPE GetType() { return INSTANT; }
	char* GetLabel() { return "Infernal Tutor"; }
	bool CanCast();
	void Cast();
};

class Coercion : public BaseCard
{
public:
	int GetCost() { return 3; }
	CARD_NAME GetName() { return COERCION; }
	CARD_TYPE GetType() { return SORCERY; }
	char* GetLabel() { return "Coercion"; }
	bool CanCast();
	void Cast();
};

class Brainspoil : public BaseCard
{
public:
	int GetCost() { return 5; }
	CARD_NAME GetName() { return BRAINSPOIL; }
	CARD_TYPE GetType() { return SORCERY; }
	char* GetLabel() { return "Brainspoil"; }

	bool CanCast();
	void Cast();

	bool CanKill();
	bool CanTransmute();

	void Kill();
	void Transmute();
};

class DemonicCollusion : public BaseCard
{
public:
	int GetCost() { return 5; }
	CARD_NAME GetName() { return DEMONIC_COLLUSION; }
	CARD_TYPE GetType() { return SORCERY; }
	char* GetLabel() { return "Demonic Collusion"; }

	bool CanCast();
	void Cast();
};

class LeylineOfSanctity : public BaseCard
{
public:
	int GetCost() { return 4; }
	CARD_NAME GetName() { return LEYLINE_OF_SANCTITY; }
	CARD_TYPE GetType() { return ENCHANTMENT; }
	char* GetLabel() { return "Leyline of Sanctity"; }

	bool CanCast();
	void Cast();
};

class SneakAttack : public BaseCard
{
public:
	int GetCost() { return 4; }
	CARD_NAME GetName() { return SNEAK_ATTACK; }
	CARD_TYPE GetType() { return ENCHANTMENT; }
	char* GetLabel() { return "Sneak Attack"; }

	bool CanCast();
	void Cast();
};

class SorinsVengeance : public BaseCard
{
public:
	int GetCost() { return 7; }
	CARD_NAME GetName() { return SORINS_VENGEANCE; }
	CARD_TYPE GetType() { return INSTANT; }
	char* GetLabel() { return "Sorin's Vengeance"; }

	bool CanCast();
	void Cast();
};


class Snap : public BaseCard
{
public:
	int GetCost() { return 2; }
	CARD_NAME GetName() { return SNAP; }
	CARD_TYPE GetType() { return INSTANT; }
	char* GetLabel() { return "Snap"; }

	bool CanCast();
	void Cast();
};

class OneWithNothing : public BaseCard
{
public:
	int GetCost() { return 1; }
	CARD_NAME GetName() { return ONE_WITH_NOTHING; }
	CARD_TYPE GetType() { return INSTANT; }
	char* GetLabel() { return "One With Nothing"; }

	bool CanCast();
	void Cast();
};

class ChimneyImp : public BaseCard
{
	int GetCost() { return 5; }
	CARD_NAME GetName() { return CHIMNEY_IMP; }
	CARD_TYPE GetType() { return CREATURE; }
	char* GetLabel() { return "Chimney Imp"; }

	bool CanCast();
	void Cast();
};

class PastInFlames : public BaseCard
{
	int GetCost() { return 4; }
	CARD_NAME GetName() { return PAST_IN_FLAMES; }
	CARD_TYPE GetType() { return SORCERY; }
	char* GetLabel() { return "Past In Flames"; }

	bool CanCast();
	void Cast();
};

class TendrilsOfAgony : public BaseCard
{
	int GetCost() { return 4; }
	CARD_NAME GetName() { return TENDRILS_OF_AGONY; }
	CARD_TYPE GetType() { return SORCERY; }
	char* GetLabel() { return "Tendrils of Agony"; }

	bool CanCast();
	void Cast();
};

class SpittingImage : public BaseCard
{
	int GetCost() { return 6; }
	CARD_NAME GetName() { return SPITTING_IMAGE; }
	CARD_TYPE GetType() { return SORCERY; }
	char* GetLabel() { return "Spitting Image"; }

	bool CanCast();
	void Cast();

	void Retrace();
};

class GreatWhale : public BaseCard
{
public:
	int GetCost() { return 7; }
	CARD_NAME GetName() { return GREAT_WHALE; }
	CARD_TYPE GetType() { return CREATURE; }
	char* GetLabel() { return "Great Whale"; }

	bool CanCast();
	void Cast();
};

class Hoodwink : public BaseCard
{
public:
	int GetCost() { return 2; }
	CARD_NAME GetName() { return HOODWINK; }
	CARD_TYPE GetType() { return INSTANT; }
	char* GetLabel() { return "Hoodwink"; }

	bool CanCast();
	void Cast();
};

class BasicLand : public BaseCard
{
public:
	int GetCost() { return 0; }
	CARD_NAME GetName() { return BASIC_LAND; }
	CARD_TYPE GetType() { return LAND; }
	char* GetLabel() { return "BASIC_LAND"; }

	bool CanCast();
	void Cast();
};

#endif