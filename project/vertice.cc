export module board:vertice;

import <memory>;
import <string>;
import <vector>;
import material;
import :settlement;

export class Edge;

export class Vertice {
  int number;
  std::unique_ptr<Settlement> house = nullptr;
  std::vector<Edge *> edges;
  public:
    Vertice(int number);
    bool buildSettlement(Player *owner);
    bool improve(Player *owner);
    void giveMat(Material mat);
    std::string display() const;
    void addEdge(Edge *e);
    const std::vector<Edge *> &getEdges() const;
    Settlement *getSettlement() const;
    int getNumber() const;
};
