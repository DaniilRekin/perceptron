#include "Perceptron.h"

Perceptron::Perceptron(const std::vector<size_t>& form) : 
  layers_(form.size()),
  form_(form),
  ivalue_(form.size()),
  ovalue_(form.size()),
  bias_(form.size()),
  weight_(form.size() - 1),
  activation_func_(std::make_shared<SigmFunc>())
{
  for (size_t i = 0; i < layers_; i++) {
    ivalue_[i].resize(form[i]);
    ovalue_[i].resize(form[i]);
    bias_[i].resize(form[i]);
  }

  for (size_t i = 0; i < layers_ - 1; i++) {
    weight_[i].resize(form[i]);
    for (size_t j = 0; j < form[i]; j++) {
      weight_[i][j].resize(form[i + 1]);
    }
  }

  SetRandomSeed();
}

void Perceptron::SetRandomWeights() {
  for (size_t k = 0; k < layers_ - 1; k++) {
    for (size_t i = 0; i < form_[k]; i++) {
      for (size_t j = 0; j < form_[k + 1]; j++) {
        weight_[k][i][j] = static_cast<double>(std::rand()) / RAND_MAX * 2.0 - 1.0;
        //weight_[k][i][j] *= 0.1;
      }
    }
  }
}

void Perceptron::SetInput(const std::vector<double>& dt) {
  if (ivalue_[0].size() != dt.size()) {
    throw std::invalid_argument("dt .size() incorrect");
  }
  ivalue_[0] = dt;
}

const std::vector<double>& Perceptron::GetOutput() {
  return ovalue_.back();
}

void Perceptron::Execute() {
  for (size_t i = 0; i < form_[0]; i++) {
    ovalue_[0][i] = ivalue_[0][i];
  }

  for (size_t k = 0; k < layers_ - 1; k++) {
    for (size_t i = 0; i < form_[k + 1]; i++) {
      ivalue_[k + 1][i] = 0;
      for (size_t j = 0; j < form_[k]; j++) {
        ivalue_[k + 1][i] += ovalue_[k][j] * weight_[k][j][i];
      }
      ovalue_[k + 1][i] = (*activation_func_)(ivalue_[k + 1][i] + bias_[k + 1][i]);
    }
  }
}

void Perceptron::Mutate(double weight_max_diff, double bias_max_diff) {
  for (size_t k = 0; k < layers_ - 1; k++) {
    for (size_t i = 0; i < form_[k]; i++) {
      for (size_t j = 0; j < form_[k + 1]; j++) {
        weight_[k][i][j] += (static_cast<double>(std::rand()) / RAND_MAX * 2.0 - 1.0) * weight_max_diff;
      }
    }
  }

  for (size_t k = 0; k < layers_; k++) {
    for (size_t i = 0; i < form_[k]; i++) {
      bias_[k][i] += (static_cast<double>(std::rand() * 2) / RAND_MAX * 2.0 - 1.0) * bias_max_diff;
    }
  }
}

void Perceptron::SetRandomSeed() {
  static bool seed_set = false;
  if (!seed_set) {
    seed_set = true;
    std::srand(std::time(NULL));
  }
}