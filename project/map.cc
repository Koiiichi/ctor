export module map;

import <utility>;
import <string>;
import tile;
import edge;
import vertice;
import player;
import subj_obs;

export class Map: public Subject {
  std::vector<Tile> tiles;
  std::vector<Edge> edges;
  std::vector<Vertice> vertices;
  Tile* geesed;
  public:
    void placeGoose(int tileIndex);
    bool canBuildRoad(int edge, Player* p);
    bool canBuildResidence(int vertex, Player* p);
    vector<Player*> buildersOnTile(int tileIndex);
    string display();
};
