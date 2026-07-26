export module board:tile;

import <vector>;
import material;
import tileInfo;
import subj_obs;
import :edge;
import :vertice;

export class Tile: public Observer {
  Material mat;
  int value;
  bool hasGeese = false;
  std::vector<Edge *> edges;
  std::vector<Vertice *> vertices;

  public:
    Tile(Material mat, int value);
    void update(int rollValue) override;
    TileInfo display() const;
    void addEdge(Edge *e);
    void addVertice(Vertice *v);
    const std::vector<Edge *> &getEdges() const;
    const std::vector<Vertice *> &getVertices() const;
    void setGeese(bool present);
    bool getGeese() const;
};
