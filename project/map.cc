export module map;

import <utility>;
import <vector>;
import <string>;
import tile;
import edge;
import vertice;
import player;
import subj_obs;
import colour;

export class Map: public Subject {
  std::vector<Tile> tiles;
  std::vector<Edge> edges;
  std::vector<Vertice> vertices;
  Tile* geesed;
  std::vector<Player*> players;
  public:
    Map(std::vector<Tile> tiles, std::vector<Edge> edges, 
      std::vector<Vertice> vertices, std::vector<Player*> players);
    void placeGoose(int tileIndex);
    bool canBuildRoad(int edge, Player* p);
    bool canBuildResidence(int vertex, Player* p);
    std::vector<Player*> buildersOnTile(int tileIndex);
    std::string display();
    Vertice getVertice(int index);
    Edge getEdge(int index);
    Player* getPlayer(Colour colour);
};
