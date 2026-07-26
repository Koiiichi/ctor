module boardSource;

import <algorithm>;
import <fstream>;
import <random>;
import <string>;
import <vector>;

using namespace std;

static Material toMaterial(int code) {
  if (code == 0) return Material::Brick;
  if (code == 1) return Material::Energy;
  if (code == 2) return Material::Glass;
  if (code == 3) return Material::Heat;
  if (code == 4) return Material::Wifi;
  return Material::Park;
}

RandomBoardSource::RandomBoardSource(unsigned seed): seed{seed} {}

vector<TileInfo> RandomBoardSource::getTiles() {
  vector<Material> mats;
  for (int i = 0; i < 4; ++i) mats.emplace_back(Material::Brick);
  for (int i = 0; i < 4; ++i) mats.emplace_back(Material::Energy);
  for (int i = 0; i < 4; ++i) mats.emplace_back(Material::Glass);
  for (int i = 0; i < 3; ++i) mats.emplace_back(Material::Heat);
  for (int i = 0; i < 3; ++i) mats.emplace_back(Material::Wifi);
  mats.emplace_back(Material::Park);

  vector<int> values{2, 12};
  for (int v : {3, 4, 5, 6, 8, 9, 10, 11}) {
    values.emplace_back(v);
    values.emplace_back(v);
  }

  mt19937 rng{seed};
  shuffle(mats.begin(), mats.end(), rng);
  shuffle(values.begin(), values.end(), rng);

  vector<TileInfo> tiles;
  size_t next = 0;
  for (auto mat : mats) {
    if (mat == Material::Park) {
      tiles.emplace_back(TileInfo{mat, 7, false});
    } else {
      tiles.emplace_back(TileInfo{mat, values[next], false});
      ++next;
    }
  }
  return tiles;
}

FileBoardSource::FileBoardSource(const string &file): file{file} {}

vector<TileInfo> FileBoardSource::getTiles() {
  vector<TileInfo> tiles;
  ifstream in{file};
  int code;
  int value;
  while (in >> code >> value) {
    tiles.emplace_back(TileInfo{toMaterial(code), value, false});
  }
  return tiles;
}
