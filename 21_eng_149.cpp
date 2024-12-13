#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <iomanip>
#include <map>
#include <string>
#include <fstream>

using namespace std;

enum Shape { Club, Diamond, Heart, Spade };
enum Rank { Two = 2, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };


class Card {
public:
	
	Shape cardshape;
	Rank cardrank;
	/*Card(Shape shape, Rank rank) {
		this->cardshape = shape;
		this->cardrank = rank;
	};*/
	
};

class cardpack {
public:
	vector<Card> CARDS;
	
	cardpack(){
		Card card0;
		
		for (int i = Two; i <= Ace; i++)
		{
			card0.cardrank= static_cast<Rank>(i);
			for (int j = Club; j <= Spade; j++)
			{
				card0.cardshape = static_cast<Shape>(j);
				CARDS.push_back(card0);
			}
		}
		//function to shuffle the deck of cards Fisher - Yates shuffle algorithm
			
			shuffledeck();
		}
	void shuffledeck() {

		//random number
		random_device random;
		//mt19937 gen(random());

	    mt19937 randomGenerator(random());
		shuffle(CARDS.begin(), CARDS.end(), randomGenerator);
		
	}
};

class Player {
public:
	vector<Card> hand;
	bool withdrawing;
	int totalscore;
	float winning_prob;
	bool playing;
	enum combination{HighCard = 100,OnePair = 150,TwoPair = 200,ThreeOfAKind = 250,Straight = 300,Flush = 350,FullHouse = 400,FourOfAKind = 450,StraightFlush = 500};

	//Player() : totalscore(0), winning_prob(0), withdrawing(true) {};
	Player():totalscore(0), winning_prob(0), playing(true), withdrawing(0){};

	void AddCard(const Card& caard) {
		hand.push_back(caard);
	}

	void Print_Hand() {
		for (int i = 0; i < 5; i++) {
			cout << hand[i].cardshape << " " << hand[i].cardrank << endl;
		}
	}


	int handScore() {
		//score
		int val = 0;

		map< Rank, int> value;
		for (const auto& card0 : hand)
		{
			value[card0.cardrank]++;
		}

		int maximumcount = 0;
		for (const auto& count : value)
		{
			maximumcount = max(maximumcount, count.second);
		}

		map<Shape, int> theshape;
		for (const auto& card0 : hand)
		{
			theshape[card0.cardshape]++;
		}

		bool IsFlush = false;
		for (const auto& count : theshape)
		{
			if (count.second == 5)
			{
				IsFlush = true;
				break;
			}
		}

		vector<Rank> ranks;
		for (const auto& card0 : hand)
		{
			ranks.push_back(card0.cardrank);
		}
		std::sort(ranks.begin(), ranks.end());

		bool IsStraight = false;
		for (unsigned int i = 0; i < ranks.size() - 4; i++)
		{
			if (ranks[i + 4] - ranks[i] == 4)
			{
				IsStraight = true;
				break;
			}
		}

		Rank HIGHESTRANK = ranks.back();

		if (IsStraight && IsFlush)
		{
			val = StraightFlush + HIGHESTRANK;
		}
		else if (maximumcount == 4)
		{
			val = FourOfAKind + HIGHESTRANK;
		}
		else if (maximumcount == 3 && value.size() == 2)
		{
			val = FullHouse + HIGHESTRANK;
		}
		else if (IsFlush)
		{
			val = Flush + HIGHESTRANK;
		}
		else if (IsStraight)
		{
			val = Straight + HIGHESTRANK;
		}
		else if (maximumcount == 3)
		{
			val = ThreeOfAKind + HIGHESTRANK;
		}
		else if (value.size() == 3)
		{
			val = TwoPair + HIGHESTRANK;
		}
		else if (value.size() == 4)
		{
			val = OnePair + HIGHESTRANK;
		}
		else
		{
			val = HighCard + HIGHESTRANK;
		}

		return val;

	}

};

string displaycard(Player PLAYER)
{
	string realvalue = " ";
	for (unsigned int i = 0; i < PLAYER.hand.size(); i++)
	{
		// value
		switch (PLAYER.hand[i].cardrank)
		{
		case Two:
			realvalue += "Two of ";
			break;
		case Three:
			realvalue += "Three of ";
			break;
		case Four:
			realvalue += "Four of ";
			break;
		case Five:
			realvalue += "Five of ";
			break;
		case Six:
			realvalue += "Six of ";
			break;
		case Seven:
			realvalue += "Seven of ";
			break;
		case Eight:
			realvalue += "Eight of ";
			break;
		case Nine:
			realvalue += "Nine of ";
			break;
		case Ten:
			realvalue += "Ten of ";
			break;
		case Jack:
			realvalue += "Jack of ";
			break;
		case Queen:
			realvalue += "Queen of ";
			break;
		case King:
			realvalue += "King of ";
			break;
		case Ace:
			realvalue += "Ace of ";

		default:
			break;
		}

		//realvalue += "of ";

		// shape
		switch (PLAYER.hand[i].cardshape)
		{
		case Diamond:
			realvalue += "Diamonds";
			break;
		case Spade:
			realvalue += "Spades";
			break;
		case Heart:
			realvalue += "Hearts";
			break;
		case Club:
			realvalue += "Clubs";
			break;
		default:
			break;
		}

		realvalue += "   ";
	}

	return realvalue;
}


void getscore(vector<Player>& Players) {
	int maximumScore = 0;
	int WINNER = 0;
	int fullroundscore = 0;
	for (unsigned int i = 0; i < Players.size(); i++) {
		if (!Players[i].playing) {
			continue;
		}
		int roundscore = Players[i].handScore();
		if (roundscore > maximumScore) {
			maximumScore = roundscore;
			WINNER = i;
			
		}
	}
	for (unsigned int i = 0; i < Players.size(); i++) {
		if (!Players[i].playing)
			continue;
		if (i == WINNER)
			continue;
		Players[i].totalscore += -1;
	}
	Players[WINNER].totalscore += 10;


	for (unsigned int i = 0; i < Players.size(); i++) {
		if (Players[i].playing)
			fullroundscore += Players[i].handScore();
	}

	for (unsigned int i = 0; i < Players.size(); i++) {
		if (Players[i].playing)
			Players[i].winning_prob = (float(Players[i].handScore()) / float(fullroundscore)) * 100;
		else
			Players[i].winning_prob = 0;
		
	}
}



void DealersJob(vector<Player> &players, cardpack deck) {
	//remove first two cardd
	deck.CARDS.erase(deck.CARDS.begin());
	deck.CARDS.erase(deck.CARDS.begin());
	

		for (unsigned int i = 0; i < players.size(); i++) {
			players[i].hand.clear();
			for (int j = 0; j < 5; j++)
			{
				if (!players[i].playing)
					continue;

				players[i].hand.push_back(deck.CARDS[0]);
				deck.CARDS.erase(deck.CARDS.begin());
			}
		}
		
	
}

void PrintDealerHand(vector<Player>& Players ,int num_players) {
	cout << "\nDealer's hand :" << endl<<endl;
	cout << displaycard(Players[num_players-1]) << endl;
}

//function to write data to txt file
void write(string data)
{
	ofstream writefile("logfile.txt", ofstream::app);

	if (writefile.is_open()) {
		writefile << data << endl;
		writefile.close();
	}
	else {
		cout << "\nError opening the file" << endl;
	}
}

int main() {

	int numPlayers=0;
	int rounds=0;
	char withdraw;
	cout << "\t------Welcome to HAPPY POKER !!!------" << endl;
	cout << "\nEnter the number of player (Including the Dealer) : ";
	cin >> numPlayers;
	cout << "\nEnter the number of rounds : ";
	cin >> rounds;

	vector<Player> PLAYERS;
	cardpack Deck;
	// Initialize the players
	for (int i = 0; i < numPlayers; i++) {
		PLAYERS.push_back(Player());
	}

	for (int k = 1; k <= rounds; k++) {
		//cardpack Deck;
		
		cout << "\n\t--------Round-------- " << k << "!" << endl;
		write("\n\t--------Round-------- " + to_string(k));

		Deck.shuffledeck();
		cout << "Card shuffled" << endl;

		int dealer_index = numPlayers - 1;
		DealersJob(PLAYERS, Deck);

		PrintDealerHand(PLAYERS, numPlayers);

		for (unsigned int j = 0; j < numPlayers-1 ; j++) {

			if (!PLAYERS[j].playing) {
				continue;
			}
			cout << "\nPlayer " << j + 1 << " Would you like to withdraw ? \nIf yes press 'Y'. If not press 'N' : ";
			cin >> withdraw;

			switch (withdraw) {
			case 'y':
			case 'Y':
				cout << "\nPlayer " << j+1 << " withdrawed from the round " << k << endl;
				PLAYERS[j].playing = false;
				write("\nPlayer " + to_string(j+1)+"withdrawed.");
				break;
			case 'n':
			case 'N':
				break;
			default:
				cout << "\nWrong input!" << endl;
				write("\nWrong input!");
				return 0;
			}

		}

		getscore(PLAYERS);

		for (int i = 0; i < PLAYERS.size(); i++)
		{
			string content ;
			content += "Player " + to_string(i+1) ;
			content += "\nHand \n" + displaycard(PLAYERS[i]) ;
			content += "\nWinning probability \n" + to_string(PLAYERS[i].winning_prob) + "\n\n";

			write(content);
		}

		for (unsigned int i = 0; i < PLAYERS.size()-1; i++) {
			
			cout << "\n\n-----Player " << i +1<< "-----" << endl;
			cout << "score: " << PLAYERS[i].totalscore << endl << "winning probability: " << PLAYERS[i].winning_prob << fixed << setprecision(2) << " %" << endl;
		}


		int Winner = 0;
		int winnerScore = 0;

		for (int i = 0; i < PLAYERS.size() - 1; i++) {
			if (PLAYERS[i].totalscore > winnerScore) {
				winnerScore = PLAYERS[i].totalscore;
				Winner = i;
			}
		}
		cout << endl << "\nWinner of the Round " << k << "- Player " << Winner+1 << endl;
		write("\nWinner of the round" + to_string(k) + "Player " + to_string(Winner));
	}

	cin.clear();
	cin.get();
	


	return 0;
}


