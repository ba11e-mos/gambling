#pragma once
#include "std_lib_facilities.h"

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
	jack = 10,
	queen = 10,
	king = 10,
	ace
};

const map<Rank, string> rankToStringMap {
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

const map<Suit, string> SuitToStringMap {
	{Suit::clubs, "clubs"},
	{Suit::diamonds, "diamonds"},
	{Suit::hearts, "hearts"},
	{Suit::spades, "spades"}
};


string suitToString(Suit suit);

string rankToString(Rank rank);

class Card {
	private:
		Suit s;
		Rank r;
	public:
		Card(Suit suit, Rank rank);
		Suit getSuit();
		Rank getRank();
		string toString();
};
