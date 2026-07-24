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
  std::Vector<Player*> players;
  public:
    Map(std::vector<Tile> tiles, std::vector<Edge> edges, 
      std::vector<Vertice> vertices, std::vector<Player*> players);
    void placeGoose(int tileIndex);
    bool canBuildRoad(int edge, Player* p);
    bool canBuildResidence(int vertex, Player* p);
    vector<Player*> buildersOnTile(int tileIndex);
    string display();
    Vertice getVertice(int index);
    Edge getEdge(int index);
    Player* getPlayer(Colour colour);
};
