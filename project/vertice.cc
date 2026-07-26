export module vertice;

import <memory>;
import <utility>;
import <string>;
import <vector>;
import material;
import player;
import settlement;

class Edge;

export class Vertice {
  int number;
  std::unique_ptr<Settlement> house = nullptr;
  std::vector<Edge*> edges;
  public:
    Vertice(int num, std::vector<Edge*> edges);
    bool buildSettlement(Player* owner);
    bool improve(Player* owner);
    void giveMat(Material mat);
    void setSettlement(std::unique_ptr<Settlement> newHouse);
    std::string display();
};

