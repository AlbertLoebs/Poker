#include "deckFuncs.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <map>
#include <random>
#include <set>

// Shuffle the deck using a random seed based on current time
void shuffleDeck(std::vector<std::string>& deck) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));
}

// Deal two cards to each player
void dealCards(std::vector<std::string>& deck, std::vector<std::string>& yourHand, std::vector<std::string>& otherHand) {
    shuffleDeck(deck);

    for (int i = 0; i < 2; ++i) {
        yourHand.push_back(deck.back());
        deck.pop_back();
        otherHand.push_back(deck.back());
        deck.pop_back();
    }

    // Print hands
    std::cout << "Your hand: ";
    for (const auto& card : yourHand) {
        std::cout << card << " | ";
    }
    std::cout << "\nOther player's hand: ";
    for (const auto& card : otherHand) {
        std::cout << card << " | ";
    }
    std::cout << std::endl;
}

// Deal five community cards (flop, turn, river)
void dealCommunityCards(std::vector<std::string>& deck, std::vector<std::string>& communityHand) {
    // Burn a card
    deck.pop_back();

    // Flop: 3 community cards
    for (int i = 0; i < 3; ++i) {
        communityHand.push_back(deck.back());
        deck.pop_back();
    }
    std::cout << "Flop: ";
    for (const auto& card : communityHand) {
        std::cout << card << " | ";
    }
    std::cout << std::endl;

    // Burn a card
    deck.pop_back();

    // Turn: 1 community card
    communityHand.push_back(deck.back());
    deck.pop_back();
    std::cout << "Turn: " << communityHand.back() << std::endl;

    // Burn a card
    deck.pop_back();

    // River: 1 community card
    communityHand.push_back(deck.back());
    deck.pop_back();
    std::cout << "River: " << communityHand.back() << std::endl;
}

// Extract the suit from a card string
std::string getSuit(const std::string& card) {
    size_t pos = card.find(" of ");
    if (pos != std::string::npos) {
        return card.substr(pos + 4); // Extracts the suit name
    }
    return "";
}

// Extract the rank from a card string
std::string getRank(const std::string& card) {
    size_t pos = card.find(" of ");
    if (pos != std::string::npos) {
        return card.substr(0, pos);
    }
    return "";
}

// Convert card rank to numerical value
int cardValue(const std::string& card) {
    std::string rank = getRank(card);
    if (rank == "Ace") return 14;
    if (rank == "King") return 13;
    if (rank == "Queen") return 12;
    if (rank == "Jack") return 11;
    if (rank == "10") return 10;
    if (rank == "9") return 9;
    if (rank == "8") return 8;
    if (rank == "7") return 7;
    if (rank == "6") return 6;
    if (rank == "5") return 5;
    if (rank == "4") return 4;
    if (rank == "3") return 3;
    if (rank == "2") return 2;
    return 0;
}

// Get the highest card in the hand
std::string getHighCard(const std::vector<std::string>& hand) {
    return *std::max_element(hand.begin(), hand.end(), [](const std::string& a, const std::string& b) {
        return cardValue(a) < cardValue(b);
    });
}

// Check for Royal Flush
bool isRoyalFlush(const std::vector<std::string>& hand) {
    std::map<std::string, std::set<std::string>> suitMap;
    for (const auto& card : hand) {
        suitMap[getSuit(card)].insert(getRank(card));
    }

    std::set<std::string> royalRanks = {"10", "Jack", "Queen", "King", "Ace"};

    for (const auto& [suit, ranks] : suitMap) {
        if (std::includes(ranks.begin(), ranks.end(), royalRanks.begin(), royalRanks.end())) {
            return true;
        }
    }

    return false;
}

// Check for Straight Flush
bool isStraightFlush(const std::vector<std::string>& hand) {
    std::map<std::string, std::set<int>> suitMap;
    std::vector<std::string> ranksOrder = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};

    for (const auto& card : hand) {
        std::string suit = getSuit(card);
        std::string rank = getRank(card);
        auto it = std::find(ranksOrder.begin(), ranksOrder.end(), rank);
        if (it != ranksOrder.end()) {
            int index = std::distance(ranksOrder.begin(), it);
            suitMap[suit].insert(index);
        }
    }

    // Check each suit for a straight
    for (const auto& [suit, indices] : suitMap) {
        if (indices.size() < 5) continue;

        std::vector<int> sortedIndices(indices.begin(), indices.end());
        std::sort(sortedIndices.begin(), sortedIndices.end());

        // Check for consecutive sequences of 5
        for (size_t i = 0; i + 4 < sortedIndices.size(); ++i) {
            bool straight = true;
            for (size_t j = 1; j < 5; ++j) {
                if (sortedIndices[i + j] != sortedIndices[i] + j) {
                    straight = false;
                    break;
                }
            }
            if (straight) return true;
        }

        // Special case: Ace-low straight (Ace, 2, 3, 4, 5)
        if (indices.count(0) && indices.count(1) && indices.count(2) && indices.count(3) && indices.count(12)) {
            return true;
        }
    }

    return false;
}

// Check for Four of a Kind
bool isFourOfAKind(const std::vector<std::string>& hand) {
    std::map<std::string, int> rankCount;
    for (const auto& card : hand) {
        rankCount[getRank(card)]++;
    }

    for (const auto& [rank, count] : rankCount) {
        if (count >= 4) return true;
    }

    return false;
}

// Check for Full House
bool isFullHouse(const std::vector<std::string>& hand) {
    std::map<std::string, int> rankCount;
    for (const auto& card : hand) {
        rankCount[getRank(card)]++;
    }

    bool hasThree = false;
    bool hasTwo = false;

    for (const auto& [rank, count] : rankCount) {
        if (count >= 3) hasThree = true;
        else if (count >= 2) hasTwo = true;
    }

    return hasThree && hasTwo;
}

// Check for Flush
bool isFlush(const std::vector<std::string>& hand) {
    std::map<std::string, int> suitCount;
    for (const auto& card : hand) {
        suitCount[getSuit(card)]++;
    }

    for (const auto& [suit, count] : suitCount) {
        if (count >= 5) return true;
    }

    return false;
}

// Check for Straight
bool isStraight(const std::vector<std::string>& hand) {
    std::set<int> rankIndices;
    std::vector<std::string> ranksOrder = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};

    for (const auto& card : hand) {
        std::string rank = getRank(card);
        auto it = std::find(ranksOrder.begin(), ranksOrder.end(), rank);
        if (it != ranksOrder.end()) {
            rankIndices.insert(std::distance(ranksOrder.begin(), it));
        }
    }

    std::vector<int> sortedIndices(rankIndices.begin(), rankIndices.end());
    std::sort(sortedIndices.begin(), sortedIndices.end());

    // Check for consecutive sequences of 5
    for (size_t i = 0; i + 4 < sortedIndices.size(); ++i) {
        bool straight = true;
        for (size_t j = 1; j < 5; ++j) {
            if (sortedIndices[i + j] != sortedIndices[i] + j) {
                straight = false;
                break;
            }
        }
        if (straight) return true;
    }

    // Special case: Ace-low straight (Ace, 2, 3, 4, 5)
    if (rankIndices.count(0) && rankIndices.count(1) && rankIndices.count(2) && rankIndices.count(3) && rankIndices.count(12)) {
        return true;
    }

    return false;
}

// Check for Three of a Kind
bool isThreeOfAKind(const std::vector<std::string>& hand) {
    std::map<std::string, int> rankCount;
    for (const auto& card : hand) {
        rankCount[getRank(card)]++;
    }

    for (const auto& [rank, count] : rankCount) {
        if (count >= 3) return true;
    }

    return false;
}

// Check for Two Pair
bool isTwoPair(const std::vector<std::string>& hand) {
    std::map<std::string, int> rankCount;
    for (const auto& card : hand) {
        rankCount[getRank(card)]++;
    }

    int pairs = 0;
    for (const auto& [rank, count] : rankCount) {
        if (count >= 2) pairs++;
    }

    return pairs >= 2;
}

// Check for One Pair
bool isPair(const std::vector<std::string>& hand) {
    std::map<std::string, int> rankCount;
    for (const auto& card : hand) {
        rankCount[getRank(card)]++;
    }

    for (const auto& [rank, count] : rankCount) {
        if (count >= 2) return true;
    }

    return false;
}

// Determine the winner based on hand rankings
bool determineWinner(const std::vector<std::string>& yourHand, const std::vector<std::string>& otherHand, const std::vector<std::string>& communityHand) {
    std::vector<std::string> yourCompleteHand = yourHand;
    std::vector<std::string> otherCompleteHand = otherHand;

    yourCompleteHand.insert(yourCompleteHand.end(), communityHand.begin(), communityHand.end());
    otherCompleteHand.insert(otherCompleteHand.end(), communityHand.begin(), communityHand.end());

    // Hand Rankings: Royal Flush > Straight Flush > Four of a Kind > Full House > Flush > Straight > Three of a Kind > Two Pair > One Pair > High Card

    // Royal Flush
    if (isRoyalFlush(yourCompleteHand)) {
        std::cout << "You win with a Royal Flush!" << std::endl;
        return true;
    }
    if (isRoyalFlush(otherCompleteHand)) {
        std::cout << "Other player wins with a Royal Flush!" << std::endl;
        return false;
    }

    // Straight Flush
    if (isStraightFlush(yourCompleteHand)) {
        std::cout << "You win with a Straight Flush!" << std::endl;
        return true;
    }
    if (isStraightFlush(otherCompleteHand)) {
        std::cout << "Other player wins with a Straight Flush!" << std::endl;
        return false;
    }

    // Four of a Kind
    if (isFourOfAKind(yourCompleteHand)) {
        std::cout << "You win with Four of a Kind!" << std::endl;
        return true;
    }
    if (isFourOfAKind(otherCompleteHand)) {
        std::cout << "Other player wins with Four of a Kind!" << std::endl;
        return false;
    }

    // Full House
    if (isFullHouse(yourCompleteHand)) {
        std::cout << "You win with a Full House!" << std::endl;
        return true;
    }
    if (isFullHouse(otherCompleteHand)) {
        std::cout << "Other player wins with a Full House!" << std::endl;
        return false;
    }

    // Flush
    if (isFlush(yourCompleteHand)) {
        std::cout << "You win with a Flush!" << std::endl;
        return true;
    }
    if (isFlush(otherCompleteHand)) {
        std::cout << "Other player wins with a Flush!" << std::endl;
        return false;
    }

    // Straight
    if (isStraight(yourCompleteHand)) {
        std::cout << "You win with a Straight!" << std::endl;
        return true;
    }
    if (isStraight(otherCompleteHand)) {
        std::cout << "Other player wins with a Straight!" << std::endl;
        return false;
    }

    // Three of a Kind
    if (isThreeOfAKind(yourCompleteHand)) {
        std::cout << "You win with Three of a Kind!" << std::endl;
        return true;
    }
    if (isThreeOfAKind(otherCompleteHand)) {
        std::cout << "Other player wins with Three of a Kind!" << std::endl;
        return false;
    }

    // Two Pair
    if (isTwoPair(yourCompleteHand)) {
        std::cout << "You win with Two Pair!" << std::endl;
        return true;
    }
    if (isTwoPair(otherCompleteHand)) {
        std::cout << "Other player wins with Two Pair!" << std::endl;
        return false;
    }

    // One Pair
    if (isPair(yourCompleteHand)) {
        std::cout << "You win with One Pair!" << std::endl;
        return true;
    }
    if (isPair(otherCompleteHand)) {
        std::cout << "Other player wins with One Pair!" << std::endl;
        return false;
    }

    // High Card
    std::string yourHighCard = getHighCard(yourCompleteHand);
    std::string otherHighCard = getHighCard(otherCompleteHand);

    std::cout << "Your high card: " << yourHighCard << std::endl;
    std::cout << "Other player's high card: " << otherHighCard << std::endl;

    if (cardValue(yourHighCard) > cardValue(otherHighCard)) {
        std::cout << "You win with a higher High Card!" << std::endl;
        return true;
    } else if (cardValue(yourHighCard) < cardValue(otherHighCard)) {
        std::cout << "Other player wins with a higher High Card!" << std::endl;
        return false;
    } else {
        std::cout << "It's a tie based on High Cards!" << std::endl;
        return false;
    }
}
