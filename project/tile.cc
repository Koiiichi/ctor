export module tile;

import <utility>;
import <vector>;
import material;
import tileInfo;

class Edge;
class Vertice;

export class Tile {
  Material mat;
  int value;
  bool hasGeese;
  std::vector<Edge*> edges;
  std::vector<Vertice*> vertices;

  public:
    void update(int rollValue);
    TileInfo display();
};


