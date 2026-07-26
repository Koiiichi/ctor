module board;

import<iostream>;

using namespace std;
Player::Player(Colour colour, unique_ptr<Dice> dice, Map& map):
  colour{colour}, dice{dice}, map{map} {}

bool Player::buildResidence(int vertex) {
  bool result = map.getVertice(vertex).buildSettlement(this);
  if(result) {
    Settlement temp = Settlement();
    map.getVertice(vertex).setSettlement(make_unique<temp>());
    settlements.add(temp)
  }
  return result;
}

bool Player::improveResidence(int vertex) {
  return map.getVertice(vertex).improveSettlement(this);
}

bool Player::buildRoad(int edge) {
  return map.getVertice(edge).build(this);
}

void Player::trade(Material give, Material take) {
  if(give == Material::Brick) {
    numBricks++;
  }
  else if(give == Material::Energy) {
    numEnergy++;
  }
  else if(give == Material::Glass) {
    numGlass++;
  }
  else if(give == Material::Heat) {
    numHeat++;
  }
  else if(give == Material::Wifi) {
    numWifi++;
  }

  if(take == Material::Brick) {
    numBricks--;
  }
  else if(take == Material::Energy) {
    numEnergy--;
  }
  else if(take == Material::Glass) {
    numGlass--;
  }
  else if(take == Material::Heat) {
    numHeat--;
  }
  else if(take == Material::Wifi) {
    numWifi--;
  }
}

int Player::rollDice() {
  dice.roll();
}

void Player::setDice(unique_ptr<Dice> newDice) {
  dice = newDice;
}

int Player::lostHalfToGeese() {
  int sum = numBricks + numEnergy + numGlass + numHeat + numWifi;
  int halfTotal = sum / 2;
  if(sum >= 10) {

    // TODO: randomly remove half (rounded down) resources
  }
  return halfTotal;
}

void Player::stealFrom(Player& victim) {
  // TODO: randomly choose a resource to steal, then 
  return;
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
