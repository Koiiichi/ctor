export module settlement;

import <string>;
import residenceLevel;
import material;
import colour;

class Player;

export class Settlement {
  ResidenceLevel level = ResidenceLevel::Basement;
  Player* player;
  public:
    void improve();
    int buildingPoints();
    int giveMat();
    ResidenceLevel getLevel();
    std::string display();
    Player* belongsTo();
};
