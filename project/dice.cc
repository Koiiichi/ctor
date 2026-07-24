export module dice;

import <algorithm>;
import <random>;
import <chrono>;

export class Dice {
  std::default_random_engine rng;
  public:
    int roll() const;
    static unsigned seed = 0;
    static setSeed(int newSeed) {
      seed = newSeed;
    };
    Dice();
};

export class LoadedDice: public Dice {
  int roll() const override;
};

export class FairDice: public Dice {
  public:
    int roll() const override;
};
