export module dice;

import <algorithm>;
import <random>;
import <chrono>;

export class Dice {
  std::default_random_engine rng;
  public:
    int roll() const;
    static int seed;
    static void setSeed(int newSeed) {
      seed = newSeed;
    }
    Dice();
};

int Dice::seed = 0;

export class LoadedDice: public Dice {
  int roll() const;
};

export class FairDice: public Dice {
  public:
    int roll() const;
};
