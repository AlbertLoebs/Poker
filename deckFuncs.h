#ifndef DECKFUNCS_H
#define DECKFUNCS_H

#include <vector>
#include <string>

// Function declarations
void shuffleDeck(std::vector<std::string>& deck);
void dealCards(std::vector<std::string>& deck, std::vector<std::string>& yourHand, std::vector<std::string>& otherHand);
void dealCommunityCards(std::vector<std::string>& deck, std::vector<std::string>& communityHand);
bool determineWinner(const std::vector<std::string>& yourHand, const std::vector<std::string>& otherHand, const std::vector<std::string>& communityHand);

// Hand evaluation functions
bool isRoyalFlush(const std::vector<std::string>& hand);
bool isStraightFlush(const std::vector<std::string>& hand);
bool isFourOfAKind(const std::vector<std::string>& hand);
bool isFullHouse(const std::vector<std::string>& hand);
bool isFlush(const std::vector<std::string>& hand);
bool isStraight(const std::vector<std::string>& hand);
bool isThreeOfAKind(const std::vector<std::string>& hand);
bool isTwoPair(const std::vector<std::string>& hand);
bool isPair(const std::vector<std::string>& hand);
std::string getHighCard(const std::vector<std::string>& hand);

// Helper functions
std::string getSuit(const std::string& card);
std::string getRank(const std::string& card);
int cardValue(const std::string& card);

#endif // DECKFUNCS_H