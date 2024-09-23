#include "json.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using json = nlohmann::json;
using namespace std;

// convert string to integer for the specific base
long long decode_value(int base, const string &value) {
  return stoll(value, nullptr, base);
}

// Function to perform Lagrange interpolation to find the constant term
double lagrange_interpolation(const vector<int> &x_values,
                              const vector<long long> &y_values,
                              int x_at_zero = 0) {
  double total = 0;
  int n = x_values.size();

  for (int i = 0; i < n; ++i) {
    double term = y_values[i];
    for (int j = 0; j < n; ++j) {
      if (i != j) {
        term *= (x_at_zero - x_values[j]) / (double)(x_values[i] - x_values[j]);
      }
    }
    total += term;
  }

  return total;
}

// Function to find the constant term (c) from the given JSON data
long long find_constant_term(const json &json_data) {
  // Step 1: Read n and k
  int n = json_data["keys"]["n"];
  int k = json_data["keys"]["k"];

  vector<int> x_values;
  vector<long long> y_values;

  // Step 2: Decode the roots
  for (int i = 1; i <= n; ++i) {
    string key = to_string(i);

    // Check if the key exists in the JSON data
    if (json_data.contains(key)) {
      string base_str = json_data[key]["base"];
      string value_str = json_data[key]["value"];

      int base = stoi(base_str);
      long long y = decode_value(base, value_str);

      x_values.push_back(i); // The key of the root object is the x value
      y_values.push_back(y); // The decoded y value
    }
  }

  // Step 3: Perform Lagrange interpolation using the first k roots
  double constant_term_double =
      lagrange_interpolation({x_values.begin(), x_values.begin() + k},
                             {y_values.begin(), y_values.begin() + k});

  // Convert the result to a long long integer (rounded)
  long long constant_term = static_cast<long long>(round(constant_term_double));

  return constant_term;
}

int main() {
  string file_path1 = "test1.json";
  string file_path2 = "test2.json";

  // Declare variables to hold JSON data
  json json_data1, json_data2;

  // Process the first JSON file
  ifstream json_file1(file_path1);
  if (json_file1.is_open()) {
    json_file1 >> json_data1;
    long long constant_term1 = find_constant_term(json_data1);
    cout << "The constant term (c) from " << file_path1 << " as an integer is: " << constant_term1 << endl;
  } else {
    cerr << "Error opening " << file_path1 << "!" << endl;
  }

  ifstream json_file2(file_path2);
  if (json_file2.is_open()) {
    json_file2 >> json_data2;
    long long constant_term2 = find_constant_term(json_data2);
    cout << "The constant term (c) from " << file_path2 << " as an integer is: " << constant_term2 << endl;
  } else {
    cerr << "Error opening " << file_path2 << "!" << endl;
  }

  return 0;
}
