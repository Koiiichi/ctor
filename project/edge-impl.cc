module edge;


using namespace std;

Edge::Edge(Vertice* v1, Vertice* v2, int number):
  v1{v1}, v2{v2}, num{number}
  {}

string Edge::display() {
  string temp;
  // if Edge has owner, return the letter corresponding to it
  if(road != nullptr) {
    temp += road->getColour();
    return temp;
  }
  // otherwise, return the edge number
  return to_string(num);

}