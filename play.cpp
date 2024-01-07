#include <windows.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <sstream>
#include <string>

using namespace std;

void typeWriterEffect(string text)
{
    for (char c : text)
    {
        cout << c << flush; // flush is used to flush the buffer and make the characters appear immediately
        Sleep(50);
    }
    cout << endl; // Move to the next line after the typing effect
}

// Enum to represent card suits
enum Suit
{
    SPADES,
    HEARTS,
    CLUBS,
    DIAMONDS,
    NONE
};

// Enum to represent card ranks
enum Rank
{
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    ACE
};

// Class to represent a Card
class Card
{
public:
    Suit suit;
    Rank rank;

    // Constructor to initialize a card
    Card(Suit s, Rank r) : suit(s), rank(r) {}
    Card() {}

    bool operator==(const Card &other) const
    {
        return rank == other.rank && suit == other.suit;
    }

    
    string suitToString() const
    {
        switch (suit)
        {
        case SPADES:
            return "Spades";
        case HEARTS:
            return "Hearts";
        case DIAMONDS:
            return "Diamonds";
        case CLUBS:
            return "Clubs";
        default:
            return "Invalid Suit";
        }
    }

    string rankToString() const
    {
        switch (rank)
        {
        case ACE:
            return "A";
        case TWO:
            return "2";
        case THREE:
            return "3";
        case FOUR:
            return "4";
        case FIVE:
            return "5";
        case SIX:
            return "6";
        case SEVEN:
            return "7";
        case EIGHT:
            return "8";
        case NINE:
            return "9";
        case TEN:
            return "10";
        case JACK:
            return "J";
        case QUEEN:
            return "Q";
        case KING:
            return "K";
        default:
            return "Invalid Rank";
        }
    }
};

// Class to represent a Deck of cards
class Deck
{
private:
    vector<Card> cards;

public:
    // Constructor to initialize a deck with one or more standard decks
    Deck(int numDecks = 1)
    {
        for (int i = 0; i < numDecks; i++)
        {
            for (int s = SPADES; s <= DIAMONDS; s++)
            {
                for (int r = TWO; r <= ACE; r++)
                {
                    cards.push_back(Card((Suit)s, (Rank)r));
                }
            }
        }
    }

    // Function to shuffle the deck
    void shuffle()
    {
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        default_random_engine g(seed);

        std::shuffle(cards.begin(), cards.end(), g);
    }

    // Function to deal a card from the deck
    Card dealCard()
    {
        Card c = cards.back();
        cards.pop_back();
        return c;
    }

    // Function to get the total num of cards in the deck
    int getTotalCards()
    {
        return cards.size();
    }
};

// Class to represent a Player
class Player
{
private:
    vector<Card> hand;
    int playerIndex;

public:
    // Function to add a card to the player's hand
    void addToHand(Card card)
    {
        hand.push_back(card);
    }

    // Function to remove a card from the player's hand
    void removeFromHand(Card card)
    {
        auto it = find(hand.begin(), hand.end(), card);
        if (it != hand.end())
        {
            hand.erase(it);
        }
    }

    // Function to check if the player has a card of a specific suit
    bool hasCardOfSuit(Suit suit) const
    {
        for (const Card &card : hand)
        {
            if (card.suit == suit)
            {
                return true;
            }
        }
        return false;
    }

    // Function to get the player's hand
    const vector<Card> &getHand() const
    {
        return hand;
    }

    void sortHand()
    {
        sort(hand.begin(), hand.end(), [](const Card &a, const Card &b)
                  {
            // Sort by suit first and then by rank
            if (a.suit == b.suit) {
                return a.rank < b.rank;
            }
            return a.suit < b.suit; });
    }

    bool operator==(const Player &other) const
    {
        return playerIndex == other.playerIndex; // Assuming playerIndex is a unique identifier for players
    }

    void setPlayerIndex(int value)
    {
        playerIndex = value;
    }
    int getPlayerIndex() const
    {
        return playerIndex;
    }

    // Function to get all cards of a specific suit from the player's hand
    vector<Card> getCardsOfSuit(Suit suit) const
    {
        vector<Card> cardsOfSuit;
        for (const Card &card : hand)
        {
            if (card.suit == suit)
            {
                cardsOfSuit.push_back(card);
            }
        }
        return cardsOfSuit;
    }
};

// Class to represent the CardGame
class CardGame
{
private:
    vector<Player> players;
    Deck deck;
    Player starter;
    Suit currentSuit;
    Rank highestCardRank;
    Player *User; // last player will be termed as user

public:
    // Constructor to initialize the game with a specified number of players and decks
    CardGame(int numPlayers, int numDecks) : deck(numDecks)
    {
        players.resize(numPlayers);
        User = &players[players.size() - 1];
    }

    // Function to start and play the game
    void playGame()
    {
        cout << "We have " << players.size() << " players with us joining today. Hope you have a great experience\n\n";

        initializeGame();
        playRound0();
        Sleep(1000);
        Player remainingPlayer;
        do
        {
            playRound();
            Sleep(2000);
        } while (!isGameEnded(remainingPlayer));

        cout << "Game has ended. Player " << remainingPlayer.getPlayerIndex() << " is the loser!\n";
    }

private:
    int takeInputSuit()
    {
    }

    Card UserTurn(Suit currentSuit, Player &starter)
    {
        printUserCards(*User);

        // take input which card u will choose
        Card userPlayingCard;

        if (starter == *User)
        {
            do
            {
                userPlayingCard = selectUserCard(*User);
                if (!isCardAvailableWithUser(*User, userPlayingCard))
                {
                    cout << "Come on man you don't have that card\n";
                }
            } while (!isCardAvailableWithUser(*User, userPlayingCard));
        }
        else
        {
            do
            {
                userPlayingCard = selectUserCard(*User);

                if (!isCardAvailableWithUser(*User, userPlayingCard))
                {
                    cout << "Come on man you don't have that card\n";
                }
                else if ((*User).hasCardOfSuit(currentSuit) && currentSuit != userPlayingCard.suit)
                {
                    cout << "Hey yo. No cheating dude!! I Know, You Know, We Know\n";
                }
            } while (!isCardAvailableWithUser(*User, userPlayingCard) || ((*User).hasCardOfSuit(currentSuit) && currentSuit != userPlayingCard.suit));
        }

        // cout << "\nYou have chosen: Suit " << userPlayingCard.suitToString() << ", Rank " << userPlayingCard.rankToString() << "\n";

        return userPlayingCard;
    }

    Card selectUserCard(Player &player)
    {
        int userSuit;
        cout << "\nIt's your turn now. Select your card: \n";
        cout << "Suit (1: SPADE, 2:HEART, 3: CLUB, 4: DIAMOND) : ";
        // Check if the input is of the correct type
        while (!(cin >> userSuit) || (userSuit < 1) || (userSuit > 4) || (cin.peek() != '\n'))
        {
            cin.clear();                                                   // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input. Please enter a whole number (1, 2, 3 or 4): ";
        }

        string selectedCard;

        cout << "Select the rank (2-10, J, Q, K, A): ";

        // Input validation loop
        while (!(cin >> selectedCard) || !isValidRank(selectedCard))
        {
            cin.clear();                                                   // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input. Please enter a valid card character: ";
        }

        Card userCard(Suit(userSuit - 1), Rank(convertToInteger(selectedCard) - 2));
        cout << "\nYou have chosen: Suit " << userCard.suitToString() << ", Rank " << userCard.rankToString() << "\n\n";
        return userCard;
    }

    int convertToInteger(const string &card)
    {
        if (card == "J")
        {
            return 11;
        }
        else if (card == "Q")
        {
            return 12;
        }
        else if (card == "K")
        {
            return 13;
        }
        else if (card == "A")
        {
            return 14;
        }
        else
        {
            istringstream iss(card);
            int value;
            if (iss >> value)
            {
                return value;
            }
            else
            {
                // Invalid input, return a special value or handle it as needed
                return -1;
            }
        }
    }

    bool isValidRank(const string &card)
    {
        if (card.size() == 1)
        {
            char firstChar = card[0];
            return (isdigit(firstChar) && (firstChar >= '2' && firstChar <= '9')) ||
                   ((firstChar) == 'A' || (firstChar) == 'J' || (firstChar) == 'Q' || (firstChar) == 'K');
        }
        else if (card.size() == 2 && card[0] == '1' && card[1] == '0')
        {
            return true; // Handle "10" as a valid input
        }
        return false;
    }

    bool isCardAvailableWithUser(Player &player, Card &UserCard)
    {
        for (const Card &card : player.getHand())
        {
            if (card == UserCard)
                return true;
        }
        return false;
    }

    // Function to initialize the game setup
    void initializeGame()
    {
        deck.shuffle();
        // delay(2);
        for (int i = 0; i < 50; i++)
        {
            cout << "Shuffle! ";
            Sleep(50);
        }
        cout << "\n";
        typeWriterEffect("Each player is now being handed their cards.");
        Sleep(2000);
        int totalCards = deck.getTotalCards();
        int cardsPerPlayer = totalCards / players.size();
        int peopleWithExtraCard = totalCards % players.size();

        // Inside the CardGame constructor
        for (int i = 0; i < players.size(); ++i)
        {
            players[i].setPlayerIndex(i + 1);
        }

        // Deal cards to each player
        for (Player &player : players)
        {
            for (int i = 0; i < cardsPerPlayer; ++i)
            { // Adjust the number of initial cards per player as needed
                Card dealtCard = deck.dealCard();
                player.addToHand(dealtCard);
            }
        }
        for (int i = 0; i < peopleWithExtraCard; i++)
        {
            Player &p = players[i];
            Card dealtCard = deck.dealCard();
            p.addToHand(dealtCard);
        }

        // Sort the player's hand after dealing
        for (Player &player : players)
        {
            player.sortHand();
        }
        typeWriterEffect("Each player has their cards now.");

        printUserCards(*User);

        // Print the initial cards of all players
        printPlayersCards();

        typeWriterEffect("So who's gonna start now. Who has the Ace of spades..................... ");
        Sleep(2000);
        // Find the player with the Ace of Spades and make them the starter
        auto starterIterator = find_if(players.begin(), players.end(), [](const Player &player)
                                            { return player.hasCardOfSuit(SPADES) && find_if(player.getHand().begin(), player.getHand().end(),
                                                                                                  [](const Card &card)
                                                                                                  { return card.rank == ACE && card.suit == SPADES; }) != player.getHand().end(); });

        if (starterIterator != players.end())
        {
            starter = *starterIterator; // Dereference the iterator to get the Player object
            cout << "Player " << distance(players.begin(), starterIterator) + 1 << " has the Ace of Spades and starts the game.\n";
        }
        else
        {
            // Handle the case where no player has the Ace of Spades
            cerr << "Error: No player has the Ace of Spades.\n";
        }

        currentSuit = SPADES;
        highestCardRank = ACE;
        cout << "\n\t\t************************************";
        cout << "\n\t\t\t\t";
        typeWriterEffect("Game Begins");
        cout << "\n\t\t************************************\n";
        // printGameStatus();
    }

    void playRound0()
    {
        cout << "Round 0: \n";
        auto starterIterator = find(players.begin(), players.end(), starter);

        if (starterIterator == players.end())
        {
            cerr << "Error: Starter player not found.\n";
            return;
        }

        auto currentPlayerIterator = starterIterator;

        do
        {
            Sleep(500);
            Player &currentPlayer = *currentPlayerIterator;

            // Check if the player has a card of the current suit (Spades)

            auto cardsOfCurrentSuit = currentPlayer.getCardsOfSuit(currentSuit);

            if (!cardsOfCurrentSuit.empty())
            {
                // Find the largest card of the current suit in the player's hand
                Card playedCard;
                
                if (currentPlayer == *User)
                {
                    if(starter==*User){
                        playedCard = Card(SPADES, ACE);
                    }
                    else{
                        playedCard = UserTurn(currentSuit, starter);
                    }
                }

                else
                {
                    auto maxCardIterator = max_element(cardsOfCurrentSuit.begin(), cardsOfCurrentSuit.end(),
                                                            [this](const Card &a, const Card &b)
                                                            {
                                                                return a.rank < b.rank;
                                                            });

                    // Play the largest card of the current suit
                    playedCard = *maxCardIterator;
                }

                currentPlayer.removeFromHand(playedCard);

                cout << "Player " << players[distance(players.begin(), currentPlayerIterator)].getPlayerIndex()
                          << " played: Suit " << playedCard.suitToString() << ", Rank " << playedCard.rankToString() << "\n";
            }
            else
            {
                // Play any big card if the player doesn't have the current suit
                Card playedCard;
                if (currentPlayer == *User)
                {
                    playedCard = UserTurn(currentSuit, starter);
                }
                else
                {
                    auto maxCardIterator = max_element(currentPlayer.getHand().begin(), currentPlayer.getHand().end(),
                                                            [](const Card &a, const Card &b)
                                                            {
                                                                return a.rank < b.rank;
                                                            });

                    // Play the largest card in the player's hand
                    playedCard = *maxCardIterator;
                }

                currentPlayer.removeFromHand(playedCard);

                cout << "Player " << players[distance(players.begin(), currentPlayerIterator)].getPlayerIndex()
                          << " played: Suit " << playedCard.suitToString() << ", Rank " << playedCard.rankToString() << " \n";
            }

            ++currentPlayerIterator;
            if (currentPlayerIterator == players.end())
            {
                currentPlayerIterator = players.begin(); // Wrap around to the first player
            }
        } while (currentPlayerIterator != starterIterator); // Continue until we reach the starter again

        printPlayersCards();
    }

    void playRound()
    {
        cout << "\n************************************\n";
        cout << "\t";
        typeWriterEffect("Next Round :");
        vector<Card> playedCards; // Temporary vector to store played cards

        Card interruptingCard;
        bool ok = true;

        auto starterIterator = find(players.begin(), players.end(), starter);

        if (starterIterator == players.end())
        {
            cerr << "Error: Starter player not found.\n";
            return;
        }

        auto currentPlayerIterator = starterIterator;
        
        while (!starter.hasCardOfSuit(currentSuit))
            currentSuit = static_cast<Suit>((static_cast<int>(currentSuit) + 1) % 4);

        do
        {
            Sleep(500);
            Player &currentPlayer = *currentPlayerIterator;

            // Skip players who are out of cards
            if (isPlayerOutOfCards(currentPlayer))
            {
                ++currentPlayerIterator;
                if (currentPlayerIterator == players.end())
                {
                    currentPlayerIterator = players.begin(); // Wrap around to the first player
                }
                continue;
            }

            // Check if the player has a card of the current suit (Spades)

            auto cardsOfCurrentSuit = currentPlayer.getCardsOfSuit(currentSuit);

            if (!cardsOfCurrentSuit.empty())
            {
                // Find the largest card of the current suit in the player's hand
                Card playedCard;
                if (currentPlayer == *User)
                {
                    if(starter==*User){
                        playedCard = UserTurn(static_cast<Suit>(static_cast<int>(4)), starter);
                        currentSuit = playedCard.suit;
                    }
                    else{
                        playedCard = UserTurn(currentSuit, starter);
                    }
                }
                else
                {

                    auto maxCardIterator = max_element(cardsOfCurrentSuit.begin(), cardsOfCurrentSuit.end(),
                                                            [this](const Card &a, const Card &b)
                                                            {
                                                                return a.rank < b.rank;
                                                            });

                    // Play the largest card of the current suit
                    playedCard = *maxCardIterator;
                }
                currentPlayer.removeFromHand(playedCard);
                playedCards.push_back(playedCard);
                cout << "Player " << players[distance(players.begin(), currentPlayerIterator)].getPlayerIndex()
                          << " played: Suit " << playedCard.suitToString() << ", Rank " << playedCard.rankToString() << "\n";
            }
            else
            {
                // Play any big card if the player doesn't have the current suit
                Card playedCard;
                if (currentPlayer == *User)
                {
                    playedCard = UserTurn(currentSuit, starter);
                }
                else
                {
                    auto maxCardIterator = max_element(currentPlayer.getHand().begin(), currentPlayer.getHand().end(),
                                                            [](const Card &a, const Card &b)
                                                            {
                                                                return a.rank < b.rank;
                                                            });

                    // Play the largest card in the player's hand
                    playedCard = *maxCardIterator;
                }
                currentPlayer.removeFromHand(playedCard);

                cout << "Player " << players[distance(players.begin(), currentPlayerIterator)].getPlayerIndex()
                          << " played: Suit " << playedCard.suitToString() << ", Rank " << playedCard.rankToString();
                cout << " \n\n\t\t\tAe ";
                Sleep(500);
                cout << "Aee ";
                Sleep(500);
                cout << "Aee ";
                Sleep(500);
                cout << "Ae ";
                Sleep(500);
                cout << "\n\n\t\t\t\tKataaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n\n";
                Sleep(2000);

                ok = false;
                interruptingCard = Card((Suit)playedCard.suit, (Rank)playedCard.rank);
                break;
            }

            ++currentPlayerIterator;
            if (currentPlayerIterator == players.end())
            {
                currentPlayerIterator = players.begin(); // Wrap around to the first player
            }
        } while (currentPlayerIterator != starterIterator); // Continue until we reach the starter again

        // Find the player with the max card among the played cards
        auto maxPlayedCardIterator = max_element(playedCards.begin(), playedCards.end(),
                                                      [](const Card &a, const Card &b)
                                                      {
                                                          if (a.suit == b.suit)
                                                          {
                                                              return a.rank < b.rank;
                                                          }
                                                          else
                                                          {
                                                              return a.suit < b.suit;
                                                          }
                                                      });

        // Determine the new starter
        Player &newStarter = players[(distance(playedCards.begin(), maxPlayedCardIterator) + distance(players.begin(), starterIterator)) % players.size()];

        // cout<<ok<<endl;
        if (!ok)
        {
            for (const auto &card : playedCards)
            {
                newStarter.addToHand(card);
            }

            newStarter.addToHand(interruptingCard);
            newStarter.sortHand();
            // cout<<"IDDHR";
        }

        playedCards.clear();

        // Update the current suit and highest card rank for the next round
        currentSuit = static_cast<Suit>((static_cast<int>(maxPlayedCardIterator->suit) + 1) % 4);
        highestCardRank = maxPlayedCardIterator->rank;
        starter = newStarter;
        printPlayersCards();
    }

    vector<Player> getRemainingPlayers() const
    {
        vector<Player> remainingPlayers;
        for (const Player &player : players)
        {
            if (!isPlayerOutOfCards(player))
            {
                remainingPlayers.push_back(player);
            }
        }
        return remainingPlayers;
    }

    bool isGameEnded(Player &remainingPlayer)
    {
        players.erase(remove_if(players.begin(), players.end(),
                                     [](const Player &player)
                                     { return player.getHand().empty(); }),
                      players.end());

        if (players.size() == 1)
        {
            remainingPlayer = players.front();
            return true; // Only one player is remaining, game has ended
        }
        else
        {
            return false; // More than one player is remaining, game not ended
        }
    }
    // Function to check if a player is out of cards
    bool isPlayerOutOfCards(const Player &player) const
    {
        return player.getHand().size() == 0;
    }

    // Inside the CardGame class
    void printPlayersCards() const
    {
        cout << "\nCurrent Status of Players:\n";
        for (const Player &player : players)
        {
            cout << "Player " << players[distance(players.begin(), find(players.begin(), players.end(), player))].getPlayerIndex() << "'s cards: ";
            // for (const Card& card : player.getHand()) {
            //     cout << "(" << card.suitToString() << ", " << card.rankToString() << ") ";
            // }
            cout << "has " << player.getHand().size() << " cards";
            cout << "\n";
            Sleep(500);
        }
    }

    void printUserCards(Player &player)
    {
        cout << "\nUser's Hand: ";
        for (const Card &card : player.getHand())
        {
            cout << "(" << card.suitToString() << ", " << card.rankToString() << ") ";
        }
        cout << "\n\n";
        Sleep(500);
    }
};

// Main function to start the game
int main()
{
    
    cout << "\t\t*******************************************************************\n";
    cout << "\t\t\t      ";
    typeWriterEffect("Welcome to the PABO Card Game!");
    cout << "\t\t*******************************************************************\n";
    cout << "\t\t\t";
    typeWriterEffect("Get ready for an exciting card-playing experience!");
    cout << "\t\t*******************************************************************\n\n";
    // Get the number of players and decks from the user
    int numPlayers, numDecks;
    typeWriterEffect("Enter the number of players: ");
    cin >> numPlayers;
    typeWriterEffect("Enter the number of decks: ");
    cin >> numDecks;

    if (numPlayers <= 0 or numDecks <= 0 or numPlayers > 52 * numDecks)
    {
        cout << "No Game Possible. Sorry!!";
    }
    else if (numDecks > 2 or numPlayers > 12 or (52 * numDecks) / numPlayers < 6)
    {
        cout << "No Fun Possible. Alas!!";
    }
    else
    {
        cout << "Lesss goo !!" << endl;
        // Create and start the card game
        CardGame game(numPlayers, numDecks);
        game.playGame();
    }

    cout << "\t\t*******************************************************************\n";
    cout << "\t\t\t\t\t";
    typeWriterEffect("Thank you for Playing!!");
    cout << "\t\t*******************************************************************\n\n";

    return 0;
}
