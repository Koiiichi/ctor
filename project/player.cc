export module board:player;

import <memory>;
import <utility>;
import <vector>;

import colour;
import dice;
import material;
import :map;
import :settlement;

export class Player {
  int numBricks = 0;
  int numEnergy = 0;
  int numGlass = 0;
  int numHeat = 0;
  int numWifi = 0;
  int buildingPoints = 0;
  Colour colour;
  std::unique_ptr<Dice> dice;
  Map& map;
  std::vector<Settlement> settlements;
  public:
    Player(Colour colour, std::unique_ptr<Dice> dice, Map& map);
    bool buildResidence(int vertex);
    bool improveResidence(int vertex);
    bool buildRoad(int edge);
    // this simluates a one-sided trade, interface will call on the other side when interpreting the commands
    void trade(Material give, Material take);
    int rollDice();
    void setDice(unique_ptr<Dice>);
    int loseHalfToGeese();
    void stealFrom(Player& victim);
    void reduce(Material material, int amount);
    void increase(Material material, int amount);
    char getColour();

    std::vector<int> giveMaterialAmount();
};


