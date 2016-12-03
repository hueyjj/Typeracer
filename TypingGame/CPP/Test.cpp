#include <chrono>
#include <ctime>
#include <ratio>
#include "../Header/TextProcessor.h"

int main(){
  TextProcessor tp;
  tp.loadFile("File.txt");
  tp.printAll();

  // Execution time start
  // using namespace std::chrono;
  // steady_clock::time_point start = steady_clock::now();

  // cout << "Testing Test cpp" << endl;
  // const char* file = "File.txt";
  // TextProcessor proc(file);
  // proc.printAll();

  // // Execution time end
  // steady_clock::time_point stop = steady_clock::now();
  // duration<double, milli> time_span = duration_cast<duration<double, milli>>(stop - start);
  // cout << "Execution time: " << time_span.count() << " ms." << endl;
  return 0;
}
