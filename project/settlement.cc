export module settlement;

import <string>;
import residenceLevel;

export class Settlement {
  ResidenceLevel level;
  bool improve();
  int buildingPoints();
  string display();
};
