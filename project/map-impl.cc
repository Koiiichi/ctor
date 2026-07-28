module board;

import <string>;
import <vector>;

using namespace std;

static const int TILE_ROWS = 9;
static const int VERTEX_ROWS = 11;
static const int VERTEX_COLS = 6;
static const int DISPLAY_ROWS = 41;
static const int DISPLAY_COLS = 54;
static const int MIN_LONGEST_ROAD = 5;

static int roadWalk(Vertice *v, Player *p, vector<bool> &used) {
  int best = 0;
  for (auto *e : v->getEdges()) {
    if (e->getRoad() != p) continue;
    int index = e->getNumber();
    if (used[index]) continue;
    Vertice *other = e->getV1() == v ? e->getV2() : e->getV1();
    Settlement *house = other->getSettlement();
    if (house != nullptr && house->getOwner() != p) {
      if (best < 1) best = 1;
      continue;
    }
    used[index] = true;
    int len = 1 + roadWalk(other, p, used);
    used[index] = false;
    if (len > best) best = len;
  }
  return best;
}

static vector<int> rowPattern(int row) {
  if (row == 0 || row == TILE_ROWS - 1) return {2};
  if (row % 2 == 1) return {1, 3};
  return {0, 2, 4};
}

static vector<int> horizontalPattern(int row) {
  if (row == 0 || row == VERTEX_ROWS - 1) return {2};
  if (row % 2 == 1) return {1, 3};
  return {0, 2, 4};
}

static vector<int> vertexPattern(int row) {
  if (row == 0 || row == VERTEX_ROWS - 1) return {2, 3};
  if (row == 1 || row == VERTEX_ROWS - 2) return {1, 2, 3, 4};
  return {0, 1, 2, 3, 4, 5};
}

static string materialName(Material mat) {
  if (mat == Material::Brick) return "BRICK";
  if (mat == Material::Energy) return "ENERGY";
  if (mat == Material::Glass) return "GLASS";
  if (mat == Material::Heat) return "HEAT";
  if (mat == Material::Wifi) return "WIFI";
  return "PARK";
}

static string pad2(const string &s) {
  if (s.length() >= 2) return s;
  return " " + s;
}

static void place(vector<string> &canvas, int row, int col, const string &s) {
  for (size_t i = 0; i < s.length(); ++i) {
    canvas[row][col + i] = s[i];
  }
}

Map::Map(BoardSource &source) {
  vector<vector<int>> vertexId(VERTEX_ROWS, vector<int>(VERTEX_COLS, -1));
  vector<vector<int>> horizontalId(VERTEX_ROWS, vector<int>(VERTEX_COLS, -1));
  vector<vector<int>> verticalId(VERTEX_ROWS, vector<int>(VERTEX_COLS, -1));

  int nextVertex = 0;
  for (int vr = 0; vr < VERTEX_ROWS; ++vr) {
    for (int vc : vertexPattern(vr)) {
      vertexId[vr][vc] = nextVertex;
      ++nextVertex;
    }
  }

  vertices.reserve(nextVertex);
  for (int i = 0; i < nextVertex; ++i) {
    vertices.emplace_back(i);
  }

  vector<int> edgeFrom;
  vector<int> edgeTo;
  for (int vr = 0; vr < VERTEX_ROWS; ++vr) {
    for (int vc : horizontalPattern(vr)) {
      if (vertexId[vr][vc] < 0 || vertexId[vr][vc + 1] < 0) continue;
      horizontalId[vr][vc] = static_cast<int>(edgeFrom.size());
      edgeFrom.emplace_back(vertexId[vr][vc]);
      edgeTo.emplace_back(vertexId[vr][vc + 1]);
    }
    if (vr + 1 >= VERTEX_ROWS) continue;
    for (int vc : vertexPattern(vr)) {
      if (vertexId[vr + 1][vc] < 0) continue;
      verticalId[vr][vc] = static_cast<int>(edgeFrom.size());
      edgeFrom.emplace_back(vertexId[vr][vc]);
      edgeTo.emplace_back(vertexId[vr + 1][vc]);
    }
  }

  edges.reserve(edgeFrom.size());
  for (size_t i = 0; i < edgeFrom.size(); ++i) {
    edges.emplace_back(&vertices[edgeFrom[i]], &vertices[edgeTo[i]],
                       static_cast<int>(i));
  }
  for (size_t i = 0; i < edges.size(); ++i) {
    vertices[edgeFrom[i]].addEdge(&edges[i]);
    vertices[edgeTo[i]].addEdge(&edges[i]);
  }

  vector<TileInfo> info = source.getTiles();
  vector<int> tileRow;
  vector<int> tileCol;
  for (int r = 0; r < TILE_ROWS; ++r) {
    for (int vc : rowPattern(r)) {
      tileRow.emplace_back(r);
      tileCol.emplace_back(vc);
    }
  }

  tiles.reserve(tileRow.size());
  for (size_t i = 0; i < tileRow.size() && i < info.size(); ++i) {
    tiles.emplace_back(info[i].mat, info[i].value);
  }

  for (size_t i = 0; i < tiles.size(); ++i) {
    int r = tileRow[i];
    int vc = tileCol[i];
    for (int dr = 0; dr < 3; ++dr) {
      tiles[i].addVertice(&vertices[vertexId[r + dr][vc]]);
      tiles[i].addVertice(&vertices[vertexId[r + dr][vc + 1]]);
    }
    tiles[i].addEdge(&edges[horizontalId[r][vc]]);
    tiles[i].addEdge(&edges[verticalId[r][vc]]);
    tiles[i].addEdge(&edges[verticalId[r][vc + 1]]);
    tiles[i].addEdge(&edges[verticalId[r + 1][vc]]);
    tiles[i].addEdge(&edges[verticalId[r + 1][vc + 1]]);
    tiles[i].addEdge(&edges[horizontalId[r + 2][vc]]);
    addObserver(&tiles[i]);
  }
}

void Map::placeGoose(int tileIndex) {
  if (tileIndex < 0 || tileIndex >= static_cast<int>(tiles.size())) return;
  if (goosed != nullptr) {
    goosed->setGeese(false);
  }
  goosed = &tiles[tileIndex];
  goosed->setGeese(true);
}

bool Map::canBuildRoad(int edge, Player *p) {
  if (edge < 0 || edge >= static_cast<int>(edges.size())) return false;
  if (edges[edge].getRoad() != nullptr) return false;
  Vertice *ends[2] = {edges[edge].getV1(), edges[edge].getV2()};
  for (auto *v : ends) {
    Settlement *house = v->getSettlement();
    if (house != nullptr) {
      if (house->getOwner() == p) return true;
      continue;
    }
    for (auto *e : v->getEdges()) {
      if (e->getRoad() == p) return true;
    }
  }
  return false;
}

bool Map::canBuildResidence(int vertex, Player *p, bool beginning) {
  if (vertex < 0 || vertex >= static_cast<int>(vertices.size())) return false;
  if (vertices[vertex].getSettlement() != nullptr) return false;
  for (auto *e : vertices[vertex].getEdges()) {
    Vertice *other =
        e->getV1() == &vertices[vertex] ? e->getV2() : e->getV1();
    if (other->getSettlement() != nullptr) return false;
  }
  if (beginning) return true;
  for (auto *e : vertices[vertex].getEdges()) {
    if (e->getRoad() == p) return true;
  }
  return false;
}

int Map::getGooseTile() const {
  for (int i = 0; i < static_cast<int>(tiles.size()); ++i) {
    if (&tiles[i] == goosed) return i;
  }
  return -1;
}

int Map::longestRoadFor(Player *p) {
  vector<bool> used(edges.size(), false);
  int best = 0;
  for (auto &v : vertices) {
    int len = roadWalk(&v, p, used);
    if (len > best) best = len;
  }
  return best;
}

void Map::updateLongestRoad() {
  if (longestRoadHolder != nullptr) {
    longestRoadLength = longestRoadFor(longestRoadHolder);
  }
  vector<Player *> owners;
  for (auto &e : edges) {
    Player *road = e.getRoad();
    if (road == nullptr || road == longestRoadHolder) continue;
    bool seen = false;
    for (auto *o : owners) {
      if (o == road) seen = true;
    }
    if (!seen) owners.emplace_back(road);
  }
  for (auto *p : owners) {
    int len = longestRoadFor(p);
    if (len < MIN_LONGEST_ROAD) continue;
    if (longestRoadHolder == nullptr || len > longestRoadLength) {
      longestRoadHolder = p;
      longestRoadLength = len;
    }
  }
}

bool Map::hasLongestRoad(const Player *p) const {
  return p != nullptr && p == longestRoadHolder;
}

vector<Player *> Map::buildersOnTile(int tileIndex) const {
  vector<Player *> builders;
  if (tileIndex < 0 || tileIndex >= static_cast<int>(tiles.size())) {
    return builders;
  }
  for (auto *v : tiles[tileIndex].getVertices()) {
    Settlement *house = v->getSettlement();
    if (house == nullptr) continue;
    Player *owner = house->getOwner();
    bool seen = false;
    for (auto *b : builders) {
      if (b == owner) seen = true;
    }
    if (!seen) builders.emplace_back(owner);
  }
  return builders;
}

string Map::display() const {
  vector<string> canvas(DISPLAY_ROWS, string(DISPLAY_COLS, ' '));

  vector<vector<int>> vertexId(VERTEX_ROWS, vector<int>(VERTEX_COLS, -1));
  int nextVertex = 0;
  for (int vr = 0; vr < VERTEX_ROWS; ++vr) {
    for (int vc : vertexPattern(vr)) {
      vertexId[vr][vc] = nextVertex;
      ++nextVertex;
    }
  }

  int nextEdge = 0;
  for (int vr = 0; vr < VERTEX_ROWS; ++vr) {
    for (int vc : vertexPattern(vr)) {
      place(canvas, vr * 4, vc * 10,
            "|" + pad2(vertices[vertexId[vr][vc]].display()) + "|");
    }
    for (int vc : horizontalPattern(vr)) {
      if (vertexId[vr][vc] < 0 || vertexId[vr][vc + 1] < 0) continue;
      place(canvas, vr * 4, vc * 10 + 4,
            "--" + pad2(edges[nextEdge].display()) + "--");
      ++nextEdge;
    }
    if (vr + 1 >= VERTEX_ROWS) continue;
    for (int vc : vertexPattern(vr)) {
      if (vertexId[vr + 1][vc] < 0) continue;
      place(canvas, vr * 4 + 1, vc * 10 + 2, "|");
      place(canvas, vr * 4 + 2, vc * 10 + 1,
            pad2(edges[nextEdge].display()));
      place(canvas, vr * 4 + 3, vc * 10 + 2, "|");
      ++nextEdge;
    }
  }

  int index = 0;
  for (int r = 0; r < TILE_ROWS; ++r) {
    for (int vc : rowPattern(r)) {
      if (index >= static_cast<int>(tiles.size())) break;
      TileInfo info = tiles[index].display();
      int row = r * 4;
      int col = vc * 10;
      place(canvas, row + 2, col + 6, pad2(to_string(index)));
      string name = materialName(info.mat);
      place(canvas, row + 3, col + 3 + (9 - static_cast<int>(name.length())) / 2,
            name);
      if (info.mat != Material::Park) {
        place(canvas, row + 4, col + 6, pad2(to_string(info.value)));
      }
      if (info.geesePresent) {
        place(canvas, row + 5, col + 5, "GEESE");
      }
      ++index;
    }
  }

  string out;
  for (auto &line : canvas) {
    size_t end = line.find_last_not_of(' ');
    if (end != string::npos) {
      out += line.substr(0, end + 1);
    }
    out += "\n";
  }
  return out;
}

Vertice &Map::getVertice(int index) {
  return vertices[index];
}

Edge &Map::getEdge(int index) {
  return edges[index];
}

Tile &Map::getTile(int index) {
  return tiles[index];
}

int Map::numTiles() const {
  return static_cast<int>(tiles.size());
}

int Map::numEdges() const {
  return static_cast<int>(edges.size());
}

int Map::numVertices() const {
  return static_cast<int>(vertices.size());
}
