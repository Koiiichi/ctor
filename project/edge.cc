export module edge;

import <string>;
import vertice;
import player;

export class Edge {
  Vertice* v1;
  Vertice* v2;
  Player* road;
  public: 
    bool build(Player* owner);
    string display();
};
