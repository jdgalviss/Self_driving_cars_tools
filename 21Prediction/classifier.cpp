#include "classifier.h"
#include <math.h>
#include <string>
#include <vector>
#include <iostream>

using Eigen::ArrayXd;
using std::string;
using std::vector;

// Initializes GNB
GNB::GNB() {
  /**
   * TODO: Initialize GNB, if necessary. May depend on your implementation.
   */
  
}

GNB::~GNB() {}

void GNB::train(const vector<vector<double>> &data, 
                const vector<string> &labels) {
                    
  
  /**
   * Trains the classifier with N data points and labels.
   * @param data - array of N observations
   *   - Each observation is a tuple with 4 values: s, d, s_dot and d_dot.
   *   - Example : [[3.5, 0.1, 5.9, -0.02],
   *                [8.0, -0.3, 3.0, 2.2],
   *                 ...
   *                ]
   * @param labels - array of N labels
   *   - Each label is one of "left", "keep", or "right".
   *
   * TODO: Implement the training function for your classifier.
   */
   vector<vector<double> > keep_data;
   vector<vector<double> > right_data;
   vector<vector<double> > left_data;
   vector<vector< vector<double> > > data_complete;
   vector<string> classes{ "keep", "right", "left" };
   
   double count_left = 0;
   double count_keep = 0;
   double count_right = 0;
   for(unsigned int i = 0; i < labels.size(); ++i){
     vector<double> measurement = data[i];
     if(labels[i].compare("keep")== 0){
       count_keep++;
       keep_data.push_back(measurement);
     }
     else if(labels[i].compare("right") == 0){
       count_right++;
       right_data.push_back(measurement);
     }
     else if(labels[i].compare("left") == 0){
       count_left++;
       left_data.push_back(measurement);
     }
   }
   data_complete.push_back(keep_data);
   data_complete.push_back(right_data);
   data_complete.push_back(left_data);
   
   //calculate prior probability
   prior_probability.push_back(count_keep/(count_keep + count_left + count_right ));
   prior_probability.push_back(count_right/(count_keep + count_left + count_right ));
   prior_probability.push_back(count_left/(count_left + count_left + count_right ));
   //calculate for each behavior
   for(unsigned int i = 0; i <  data_complete.size() ; ++i){
     vector<double> s;
     vector<double> s_dot;
     vector<double> d;
     vector<double> d_dot;
     //calculate for each measurement
     for(unsigned int j = 0; j <  data_complete.at(i).size() ; ++j){
        s.push_back(data_complete.at(i).at(j).at(0));
        double mod = (data_complete.at(i).at(j).at(1));
        d.push_back(mod);
        s_dot.push_back(data_complete.at(i).at(j).at(2));
        d_dot.push_back(data_complete.at(i).at(j).at(3));
     }
     //calculate mean
     vector<double> avg{accumulate(s.begin(), s.end(), 0.0)/s.size(),
                        accumulate(d.begin(), d.end(), 0.0)/d.size(),
                        accumulate(s_dot.begin(), s_dot.end(), 0.0)/s_dot.size(),
                        accumulate(d_dot.begin(), d_dot.end(), 0.0)/d_dot.size()};
     mean.push_back(avg);
     
     //calculate std
     
     double sq_sum_s = std::inner_product(s.begin(), s.end(), s.begin(), 0.0);
     double sq_sum_s_dot = std::inner_product(s_dot.begin(), s_dot.end(), s_dot.begin(), 0.0);
     double sq_sum_d = std::inner_product(d.begin(), d.end(), d.begin(), 0.0);
     double sq_sum_d_dot = std::inner_product(d_dot.begin(), d_dot.end(), d_dot.begin(), 0.0);

     vector<double> std_i{std::sqrt(sq_sum_s / s.size() - avg[0] * avg[0]),
                          std::sqrt(sq_sum_d / d.size() - avg[1] * avg[1]),
                          std::sqrt(sq_sum_s_dot / s_dot.size() - avg[2] * avg[2]),
                          std::sqrt(sq_sum_d_dot / d_dot.size() - avg[3] * avg[3])};
                          
     std.push_back(std_i);
   }
   
   std::cout << "keep number of elements " << mean[0].size() << std::endl;
   std::cout << "right number of elements " << prior_probability.size() << std::endl;
   std::cout << "left number of elements " << count_left << std::endl;
  
}

string GNB::predict(const vector<double> &sample) {
  /**
   * Once trained, this method is called and expected to return 
   *   a predicted behavior for the given observation.
   * @param observation - a 4 tuple with s, d, s_dot, d_dot.
   *   - Example: [3.5, 0.1, 8.5, -0.2]
   * @output A label representing the best guess of the classifier. Can
   *   be one of "left", "keep" or "right".
   *
   * TODO: Complete this function to return your classifier's prediction
   */
   vector<double> probabilities;
   for(unsigned int i = 0 ; i < possible_labels.size(); ++i ){
       double p = 1.0;
       for(unsigned int j = 0 ; j < sample.size(); ++j ){
           p *= exp(-pow(sample.at(j)-mean.at(i).at(j),2.0f)/(2*pow(std.at(i).at(j),2.0f)))/(sqrt(2*M_PI*pow(std.at(i).at(j),2.0f)));
       }
       probabilities.push_back(p*prior_probability.at(i));
   }
   int argmax = std::max_element(probabilities.begin(),probabilities.end()) - probabilities.begin();
   //std::cout<<"argmax:"<<argmax<<"prob> "<<probabilities.at(argmax)<<std::endl;
   return this -> possible_labels[argmax];
}