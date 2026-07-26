export module boardSource;
import <utility>;
import <vector>;
import tileInfo;

class BoardSource {
  public:
   virtual std::vector<TileInfo> getTiles() ;
};

class RandomBoardSource: public BoardSource {
  public:
    std::vector<TileInfo> getTiles() override;
};

class FileBoardSource: public BoardSource {
  public:
    std::vector<TileInfo> getTiles() override;
};
