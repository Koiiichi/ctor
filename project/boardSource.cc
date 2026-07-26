export module boardSource;

import <string>;
import <vector>;
import material;
import tileInfo;

export class BoardSource {
  public:
    virtual std::vector<TileInfo> getTiles() = 0;
    virtual ~BoardSource() = default;
};

export class RandomBoardSource: public BoardSource {
  unsigned seed;
  public:
    RandomBoardSource(unsigned seed);
    std::vector<TileInfo> getTiles() override;
};

export class FileBoardSource: public BoardSource {
  std::string file;
  public:
    FileBoardSource(const std::string &file);
    std::vector<TileInfo> getTiles() override;
};
