#pragma once
#include <cmath>
#include <vector>
#include <random>
#include <ctime>
#include <exception>
#include <stdexcept>
#include <memory>

class Perceptron {
 public:
  struct IActivationFunc {
    virtual double operator()(double) = 0;
    virtual ~IActivationFunc() = default;
  };

  struct SigmFunc : IActivationFunc {
    double operator()(double v) override {
      return 1 / (1 + std::exp(-v));
    }
  };

 private:
  size_t layers_ = 0;
  std::vector<size_t> form_;
  std::vector<std::vector<double>> ivalue_;
  std::vector<std::vector<double>> ovalue_;
  std::vector<std::vector<double>> bias_;
  std::vector<std::vector<std::vector<double>>> weight_; // [layer][from][to]
  std::shared_ptr<IActivationFunc> activation_func_;

  static void SetRandomSeed();

 public:
  Perceptron(const std::vector<size_t>& form);

 public:
  void SetRandomWeights();

  void SetInput(const std::vector<double>& dt);

  const std::vector<double>& GetOutput();

  void Execute();

  void Mutate(double weight_max_diff, double bias_max_diff);
};