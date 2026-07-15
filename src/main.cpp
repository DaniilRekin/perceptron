#include <iostream>
#include <iomanip>
#include "Perceptron.h"
#include "GeneticAlgorithm.h"
#include "load.h"

// Сумма >= 2
int main1() {
  GeneticAlgorithm alg;
  Perceptron p = alg(
    200,
    500,
    0.05,
    0.05,
    {
      {{0, 0, 0}, {0}},
      {{0, 0, 1}, {0}},
      {{0, 1, 0}, {0}},
      {{0, 1, 1}, {1}},
      {{1, 0, 0}, {0}},
      {{1, 0, 1}, {1}},
      {{1, 1, 0}, {1}},
      {{1, 1, 1}, {1}},
    },
    {3, 3, 1}
  );

  p.SetInput({1, 1, 1});
  p.Execute();
  
  std::cout << p.GetOutput()[0];

  return 0;
}




// 
int main() {
  GeneticAlgorithm alg;
  Perceptron p = alg(
    100,
    300,
    0.05,
    0.005,
    get_data(),
    {1024, 128, 64, 64, 10}
  );

  std::cout << "Ready\n";
  while (true) {
    std::vector<double> task(1024);
    for (int i = 0; i < 1024; i++) {
      std::cin >> task[i];
    }

    p.SetInput(task);
    p.Execute();

    std::cout << "output:\n";
    std::cout << std::fixed << std::setprecision(2);
    for (auto v : p.GetOutput()) {
      std::cout << v << '\t';
    }
    std::cout << "\n\n";
  }

  return 0;
}