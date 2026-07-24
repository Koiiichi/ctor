module vertice;

import settlement;
import settlementLevel;

using namespace std;

Vertice::Vertice(int num, vector<Edge*> edges):
  number{num}, edges{edges} {}

bool Vertice::buildSettlement(Player* owner) {
  // if house isn't on vertice
  if(house != nullptr) {
    // pointer to first element in array of 5 ints
    vector<int> resources = owner->giveMaterialAmount();
    // if player has enough resources
    if(resources[0] >= 1 && resources[1] >= 1 
      && resources[3] >= 1 && resources[4] >= 1) {
        // remove resources and return true to signify that a house was made
        owner->reduce(Material::Brick);
        owner->reduce(Material::Energy);
        owner->reduce(Material::Glass);
        owner->reduce(Material::Wifi);
        return true;
      }
  }
  // house is taken already, cannot build
  return false;
}

bool Vertice::improve(Player* owner) {
  // if house is non-existent, it's impossible to improve
  if(house == nullptr) {
    return false;
  }
  vector<int> resources = owner->giveMaterialAmount();
  // check if the player has enough resources to upgrade that settlement
  if(house->getLevel() == SettlementLevel::Basement) {
    // enough to improve basement to house
    if(resources[2] >= 2 && resources[3] >= 3) {
      owner->reduce(Material::Glass);
      owner->reduce(Material::Glass);
      owner->reduce(Material::Heat);
      owner->reduce(Material::Heat);
      owner->reduce(Material::Heat);
      return true;
    }
  }
  else if(house->getLevel() == SettlementLevel::House) {
    // enough to improve house to tower
    if(resources[0] >= 3 && resources[1] >= 2 && resources[2] >= 2
    && resources[3] >= 2 && resources[4] >= 1) {
      owner->reduce(Material::Brick);
      owner->reduce(Material::Brick);
      owner->reduce(Material::Brick);
      owner->reduce(Material::Energy);
      owner->reduce(Material::Energy);
      owner->reduce(Material::Glass);
      owner->reduce(Material::Glass);
      owner->reduce(Material::Heat);
      owner->reduce(Material::Heat);
      owner->reduce(Material::Wifi);
      return true;
    }
  }
  // house is tower and cannot be improved
  return false;
}

void Vertice::giveMat(Material mat) {
  if(house != nullptr) {
    house->belongsTo()->increase(mat, house->getMat());
  }
  
}

void Vertice::setSettlement(unique_ptr<Settlement> newHouse) {
  house = newHouse;
}

string Vertice::display() {
  if(house != nullptr) {
    return house->display();
  }
  return return number;
}

