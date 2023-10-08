#include <iostream>
#include <string>
#include <algorithm>  // for std::find
#include <iterator>   // for std::begin, std::end
#include <cmath>      // for std::ceil

using namespace std;

/* POKER FUNCTIONS */
bool heart_royal_flush(int hand[])
{
  sort(hand, hand + 5);
  int suit = hand[0] / 13;
  int flush[] = {1, 10, 11, 12, 13};

  for(int i = 0; i < 5; i++)
  {
    int card = hand[i] % 13;
    card = card == 0 ? 13 : card;
    if(card != flush[i])
    {
      return false;
    }
  }

  return true;
}

bool black_two_pair(int hand[])
{
  int cards[5];

  // Checks if all suits are black
  for(int i = 0; i < 5; i++)
  {
    int suit = ceil((double) hand[0] / 13);

    if(suit != 1 && suit != 2)
    {
      return false;
    }

    int card = hand[i] % 13;
    cards[i] = card == 0 ? 13 : card;
  }

  sort(cards, cards + 5);
  int pair = 0;
  // Checks for 2 pairs
  for(int i = 1; i < 5; i++)
  {
    if(cards[i] == cards[i - 1])
    {
      pair++;
    }
  }

  return pair == 2;
}


/* MAIN CODE */
int main() 
{
  // Checks Your Hand, If You Got a Whacky Poker Combination
  int hand[5];
  int i = 0;
  int temp;

  while(i < 5)
  {
    cout << "Enter a number between 1 - 52: ";
    cin >> temp;
    bool exists = find(begin(hand), end(hand), temp) != end(hand);
    while(exists || temp < 1 || temp > 52) {
      cout << "You have already used this number or its out of range. Please enter a number between 1 - 52: ";
      cin >> temp;
      exists = find(begin(hand), end(hand), temp) != end(hand);
    }
    hand[i++] = temp;
  }

  if(heart_royal_flush(hand))
  {
    cout << "Congrats! You got a hearty royal flush!";
  }
  else if(black_two_pair(hand))
  {
    cout << "Congrats! You got a black two pair!";
  }
  else
  {
    cout << "Aww man...seems like you didn't get any combo! For sure next time!";
  }
}