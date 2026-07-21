export module tile;

import <utility>;
import material;
import edge;
import vertice;
import tileInfo;

class Tile {
  Material mat;
  int value;
  bool hasGeese;
  std::vector<Edge*> edges;
  std::vector<Vertice*> vertices;

  public:
    void update(int rollValue);
    TileInfo display();
};


