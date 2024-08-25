#include <iostream> // For basic input/output
#include <ctime>    // Used to get time for seeding random number
#include <cctype>   // Used for 'toupper' function

// Black Jack Game 
const std::string versionNumber = "1.1"; // Updated Version Number

// Card array variables
const char permaCards[] = {'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K',
                           'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K',
                           'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K',
                           'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'};

char cards[52];
char deck[52];
char playerHand[12];
char dealerHand[12];

int playerHandScore[12] = {0};
int dealerHandScore[12] = {0};
int playerHandTotal = 0;
int dealerHandTotal = 0;

// Counting and logic variables
int dealerHandCount = 0;
int playerHandCount = 0;
int deckCount = 1;
bool playerDone = false;
bool dealerDone = false;
bool playerBust = false;
bool dealerBust = false;
bool gameOver = false;
bool keepPlaying = true;

// Other variables
int playerChips = 1000;
int playerBet;
char hitSelect = 'Q';
char playAgainChar = 'Q';

void updateHandTotal(int &handTotal, int handScore[]) {
    handTotal = 0;
    for (int i = 0; i < 12; i++) {
        handTotal += handScore[i];
    }
}

void updateHandValues(char hand[], int handScore[], int &handCount, bool &done) {
    if (!done) {
        for (int i = 0; i < handCount; i++) {
            switch (hand[i]) {
                case 'K': case 'Q': case 'J': case 'T':
                    handScore[i] = 10;
                    break;
                case '9':
                    handScore[i] = 9;
                    break;
                case '8':
                    handScore[i] = 8;
                    break;
                case '7':
                    handScore[i] = 7;
                    break;
                case '6':
                    handScore[i] = 6;
                    break;
                case '5':
                    handScore[i] = 5;
                    break;
                case '4':
                    handScore[i] = 4;
                    break;
                case '3':
                    handScore[i] = 3;
                    break;
                case '2':
                    handScore[i] = 2;
                    break;
                case 'A':
                    std::cout << "\nCard #" << i + 1 << " in your hand is an Ace.\n"
                              << "Should it be counted as 1 or 11?\n";
                    int aceSelect;
                    std::cin >> aceSelect;
                    handScore[i] = (aceSelect == 11) ? 11 : 1;
                    break;
                default:
                    handScore[i] = 0;
            }
        }
    }
}

void playerTurn() {
    while (!playerDone) {
        std::cout << "\nDo you want to HIT or STAY? (Please enter 'H' or 'S'): ";
        std::cin >> hitSelect;
        hitSelect = toupper(hitSelect);
        if (hitSelect == 'H') {
            std::cout << "\nDealing card " << deckCount << " to player . . .\n";
            playerHand[playerHandCount] = deck[deckCount++];
            playerHandCount++;
            updateHandValues(playerHand, playerHandScore, playerHandCount, playerDone);
            updateHandTotal(playerHandTotal, playerHandScore);
            std::cout << "\nYour hand: ";
            for (int i = 0; i < playerHandCount; i++) {
                std::cout << playerHand[i] << (i < playerHandCount - 1 ? ", " : "");
            }
            std::cout << "\nThe total of your current hand is: " << playerHandTotal << ".";
            if (playerHandTotal > 21) {
                playerDone = true;
                playerBust = true;
            }
        } else if (hitSelect == 'S') {
            std::cout << "\nPlayer will STAY with " << playerHandTotal << ".";
            playerDone = true;
        } else {
            std::cout << "Invalid entry. Please enter 'H' or 'S' when prompted.";
        }
    }
}

void dealerTurn() {
    while (!dealerDone) {
        updateHandTotal(dealerHandTotal, dealerHandScore);
        if (dealerHandTotal < 17) {
            std::cout << "\nDealing card " << deckCount << " to dealer . . .\n";
            dealerHand[dealerHandCount] = deck[deckCount++];
            dealerHandCount++;
            updateHandValues(dealerHand, dealerHandScore, dealerHandCount, dealerDone);
            updateHandTotal(dealerHandTotal, dealerHandScore);
        } else {
            std::cout << "\nDealer will STAY.";
            dealerDone = true;
        }
    }
}

void checkWin() {
    updateHandTotal(playerHandTotal, playerHandScore);
    updateHandTotal(dealerHandTotal, dealerHandScore);
    if (playerHandTotal > 21) playerBust = true;
    if (dealerHandTotal > 21) dealerBust = true;

    if (playerBust && !dealerBust) {
        std::cout << "\nPlayer went BUST. Dealer WINS.";
        playerChips -= playerBet;
    } else if (!playerBust && dealerBust) {
        std::cout << "\nDealer went BUST. Player WINS!";
        playerChips += playerBet;
    } else if (playerBust && dealerBust) {
        std::cout << "\nBoth Player and Dealer went BUST. Dealer WINS.";
        playerChips -= playerBet;
    } else if (playerDone && dealerDone) {
        if (playerHandTotal > dealerHandTotal) {
            std::cout << "\nPlayer wins with " << playerHandTotal << "!";
            playerChips += playerBet;
        } else if (playerHandTotal < dealerHandTotal) {
            std::cout << "\nDealer wins with " << dealerHandTotal << ".";
            playerChips -= playerBet;
        } else {
            std::cout << "\nIt's a PUSH!";
        }
    }
    gameOver = true;
}

void shuffleDeck() {
    std::cout << "Shuffling deck of cards . . .";
    std::copy(std::begin(permaCards), std::end(permaCards), std::begin(cards));
    for (int i = 0; i < 52; i++) {
        bool validCard = false;
        while (!validCard) {
            int randNum = rand() % 52;
            if (cards[randNum] != 'x') {
                deck[i] = cards[randNum];
                cards[randNum] = 'x';
                validCard = true;
            }
        }
    }
}

void game() {
    shuffleDeck();
    gameOver = false;
    playerDone = false;
    dealerDone = false;
    playerBust = false;
    dealerBust = false;
    playerHandCount = 0;
    dealerHandCount = 0;
    deckCount = 1;

    std::cout << "\nYou have $" << playerChips << ". How much do you want to bet?\n";
    std::cin >> playerBet;
    std::cout << "\nBetting $" << playerBet << ". . .\n";

    for (int i = 0; i < 2; i++) {
        playerHand[playerHandCount++] = deck[deckCount++];
        dealerHand[dealerHandCount++] = deck[deckCount++];
    }

    std::cout << "\nYour hand: " << playerHand[0] << ", " << playerHand[1];
    std::cout << "\nDealer's hand: [?], " << dealerHand[1];

    updateHandValues(playerHand, playerHandScore, playerHandCount, playerDone);
    updateHandValues(dealerHand, dealerHandScore, dealerHandCount, dealerDone);
    updateHandTotal(playerHandTotal, playerHandScore);
    std::cout << "\nYour current hand total is: " << playerHandTotal << ".";

    while (!gameOver) {
        playerTurn();
        if (!playerDone) checkWin();
        if (!gameOver && !dealerDone) {
            dealerTurn();
            checkWin();
        }
    }

    std::cout << "\n\nDo you want to play again? (Y/N): ";
    std::cin >> playAgainChar;
    playAgainChar = toupper(playAgainChar);
    if (playAgainChar == 'Y') {
        keepPlaying = true;
    } else {
        keepPlaying = false;
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    while (keepPlaying && playerChips > 0) {
        game();
    }
    if (playerChips <= 0) {
        std::cout << "\nYou have run out of chips!";
    }
    std::cout << "\nThanks for playing!\n";
    return 0;
}
