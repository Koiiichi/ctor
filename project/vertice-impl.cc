module board;

import <memory>;
import <string>;
import <vector>;

using namespace std;

Vertice::Vertice(int number): number{number} {}

bool Vertice::buildSettlement(Player *owner, bool freeBuild) {
  if (house != nullptr) {
    return false;
  }
  if (!freeBuild) {
    vector<int> resources = owner->giveMaterialAmount();
    if (resources[0] < 1 || resources[1] < 1 || resources[2] < 1 ||
        resources[4] < 1) {
      return false;
    }
    owner->reduce(Material::Brick, 1);
    owner->reduce(Material::Energy, 1);
    owner->reduce(Material::Glass, 1);
    owner->reduce(Material::Wifi, 1);
  }
  house = make_unique<Settlement>(owner);
  return true;
}

bool Vertice::improve(Player *owner) {
  if (house == nullptr || house->getOwner() != owner) {
    return false;
  }
  vector<int> resources = owner->giveMaterialAmount();
  if (house->getLevel() == ResidenceLevel::Basement) {
    if (resources[2] < 2 || resources[3] < 3) {
      return false;
    }
    owner->reduce(Material::Glass, 2);
    owner->reduce(Material::Heat, 3);
    return house->improve();
  }
  if (house->getLevel() == ResidenceLevel::House) {
    if (resources[0] < 3 || resources[1] < 2 || resources[2] < 2 ||
        resources[3] < 2 || resources[4] < 1) {
      return false;
    }
    owner->reduce(Material::Brick, 3);
    owner->reduce(Material::Energy, 2);
    owner->reduce(Material::Glass, 2);
    owner->reduce(Material::Heat, 2);
    owner->reduce(Material::Wifi, 1);
    return house->improve();
  }
  return false;
}

void Vertice::giveMat(Material mat) {
  if (house != nullptr) {
    house->give(mat);
  }
}

string Vertice::display() const {
  if (house != nullptr) {
    return house->display();
  }
  return to_string(number);
}

void Vertice::addEdge(Edge *e) {
  edges.emplace_back(e);
}

const vector<Edge *> &Vertice::getEdges() const {
  return edges;
}

Settlement *Vertice::getSettlement() const {
  return house.get();
}

int Vertice::getNumber() const {
  return number;
}
