#ifndef BASECARD_H_
#define BASECARD_H_

#include <cstring>

class Game;

enum class CARD_NAME
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

enum class CARD_TYPE
{
	CARDTYPE_UNDEF,
	CREATURE,
	INSTANT,
	SORCERY,
	ENCHANTMENT,
	LAND,
};

enum class ZONE_TYPE 
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
protected:
	Game* game;
public:
	BaseCard(Game*);

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
	using BaseCard::BaseCard;

	int GetCost() { return 6; }
	CARD_NAME GetName() { return CARD_NAME::BETRAYAL_OF_FLESH; }
	CARD_TYPE GetType() { return CARD_TYPE::INSTANT; }
	char* GetLabel() { return "Betrayal of Flesh"; }
	bool CanCast();

	void Cast();

	void Kill();
	void Revive();
};

class InfernalTutor : public BaseCard
{
public:
	using BaseCard::BaseCard;

	int GetCost() { return 2; }
	CARD_NAME GetName() { return CARD_NAME::INFERNAL_TUTOR; }
	CARD_TYPE GetType() { return CARD_TYPE::INSTANT; }
	char* GetLabel() { return "Infernal Tutor"; }
	bool CanCast();
	void Cast();
};

class Coercion : public BaseCard
{
public:
	using BaseCard::BaseCard;

	int GetCost() { return 3; }
	CARD_NAME GetName() { return CARD_NAME::COERCION; }
	CARD_TYPE GetType() { return CARD_TYPE::SORCERY; }
	char* GetLabel() { return "Coercion"; }
	bool CanCast();
	void Cast();
};

class Brainspoil : public BaseCard
{
public:
	using BaseCard::BaseCard;

	int GetCost() { return 5; }
	CARD_NAME GetName() { return CARD_NAME::BRAINSPOIL; }
	CARD_TYPE GetType() { return CARD_TYPE::SORCERY; }
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
	using BaseCard::BaseCard;

	int GetCost() { return 5; }
	CARD_NAME GetName() { return CARD_NAME::DEMONIC_COLLUSION; }
	CARD_TYPE GetType() { return CARD_TYPE::SORCERY; }
	char* GetLabel() { return "Demonic Collusion"; }

	bool CanCast();
	void Cast();
};

class LeylineOfSanctity : public BaseCard
{
public:
	using BaseCard::BaseCard;

	int GetCost() { return 4; }
	CARD_NAME GetName() { return CARD_NAME::LEYLINE_OF_SANCTITY; }
	CARD_TYPE GetType() { return CARD_TYPE::ENCHANTMENT; }
	char* GetLabel() { return "Leyline of Sanctity"; }

	bool CanCast();
	void Cast();
};

class SneakAttack : public BaseCard
{
public:
	using BaseCard::BaseCard;

	int GetCost() { return 4; }
	CARD_NAME GetName() { return CARD_NAME::SNEAK_ATTACK; }
	CARD_TYPE GetType() { return CARD_TYPE::ENCHANTMENT; }
	char* GetLabel() { return "Sneak Attack"; }

	bool CanCast();
	void Cast();
};

class SorinsVengeance : public BaseCard
{
public:
	using BaseCard::BaseCard;

	int GetCost() { return 7; }
	CARD_NAME GetName() { return CARD_NAME::SORINS_VENGEANCE; }
	CARD_TYPE GetType() { return CARD_TYPE::INSTANT; }
	char* GetLabel() { return "Sorin's Vengeance"; }

	bool CanCast();
	void Cast();
};


class Snap : public BaseCard
{
public:
	using BaseCard::BaseCard;

	int GetCost() { return 2; }
	CARD_NAME GetName() { return CARD_NAME::SNAP; }
	CARD_TYPE GetType() { return CARD_TYPE::INSTANT; }
	char* GetLabel() { return "Snap"; }

	bool CanCast();
	void Cast();
};

class OneWithNothing : public BaseCard
{
public:
	using BaseCard::BaseCard;

	int GetCost() { return 1; }
	CARD_NAME GetName() { return CARD_NAME::ONE_WITH_NOTHING; }
	CARD_TYPE GetType() { return CARD_TYPE::INSTANT; }
	char* GetLabel() { return "One With Nothing"; }

	bool CanCast();
	void Cast();
};

class ChimneyImp : public BaseCard
{
public:
	using BaseCard::BaseCard;

	int GetCost() { return 5; }
	CARD_NAME GetName() { return CARD_NAME::CHIMNEY_IMP; }
	CARD_TYPE GetType() { return CARD_TYPE::CREATURE; }
	char* GetLabel() { return "Chimney Imp"; }

	bool CanCast();
	void Cast();
};

class PastInFlames : public BaseCard
{
public:
	using BaseCard::BaseCard;

	int GetCost() { return 4; }
	CARD_NAME GetName() { return CARD_NAME::PAST_IN_FLAMES; }
	CARD_TYPE GetType() { return CARD_TYPE::SORCERY; }
	char* GetLabel() { return "Past In Flames"; }

	bool CanCast();
	void Cast();
};

class TendrilsOfAgony : public BaseCard
{
public:
	using BaseCard::BaseCard;

	int GetCost() { return 4; }
	CARD_NAME GetName() { return CARD_NAME::TENDRILS_OF_AGONY; }
	CARD_TYPE GetType() { return CARD_TYPE::SORCERY; }
	char* GetLabel() { return "Tendrils of Agony"; }

	bool CanCast();
	void Cast();
};

class SpittingImage : public BaseCard
{
public:
	using BaseCard::BaseCard;

	int GetCost() { return 6; }
	CARD_NAME GetName() { return CARD_NAME::SPITTING_IMAGE; }
	CARD_TYPE GetType() { return CARD_TYPE::SORCERY; }
	char* GetLabel() { return "Spitting Image"; }

	bool CanCast();
	void Cast();

	void Retrace();
};

class GreatWhale : public BaseCard
{
public:
	using BaseCard::BaseCard;

	int GetCost() { return 7; }
	CARD_NAME GetName() { return CARD_NAME::GREAT_WHALE; }
	CARD_TYPE GetType() { return CARD_TYPE::CREATURE; }
	char* GetLabel() { return "Great Whale"; }

	bool CanCast();
	void Cast();
};

class Hoodwink : public BaseCard
{
public:
	using BaseCard::BaseCard;

	int GetCost() { return 2; }
	CARD_NAME GetName() { return CARD_NAME::HOODWINK; }
	CARD_TYPE GetType() { return CARD_TYPE::INSTANT; }
	char* GetLabel() { return "Hoodwink"; }

	bool CanCast();
	void Cast();
};

class BasicLand : public BaseCard
{
public:
	using BaseCard::BaseCard;

	int GetCost() { return 0; }
	CARD_NAME GetName() { return CARD_NAME::BASIC_LAND; }
	CARD_TYPE GetType() { return CARD_TYPE::LAND; }
	char* GetLabel() { return "BASIC_LAND"; }

	bool CanCast();
	void Cast();
};

#endif