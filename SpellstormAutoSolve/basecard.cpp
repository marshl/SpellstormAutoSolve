#include "basecard.h"

#include <cassert>
#include <iostream>

#include "cardgame.h"

BaseCard::BaseCard(Game* game)
{
	this->game = game;
	this->haste = false;
	this->hasFlashback = false;
	this->isToken = false;
	this->zone = ZONE_TYPE::ZONETYPE_UNDEF;
}

bool  BetrayalOfFlesh::CanCast()
{
	return false;
	/*return this->game->p1->untappedLand >= 5 &&
		(this->game->p1->graveyard->ContainsOfCardType(CREATURE) > 0
			|| this->game->p1->battlefield->ContainsOfCardType(CREATURE) > 0)
		&& (this->zone == HAND || this->zone == GRAVEYARD && this->hasFlashback);*/
}

void BetrayalOfFlesh::Cast()
{
	assert(this->CanCast());

	const bool canRevive = this->game->p1->graveyard->ContainsOfCardType(CARD_TYPE::CREATURE) > 0;
	const bool canKill = this->game->p1->battlefield->ContainsOfCardType(CARD_TYPE::CREATURE) > 0;

	assert(this->game->p1->untappedLand >= 5);
	assert(canKill || canRevive);

	this->game->p1->TapLand(5);
	this->game->p1->spellsCast++;

	if (!canRevive && canKill)
	{
		this->Kill();
	}
	else if (canRevive && !canKill)
	{
		this->Revive();
	}
	else
	{
		int choice = rand() % 3;
		if (choice == 0)
		{
			this->Kill();
		}
		else if (choice == 1)
		{
			this->Revive();
		}
		else // Entwined
		{
			this->Kill();
			this->Revive();
			this->game->p1->totalLand -= 3;
		}
	}

	this->game->MoveCardToZone(1, this->zone == ZONE_TYPE::HAND ? ZONE_TYPE::GRAVEYARD : ZONE_TYPE::EXILE, this);
}

void BetrayalOfFlesh::Kill()
{
	std::vector<BaseCard*> targets = this->game->p1->battlefield->GetCardsOfType(CARD_TYPE::CREATURE);
	assert(!targets.empty());
	int randIndex = rand() % targets.size();
	BaseCard* card = targets[randIndex];
	this->game->MoveCardToZone(1, ZONE_TYPE::GRAVEYARD, card);
	this->game->outstr << "Killing " << card->GetLabel() << " with Betrayal of Flesh\n";
}

void BetrayalOfFlesh::Revive()
{
	std::vector<BaseCard*> targets = this->game->p1->graveyard->GetCardsOfType(CARD_TYPE::CREATURE);
	assert(!targets.empty());
	int randIndex = rand() % targets.size();
	BaseCard* card = targets[randIndex];
	this->game->MoveCardToZone(1, ZONE_TYPE::BATTLEFIELD, card);
	this->game->outstr << "Reviving " << card->GetLabel() << " with Betrayal of Flesh\n";
}

bool InfernalTutor::CanCast()
{
	bool canDiscard = false;
	for each (BaseCard * card in this->game->p1->hand->list)
	{
		if (this->game->p1->library->ContainsOfCardName(card->GetName()) >= 1)
		{
			canDiscard = true;
			break;
		}
	}

	return this->game->p1->untappedLand >= 1
		&& (this->zone == ZONE_TYPE::HAND && (this->game->p1->hand->list.size() == 1 || canDiscard)
			|| (this->zone == ZONE_TYPE::GRAVEYARD && this->hasFlashback && (this->game->p1->hand->list.size() == 0 || canDiscard)));
}

void InfernalTutor::Cast()
{
	assert(this->CanCast());
	this->game->p1->TapLand(1);
	this->game->p1->spellsCast++;

	this->game->MoveCardToZone(1, this->zone == ZONE_TYPE::HAND ? ZONE_TYPE::GRAVEYARD : ZONE_TYPE::EXILE, this);

	// Hellbent mode
	if (this->game->p1->hand->list.size() == 0)
	{
		std::vector<BaseCard*> targets = this->game->p1->library->GetAllCards();
		if (!targets.empty()) {
			int randIndex = rand() % targets.size();
			BaseCard* target = targets[randIndex];
			this->game->MoveCardToZone(1, ZONE_TYPE::HAND, target);
			this->game->outstr << "Tutored for " << target->GetLabel() << "\n";
		}
		else {
			this->game->outstr << "Nothing to tutor for" << "\n";
		}

	}
	else // Normal mode
	{
		std::vector<BaseCard*> hand = this->game->p1->hand->GetAllCards();
		std::vector<BaseCard*> discardables;
		assert(hand.empty() == false);
		BaseCard* discard = nullptr;

		for (unsigned int i = 0; i < hand.size(); ++i)
		{
			if (this->game->p1->library->ContainsOfCardName(hand[i]->GetName()) >= 1)
			{
				discardables.push_back(hand[i]);
			}
		}

		assert(discardables.size() > 0);

		int randIndex = rand() % discardables.size();
		discard = discardables[randIndex];

		this->game->outstr << "Discarding " << discard->GetLabel() << " to Infernal tutor\n";
		std::vector<BaseCard*> targets = this->game->p1->library->GetCardsWithName(discard->GetName());
		if (targets.size() > 0)
		{
			int randIndex = rand() % targets.size();
			BaseCard* target = targets[randIndex];
			this->game->MoveCardToZone(1, ZONE_TYPE::HAND, target);
			this->game->outstr << "Tutored for " << target->GetLabel() << "\n";
		}
		else
		{
			this->game->outstr << "Found nothing\n";
		}

		this->game->MoveCardToZone(1, ZONE_TYPE::GRAVEYARD, discard);

	}

	//this->game->MoveCardToZone( 1, this->zone == HAND ? GRAVEYARD : EXILE, this );
}

bool Coercion::CanCast()
{
	return false;
	/*return this->game->p1->untappedLand >= 2
		&& this->game->p2->battlefield->ContainsOfCardName(LEYLINE_OF_SANCTITY) == 0
		&& (this->zone == HAND || this->zone == GRAVEYARD && this->hasFlashback);*/
}

void Coercion::Cast()
{
	assert(this->CanCast());

	this->game->p1->TapLand(2);
	this->game->p1->spellsCast++;

	std::vector<BaseCard*> targets = this->game->p2->hand->GetAllCards();

	if (!targets.empty())
	{
		int randIndex = rand() % targets.size();
		BaseCard* target = targets[randIndex];
		this->game->MoveCardToZone(2, ZONE_TYPE::GRAVEYARD, target);
		this->game->outstr << "Discarded " << target->GetLabel() << "\n";
	}
	else
	{
		this->game->outstr << "Discarded nothing\n";
	}
	this->game->MoveCardToZone(1, this->zone == ZONE_TYPE::HAND ? ZONE_TYPE::GRAVEYARD : ZONE_TYPE::EXILE, this);
}

bool Brainspoil::CanCast()
{
	return this->CanKill() || this->CanTransmute();
}

bool Brainspoil::CanKill() {
	return false;
	/*return this->game->p1->battlefield->ContainsOfCardType(CREATURE) >= 1 &&
		this->game->p1->untappedLand >= 4
		&& (this->zone == HAND || this->zone == GRAVEYARD && this->hasFlashback);*/
}

bool Brainspoil::CanTransmute() {
	return false;
	/*return this->game->p1->untappedLand >= 3
		&& this->zone == HAND;*/
}

void Brainspoil::Cast()
{
	assert(this->CanCast());

	bool canKill = this->CanKill();
	bool canTransmute = this->CanTransmute();

	if (canKill && canTransmute)
	{
		if (rand() % 2)
		{
			this->Kill();
		}
		else
		{
			this->Transmute();
		}
	}
	else if (canKill)
	{
		this->Kill();
	}
	else if (canTransmute)
	{
		this->Transmute();
	}
	else
	{
		assert(false);
	}
	this->game->MoveCardToZone(1, this->zone == ZONE_TYPE::HAND ? ZONE_TYPE::GRAVEYARD : ZONE_TYPE::EXILE, this);
}

void Brainspoil::Kill()
{
	assert(this->game->p1->battlefield->ContainsOfCardType(CARD_TYPE::CREATURE) >= 1);

	this->game->p1->TapLand(4);
	if (this->zone == ZONE_TYPE::GRAVEYARD) {
		this->game->MoveCardToZone(1, ZONE_TYPE::EXILE, this);
	}
	else {
		this->game->MoveCardToZone(1, ZONE_TYPE::GRAVEYARD, this);
	}

	this->game->p1->spellsCast++;

	std::vector<BaseCard*> targets = this->game->p1->battlefield->GetCardsOfType(CARD_TYPE::CREATURE);
	assert(!targets.empty());

	int randIndex = rand() % targets.size();
	BaseCard* target = targets[randIndex];
	this->game->MoveCardToZone(1, ZONE_TYPE::GRAVEYARD, target);
	this->game->outstr << "Killed " << target->GetLabel() << "\n";
}

void Brainspoil::Transmute()
{
	this->game->p1->TapLand(3);
	std::vector<BaseCard*> targets = this->game->p1->library->GetCardsWithCost(5);
	if (!targets.empty())
	{
		int randIndex = rand() % targets.size();
		BaseCard* target = targets[randIndex];
		this->game->MoveCardToZone(1, ZONE_TYPE::HAND, target);
		this->game->outstr << "Transmuted for " << target->GetLabel() << "\n";
	}
	else
	{
		this->game->outstr << "Transmuted for nothing\n";
	}
}

bool DemonicCollusion::CanCast()
{
	return this->game->p1->untappedLand >= 4
		&& (this->zone == ZONE_TYPE::HAND || this->zone == ZONE_TYPE::GRAVEYARD && this->hasFlashback);
}

void DemonicCollusion::Cast()
{
	assert(this->CanCast());
	ZONE_TYPE startingZone = this->zone;

	this->game->MoveCardToZone(1, this->zone == ZONE_TYPE::HAND ? ZONE_TYPE::GRAVEYARD : ZONE_TYPE::EXILE, this);
	this->game->p1->spellsCast++;
	this->game->p1->TapLand(4);
	bool canBuyback = this->game->p1->hand->list.size() >= 2 && startingZone == ZONE_TYPE::HAND;

	if (canBuyback && rand() % 2)
	{
		std::vector<BaseCard*> hand = this->game->p1->hand->GetAllCards();

		BaseCard* discard1 = nullptr;
		BaseCard* discard2 = nullptr;

		int randIndex = rand() % hand.size();
		discard1 = hand[randIndex];
		this->game->MoveCardToZone(1, ZONE_TYPE::GRAVEYARD, discard1);

		hand = this->game->p1->hand->GetAllCards();
		randIndex = rand() % hand.size();
		discard2 = hand[randIndex];
		this->game->MoveCardToZone(1, ZONE_TYPE::GRAVEYARD, discard2);

		if (this->zone == ZONE_TYPE::GRAVEYARD)
		{
			this->game->MoveCardToZone(1, ZONE_TYPE::HAND, this);
		}
		this->game->outstr << "Buybacked: discarding " << discard1->GetLabel() << " and " << discard2->GetLabel() << "\n";
	}

	std::vector<BaseCard*> targets = this->game->p1->library->GetAllCards();
	if (targets.size() > 0)
	{
		int randIndex = rand() % targets.size();
		BaseCard* target = targets[randIndex];
		this->game->MoveCardToZone(1, ZONE_TYPE::HAND, target);
		this->game->outstr << "Tutored for " << target->GetLabel() << "\n";
	}
}

bool LeylineOfSanctity::CanCast()
{
	assert(false);
	return false;
}
void LeylineOfSanctity::Cast()
{
	assert(false);
}

bool SneakAttack::CanCast()
{
	return this->game->p1->hand->ContainsOfCardType(CARD_TYPE::CREATURE) >= 1
		&& this->game->p1->untappedLand >= 1
		&& this->zone == ZONE_TYPE::BATTLEFIELD;
}

void SneakAttack::Cast()
{
	assert(this->CanCast());

	this->game->p1->TapLand(1);
	std::vector<BaseCard*> targets = this->game->p1->hand->GetCardsOfType(CARD_TYPE::CREATURE);

	int randIndex = rand() % targets.size();
	BaseCard* target = targets[randIndex];
	this->game->MoveCardToZone(1, ZONE_TYPE::BATTLEFIELD, target);
	target->haste = true;

	if (target->GetName() == CARD_NAME::GREAT_WHALE)
	{
		this->game->p1->UntapLand(7);
	}
	this->game->outstr << "Put " << target->GetLabel() << " on the battlefield\n";
}

bool SorinsVengeance::CanCast()
{
	return this->game->p1->untappedLand >= 6
		&& this->game->p2->battlefield->ContainsOfCardName(CARD_NAME::LEYLINE_OF_SANCTITY) == 0
		&& (this->zone == ZONE_TYPE::HAND || this->zone == ZONE_TYPE::GRAVEYARD && this->hasFlashback);
}

void SorinsVengeance::Cast()
{
	assert(this->CanCast());

	this->game->p1->TapLand(6);
	this->game->p1->life += 10;
	this->game->p2->life -= 10;

	this->game->MoveCardToZone(1, ZONE_TYPE::GRAVEYARD, this);
	this->game->p1->spellsCast++;

	this->game->outstr << "Damage dealt\n";
}


bool Snap::CanCast()
{
	return this->game->p1->battlefield->ContainsOfCardType(CARD_TYPE::CREATURE)
		&& this->game->p1->untappedLand >= 1
		&& (this->zone == ZONE_TYPE::HAND || this->zone == ZONE_TYPE::GRAVEYARD && this->hasFlashback);
}

void Snap::Cast()
{
	assert(this->CanCast());

	this->game->p1->TapLand(1);
	this->game->p1->UntapLand(2);

	std::vector<BaseCard*> targets = this->game->p1->battlefield->GetCardsOfType(CARD_TYPE::CREATURE);
	assert(targets.size() >= 1);
	int randIndex = rand() % targets.size();
	BaseCard* target = targets[randIndex];

	this->game->MoveCardToZone(1, ZONE_TYPE::HAND, target);
	this->game->MoveCardToZone(1, this->zone == ZONE_TYPE::HAND ? ZONE_TYPE::GRAVEYARD : ZONE_TYPE::EXILE, this);

	this->game->p1->spellsCast++;

	this->game->outstr << "Returned " << target->GetLabel() << " to hand\n";
}

bool OneWithNothing::CanCast()
{
	return this->game->p1->untappedLand >= 1
		&& (this->zone == ZONE_TYPE::HAND
			|| (this->hasFlashback && this->zone == ZONE_TYPE::GRAVEYARD));
}

void OneWithNothing::Cast()
{
	assert(this->CanCast());
	this->game->p1->TapLand(1);
	std::vector<BaseCard*> targets = this->game->p1->hand->GetAllCards();

	for (unsigned int i = 0; i < targets.size(); ++i)
	{
		this->game->MoveCardToZone(1, ZONE_TYPE::GRAVEYARD, targets[i]);
		this->game->outstr << "Discarded " << targets[i]->GetLabel() << "\n";
	}

	this->game->p1->spellsCast++;
}

bool ChimneyImp::CanCast()
{
	return false;//this->game->p1->untappedLand >= 4 && 
		//( this->zone == HAND || 
			//this->haste && this->zone == BATTLEFIELD;
		//);
}

void ChimneyImp::Cast()
{
	assert(this->CanCast());
	/*if ( this->haste && this->zone == BATTLEFIELD )
	{
		this->game->p2->life -= 1;
		this->haste = false;
		this->game->outstr << "Attacked for 1 damage\n";
	}*/
	/*else if ( this->zone == HAND )
	{
		this->game->MoveCardToZone( 1, BATTLEFIELD, this );
		this->game->outstr << "Put Chimney Imp on the battlefield\n";
		this->game->p1->spellsCast++;
		this->game->p1->TapLand( 4 );
	}*/

}

bool PastInFlames::CanCast()
{
	return this->game->p1->untappedLand >= 3 && this->zone == ZONE_TYPE::HAND
		|| this->game->p1->untappedLand >= 4 && this->zone == ZONE_TYPE::GRAVEYARD;
}

void PastInFlames::Cast()
{
	assert(this->CanCast());

	if (this->zone == ZONE_TYPE::GRAVEYARD)
	{
		this->game->p1->TapLand(4);
		this->game->MoveCardToZone(1, ZONE_TYPE::EXILE, this);
	}

	std::vector<BaseCard*> instants = this->game->p1->graveyard->GetCardsOfType(CARD_TYPE::INSTANT);
	std::vector<BaseCard*> sorceries = this->game->p1->graveyard->GetCardsOfType(CARD_TYPE::SORCERY);
	for (unsigned int i = 0; i < instants.size(); ++i)
	{
		this->game->outstr << "Gave " << instants[i]->GetLabel() << " flashback\n";
		instants[i]->hasFlashback = true;
	}

	for (unsigned int i = 0; i < sorceries.size(); ++i)
	{
		this->game->outstr << "Gave " << sorceries[i]->GetLabel() << " flashback\n";
		sorceries[i]->hasFlashback = true;
	}

	if (this->zone == ZONE_TYPE::HAND)
	{
		this->game->p1->TapLand(3);
		this->game->MoveCardToZone(1, ZONE_TYPE::GRAVEYARD, this);
	}

	this->game->p1->spellsCast++;
}

bool TendrilsOfAgony::CanCast()
{
	if (this->game->p2->battlefield->ContainsOfCardName(CARD_NAME::LEYLINE_OF_SANCTITY)) {
		return false;
	}
	return this->game->p1->untappedLand >= 3
		&& (this->zone == ZONE_TYPE::HAND || this->zone == ZONE_TYPE::GRAVEYARD && this->hasFlashback);
}

void TendrilsOfAgony::Cast()
{
	assert(this->CanCast());

	this->game->p2->life -= (1 + this->game->p1->spellsCast) * 2;
	this->game->p1->life += (1 + this->game->p1->spellsCast) * 2;

	if (this->zone == ZONE_TYPE::GRAVEYARD)
	{
		this->game->MoveCardToZone(1, ZONE_TYPE::EXILE, this);
	}
	else
	{
		this->game->MoveCardToZone(1, ZONE_TYPE::GRAVEYARD, this);
	}
	this->game->p1->TapLand(3);
	this->game->p1->spellsCast++;
	this->game->outstr << "Hit with " << this->game->p1->spellsCast << " tendrils\n";
}

bool SpittingImage::CanCast()
{
	return this->game->p1->battlefield->ContainsOfCardType(CARD_TYPE::CREATURE) >= 1
		&& this->game->p1->untappedLand >= 5
		&& (this->zone == ZONE_TYPE::HAND || this->zone == ZONE_TYPE::GRAVEYARD && this->hasFlashback
			|| this->zone == ZONE_TYPE::GRAVEYARD && this->game->p1->hand->ContainsOfCardType(CARD_TYPE::LAND) >= 1);
}

void SpittingImage::Cast()
{
	assert(this->CanCast());

	if (this->zone == ZONE_TYPE::HAND)
	{
		this->game->MoveCardToZone(1, ZONE_TYPE::GRAVEYARD, this);
	}
	else if (this->zone == ZONE_TYPE::GRAVEYARD)
	{
		bool canFlashback = this->hasFlashback;
		bool canRetrace = this->game->p1->hand->ContainsOfCardType(CARD_TYPE::LAND) >= 1;

		if (canFlashback && !canRetrace)
		{
			this->game->MoveCardToZone(1, ZONE_TYPE::EXILE, this);
		}
		else if (!canFlashback && canRetrace)
		{
			this->Retrace();
		}
		else if (canFlashback && canRetrace)
		{
			if (rand() % 2) // Flashback
			{
				this->game->MoveCardToZone(1, ZONE_TYPE::EXILE, this);
			}
			else
			{
				this->Retrace();
			}
		}
		else
		{
			assert(false);
		}
	}

	std::vector<BaseCard*> targets = this->game->p1->battlefield->GetCardsOfType(CARD_TYPE::CREATURE);
	int randIndex = rand() % targets.size();
	BaseCard* target = targets[randIndex];

	this->game->p1->TapLand(5);
	this->game->p1->spellsCast++;

	if (target->GetName() == CARD_NAME::GREAT_WHALE)
	{
		GreatWhale* newWhale = new GreatWhale(this->game);
		this->game->p1->UntapLand(7);
		this->game->p1->battlefield->AddCard(newWhale);
		this->game->outstr << "Created a copy of Great Whale\n";
	}
	else if (target->GetName() == CARD_NAME::CHIMNEY_IMP)
	{
		ChimneyImp* newImp = new ChimneyImp(this->game);
		this->game->p1->battlefield->AddCard(newImp);
		this->game->outstr << "Created a copy of Chimney Imp\n";
	}
	else
	{
		assert(false);
	}
}

void SpittingImage::Retrace()
{
	std::vector<BaseCard*> discards = this->game->p1->hand->GetCardsOfType(CARD_TYPE::LAND);
	assert(discards.size() > 0);

	int randIndex = rand() % discards.size();
	BaseCard* discard = discards[randIndex];

	this->game->MoveCardToZone(1, ZONE_TYPE::GRAVEYARD, discard);
	this->game->outstr << "Retraced discarding " << discard->GetLabel() << "\n";
}

bool GreatWhale::CanCast()
{
	return (this->zone == ZONE_TYPE::BATTLEFIELD && this->haste && !this->game->p1->attacked) ||
		(this->zone == ZONE_TYPE::HAND && this->game->p1->untappedLand >= 6);
}

void GreatWhale::Cast()
{
	assert(this->CanCast());

	if (this->zone == ZONE_TYPE::BATTLEFIELD && this->haste && !this->game->p1->attacked)
	{
		this->game->p2->life -= 5;
		this->game->p1->attacked = true;
		this->game->outstr << "Attacked for 5 damage\n";
	}
	else if (this->zone == ZONE_TYPE::HAND)
	{
		this->game->MoveCardToZone(1, ZONE_TYPE::BATTLEFIELD, this);
		this->game->p1->TapLand(6);
		this->game->p1->UntapLand(7);
		this->game->p1->spellsCast++;
		this->game->outstr << "Cast Great Whale\n";
	}
	else
	{
		assert(false);
	}
}

bool Hoodwink::CanCast()
{
	return this->game->p1->untappedLand >= 1
		&& (this->zone == ZONE_TYPE::HAND || this->zone == ZONE_TYPE::GRAVEYARD && this->hasFlashback);
}

void Hoodwink::Cast()
{
	assert(this->CanCast());

	bool hasLeyline = this->game->p2->battlefield->ContainsOfCardName(CARD_NAME::LEYLINE_OF_SANCTITY) >= 1;

	if (hasLeyline && rand() % 2) // Bounce enchantment
	{
		BaseCard* leyline = this->game->p2->battlefield->GetCardsWithName(CARD_NAME::LEYLINE_OF_SANCTITY)[0];
		this->game->MoveCardToZone(2, ZONE_TYPE::HAND, leyline);
		this->game->outstr << "Bounced leyline\n";
	}
	else // Bounce land
	{
		std::vector<BaseCard*> targets = this->game->p1->battlefield->GetCardsOfType(CARD_TYPE::LAND);
		assert(targets.size() > 0);
		this->game->MoveCardToZone(1, ZONE_TYPE::HAND, targets[0]);
		this->game->p1->totalLand--;
		this->game->outstr << "Bounced land\n";
	}
	this->game->p1->TapLand(1);
	this->game->p1->spellsCast++;
	this->game->MoveCardToZone(1, this->zone == ZONE_TYPE::HAND ? ZONE_TYPE::GRAVEYARD : ZONE_TYPE::EXILE, this);
}

bool BasicLand::CanCast()
{
	return this->zone == ZONE_TYPE::HAND;
}

void BasicLand::Cast()
{
	assert(this->CanCast());
	this->game->MoveCardToZone(1, ZONE_TYPE::BATTLEFIELD, this);
	this->game->p1->untappedLand++;
	this->game->p1->totalLand++;

	this->game->outstr << "Put land on battlefield\n";
}