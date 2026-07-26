module board;

import <string>;

using namespace std;

Settlement::Settlement(Player *player): player{player} {}

void Settlement::give(Material mat) {
  player->increase(mat, buildingPoints());
}

bool Settlement::improve() {
  if (level == ResidenceLevel::Basement) {
    level = ResidenceLevel::House;
    return true;
  }
  if (level == ResidenceLevel::House) {
    level = ResidenceLevel::Tower;
    return true;
  }
  return false;
}

int Settlement::buildingPoints() const {
  if (level == ResidenceLevel::Basement) {
    return 1;
  }
  if (level == ResidenceLevel::House) {
    return 2;
  }
  return 3;
}

ResidenceLevel Settlement::getLevel() const {
  return level;
}

Player *Settlement::getOwner() const {
  return player;
}

string Settlement::display() const {
  string rVal;
  rVal += player->getColour();
  if (level == ResidenceLevel::Basement) {
    rVal += 'B';
  } else if (level == ResidenceLevel::House) {
    rVal += 'H';
  } else {
    rVal += 'T';
  }
  return rVal;
}
