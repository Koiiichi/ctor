export module board:edge;

import <string>;
import :vertice;

export class Edge {
  Vertice *v1;
  Vertice *v2;
  Player *road = nullptr;
  int num;
  public:
    Edge(Vertice *v1, Vertice *v2, int number);
    bool build(Player *owner);
    std::string display() const;
    Vertice *getV1() const;
    Vertice *getV2() const;
    Player *getRoad() const;
    int getNumber() const;
};
