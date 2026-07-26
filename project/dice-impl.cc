module dice;

import <iostream>;
import <string>;

using namespace std;
int LoadedDice::roll() {
  cout << "enter a number between 2 and 12: " ;
  string input;
  cin >> input;
  try {
    int temp{stoi(input)};
    return temp;
  }
  catch (...) {
    cout << "Invalid argument";
    return roll();
  }
}

int FairDice::roll() {
  return (rng() % 6) + (rng() % 6) + 2;
}
