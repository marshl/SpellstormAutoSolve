#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cassert>
#include <ctime>
#include <fstream>

#include "basecard.h"
#include "cardgame.h"


char* ZoneTypeToString(ZONE_TYPE _zone)
{
	switch (_zone)
	{
	case ZONE_TYPE::BATTLEFIELD:
		return "battlefield";
	case ZONE_TYPE::EXILE:
		return "exile";
	case ZONE_TYPE::HAND:
		return "hand";
	case ZONE_TYPE::GRAVEYARD:
		return "graveyard";
	case ZONE_TYPE::LIBRARY:
		return "library";
	default:
		assert(false);
		return "ERROR";
	};
}

Game* create_new_game() {
	//std::cout << i << "\r";
	Game* game = new Game();
	game->p1->life = 1;
	game->p2->life = 65;//65 - 5 - 1;
	game->p1->totalLand = game->p1->untappedLand = 19;

	Player* p1 = game->p1;
	Player* p2 = game->p2;

	// HAND
	p1->hand->AddCard(new BetrayalOfFlesh(game));
	p1->hand->AddCard(new InfernalTutor(game));
	p1->hand->AddCard(new Coercion(game));
	p1->hand->AddCard(new Brainspoil(game));
	p1->hand->AddCard(new DemonicCollusion(game));

	// Graveyard
	p1->graveyard->AddCard(new DemonicCollusion(game));

	// Battlefield
	p1->battlefield->AddCard(new SneakAttack(game));
	p2->battlefield->AddCard(new LeylineOfSanctity(game));

	for (int i = 0; i < 19; ++i)
	{
		p1->battlefield->AddCard(new BasicLand(game));
	}

	// Library
	//p1->library->AddCard(new SorinsVengeance());
	//p1->library->AddCard(new SorinsVengeance());
	p1->library->AddCard(new SorinsVengeance(game));

	p1->library->AddCard(new Brainspoil(game));
	p1->library->AddCard(new Snap(game));
	//p1->library->AddCard(new OneWithNothing());
	//p1->library->AddCard(new ChimneyImp());
	p1->library->AddCard(new PastInFlames(game));
	p1->library->AddCard(new TendrilsOfAgony(game));
	p1->library->AddCard(new SpittingImage(game));
	p1->library->AddCard(new Hoodwink(game));
	p1->library->AddCard(new GreatWhale(game));

	for (int i = 0; i < 3; ++i)
	{
		//p1->library->AddCard(new BasicLand(game));
	}

	return game;
}

int main(int argc, char* argv[])
{
	std::ofstream fout("output.txt");
	srand((unsigned int)time(NULL));

	//std::vector<int> currentMoves;
	//currentMoves.push_back(0);

	const int totalRounds = 1000000;
	for (long round = 0; round < totalRounds; ++round)
	{
		if (round % (totalRounds/ 100) == 0) {
			std::cout << "\r" << round;
		}
		
		Game* game = create_new_game();

		int turn = 0;
		while (true)
		{
			++turn;
			game->outstr << "\n +++ ROUND " << round << " STEP " << turn << " +++ \n";
			std::vector<BaseCard*> usableCards;
			for each (BaseCard * card in game->p1->battlefield->list)
			{
				if (card->CanCast())
				{
					usableCards.push_back(card);
				}
			}
			for each (BaseCard * card in game->p1->hand->list)
			{
				if (card->CanCast())
				{
					usableCards.push_back(card);
				}
			}
			for each (BaseCard * card in game->p1->graveyard->list)
			{
				if (card->CanCast())
				{
					usableCards.push_back(card);
				}
			}

			game->outstr << "Usable cards:\n";
			for each (BaseCard * card in usableCards)
			{
				game->outstr << " + " << card->GetLabel() << " is usable from " << ZoneTypeToString(card->zone) << "\n";
			}

			if (usableCards.empty())
			{
				game->outstr << "Out of usable cards\n";
				char buffer[81];
				memset(buffer, '-', 80);
				buffer[80] = '\0';
				game->outstr << buffer << "\n\n";
				if (game->p2->life <= 5)
				{
					//Game::outstr << "LIFE TOTAL LOW\n";
					//std::cout << Game::outstr.str();
					fout << game->outstr.str() << std::endl;
					//system( "pause" );
				}

				break; // Next iteration
			}

			int randIndex = rand() % usableCards.size();
			BaseCard* card = usableCards[randIndex];
			game->outstr << "Casting " << card->GetLabel() << " from " << ZoneTypeToString(card->zone) << "\n";

			card->Cast();

			game->outstr << "Life: Player: " << game->p1->life << " Opponent: " << game->p2->life << "\t";
			game->outstr << "Lands: " << game->p1->untappedLand << "/" << game->p1->totalLand << "\t";
			game->outstr << "Cast spells: " << game->p1->spellsCast << "\n";
		}

		game->outstr.str("");
		game->outstr.clear();
		delete game;
	}
	fout.close();
	return 0;
}