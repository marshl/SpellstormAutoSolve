#include "cardgame.h"

#include <cassert>
#include <algorithm>

#include "basecard.h"

Game* Game::instance = nullptr;

Zone::Zone( ZONE_TYPE _type )
{
	this->type = _type;
}

Zone::~Zone()
{
	for each ( BaseCard* card in this->list )
	{
		delete card;
	}
}

int Zone::ContainsOfCardName( CARD_NAME _cardName )
{
	int total = 0;
	
	for each ( BaseCard* card in this->list )
	{
		if ( card->GetName() == _cardName )
		{
			++total;
		}
	}
	return total;
}

int Zone::ContainsOfCardType( CARD_TYPE _cardType )
{
	int total = 0;
	
	for each ( BaseCard* card in this->list )
	{
		if ( card->GetType() == _cardType )
		{
			++total;
		}
	}
	return total;
}

std::vector<BaseCard*> Zone::GetCardsWithName( CARD_NAME _cardName )
{
	std::vector<BaseCard*> out;
	for each ( BaseCard* card in this->list )
	{
		if ( card->GetName() == _cardName )
		{
			out.push_back( card );
		}
	}
	return out;
}

std::vector<BaseCard*> Zone::GetCardsWithCost( int _cmc )
{
	std::vector<BaseCard*> out;
	for each ( BaseCard* card in this->list )
	{
		if ( card->GetCost() == _cmc )
		{
			out.push_back( card );
		}
	}
	return out;
}

std::vector<BaseCard*> Zone::GetAllCards()
{
	std::vector<BaseCard*> out;
	for each ( BaseCard* card in this->list )
	{
		out.push_back( card );
	}
	return out;
}

void Zone::AddCard( BaseCard* _card )
{
	assert( _card->zone == ZONETYPE_UNDEF );
	assert( std::find( this->list.begin(), this->list.end(), _card ) == this->list.end() );
	this->list.push_back( _card );
	_card->zone = this->type;
}

std::vector<BaseCard*> Zone::GetCardsOfType( CARD_TYPE _cardType )
{
	std::vector<BaseCard*> out;
	for each ( BaseCard* card in this->list )
	{
		if ( card->GetType() == _cardType )
		{
			out.push_back( card );
		}
	}
	return out;
}

Player::Player()
{
	this->zones.insert( std::pair<ZONE_TYPE, Zone*>( BATTLEFIELD, this->battlefield = new Zone(BATTLEFIELD) ) );
	this->zones.insert( std::pair<ZONE_TYPE, Zone*>( HAND, this->hand = new Zone(HAND) ) );
	this->zones.insert( std::pair<ZONE_TYPE, Zone*>( GRAVEYARD, this->graveyard = new Zone(GRAVEYARD) ) );
	this->zones.insert( std::pair<ZONE_TYPE, Zone*>( LIBRARY, this->library = new Zone(LIBRARY) ) );
	this->zones.insert( std::pair<ZONE_TYPE, Zone*>( EXILE, this->exile = new Zone(EXILE) ) );

	this->spellsCast = 0;
}

Player::~Player()
{
	delete this->battlefield;
	delete this->hand;
	delete this->graveyard;
	delete this->library;
	delete this->exile;
}

void Player::TapLand( int _number )
{
	assert( this->untappedLand >= _number );
	this->untappedLand -= _number;
}

void Player::UntapLand( int _number )
{
	this->untappedLand = std::min( this->untappedLand + _number, this->totalLand );
}

Game::Game()
{
	this->p1 = new Player();
	this->p2 = new Player();
}

Game::~Game()
{
	delete this->p1;
	delete this->p2;
}

void Game::MoveCardToZone( int _playerID, ZONE_TYPE _to, BaseCard* _card )
{
	assert( _card->zone != ZONETYPE_UNDEF );
	//assert( _from != _to );
	assert( _to != ZONETYPE_UNDEF );
	assert( _card != nullptr );
	assert( _playerID == 1 || _playerID == 2 );
	Player* player = _playerID == 1 ? this->p1 : this->p2;

	Zone* f = player->zones[ _card->zone ];
	Zone* t = player->zones[ _to ];

	assert( std::find( f->list.begin(), f->list.end(), _card ) != f->list.end() );

	f->list.remove( _card );
	t->list.push_back( _card );
	_card->zone = _to;
}
