#pragma once
#include <vector>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <thread>
#include "Perceptron.h"

/*
 - нужно ли скрещивать между собой сети [количество сетей для скрещивания]
 - величина мутации

*/

class GeneticAlgorithm {
 public:
  struct Task {
    std::vector<double> input;
    std::vector<double> answer;
  };

 private:
  double CalcError(const std::vector<double>& answer, const std::vector<double>& result) {
    if (answer.size() != result.size()) {
      throw std::invalid_argument("(2)");
    }

    
    double error = 0;
    for (size_t i = 0; i < answer.size(); i++) {
      error += std::pow(answer[i] - result[i], 2);
    }
    return error;
  }

 public:
  Perceptron operator()(
    size_t population,
    size_t generations,
    double max_weight_diff,
    double max_bias_diff,
    std::vector<Task> tasks,
    std::vector<size_t> form
    ) {
    std::vector<Perceptron> networks(population, form);
    for (size_t i = 0; i < population; i++) {
      networks[i].SetRandomWeights();
    }
    for (size_t iter = 0; iter < generations; iter++) {
      // Выбор лучшей сети
      std::vector<double> error(population);

      int threads_count = 20;
      int nets_for_thread = population / threads_count;

      auto task1 = [&](size_t first, size_t last) {
        for (size_t net = first; net < last; net++) {
          for (size_t task = 0; task < tasks.size(); task++) {
            networks[net].SetInput(tasks[task].input);
            networks[net].Execute();
            error[net] += CalcError(tasks[task].answer, networks[net].GetOutput());
          }
        }
      };

      {
        std::vector<std::thread> threads;
        for (size_t i = 0; i < threads_count - 1; i++) {
          threads.emplace_back(task1, nets_for_thread * i, nets_for_thread * (i + 1));
        }
        threads.emplace_back(task1, nets_for_thread * (threads_count - 1), population);

        for (size_t i = 0; i < threads_count; i++) {
          threads[i].join();
        }
      }

      size_t best = std::min_element(error.begin(), error.end()) - error.begin();

      std::cout << iter << ") " << error[best] << '\n';

      // Создание копий сети
      auto task2 = [&](size_t first, size_t last) {
        for (size_t i = first; i < last; i++) {
          if (best != i) {
            networks[i] = networks[best];
            networks[i].Mutate(max_weight_diff, max_bias_diff);
          }
        }
      };

      {
        std::vector<std::thread> threads;
        for (size_t i = 0; i < threads_count - 1; i++) {
          threads.emplace_back(task2, nets_for_thread * i, nets_for_thread * (i + 1));
        }
        threads.emplace_back(task2, nets_for_thread * (threads_count - 1), population);

        for (size_t i = 0; i < threads_count; i++) {
          threads[i].join();
        }
      }
    }


    std::vector<double> error(population);
    for (size_t net = 0; net < population; net++) {
      for (size_t task = 0; task < tasks.size(); task++) {
        networks[net].SetInput(tasks[task].input);
        networks[net].Execute();
        error[net] += CalcError(tasks[task].answer, networks[net].GetOutput());
      }
    }
    size_t best = std::min_element(error.begin(), error.end()) - error.begin();
    return networks[best];
  }





};