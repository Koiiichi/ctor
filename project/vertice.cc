export module vertice;

import <memory>;
import <utility>;
import <string>;
import material;
import player;

export class Vertice {
  int number;
  std::unique_ptr<SettleMent> house = nullptr;
  std::vector<Edge*> edges;
  public:
    Vertice(int num, std::vector<Edge*> edges);
    bool buildSettlement(Player* owner);
    bool improve(Player* owner);
    void giveMat(Material mat);
    void setSettlement(std::unique_ptr<Settlement> newHouse);
    string display();
};

