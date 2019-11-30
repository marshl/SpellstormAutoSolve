#include "basecard.h"

#include <cassert>
#include <iostream>

#include "cardgame.h"

BaseCard::BaseCard()
{
	this->haste = false;
	this->hasFlashback = false;
	this->isToken = false;
	this->zone = ZONETYPE_UNDEF;
}

bool  BetrayalOfFlesh::CanCast()
{
	return Game::instance->p1->untappedLand >= 5 &&
		(Game::instance->p1->graveyard->ContainsOfCardType(CREATURE) > 0
			|| Game::instance->p1->battlefield->ContainsOfCardType(CREATURE) > 0)
		&& (this->zone == HAND || this->zone == GRAVEYARD && this->hasFlashback);
}

void BetrayalOfFlesh::Cast()
{
	assert(this->CanCast());

	bool canRevive = Game::instance->p1->graveyard->ContainsOfCardType(CREATURE) > 0;
	bool canKill = Game::instance->p1->battlefield->ContainsOfCardType(CREATURE) > 0;

	assert(Game::instance->p1->untappedLand >= 5);
	assert(canKill || canRevive);

	Game::instance->p1->TapLand(5);
	Game::instance->p1->spellsCast++;

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
			Game::instance->p1->totalLand -= 3;
		}
	}

	Game::instance->MoveCardToZone(1, this->zone == HAND ? GRAVEYARD : EXILE, this);
}

void BetrayalOfFlesh::Kill()
{
	std::vector<BaseCard*> targets = Game::instance->p1->battlefield->GetCardsOfType(CREATURE);
	assert(!targets.empty());
	int randIndex = rand() % targets.size();
	BaseCard* card = targets[randIndex];
	Game::instance->MoveCardToZone(1, GRAVEYARD, card);
	Game::outstr << "Killing " << card->GetLabel() << " with Betrayal of Flesh\n";
}

void BetrayalOfFlesh::Revive()
{
	std::vector<BaseCard*> targets = Game::instance->p1->graveyard->GetCardsOfType(CREATURE);
	assert(!targets.empty());
	int randIndex = rand() % targets.size();
	BaseCard* card = targets[randIndex];
	Game::instance->MoveCardToZone(1, BATTLEFIELD, card);
	Game::outstr << "Reviving " << card->GetLabel() << " with Betrayal of Flesh\n";
}

bool InfernalTutor::CanCast()
{
	bool canDiscard = false;
	for each (BaseCard * card in Game::instance->p1->hand->list)
	{
		if (Game::instance->p1->library->ContainsOfCardName(card->GetName()) >= 1)
		{
			canDiscard = true;
			break;
		}
	}

	return Game::instance->p1->untappedLand >= 1
		&& (this->zone == HAND && (Game::instance->p1->hand->list.size() == 1 || canDiscard)
			|| this->zone == GRAVEYARD && this->hasFlashback && canDiscard);
}

void InfernalTutor::Cast()
{
	assert(this->CanCast());
	Game::instance->p1->TapLand(1);
	Game::instance->p1->spellsCast++;

	Game::instance->MoveCardToZone(1, this->zone == HAND ? GRAVEYARD : EXILE, this);

	// Hellbent mode
	if (Game::instance->p1->hand->list.size() == 0)
	{
		std::vector<BaseCard*> targets = Game::instance->p1->library->GetAllCards();
		assert(targets.empty() == false);
		int randIndex = rand() % targets.size();
		BaseCard* target = targets[randIndex];
		Game::instance->MoveCardToZone(1, HAND, target);
		Game::outstr << "Tutored for " << target->GetLabel() << "\n";
	}
	else // Normal mode
	{
		std::vector<BaseCard*> hand = Game::instance->p1->hand->GetAllCards();
		std::vector<BaseCard*> discardables;
		assert(hand.empty() == false);
		BaseCard* discard = nullptr;

		for (unsigned int i = 0; i < hand.size(); ++i)
		{
			if (Game::instance->p1->library->ContainsOfCardName(hand[i]->GetName()) >= 1)
			{
				discardables.push_back(hand[i]);
			}
		}

		assert(discardables.size() > 0);

		int randIndex = rand() % discardables.size();
		discard = discardables[randIndex];

		Game::outstr << "Discarding " << discard->GetLabel() << " to Infernal tutor\n";
		std::vector<BaseCard*> targets = Game::instance->p1->library->GetCardsWithName(discard->GetName());
		if (targets.size() > 0)
		{
			int randIndex = rand() % targets.size();
			BaseCard* target = targets[randIndex];
			Game::instance->MoveCardToZone(1, HAND, target);
			Game::outstr << "Tutored for " << target->GetLabel() << "\n";
		}
		else
		{
			Game::outstr << "Found nothing\n";
		}

		Game::instance->MoveCardToZone(1, GRAVEYARD, discard);

	}

	//Game::instance->MoveCardToZone( 1, this->zone == HAND ? GRAVEYARD : EXILE, this );
}

bool Coercion::CanCast()
{
	return Game::instance->p1->untappedLand >= 2
		&& Game::instance->p2->battlefield->ContainsOfCardName(LEYLINE_OF_SANCTITY) == 0
		&& (this->zone == HAND || this->zone == GRAVEYARD && this->hasFlashback);
}

void Coercion::Cast()
{
	assert(this->CanCast());

	Game::instance->p1->TapLand(2);
	Game::instance->p1->spellsCast++;

	std::vector<BaseCard*> targets = Game::instance->p2->hand->GetAllCards();

	if (!targets.empty())
	{
		int randIndex = rand() % targets.size();
		BaseCard* target = targets[randIndex];
		Game::instance->MoveCardToZone(2, GRAVEYARD, target);
		Game::outstr << "Discarded " << target->GetLabel() << "\n";
	}
	else
	{
		Game::outstr << "Discarded nothing\n";
	}
	Game::instance->MoveCardToZone(1, this->zone == HAND ? GRAVEYARD : EXILE, this);
}

bool Brainspoil::CanCast()
{
	return (Game::instance->p1->untappedLand >= 4
		&& Game::instance->p1->battlefield->ContainsOfCardType(CREATURE) >= 1
		|| Game::instance->p1->untappedLand >= 3)
		&& (this->zone == HAND || this->zone == GRAVEYARD && this->hasFlashback);
}

void Brainspoil::Cast()
{
	assert(this->CanCast());

	bool canKill = (Game::instance->p1->untappedLand >= 4
		&& Game::instance->p1->battlefield->ContainsOfCardType(CREATURE) >= 1);

	bool canTransmute = Game::instance->p1->untappedLand >= 3;

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
	Game::instance->MoveCardToZone(1, this->zone == HAND ? GRAVEYARD : EXILE, this);
}

void Brainspoil::Kill()
{
	assert(Game::instance->p1->battlefield->ContainsOfCardType(CREATURE) >= 1);

	Game::instance->p1->TapLand(4);
	Game::instance->MoveCardToZone(1, GRAVEYARD, this);
	Game::instance->p1->spellsCast++;

	std::vector<BaseCard*> targets = Game::instance->p1->battlefield->GetCardsOfType(CREATURE);
	assert(!targets.empty());

	int randIndex = rand() % targets.size();
	BaseCard* target = targets[randIndex];
	Game::instance->MoveCardToZone(1, GRAVEYARD, target);
	Game::outstr << "Killed " << target->GetLabel() << "\n";
}

void Brainspoil::Transmute()
{
	Game::instance->p1->TapLand(3);

	std::vector<BaseCard*> targets = Game::instance->p1->library->GetCardsWithCost(5);
	if (!targets.empty())
	{
		int randIndex = rand() % targets.size();
		BaseCard* target = targets[randIndex];
		Game::instance->MoveCardToZone(1, HAND, target);
		Game::outstr << "Transmuted for " << target->GetLabel() << "\n";
	}
	else
	{
		Game::outstr << "Transmuted for nothing\n";
	}
}

bool DemonicCollusion::CanCast()
{
	return Game::instance->p1->untappedLand >= 4
		&& (this->zone == HAND || this->zone == GRAVEYARD && this->hasFlashback);
}

void DemonicCollusion::Cast()
{
	assert(Game::instance->p1->untappedLand >= 4);

	Game::instance->MoveCardToZone(1, this->zone == HAND ? GRAVEYARD : EXILE, this);
	Game::instance->p1->spellsCast++;
	Game::instance->p1->TapLand(4);
	bool canBuyback = Game::instance->p1->hand->list.size() >= 2;

	if (canBuyback && rand() % 2)
	{
		std::vector<BaseCard*> hand = Game::instance->p1->hand->GetAllCards();

		BaseCard* discard1 = nullptr;
		BaseCard* discard2 = nullptr;

		int randIndex = rand() % hand.size();
		discard1 = hand[randIndex];
		Game::instance->MoveCardToZone(1, GRAVEYARD, discard1);

		hand = Game::instance->p1->hand->GetAllCards();
		randIndex = rand() % hand.size();
		discard2 = hand[randIndex];
		Game::instance->MoveCardToZone(1, GRAVEYARD, discard2);

		if (this->zone == GRAVEYARD)
		{
			Game::instance->MoveCardToZone(1, HAND, this);
		}
		Game::outstr << "Buybacked: discarding " << discard1->GetLabel() << " and " << discard2->GetLabel() << "\n";
	}

	std::vector<BaseCard*> targets = Game::instance->p1->library->GetAllCards();
	if (targets.size() > 0)
	{
		int randIndex = rand() % targets.size();
		BaseCard* target = targets[randIndex];
		Game::instance->MoveCardToZone(1, HAND, target);
		Game::outstr << "Tutored for " << target->GetLabel() << "\n";
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
	return Game::instance->p1->hand->ContainsOfCardType(CREATURE) >= 1
		&& Game::instance->p1->untappedLand >= 1 && (Game::instance->p1->totalLand - Game::instance->p1->untappedLand - 1) >= 7
		&& this->zone == BATTLEFIELD;
}

void SneakAttack::Cast()
{
	assert(this->CanCast());

	Game::instance->p1->TapLand(1);
	std::vector<BaseCard*> targets = Game::instance->p1->hand->GetCardsOfType(CREATURE);

	int randIndex = rand() % targets.size();
	BaseCard* target = targets[randIndex];
	Game::instance->MoveCardToZone(1, BATTLEFIELD, target);
	target->haste = true;

	if (target->GetName() == GREAT_WHALE)
	{
		Game::instance->p1->UntapLand(7);
	}
	Game::outstr << "Put " << target->GetLabel() << " on the battlefield\n";
}

bool SorinsVengeance::CanCast()
{
	return Game::instance->p1->untappedLand >= 6
		&& Game::instance->p2->battlefield->ContainsOfCardName(LEYLINE_OF_SANCTITY) == 0
		&& (this->zone == HAND || this->zone == GRAVEYARD && this->hasFlashback);
}

void SorinsVengeance::Cast()
{
	assert(this->CanCast());

	Game::instance->p1->TapLand(6);
	Game::instance->p1->life += 10;
	Game::instance->p2->life -= 10;

	Game::instance->MoveCardToZone(1, GRAVEYARD, this);
	Game::instance->p1->spellsCast++;

	Game::outstr << "Damage dealt\n";
}


bool Snap::CanCast()
{
	return Game::instance->p1->battlefield->ContainsOfCardType(CREATURE)
		&& Game::instance->p1->untappedLand >= 1
		&& (this->zone == HAND || this->zone == GRAVEYARD && this->hasFlashback);
}

void Snap::Cast()
{
	assert(this->CanCast());

	Game::instance->p1->TapLand(1);
	Game::instance->p1->UntapLand(2);

	std::vector<BaseCard*> targets = Game::instance->p1->battlefield->GetCardsOfType(CREATURE);

	int randIndex = rand() % targets.size();
	BaseCard* target = targets[randIndex];

	Game::instance->MoveCardToZone(1, HAND, target);
	Game::instance->MoveCardToZone(1, this->zone == HAND ? GRAVEYARD : EXILE, this);

	Game::instance->p1->spellsCast++;

	Game::outstr << "Returned " << target->GetLabel() << " to hand\n";
}

bool OneWithNothing::CanCast()
{
	return Game::instance->p1->untappedLand >= 1
		&& (this->zone == HAND
			|| (this->hasFlashback && this->zone == GRAVEYARD));
}

void OneWithNothing::Cast()
{
	assert(this->CanCast());
	Game::instance->p1->TapLand(1);
	std::vector<BaseCard*> targets = Game::instance->p1->hand->GetAllCards();

	for (unsigned int i = 0; i < targets.size(); ++i)
	{
		Game::instance->MoveCardToZone(1, GRAVEYARD, targets[i]);
		Game::outstr << "Discarded " << targets[i]->GetLabel() << "\n";
	}

	Game::instance->p1->spellsCast++;
}

bool ChimneyImp::CanCast()
{
	return false;//Game::instance->p1->untappedLand >= 4 && 
		//( this->zone == HAND || 
			//this->haste && this->zone == BATTLEFIELD;
		//);
}

void ChimneyImp::Cast()
{
	assert(this->CanCast());
	/*if ( this->haste && this->zone == BATTLEFIELD )
	{
		Game::instance->p2->life -= 1;
		this->haste = false;
		Game::outstr << "Attacked for 1 damage\n";
	}*/
	/*else if ( this->zone == HAND )
	{
		Game::instance->MoveCardToZone( 1, BATTLEFIELD, this );
		Game::outstr << "Put Chimney Imp on the battlefield\n";
		Game::instance->p1->spellsCast++;
		Game::instance->p1->TapLand( 4 );
	}*/

}

bool PastInFlames::CanCast()
{
	return Game::instance->p1->untappedLand >= 3 && this->zone == HAND
		|| Game::instance->p1->untappedLand >= 4 && this->zone == GRAVEYARD;
}

void PastInFlames::Cast()
{
	assert(this->CanCast());

	if (this->zone == GRAVEYARD)
	{
		Game::instance->p1->TapLand(4);
		Game::instance->MoveCardToZone(1, EXILE, this);
	}

	std::vector<BaseCard*> instants = Game::instance->p1->graveyard->GetCardsOfType(INSTANT);
	std::vector<BaseCard*> sorceries = Game::instance->p1->graveyard->GetCardsOfType(SORCERY);
	for (unsigned int i = 0; i < instants.size(); ++i)
	{
		Game::outstr << "Gave " << instants[i]->GetLabel() << " flashback\n";
		instants[i]->hasFlashback = true;
	}

	for (unsigned int i = 0; i < sorceries.size(); ++i)
	{
		Game::outstr << "Gave " << sorceries[i]->GetLabel() << " flashback\n";
		sorceries[i]->hasFlashback = true;
	}

	if (this->zone == HAND)
	{
		Game::instance->p1->TapLand(3);
		Game::instance->MoveCardToZone(1, GRAVEYARD, this);
	}

	Game::instance->p1->spellsCast++;
}

bool TendrilsOfAgony::CanCast()
{
	return Game::instance->p1->untappedLand >= 3
		&& (this->zone == HAND || this->zone == GRAVEYARD && this->hasFlashback);
}

void TendrilsOfAgony::Cast()
{
	assert(this->CanCast());

	Game::instance->p2->life -= (1 + Game::instance->p1->spellsCast) * 2;
	Game::instance->p1->life += (1 + Game::instance->p1->spellsCast) * 2;

	if (this->zone == GRAVEYARD)
	{
		Game::instance->MoveCardToZone(1, EXILE, this);
	}
	else
	{
		Game::instance->MoveCardToZone(1, GRAVEYARD, this);
	}
	Game::instance->p1->TapLand(3);
	Game::instance->p1->spellsCast++;
	Game::outstr << "Hit with " << Game::instance->p1->spellsCast << " tendrils\n";
}

bool SpittingImage::CanCast()
{
	return Game::instance->p1->battlefield->ContainsOfCardType(CREATURE) >= 1
		&& Game::instance->p1->untappedLand >= 5
		&& (this->zone == HAND || this->zone == GRAVEYARD && this->hasFlashback
			|| this->zone == GRAVEYARD && Game::instance->p1->hand->ContainsOfCardType(LAND) >= 1);
}

void SpittingImage::Cast()
{
	assert(this->CanCast());

	if (this->zone == HAND)
	{
		Game::instance->MoveCardToZone(1, GRAVEYARD, this);
	}
	else if (this->zone == GRAVEYARD)
	{
		bool canFlashback = this->hasFlashback;
		bool canRetrace = Game::instance->p1->hand->ContainsOfCardType(LAND) >= 1;

		if (canFlashback && !canRetrace)
		{
			Game::instance->MoveCardToZone(1, EXILE, this);
		}
		else if (!canFlashback && canRetrace)
		{
			this->Retrace();
		}
		else if (canFlashback && canRetrace)
		{
			if (rand() % 2) // Flashback
			{
				Game::instance->MoveCardToZone(1, EXILE, this);
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

	std::vector<BaseCard*> targets = Game::instance->p1->battlefield->GetCardsOfType(CREATURE);
	int randIndex = rand() % targets.size();
	BaseCard* target = targets[randIndex];

	if (target->GetName() == GREAT_WHALE)
	{
		GreatWhale* newWhale = new GreatWhale();
		Game::instance->p1->UntapLand(7);
		Game::instance->p1->battlefield->AddCard(newWhale);
		Game::outstr << "Created a copy of Great Whale";
	}
	else if (target->GetName() == CHIMNEY_IMP)
	{
		ChimneyImp* newImp = new ChimneyImp();
		Game::instance->p1->battlefield->AddCard(newImp);
		Game::outstr << "Created a copy of Chimney Imp\n";
	}
	else
	{
		assert(false);
	}

	Game::instance->p1->TapLand(5);
	Game::instance->p1->spellsCast++;
}

void SpittingImage::Retrace()
{
	std::vector<BaseCard*> discards = Game::instance->p1->hand->GetCardsOfType(LAND);
	assert(discards.size() > 0);

	int randIndex = rand() % discards.size();
	BaseCard* discard = discards[randIndex];

	Game::instance->MoveCardToZone(1, GRAVEYARD, discard);
	Game::outstr << "Retraced discarding " << discard->GetLabel() << "\n";
}

bool GreatWhale::CanCast()
{
	return false;
	//return this->zone == BATTLEFIELD && this->haste;
	//	|| this->zone == HAND && Game::instance->p1->untappedLand >= 6;
}

void GreatWhale::Cast()
{
	assert(this->CanCast());

	/*if ( this->zone == BATTLEFIELD )
	{
		this->haste = false;
		Game::instance->p2->life -= 5;
		Game::outstr << "Attacked for 5 damage\n";
	}*/
	/*else if ( this->zone == HAND )
	{
		Game::instance->MoveCardToZone( 1, BATTLEFIELD, this );
		Game::instance->p1->TapLand( 6 );
		Game::instance->p1->UntapLand( 7 );
		Game::instance->p1->spellsCast++;
		Game::outstr << "Cast\n";
	}*/
	//else
	{
		assert(false);
	}
}

bool Hoodwink::CanCast()
{
	return Game::instance->p1->untappedLand >= 1
		&& (this->zone == HAND || this->zone == GRAVEYARD && this->hasFlashback);
}

void Hoodwink::Cast()
{
	assert(this->CanCast());

	bool canLeyline = Game::instance->p2->battlefield->ContainsOfCardName(LEYLINE_OF_SANCTITY) >= 1;

	if (canLeyline && rand() % 2) // Bounce enchantment
	{
		BaseCard* leyline = Game::instance->p2->battlefield->GetCardsWithName(LEYLINE_OF_SANCTITY)[0];
		Game::instance->MoveCardToZone(2, HAND, leyline);
		Game::outstr << "Bounced leyline\n";
	}
	else // Bounce land
	{
		std::vector<BaseCard*> targets = Game::instance->p1->battlefield->GetCardsOfType(LAND);
		assert(targets.size() > 0);
		Game::instance->MoveCardToZone(1, HAND, targets[0]);
		Game::instance->p1->totalLand--;
		Game::outstr << "Bounced land\n";
	}

	Game::instance->p1->spellsCast++;
	Game::instance->p1->TapLand(1);
	Game::instance->MoveCardToZone(1, this->zone == HAND ? GRAVEYARD : EXILE, this);
}

bool BasicLand::CanCast()
{
	return this->zone == HAND;
}

void BasicLand::Cast()
{
	assert(this->CanCast());
	Game::instance->MoveCardToZone(1, BATTLEFIELD, this);
	Game::instance->p1->untappedLand++;
	Game::instance->p1->totalLand++;

	Game::outstr << "Put land on battlefield\n";
}