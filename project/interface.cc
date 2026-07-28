export module interface;

import <memory>;
import <string>;
import <vector>;
import board;
import boardSource;
import material;

export class Interface {
  std::unique_ptr<Map> map;
  std::vector<std::unique_ptr<Player>> players;
  int curTurn = 0;
  bool loadedGame = false;
  std::string boardFile;
  bool useRandomBoard = false;
  bool bonusEnabled = false;
  std::string loadFile;

  void setUpBoard();
  void setUpPlayers();
  void openingBasements();
  bool beginningOfTurn();
  bool duringTurn();
  void handleSeven();
  void distribute(int roll);
  void saveGame(const std::string &file) const;
  bool loadGame(const std::string &file);
  std::string boardLayout() const;
  void printStatus(int index) const;
  void printResidences(int index) const;
  int winner() const;

  public:
    Interface(int argc, char *argv[]);
    void play();
};
