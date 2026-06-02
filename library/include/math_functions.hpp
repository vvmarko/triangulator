#ifndef TRIANGULATOR_MATH_FUNCTIONS_H_INCLUDED
#define TRIANGULATOR_MATH_FUNCTIONS_H_INCLUDED

// #####################
// Function declarations
// #####################

double real_valued_determinant(const vector<vector<double>>* matrix);

vector<int> evaluate_signature_of_matrix( const vector<vector<double>> *matrix );

vector<vector<double>> cayley_menger_matrix(KSimplex*);

void print_vector_of_a_vector(vector<vector<double>>);

#endif
