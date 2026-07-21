export module dice;

export class Dice {
  public:
    int roll() const;
};

export class LoadedDice: public Dice {
  int roll() const override;
};

export class FairDice: public Dice {
  int roll() const override;
};
