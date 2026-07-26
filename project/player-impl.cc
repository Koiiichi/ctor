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
      other->decrease(Material::Brick, 1);
    }
    else if (take == Material::Energy) {
      numEnergy++;
      other->decrease(Material::Energy, 1);
    }
    else if (take == Material::Glass) {
      numGlass++;
      other->decrease(Material::Glass, 1);
    }
    else if (take == Material::Heat) {
      numHeat++;
      other->decrease(Material::Heat, 1);
    } 
    else if (take == Material::Wifi) {
      numWifi++;
      other->decrease(Material::Wifi, 1);
    }
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
