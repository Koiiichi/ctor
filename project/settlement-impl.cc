module settlement;

// vertice is responsible for checking legality of improve
void Settlement::improve() {
  if(level == ResidenceLevel::Basement) {
    level = ResidenceLevel::House;
  }
  else {
    // since vertice is responsible for checking, we can assume the only thing left this
    level = ResidenceLevel::Tower;
  }
}

int Settlement::buildingPoints() {
  if(level == ResidenceLevel::Basement) {
    return 1;
  }
  else if (level == ResidenceLevel::House) {
    return 2;
  }
  // last case is tower
  return 3;
}

// actually identical to building points, returns the mult for resource generation
int Settlement::giveMat() {
  if(level == ResidenceLevel::Basement) {
    return 1;
  }
  else if (level == ResidenceLevel::House) {
    return 2;
  }
  // last case is tower
  return 3;
}
ResidenceLevel Settlement::getLevel() {
  return level;
}

string display() {
  string rVal = "";
  rVal += player->getColour();
  if(level == ResidenceLevel::Basement) {
    rVal += 'B';
  }
  else if (level == ResidenceLevel::House) {
    rVal += 'H';
  }
  else {
    rVal += 'T';
  }
  return rVal;
 
}

Player* belongsTo() {
  return player;
}

