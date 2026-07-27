module board;

import<iostream>;
import<vector>;
import<random>;

using namespace std;

static const int LONGEST_ROAD_BONUS = 2;

static mt19937 &playerRng() {
  static mt19937 engine{static_cast<unsigned>(Dice::seed)};
  return engine;
}

static Material pickWeighted(const vector<int> &counts) {
  int total = counts[0] + counts[1] + counts[2] + counts[3] + counts[4];
  uniform_int_distribution<int> dist{0, total - 1};
  int r = dist(playerRng());
  const Material order[] = {Material::Brick, Material::Energy, Material::Glass,
                            Material::Heat, Material::Wifi};
  for (int i = 0; i < 5; ++i) {
    if (r < counts[i]) {
      return order[i];
    }
    r -= counts[i];
  }
  return Material::Wifi;
}

Player::Player(Colour colour, unique_ptr<Dice> dice, Map& map):
  colour{colour}, dice{std::move(dice)}, map{map} {}

bool Player::buildResidence(int vertex, bool beginning) {
  bool result = map.getVertice(vertex).buildSettlement(this, beginning);
  if(result) {
    settlements.emplace_back(map.getVertice(vertex).getSettlement());
  }
  return result;
}

bool Player::improveResidence(int vertex) {
  return map.getVertice(vertex).improve(this);
}

bool Player::buildRoad(int edge) {
  return map.getEdge(edge).build(this);
}

bool Player::trade(Material give, Material take, Player* other) {
  vector<int> otherMat = other->giveMaterialAmount();
  // if player 1 has enough resources to trade
  if(((give == Material::Brick && numBricks >= 1) || (give == Material::Energy && numEnergy >= 1)
    || (give == Material::Glass && numGlass >= 1) || (give == Material::Heat && numHeat >= 1) 
    || (give == Material::Wifi && numWifi >= 1)) &&
    // and player 2 has enough resources to trade
    ((take == Material::Brick && otherMat[0] >= 1) || (take == Material::Energy && otherMat[1] >= 1)
    || (take == Material::Glass && otherMat[2] >= 1) || (take == Material::Heat && otherMat[3] >= 1) 
    || (take == Material::Wifi && otherMat[4] >= 1))) {
    // this ugly paragraph of a condition checks if the trade can legally happen
    // it's not elegant, but this avoids checking and reducing for player 1, 
    // then finding out player 2 doesn't have the resources and needing to refund player 1
    if (give == Material::Brick) {
      numBricks -= 1;
      other->increase(Material::Brick, 1);
    }
    else if (give == Material::Energy) {
      numEnergy -= 1;
      other->increase(Material::Energy, 1);
    }
    else if (give == Material::Glass) {
      numGlass -= 1;
      other->increase(Material::Glass, 1);
    }
    else if (give == Material::Heat) {
      numHeat -= 1;
      other->increase(Material::Heat, 1);
    }
    else if (give == Material::Wifi) {
      numWifi -= 1;
      other->increase(Material::Wifi, 1);
    }
    if (take == Material::Brick) {
      numBricks++;
      other->reduce(Material::Brick, 1);
    }
    else if (take == Material::Energy) {
      numEnergy++;
      other->reduce(Material::Energy, 1);
    }
    else if (take == Material::Glass) {
      numGlass++;
      other->reduce(Material::Glass, 1);
    }
    else if (take == Material::Heat) {
      numHeat++;
      other->reduce(Material::Heat, 1);
    } 
    else if (take == Material::Wifi) {
      numWifi++;
      other->reduce(Material::Wifi, 1);
    }
    return true;
  }
  return false;
}

int Player::rollDice() {
  return dice->roll();
}

void Player::setDice(unique_ptr<Dice> newDice) {
  dice = std::move(newDice);
}

vector<int> Player::loseHalfToGeese() {
  vector<int> lost(5, 0);
  int sum = numBricks + numEnergy + numGlass + numHeat + numWifi;
  if(sum < 10) {
    return lost;
  }
  int halfTotal = sum / 2;
  const Material order[] = {Material::Brick, Material::Energy, Material::Glass,
                            Material::Heat, Material::Wifi};
  for(int i = 0; i < halfTotal; ++i) {
    Material taken = pickWeighted(giveMaterialAmount());
    reduce(taken, 1);
    for(int j = 0; j < 5; ++j) {
      if(order[j] == taken) ++lost[j];
    }
  }
  return lost;
}

Material Player::stealFrom(Player& victim) {
  Material stolen = pickWeighted(victim.giveMaterialAmount());
  victim.reduce(stolen, 1);
  increase(stolen, 1);
  return stolen;
}

int Player::getBuildingPoints(bool bonusEnabled) const {
  int total = 0;
  for(auto *s : settlements) {
    total += s->buildingPoints();
  }
  if(bonusEnabled && map.hasLongestRoad(this)) {
    total += LONGEST_ROAD_BONUS;
  }
  return total;
}

// reduce is only called within a check-then-act function
void Player::reduce(Material material, int amount) {
  if(material == Material::Brick) {
    numBricks -= amount;
  }
  else if(material == Material::Energy) {
    numEnergy -= amount;
  }
  else if(material == Material::Glass) {
    numGlass -= amount;
  }
  else if(material == Material::Heat) {
    numHeat -= amount;
  }
  else if(material == Material::Wifi) {
    numWifi -= amount;
  }
}

void Player::increase(Material material, int amount) {
  if(material == Material::Brick) {
    numBricks += amount;
  }
  else if(material == Material::Energy) {
    numEnergy += amount;
  }
  else if(material == Material::Glass) {
    numGlass += amount;
  }
  else if(material == Material::Heat) {
    numHeat += amount;
  }
  else if(material == Material::Wifi) {
    numWifi += amount;
  }
}

char Player::getColour() {
  if(colour == Colour::Blue) {
    return 'B';
  }
  else if(colour == Colour::Red) {
    return 'R';
  }
  else if(colour == Colour::Orange) {
    return 'O';
  }
  return 'Y';
}

vector<int> Player::giveMaterialAmount() {
  return {numBricks, numEnergy, numGlass, numHeat, numWifi};
}
