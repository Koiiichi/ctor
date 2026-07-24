export module edge;

import <string>;
import vertice;
import player;

export class Edge {
  Vertice* v1;
  Vertice* v2;
  Player* road = nullptr;
  int num;
  public: 
    Edge(Vertice* v1, Vertice* v2, int number);
    bool build(Player* owner);
    std::string display();
};
