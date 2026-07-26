export module board:map;

import <string>;
import <vector>;
import boardSource;
import subj_obs;
import :edge;
import :tile;
import :vertice;

export class Map: public Subject {
  std::vector<Tile> tiles;
  std::vector<Edge> edges;
  std::vector<Vertice> vertices;
  Tile *goosed = nullptr;
  public:
    Map(BoardSource &source);
    void placeGoose(int tileIndex);
    bool canBuildRoad(int edge, Player *p);
    bool canBuildResidence(int vertex, Player *p);
    std::vector<Player *> buildersOnTile(int tileIndex) const;
    std::string display() const;
    Vertice &getVertice(int index);
    Edge &getEdge(int index);
    Tile &getTile(int index);
    int numTiles() const;
    int numEdges() const;
    int numVertices() const;
};
