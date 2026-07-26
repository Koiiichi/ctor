export module dice;

import <algorithm>;
import <random>;
import <chrono>;

export class Dice {
  protected:
    std::mt19937 rng;
  public:
    virtual int roll() = 0;
    static int seed;
    static void setSeed(int newSeed) {
      seed = newSeed;
    }
    Dice();
    virtual ~Dice() = default;
};

// give it a default value
int Dice::seed = 0;

export class LoadedDice: public Dice {
  public:
    int roll() override;
};

export class FairDice: public Dice {
  public:
    int roll() override;
};
