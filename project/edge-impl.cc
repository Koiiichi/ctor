module board;

import <string>;
import <vector>;

using namespace std;

Edge::Edge(Vertice *v1, Vertice *v2, int number):
  v1{v1}, v2{v2}, num{number} {}

bool Edge::build(Player *owner) {
  if (road != nullptr) {
    return false;
  }
  vector<int> resources = owner->giveMaterialAmount();
  if (resources[3] < 1 || resources[4] < 1) {
    return false;
  }
  owner->reduce(Material::Heat, 1);
  owner->reduce(Material::Wifi, 1);
  road = owner;
  return true;
}

bool Edge::restore(Player *owner) {
  if (road != nullptr) {
    return false;
  }
  road = owner;
  return true;
}

string Edge::display() const {
  if (road != nullptr) {
    string temp;
    temp += road->getColour();
    temp += 'R';
    return temp;
  }
  return to_string(num);
}

Vertice *Edge::getV1() const {
  return v1;
}

Vertice *Edge::getV2() const {
  return v2;
}

Player *Edge::getRoad() const {
  return road;
}

int Edge::getNumber() const {
  return num;
}
