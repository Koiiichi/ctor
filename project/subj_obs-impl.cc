module subj_obs;

import <algorithm>;

void Subject::addObserver(Observer *o) {
  observers.emplace_back(o);
}

void Subject::removeObserver(Observer *o) {
  observers.erase(std::remove(observers.begin(), observers.end(), o),
                  observers.end());
}

void Subject::notify(int rollValue) {
  for (auto *o : observers) {
    o->update(rollValue);
  }
}
