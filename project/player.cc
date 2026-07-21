export module player;

import <memory>;
import <utility>;

import map;
import colour;
import dice;
import material;

class Player {
  int numBricks;
  int numEnergy;
  int numGlass;
  int numHeat;
  int numWifi;
  int buildingPoints;
  Colour colour;
  std::unique_ptr<Dice> dice;
  Map& map;
  public:
    bool buildResidence(int vertex);
    bool improveResidence(int vertex);
    bool buildRoad(int edge);
    trade(Player& other, Material give, int giveAmount, Material take, int takeAmount);
    int rollDice();
    void setDice(unique_ptr<Dice>);
    int loseHalfToGeese();
    void stealFrom(Player& victim);
};

