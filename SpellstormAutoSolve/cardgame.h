#ifndef CARDGAME_H_
#define CARDGAME_H_

#include <vector>
#include <list>
#include <map>
#include <sstream>
#include "basecard.h"

class Zone
{
public:
	Zone(  ZONE_TYPE _type );
	~Zone();

	ZONE_TYPE type;

	std::list<BaseCard*> list;

	int ContainsOfCardName( CARD_NAME _cardName );
	int ContainsOfCardType( CARD_TYPE _cardType );

	std::vector<BaseCard*> GetCardsOfType( CARD_TYPE _cardType );
	std::vector<BaseCard*> GetCardsWithName( CARD_NAME _cardName );

	std::vector<BaseCard*> GetCardsWithCost( int _cmc );

	std::vector<BaseCard*> GetAllCards();

	void AddCard( BaseCard* _card );
};

class Player
{
public:
	Player();
	~Player();

	Zone* battlefield;
	Zone* hand;
	Zone* graveyard;
	Zone* exile;
	Zone* library;

	std::map<ZONE_TYPE, Zone*> zones;

	int life;
	int totalLand;
	int untappedLand;
	int spellsCast;

	bool attacked;

	void TapLand( int _number );
	void UntapLand( int _number );
};

class Game
{
public:
	Game();
	~Game();

	static Game* instance;
	Player* p1;
	Player* p2;

	void MoveCardToZone( int _player, ZONE_TYPE _to, BaseCard* _card );

	static std::ostringstream outstr;
};


#endif