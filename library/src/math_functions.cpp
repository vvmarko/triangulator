
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
        log_report(LOG_WARN,"real_valued_determinant(): You have asked me to calculate a determinant of a nullptr matrix.");
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

bool matrix_is_square(const vector<vector<double>> *matrix){
    unsigned int matrix_dimension = matrix->size();
    unsigned int i = 0;
    for(i = 0; i < matrix_dimension; i++){
        if(matrix->at(i).size() != matrix_dimension)
            return false;
    }
    return true;
}

bool matrix_is_symmetric(const vector<vector<double>> *matrix){
    unsigned int matrix_dimension = matrix->size();
    unsigned int i = 0, j=0;
    for(i = 0; i < matrix_dimension; i++){
        for(j = i+1; j < matrix_dimension; j++){
            if((*matrix)[i][j] != (*matrix)[j][i])
                return false;
        } 
    }
    return true;
}


vector<int> evaluate_signature_of_matrix(const vector<vector<double>> *matrix){

    if(matrix == nullptr)
        log_report(LOG_ERROR,"evaluate_signature_of_matrix(): You have asked me to calculate the signature of a nullptr matrix.");
    if(matrix_is_square() == false)
        log_report(LOG_ERROR, "evaluate_signature_of_matrix(): You have passed a non-square matrix.");
    if(matrix_is_symmetric(matrix) == false)
        log_report(LOG_ERROR,"evaluate_signature_of_matrix(): You have passed non-symmetric matrix.");

    int n = matrix->size();
    vector<vector<double>> L(n, vector<double>(n, 0));
    vector<vector<double>> D(n, vector<double>(n, 0));
    
    unsigned int i,j,k;
    for(i = 0; i < n; i++) L[i][i] = 1.0;

    for (j = 0; j < n; ++j) {
        double sumD = 0;
        for (k = 0; k < j; ++k) {
            sumD += L[j][k] * L[j][k] * D[k][k];
        }
        D[j][j] = (*matrix)[j][j] - sumD;

        if (abs(D[j][j]) < 1e-15) {
            log_report(LOG_WARN,
            "Warning: Pivot D["+ j +"] is near zero. Matrix may be indefinite/singular.");
        }

        for (i = j + 1; i < n; ++i) {
            double sumL = 0;
            for (k = 0; k < j; ++k) {
                sumL += L[i][k] * L[j][k] * D[k][k];
            }
            L[i][j] = ((*matrix)[i][j] - sumL) / D[j][j];
        }
    }

    // Signature
    int pos = 0, neg = 0, zero = 0;
    double eps = 1e-9; 
    for (i = 0; i < n; i++) {
        if (D[i][i] > eps) pos++;
        else if (D[i][i] < -eps) neg++;
        else zero++;
    }

    return {pos,neg,zero};
}

//If pointer to KSimplex is NULL, function will return 1x1 zero matrix, if not, function will return (k+2)x(k+2) matrix.
//Next, if level of KSimplex is 0, function will return matrix ((0,1),(1,0)).
//If level of KSimplex is greater than 0, matrix that function returns will depend on whether or not all neighboring edges are colored with TYPE_VOLUME_SQUARED.
//If there exists neighboring edge that isn't colored, function will return (k+2)x(k+2) zero matrix.
//Otherwise, function will return Cayley Menger matrix.
vector<vector<double>> cayley_menger_matrix(KSimplex* simp)
{
    //Pointer to KSimplex cannot be NULL. If that happens, error is reported
    if (nullptr == simp)
    {
        log_report(LOG_ERROR, "cayley_menger_matrix(): Pointer to KSimplex is NULL");
        vector<vector<double>> matrix(1, vector<double>(1, 0));

        return matrix;
    }

    //When pointer isn't NULL, function will certainly return (k+2)x(k+2) matrix.
    //Because of this, idea is that we initially create (k+2)x(k+2) zero matrix and
    //and then, depending on the case, we update elements of matrix accordingly.
    int simplex_level = simp->k;
    vector<vector<double>> matrix(simplex_level + 2, vector<double>(simplex_level + 2, 0));

    //First we need to check if level of KSimplex is 0.
    if (0 == simplex_level)
    {
        //Diagonal elements of Cayley Menger matrix are zero and since we
        //initialized all elements of matrix to zero, we don't need to set them again (constructor did that for us).
        matrix[0][1] = 1;
        matrix[1][0] = 1;

        return matrix;
    }

    //Next case (k>0): value of matrix elements depend on whether or not all neighboring edges are colored with TYPE_VOLUME_SQUARED.
    //If there exists neighboring edge that isn't colored, function will return zero matrix,
    //otherwise function will return matrix which elements have appropriate value.
    //If we ascertain that not all neighboring edges are colored, we will just return created matrix (again created matrix is zero matrix),
    //and if not, we will modify matrix elements in order for them to have appropriate value.

    //Because we chose to implement KSimplex in a way that pointer to a given KSimplex isn't located
    //in attribute "neighbors" of that KSimplex, even though that is the case in theory.
    //Because of this, if forwarded KSimplex is edge (k=1), simp->neighbors->elements[1] will be empty.
    //But from mathematical point of view set of all neighboring edges will be set that contains that edge and only that edge.
    //On the other side, if forwarded KSimplex isn't edge, we can safely work with simp->neighbors->elements[1]. 
    //Thus we need to distinguish between these two cases.
    if (1 == simplex_level)
    {
        if (!Color::is_colorized_with_type(simp, TYPE_VOLUME_SQUARED))
        {
            log_report(LOG_ERROR, "cayley_menger_matrix(): Not all neighboring vertices are colored with TYPE_VOLUME_SQUARED");

            return matrix;
        }
    }
    else
    {
        for (KSimplex* edges : simp->neighbors->elements[1])
        {
            if (!Color::is_colorized_with_type(edges, TYPE_VOLUME_SQUARED))
            {
                log_report(LOG_ERROR, "cayley_menger_matrix(): Not all neighboring vertices are colored with TYPE_VOLUME_SQUARED");

                return matrix;
            }
        }
    }

    //GENERATING Cayley Menger matrix

    //Diagonal elements of Cayley Menger matrix are zero and since we
    //initialized all elements of matrix to zero, we don't need to set them again (constructor did that for us).

    //Setting elements of last row and last column to 1 (except element m[k+1][k+1] - that element is on diagonal and is equal to zero).
    //Since matrix is symmetric, we can do that with only one for loop.
    for (unsigned int i = 0; i < simplex_level + 1; i++)
    {

        matrix[simplex_level + 1][i] = 1;
        matrix[i][simplex_level + 1] = 1;
    }

    //We will choose that matrix element (i,j) corresponds to value Volume squared colour of the edge that contains
    //vertex simp->neighbors->elements[0][i] and vertex simp->neighbors->elements[0][j].
    //Again, we will use a fact that matrix is symmetric.

    //An edge that contains two given vertices is neighbour to both of them.
    //So it is sufficient that for two given vertices, we extract all edges that contain one vertex,
    //and then between those edges find the one that contains the other vertex.
    for (unsigned int i = 0; i < simplex_level + 1; i++)
    {
        //Neighbouring edges of vertex i
        vector<KSimplex*> neighboring_edges_of_vertex_i = simp->neighbors->elements[0][i]->neighbors->elements[1];

        for (unsigned int j = i + 1; j < simplex_level + 1; j++)
        {
            KSimplex* vertex_j = simp->neighbors->elements[0][j];
            bool edge_found = false;
            unsigned int l = 0;

            do
            {
                edge_found = neighboring_edges_of_vertex_i[l]->find_neighbor(vertex_j);
                l++;
            } while (!edge_found);
            //Since edge that contains vertex i and vertex j must exist,
            //we only need to check whether it is found, and we don't need to worry about reaching the end
            //of vector neighboring_edges_of_vertex_i.

            //Extracting information about value of Volume squared colour
            //Edge that contains vertex i and vertex j is at the position l-1 (l is incremented in do-while after edge is found).
            VolumeSquaredColor* pointer_to_vsq = (VolumeSquaredColor*)Color::find_pointer_to_color_type(neighboring_edges_of_vertex_i[l - 1], TYPE_VOLUME_SQUARED);

            matrix[i][j] = pointer_to_vsq->vsq;
            matrix[j][i] = pointer_to_vsq->vsq;
        }
    }

    return matrix;
}

//Function for printing vector of vector<double> to cout
void print_vector_of_a_vector(vector<vector<double>> v)
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