#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cassert>
#include <ctime>
#include <fstream>

#include "basecard.h"
#include "cardgame.h"

Game* theGame;
std::ostringstream Game::outstr;

char* ZoneTypeToString(ZONE_TYPE _zone)
{
	switch (_zone)
	{
	case BATTLEFIELD:
		return "battlefield";
	case EXILE:
		return "exile";
	case HAND:
		return "hand";
	case GRAVEYARD:
		return "graveyard";
	case LIBRARY:
		return "library";
	default:
		assert(false);
		return "ERROR";
	};
}

int main(int argc, char* argv[])
{
	std::ofstream fout("output.txt");
	srand((unsigned int)time(NULL));
	for (long round = 0; round < 1000000; ++round)
	{
		//std::cout << i << "\r";
		Game::instance = new Game();
		Game::instance->p1->life = 1;
		Game::instance->p2->life = 65;//65 - 5 - 1;
		Game::instance->p1->totalLand = Game::instance->p1->untappedLand = 19;

		Player* p1 = Game::instance->p1;
		Player* p2 = Game::instance->p2;

		// HAND
		p1->hand->AddCard(new BetrayalOfFlesh());
		p1->hand->AddCard(new InfernalTutor());
		p1->hand->AddCard(new Coercion());
		p1->hand->AddCard(new Brainspoil());
		p1->hand->AddCard(new DemonicCollusion());

		// Graveyard
		p1->graveyard->AddCard(new DemonicCollusion());

		// Battlefield
		p1->battlefield->AddCard(new SneakAttack());
		p2->battlefield->AddCard(new LeylineOfSanctity());

		for (int i = 0; i < 19; ++i)
		{
			p1->battlefield->AddCard(new BasicLand());
		}

		// Library
		//p1->library->AddCard(new SorinsVengeance());
		//p1->library->AddCard(new SorinsVengeance());
		p1->library->AddCard(new SorinsVengeance());

		p1->library->AddCard(new Brainspoil());
		p1->library->AddCard(new Snap());
		//p1->library->AddCard(new OneWithNothing());
		//p1->library->AddCard(new ChimneyImp());
		p1->library->AddCard(new PastInFlames());
		p1->library->AddCard(new TendrilsOfAgony());
		p1->library->AddCard(new SpittingImage());
		p1->library->AddCard(new Hoodwink());
		p1->library->AddCard(new GreatWhale());

		for (int i = 0; i < 3; ++i)
		{
			//p1->library->AddCard(new BasicLand());
		}

		int turn = 0;
		while (true)
		{
			++turn;
			Game::outstr << "\n +++ ROUND " << round << " STEP " << turn << " +++ \n";
			std::vector<BaseCard*> usableCards;
			for each (BaseCard * card in p1->battlefield->list)
			{
				if (card->CanCast())
				{
					usableCards.push_back(card);
				}
			}
			for each (BaseCard * card in p1->hand->list)
			{
				if (card->CanCast())
				{
					usableCards.push_back(card);
				}
			}
			for each (BaseCard * card in p1->graveyard->list)
			{
				if (card->CanCast())
				{
					usableCards.push_back(card);
				}
			}

			Game::outstr << "Usable cards:\n";
			for each (BaseCard * card in usableCards)
			{
				Game::outstr << " + " << card->GetLabel() << " is usable from " << ZoneTypeToString(card->zone) << "\n";
			}

			if (usableCards.empty())
			{
				Game::outstr << "Out of usable cards\n";
				char buffer[81];
				memset(buffer, '-', 80);
				buffer[80] = '\0';
				Game::outstr << buffer << "\n\n";
				if (p2->life <= 0)
				{
					//Game::outstr << "LIFE TOTAL LOW\n";
					//std::cout << Game::outstr.str();
					fout << Game::outstr.str() << std::endl;
					//system( "pause" );
				}

				break; // Next iteration
			}

			int randIndex = rand() % usableCards.size();
			BaseCard* card = usableCards[randIndex];
			Game::outstr << "Casting " << card->GetLabel() << " from " << ZoneTypeToString(card->zone) << "\n";

			card->Cast();

			Game::outstr << "Life: Player: " << p1->life << " Opponent: " << p2->life << "\t";
			Game::outstr << "Lands: " << p1->untappedLand << "/" << p1->totalLand << "\t";
			Game::outstr << "Cast spells: " << p1->spellsCast << "\n";



		}
		delete Game::instance;
		Game::outstr.str("");
		Game::outstr.clear();
	}
	fout.close();
	return 0;
}