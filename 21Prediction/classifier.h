#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <string>
#include <vector>
#include "Dense"

using Eigen::ArrayXd;
using std::string;
using std::vector;

class GNB {
 public:
  /**
   * Constructor
   */
  GNB();

  /**
   * Destructor
   */
  virtual ~GNB();

  /**
   * Train classifier
   */
  void train(const vector<vector<double>> &data, 
             const vector<string> &labels);

  /**
   * Predict with trained classifier
   */
  vector< vector<double>  > std;
  vector< vector<double>  > mean;
  vector<double> prior_probability;
  string predict(const vector<double> &sample);

  vector<string> possible_labels = {"keep","right","left"};
};

#endif  // CLASSIFIER_H