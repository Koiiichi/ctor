module interface;

import <cstdio>;
import <fstream>;
import <iostream>;
import <sstream>;
import <utility>;
import colour;
import dice;
import residenceLevel;
import tileInfo;

using namespace std;

static const char *COLOUR_NAMES[] = {"Blue", "Red", "Orange", "Yellow"};
static const int BANK_TRADE_RATIO = 4;

static string materialName(Material mat) {
  switch (mat) {
    case Material::Brick: return "BRICK";
    case Material::Energy: return "ENERGY";
    case Material::Glass: return "GLASS";
    case Material::Heat: return "HEAT";
    case Material::Wifi: return "WIFI";
    default: return "PARK";
  }
}

static int materialCode(Material mat) {
  switch (mat) {
    case Material::Brick: return 0;
    case Material::Energy: return 1;
    case Material::Glass: return 2;
    case Material::Heat: return 3;
    case Material::Wifi: return 4;
    default: return 5;
  }
}

static bool parseMaterial(const string &word, Material &out) {
  if (word == "BRICK") { out = Material::Brick; return true; }
  if (word == "ENERGY") { out = Material::Energy; return true; }
  if (word == "GLASS") { out = Material::Glass; return true; }
  if (word == "HEAT") { out = Material::Heat; return true; }
  if (word == "WIFI") { out = Material::Wifi; return true; }
  return false;
}

static bool parseColour(const string &word, int &out) {
  for (int i = 0; i < 4; ++i) {
    if (word == COLOUR_NAMES[i]) { out = i; return true; }
  }
  return false;
}

static bool parseInt(const string &word, int &out) {
  if (word.empty()) return false;
  size_t consumed = 0;
  try {
    int value = stoi(word, &consumed);
    if (consumed != word.size()) return false;
    out = value;
    return true;
  } catch (...) {
    return false;
  }
}

static char levelChar(ResidenceLevel level) {
  if (level == ResidenceLevel::Basement) return 'B';
  if (level == ResidenceLevel::House) return 'H';
  return 'T';
}

Interface::Interface(int argc, char *argv[]) {
  for (int i = 1; i < argc; ++i) {
    string arg = argv[i];
    if (arg == "-seed" && i + 1 < argc) {
      int value = 0;
      if (parseInt(argv[++i], value)) Dice::setSeed(value);
    } else if (arg == "-load" && i + 1 < argc) {
      loadFile = argv[++i];
    } else if (arg == "-board" && i + 1 < argc) {
      boardFile = argv[++i];
    } else if (arg == "-random-board") {
      useRandomBoard = true;
    } else if (arg == "-enablebonus") {
      bonusEnabled = true;
    }
  }
}

void Interface::setUpBoard() {
  if (!boardFile.empty()) {
    FileBoardSource src{boardFile};
    map = make_unique<Map>(src);
  } else if (useRandomBoard) {
    RandomBoardSource src{static_cast<unsigned>(Dice::seed)};
    map = make_unique<Map>(src);
  } else {
    RandomBoardSource src{static_cast<unsigned>(Dice::seed)};
    map = make_unique<Map>(src);
  }
  for (int i = 0; i < map->numTiles(); ++i) {
    if (map->getTile(i).display().mat == Material::Park) {
      map->placeGoose(i);
      break;
    }
  }
}

void Interface::setUpPlayers() {
  players.clear();
  const Colour order[] = {Colour::Blue, Colour::Red, Colour::Orange,
                          Colour::Yellow};
  for (int i = 0; i < 4; ++i) {
    players.emplace_back(
        make_unique<Player>(order[i], make_unique<LoadedDice>(), *map));
  }
}

void Interface::openingBasements() {
  const int order[] = {0, 1, 2, 3, 3, 2, 1, 0};
  for (int slot = 0; slot < 8; ++slot) {
    int p = order[slot];
    while (true) {
      cout << "Builder " << COLOUR_NAMES[p]
           << ", where do you want to build a basement?" << endl;
      cout << "> ";
      string word;
      if (!(cin >> word)) return;
      int vertex = 0;
      if (!parseInt(word, vertex) ||
          !map->canBuildResidence(vertex, players[p].get(), true)) {
        cout << "You cannot build here." << endl;
        continue;
      }
      players[p]->buildResidence(vertex, true);
      break;
    }
  }
  cout << map->display() << endl;
}

void Interface::printStatus(int index) const {
  vector<int> res = players[index]->giveMaterialAmount();
  cout << COLOUR_NAMES[index] << " has "
       << players[index]->getBuildingPoints(bonusEnabled)
       << " building points, " << res[0] << " brick, " << res[1] << " energy, "
       << res[2] << " glass, " << res[3] << " heat, and " << res[4] << " WiFi."
       << endl;
}

void Interface::printResidences(int index) const {
  cout << COLOUR_NAMES[index] << " has built:" << endl;
  for (int v = 0; v < map->numVertices(); ++v) {
    Settlement *s = map->getVertice(v).getSettlement();
    if (s != nullptr && s->getOwner() == players[index].get()) {
      cout << v << " " << levelChar(s->getLevel()) << endl;
    }
  }
}

void Interface::distribute(int roll) {
  vector<vector<int>> before;
  for (auto &p : players) before.emplace_back(p->giveMaterialAmount());
  map->notify(roll);
  bool any = false;
  for (int i = 0; i < 4; ++i) {
    vector<int> after = players[i]->giveMaterialAmount();
    bool gained = false;
    for (int j = 0; j < 5; ++j) {
      if (after[j] > before[i][j]) gained = true;
    }
    if (!gained) continue;
    any = true;
    cout << "Builder " << COLOUR_NAMES[i] << " gained:" << endl;
    const Material order[] = {Material::Brick, Material::Energy,
                              Material::Glass, Material::Heat, Material::Wifi};
    for (int j = 0; j < 5; ++j) {
      if (after[j] > before[i][j]) {
        cout << after[j] - before[i][j] << " " << materialName(order[j]) << endl;
      }
    }
  }
  if (!any) cout << "No builders gained resources." << endl;
}

void Interface::handleSeven() {
  const Material order[] = {Material::Brick, Material::Energy, Material::Glass,
                            Material::Heat, Material::Wifi};
  for (int i = 0; i < 4; ++i) {
    vector<int> lost = players[i]->loseHalfToGeese();
    int total = lost[0] + lost[1] + lost[2] + lost[3] + lost[4];
    if (total == 0) continue;
    cout << "Builder " << COLOUR_NAMES[i] << " loses " << total
         << " resources to the geese. They lose:" << endl;
    for (int j = 0; j < 5; ++j) {
      if (lost[j] > 0) cout << lost[j] << " " << materialName(order[j]) << endl;
    }
  }

  int tile = -1;
  while (true) {
    cout << "Choose where to place the GEESE." << endl;
    cout << "> ";
    string word;
    if (!(cin >> word)) return;
    if (!parseInt(word, tile) || tile < 0 || tile >= map->numTiles() ||
        tile == map->getGooseTile()) {
      cout << "You cannot build here." << endl;
      continue;
    }
    break;
  }
  map->placeGoose(tile);

  vector<Player *> onTile = map->buildersOnTile(tile);
  vector<int> candidates;
  for (int i = 0; i < 4; ++i) {
    if (players[i].get() == players[curTurn].get()) continue;
    bool present = false;
    for (auto *b : onTile) {
      if (b == players[i].get()) present = true;
    }
    if (!present) continue;
    vector<int> res = players[i]->giveMaterialAmount();
    if (res[0] + res[1] + res[2] + res[3] + res[4] > 0) candidates.emplace_back(i);
  }

  if (candidates.empty()) {
    cout << "Builder " << COLOUR_NAMES[curTurn] << " has no builders to steal from."
         << endl;
    return;
  }

  cout << "Builder " << COLOUR_NAMES[curTurn] << " can choose to steal from ";
  for (size_t i = 0; i < candidates.size(); ++i) {
    if (i > 0) cout << ", ";
    cout << COLOUR_NAMES[candidates[i]];
  }
  cout << "." << endl;

  while (true) {
    cout << "Choose a builder to steal from." << endl;
    cout << "> ";
    string word;
    if (!(cin >> word)) return;
    int victim = 0;
    bool ok = parseColour(word, victim);
    if (ok) {
      ok = false;
      for (int c : candidates) {
        if (c == victim) ok = true;
      }
    }
    if (!ok) {
      cout << "Invalid command." << endl;
      continue;
    }
    Material stolen = players[curTurn]->stealFrom(*players[victim]);
    cout << "Builder " << COLOUR_NAMES[curTurn] << " steals "
         << materialName(stolen) << " from builder " << COLOUR_NAMES[victim]
         << "." << endl;
    return;
  }
}

bool Interface::beginningOfTurn() {
  cout << map->display() << endl;
  cout << "Builder " << COLOUR_NAMES[curTurn] << "'s turn." << endl;
  printStatus(curTurn);
  while (true) {
    cout << "> ";
    string cmd;
    if (!(cin >> cmd)) return false;
    if (cmd == "load") {
      players[curTurn]->setDice(make_unique<LoadedDice>());
    } else if (cmd == "fair") {
      players[curTurn]->setDice(make_unique<FairDice>());
    } else if (cmd == "roll") {
      int roll = players[curTurn]->rollDice();
      if (!cin) return false;
      if (roll == 7) {
        handleSeven();
        if (!cin) return false;
      } else {
        distribute(roll);
      }
      return true;
    } else if (cmd == "help") {
      cout << "Valid commands:" << endl << "board" << endl << "status" << endl
           << "residences" << endl << "build-road <edge#>" << endl
           << "build-res <housing#>" << endl << "improve <housing#>" << endl
           << "trade <colour> <give> <take>" << endl
           << "bank <give> <take>" << endl << "enable-bonus" << endl
           << "disable-bonus" << endl << "next" << endl
           << "save <file>" << endl << "help" << endl;
    } else {
      cout << "Invalid command." << endl;
    }
  }
}

bool Interface::duringTurn() {
  while (true) {
    cout << "> ";
    string cmd;
    if (!(cin >> cmd)) return false;
    if (cmd == "board") {
      cout << map->display() << endl;
    } else if (cmd == "status") {
      for (int i = 0; i < 4; ++i) printStatus(i);
    } else if (cmd == "residences") {
      printResidences(curTurn);
    } else if (cmd == "next") {
      return true;
    } else if (cmd == "help") {
      cout << "Valid commands:" << endl << "board" << endl << "status" << endl
           << "residences" << endl << "build-road <edge#>" << endl
           << "build-res <housing#>" << endl << "improve <housing#>" << endl
           << "trade <colour> <give> <take>" << endl
           << "bank <give> <take>" << endl << "enable-bonus" << endl
           << "disable-bonus" << endl << "next" << endl
           << "save <file>" << endl << "help" << endl;
    } else if (cmd == "save") {
      string file;
      if (!(cin >> file)) return false;
      saveGame(file);
    } else if (cmd == "build-road") {
      string word;
      if (!(cin >> word)) return false;
      int edge = 0;
      if (!parseInt(word, edge) ||
          !map->canBuildRoad(edge, players[curTurn].get())) {
        cout << "You cannot build here." << endl;
      } else if (!players[curTurn]->buildRoad(edge)) {
        cout << "You do not have enough resources." << endl;
      } else {
        map->updateLongestRoad();
        if (winner() >= 0) return true;
      }
    } else if (cmd == "build-res") {
      string word;
      if (!(cin >> word)) return false;
      int vertex = 0;
      if (!parseInt(word, vertex) ||
          !map->canBuildResidence(vertex, players[curTurn].get(), false)) {
        cout << "You cannot build here." << endl;
      } else if (!players[curTurn]->buildResidence(vertex, false)) {
        cout << "You do not have enough resources." << endl;
      } else if (winner() >= 0) {
        return true;
      }
    } else if (cmd == "improve") {
      string word;
      if (!(cin >> word)) return false;
      int vertex = 0;
      bool legal = parseInt(word, vertex) && vertex >= 0 &&
                   vertex < map->numVertices();
      if (legal) {
        Settlement *s = map->getVertice(vertex).getSettlement();
        legal = s != nullptr && s->getOwner() == players[curTurn].get() &&
                s->getLevel() != ResidenceLevel::Tower;
      }
      if (!legal) {
        cout << "You cannot build here." << endl;
      } else if (!players[curTurn]->improveResidence(vertex)) {
        cout << "You do not have enough resources." << endl;
      } else if (winner() >= 0) {
        return true;
      }
    } else if (cmd == "trade") {
      string colourWord, giveWord, takeWord;
      if (!(cin >> colourWord >> giveWord >> takeWord)) return false;
      int other = 0;
      Material give, take;
      if (!parseColour(colourWord, other) || other == curTurn ||
          !parseMaterial(giveWord, give) || !parseMaterial(takeWord, take)) {
        cout << "Invalid command." << endl;
        continue;
      }
      cout << COLOUR_NAMES[curTurn] << " offers " << COLOUR_NAMES[other]
           << " one " << materialName(give) << " for one "
           << materialName(take) << "." << endl;
      cout << "Does " << COLOUR_NAMES[other] << " accept this offer?" << endl;
      cout << "> ";
      string answer;
      if (!(cin >> answer)) return false;
      if (answer != "yes") continue;
      if (!players[curTurn]->trade(give, take, players[other].get())) {
        cout << "You do not have enough resources." << endl;
      }
    } else if (cmd == "enable-bonus") {
      bonusEnabled = true;
      cout << "Bonus features are now enabled." << endl;
    } else if (cmd == "disable-bonus") {
      bonusEnabled = false;
      cout << "Bonus features are now disabled." << endl;
    } else if (bonusEnabled && cmd == "bank") {
      string giveWord, takeWord;
      if (!(cin >> giveWord >> takeWord)) return false;
      Material give, take;
      if (!parseMaterial(giveWord, give) || !parseMaterial(takeWord, take)) {
        cout << "Invalid command." << endl;
        continue;
      }
      vector<int> res = players[curTurn]->giveMaterialAmount();
      if (res[materialCode(give)] < BANK_TRADE_RATIO) {
        cout << "You do not have enough resources." << endl;
        continue;
      }
      players[curTurn]->reduce(give, BANK_TRADE_RATIO);
      players[curTurn]->increase(take, 1);
      cout << COLOUR_NAMES[curTurn] << " trades four " << materialName(give)
           << " to the bank for one " << materialName(take) << "." << endl;
    } else {
      cout << "Invalid command." << endl;
    }
  }
}

int Interface::winner() const {
  for (int i = 0; i < 4; ++i) {
    if (players[i]->getBuildingPoints(bonusEnabled) >= 10) return i;
  }
  return -1;
}

string Interface::boardLayout() const {
  ostringstream out;
  for (int i = 0; i < map->numTiles(); ++i) {
    TileInfo info = map->getTile(i).display();
    if (i > 0) out << " ";
    out << materialCode(info.mat) << " "
        << (info.mat == Material::Park ? 7 : info.value);
  }
  return out.str();
}

void Interface::saveGame(const string &file) const {
  ofstream out{file};
  if (!out) return;
  out << curTurn << "\n";
  for (int i = 0; i < 4; ++i) {
    vector<int> res = players[i]->giveMaterialAmount();
    out << res[0] << " " << res[1] << " " << res[2] << " " << res[3] << " "
        << res[4] << " r";
    for (int e = 0; e < map->numEdges(); ++e) {
      if (map->getEdge(e).getRoad() == players[i].get()) out << " " << e;
    }
    out << " h";
    for (int v = 0; v < map->numVertices(); ++v) {
      Settlement *s = map->getVertice(v).getSettlement();
      if (s != nullptr && s->getOwner() == players[i].get()) {
        out << " " << v << " " << levelChar(s->getLevel());
      }
    }
    out << "\n";
  }
  out << boardLayout() << "\n";
  out << map->getGooseTile() << "\n";
}

bool Interface::loadGame(const string &file) {
  ifstream in{file};
  if (!in) return false;
  int turn = 0;
  if (!(in >> turn)) return false;

  vector<vector<int>> resources;
  vector<vector<int>> roads;
  vector<vector<pair<int, char>>> housing;
  for (int i = 0; i < 4; ++i) {
    vector<int> res(5, 0);
    for (int j = 0; j < 5; ++j) {
      if (!(in >> res[j])) return false;
    }
    resources.emplace_back(res);
    string marker;
    if (!(in >> marker)) return false;
    vector<int> myRoads;
    string word;
    while (in >> word && word != "h") {
      int e = 0;
      if (parseInt(word, e)) myRoads.emplace_back(e);
    }
    roads.emplace_back(myRoads);
    vector<pair<int, char>> myHouses;
    string line;
    getline(in, line);
    istringstream rest{line};
    int vertex = 0;
    string levelWord;
    while (rest >> vertex >> levelWord) {
      myHouses.emplace_back(vertex, levelWord.empty() ? 'B' : levelWord[0]);
    }
    housing.emplace_back(myHouses);
  }

  string layoutLine;
  getline(in, layoutLine);
  if (layoutLine.find_first_not_of(" \t\r\n") == string::npos) {
    getline(in, layoutLine);
  }
  int gooseTile = -1;
  in >> gooseTile;

  ofstream tmp{".ctor-load-board"};
  tmp << layoutLine << "\n";
  tmp.close();
  FileBoardSource src{".ctor-load-board"};
  map = make_unique<Map>(src);
  remove(".ctor-load-board");

  setUpPlayers();
  curTurn = turn;

  for (int i = 0; i < 4; ++i) {
    for (auto &h : housing[i]) {
      players[i]->buildResidence(h.first, true);
      Settlement *s = map->getVertice(h.first).getSettlement();
      if (s == nullptr) continue;
      if (h.second == 'H') {
        s->improve();
      } else if (h.second == 'T') {
        s->improve();
        s->improve();
      }
    }
    for (int e : roads[i]) {
      map->getEdge(e).build(players[i].get(), true);
    }
    players[i]->increase(Material::Brick, resources[i][0]);
    players[i]->increase(Material::Energy, resources[i][1]);
    players[i]->increase(Material::Glass, resources[i][2]);
    players[i]->increase(Material::Heat, resources[i][3]);
    players[i]->increase(Material::Wifi, resources[i][4]);
  }
  if (gooseTile >= 0) map->placeGoose(gooseTile);
  map->updateLongestRoad();
  return true;
}

void Interface::play() {
  while (true) {
    bool restored = false;
    if (!loadFile.empty()) {
      restored = loadGame(loadFile);
      loadFile.clear();
    }
    if (!restored) {
      setUpBoard();
      setUpPlayers();
      openingBasements();
      if (!cin) { saveGame("backup.sv"); return; }
    }

    while (true) {
      if (!beginningOfTurn()) { saveGame("backup.sv"); return; }
      if (winner() < 0) {
        if (!duringTurn()) { saveGame("backup.sv"); return; }
      }
      int w = winner();
      if (w >= 0) {
        cout << COLOUR_NAMES[w] << " has won!" << endl;
        printStatus(w);
        break;
      }
      curTurn = (curTurn + 1) % 4;
    }

    cout << "Would you like to play again?" << endl;
    cout << "> ";
    string answer;
    if (!(cin >> answer) || answer != "yes") {
      return;
    }
    curTurn = 0;
  }
}
