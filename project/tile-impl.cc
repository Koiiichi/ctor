module board;

import <vector>;

using namespace std;

Tile::Tile(Material mat, int value): mat{mat}, value{value} {}

void Tile::update(int rollValue) {
  if (rollValue != value || hasGeese || mat == Material::Park) {
    return;
  }
  for (auto *v : vertices) {
    v->giveMat(mat);
  }
}

TileInfo Tile::display() const {
  return TileInfo{mat, value, hasGeese};
}

void Tile::addEdge(Edge *e) {
  edges.emplace_back(e);
}

void Tile::addVertice(Vertice *v) {
  vertices.emplace_back(v);
}

const vector<Edge *> &Tile::getEdges() const {
  return edges;
}

const vector<Vertice *> &Tile::getVertices() const {
  return vertices;
}

void Tile::setGeese(bool present) {
  hasGeese = present;
}

bool Tile::getGeese() const {
  return hasGeese;
}
