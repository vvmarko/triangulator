
#include "triangulator.hpp"

//Helper function for the Leibniz formula to find the sign od the permutation, 
//since the next_permutation() function from the standard library gives the
//next permutation lexicographically, so simple sign inversion is not correct.
int get_permutation_sign(const vector<int>& perm) {
    int n = perm.size();
    int sign = 1;
    
    for(int i = 0; i < n; i++) {
        for(int j = i + 1; j < n; j++) {
            if(perm[i] > perm[j]) {
                sign *= -1;
            }
        }
    }
    return sign;
}

//Calculating the determinant with the Leibniz formula, returning the value.
double leibniz_formula(const vector<vector<double>>* matrix){
    //We don't check if the matrix parameter is correct, because this helper function
    //is called from real_valued_determinant() where all conditions were checked.
    unsigned int matrix_dimension = matrix->size();
    //A vector holding the indices for calculating the determinant, this vector
    //will go through all possible permutatios.
    vector<int> index_permutation_vector(0, matrix_dimension);
    unsigned int i = 0;
    //Initializing with (0,1,.., matrix_dimension-1).
    for(i = 0; i < matrix_dimension; i++){
        index_permutation_vector.push_back(i);
    }
    double determinant = 0.0;
    int permutation_sign = 1;
    double product_of_elements = 1.0;
    do {
        product_of_elements = 1;
        for(i=0; i < matrix_dimension; i++){
            product_of_elements = product_of_elements * ((*matrix)[i][index_permutation_vector[i]]);
        }
        permutation_sign = get_permutation_sign(index_permutation_vector);
        determinant = determinant + permutation_sign* product_of_elements;

    } while(next_permutation(index_permutation_vector.begin(), index_permutation_vector.end()));

    return determinant;
}

//Returns a new matrix which will be used for the Gaussian, since we dont want to modify our original matrix.
vector<vector<double>> get_matrix_for_gaussian(const vector<vector<double>>* matrix){
    return *matrix;
}

//Helper function for Gaussian elimination to find the index of the element in the column with the largest absolute value.
//It is used to find the pivot element used in the algorithm.
unsigned int argmax(const vector<vector<double>>& input_matrix, unsigned int start, unsigned int end, unsigned int column){
    unsigned int i = 0, max_index = start;
    double maximum_absolute_value = fabs(input_matrix[start][column]);
    for(i=start; i < end;i++){
        if(fabs(input_matrix[i][column]) > maximum_absolute_value){
            maximum_absolute_value = fabs(input_matrix[i][column]);
            max_index = i;
        }
    }
    return max_index;
}


//This is the gaussian elimination formula which does not alter our original matrix.
double gaussian_elimination(const vector<vector<double>>* matrix){
    unsigned int matrix_dimension = matrix->size();
    unsigned int pivot_row = 0, pivot_column = 0, max_index = 0, i = 0, j = 0, swap_count = 0;
    double f = 0.0, determinant = 1.0;
    vector<vector<double>> gaussian_matrix = get_matrix_for_gaussian(matrix);
    while(pivot_row < matrix_dimension && pivot_column < matrix_dimension){
        max_index = argmax(gaussian_matrix, pivot_row, matrix_dimension, pivot_column);
        if(gaussian_matrix[max_index][pivot_column] == 0){
            pivot_column = pivot_column + 1;
        }
        else{
            if (max_index != pivot_row) {
                swap(gaussian_matrix[max_index], gaussian_matrix[pivot_row]);
                swap_count++;
            }
            for(i = pivot_row+1; i < matrix_dimension; i++){
                f = gaussian_matrix[i][pivot_column]/gaussian_matrix[pivot_row][pivot_column];
                gaussian_matrix[i][pivot_column] = 0;
                for(j = pivot_column+1; j < matrix_dimension; j++){
                    gaussian_matrix[i][j] = gaussian_matrix[i][j]-gaussian_matrix[pivot_row][j]*f;
                }
            }
            pivot_row = pivot_row + 1;
            pivot_column = pivot_column + 1;
        }
    }
    for(i = 0; i < matrix_dimension; i++){
        determinant = determinant * gaussian_matrix[i][i];
    }
    if(swap_count % 2 == 1){
        determinant = -determinant;
    }
    return determinant;
}

double real_valued_determinant(const vector<vector<double>>* matrix){
    //First checking whether the user passed a nullptr for the matrix in 
    //the function call. If true, we warn the user(it was probably not
    //intentional, but isnt harmful).
    if(matrix == nullptr){
        log_report(LOG_WARN,"real_valued_determinant(): You have asked me to calculate a determiant of a nullptr matrix.");
        log_report(LOG_WARN,"real_valued_determinant(): I am skipping this, but it probably should not have happened, check your code.");
        return 0;
    }
    //Checking if the dimensions match, that is if the pointer passed as
    //an argument really does point to a square matrix, otherwise we report
    //an error, beacause determinants are calculated for square matrices only.
    unsigned int matrix_dimension = matrix->size();
    unsigned int i = 0;
    for(i = 0; i < matrix_dimension; i++){
        if(matrix->at(i).size() != matrix_dimension){
            log_report(LOG_ERROR,"real_valued_determinant(): You have passed a non-square matrix.");
            return 0;
        }
    }
    //Code that can potentially speed up the algorithm by checking if the matrix
    //contains a zero row or column. It takes O(n^2) time, which is a good tradeoff
    //before going into the Leibniz computation, which takes a significant amount of time.

    /*
        unsigned int j = 0;
        bool determinant_is_zero = false;
        unsigned int count_zeros_in_row = 0;
        unsigned int count_zeros_in_column = 0;
        for(i = 0; i < matrix_dimension; i++){
            count_zeros_in_row = 0;
            count_zeros_in_column = 0;
            for(j=0; j < matrix_dimension; j++){
                if((*matrix)[i][j] == 0) count_zeros_in_row++;
                if((*matrix)[j][i] == 0) count_zeros_in_column++;
            }
            if(count_zeros_in_column == matrix_dimension || count_zeros_in_row == matrix_dimension){
                determinant_is_zero = true;
                break;
            }
        }
        if(determinant_is_zero == true){
            return 0;
        }
    */

    //If the matrix_dimension is less then the critical_determinant_size we will compute
    //the determinant with the Leibniz formula, because of better numerical stabilty
    if(matrix_dimension < triangulator_global::critical_determinant_size){
        return leibniz_formula(matrix);
    }
    //Otherwise we will use Gaussian elimination
    else{
        return gaussian_elimination(matrix);
    }
    return 0;
}
