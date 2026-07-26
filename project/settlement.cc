export module board:settlement;

import <string>;
import residenceLevel;
import material;

class Player;

export class Settlement {
  ResidenceLevel level = ResidenceLevel::Basement;
  Player *player;
  public:
    Settlement(Player *player);
    void give(Material mat);
    bool improve();
    int buildingPoints() const;
    ResidenceLevel getLevel() const;
    Player *getOwner() const;
    std::string display() const;
};
