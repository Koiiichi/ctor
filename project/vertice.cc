export module vertice;

import <memory>;
import <utility>;
import <string>;
import material;
import settlement;
import player;

export class Vertice {
  std::unique_ptr<SettleMent> house;
  std::vector<Edge*> edges;
  public:
    bool buildSettlement(Player* owner);
    bool improve(Player* owner);
    void giveMat(Material mat);
    string display();
};
