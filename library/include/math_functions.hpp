#ifndef TRIANGULATOR_MATH_FUNCTIONS_H_INCLUDED
#define TRIANGULATOR_MATH_FUNCTIONS_H_INCLUDED

// #####################
// Function declarations
// #####################

double real_valued_determinant(const vector<vector<double>>* matrix);

vector<vector<double>> cayley_menger_matrix(KSimplex*);

template<typename T>
void print_vector_of_a_vector(vector<vector<T>> v)
{
    for (unsigned int i = 0; i < v.size(); i++)
    {
        for (unsigned int j = 0; j < v[i].size(); j++)
        {
            cout << v[i][j] << "\t";
        }
        cout << endl;
    }
}

#endif
