#pragma once

enum Suit 
{
	DIAMOND = 0,
	CLUB,
	SPADE,
	HEART
};


class Card
{
public:
	Card();
	~Card();

	// Getters and Setters
	Suit GetSuit() const;
	void SetSuit(Suit suit);
	int GetCardNumber() const;
	void SetCardNumber(int cardNumber);
	

private:
	Suit suit;
	int cardNumber;
};

