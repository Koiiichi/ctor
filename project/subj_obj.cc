export module subj_obs;

import <vector>;

export class Observer {
  public:
    virtual void update(int rollValue) = 0;
    virtual ~Observer() = default;
};

export class Subject {
  std::vector<Observer *> observers;
  public:
    void addObserver(Observer *o);
    void removeObserver(Observer *o);
    void notify(int rollValue);
    virtual ~Subject() = default;
};
