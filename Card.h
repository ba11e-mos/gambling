#pragma once
#include <iostream>
#include <map>
#include <string>

enum class Suit {
	clubs, 
	diamonds,
	hearts,
	spades
};

enum class Rank {
	two = 2,
	three,
	four,
	five,
	six,
	seven,
	eight,
	nine,
	ten,
	jack,
	queen,
	king,
	ace
};

const std::map<Rank, std::string> rankToStringMap {
	{Rank::two, "two"},
	{Rank::three, "three"},
	{Rank::four, "four"},
	{Rank::five, "five"},
	{Rank::six, "six"},
	{Rank::seven, "seven"},
	{Rank::eight, "eight"},
	{Rank::nine, "nine"},
	{Rank::ten, "ten"},
	{Rank::jack, "jack"},
	{Rank::queen, "queen"},
	{Rank::king, "king"},
	{Rank::ace, "ace"}
};

const std::map<Suit, std::string> SuitToStringMap {
	{Suit::clubs, "clubs"},
	{Suit::diamonds, "diamonds"},
	{Suit::hearts, "hearts"},
	{Suit::spades, "spades"}
};


std::string suitToString(Suit suit);

std::string rankToString(Rank rank);

class Card {
	private:
		Suit s;
		Rank r;
	public:
		Card();
		Card(Suit suit, Rank rank);
		Suit getSuit() const;
		Rank getRank() const;
		std::string toString() const;
		std::string cardFileName(Card card);
};


std::ostream& operator<<(std::ostream& os, const Card& card);