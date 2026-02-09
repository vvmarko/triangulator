#include "triangulator.hpp"

int main(){
    cout << endl << "#######################################################" << endl;
    cout         << "############ BEGINNING OF THE MAIN FUNCTION ###########" << endl;
    cout         << "#######################################################" << endl << endl;

    vector<vector<double>> A = {
    {  1.732050807,  -2.414213562,   0.577350269 },
    {  0.0,          -3.141592654,   2.718281828 },
    {  0.0,           0.0,           0.693147181 }
    };
    double determinant = real_valued_determinant(&A);
    cout << "Determinant: " << determinant << endl;
    A = {
    {  1.23456789,  -2.34567891,   3.45678912 },
    { -4.56789123,   5.67891234,  -6.78912345 },
    {  7.89123456,  -8.91234567,   9.12345678 }
    };
    determinant = real_valued_determinant(&A);
    cout << "Determinant: " << determinant << endl;

    vector<vector<double>> B = {
    {  3.14159265,  -2.71828183,   1.61803399,  -0.57721566 },
    { -1.41421356,   0.69314718,  -2.30258509,   4.66920161 },
    {  2.50290788,  -3.14159265,   0.57721566,  -1.09861229 },
    { -0.30102999,   1.44269504,  -4.60517019,   2.71828183 }

    };
    determinant = real_valued_determinant(&B);
    cout << "Determinant: " << determinant << endl;
    vector<vector<double>> C = {
    {  3.14159265,  -2.71828183,   1.61803399 },
    { -1.41421356,   0.69314718,  -2.30258509,   4.66920161 },
    {  2.50290788,  -3.14159265,   0.57721566,  -1.09861229 },
    { -0.30102999,   1.44269504,  -4.60517019,   2.71828183 }
    
    };
    determinant = real_valued_determinant(&C);
    cout << "Determinant: " << determinant << endl;

    vector<vector<double>> matrix = {
    {  3.2, -1.5,  4.8,  0.7, -2.1,  5.0,  1.3, -3.4,  2.9,  0.6 },
    { -4.1,  2.7,  0.9, -1.8,  3.3, -0.5,  4.6,  1.2, -2.0,  5.1 },
    {  1.4, -3.6,  2.5,  4.9,  0.8, -1.7, -2.3,  3.0,  5.6, -0.4 },
    {  2.8,  5.1, -4.0,  1.6, -3.2,  0.9,  2.4, -1.5,  0.7,  4.3 },
    { -0.9,  3.4,  1.7, -2.6,  5.2, -4.8,  0.6,  2.1, -1.3,  3.9 },
    {  4.5, -2.8,  3.1,  0.4, -1.6,  2.9, -3.7,  5.0,  1.8, -0.2 },
    {  0.6,  1.9, -2.4,  3.7,  4.1, -0.8,  5.3, -1.0,  2.6, -3.5 },
    { -3.0,  0.5,  4.2, -1.9,  2.7,  3.6, -0.4,  1.8,  5.0, -2.1 },
    {  5.4, -1.2,  0.3,  2.8, -4.6,  1.5,  3.9, -0.7,  4.1,  0.9 },
    { -2.7,  4.0,  5.1, -0.3,  1.9, -3.4, -1.8,  2.6,  0.5,  3.2 }
    };

    auto a = &matrix;

    determinant = real_valued_determinant(&matrix);

    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[i].size(); j++){
            if((*a)[i][j] != matrix[i][j]){
                cout << "Error: The original matrix has been modified!" << endl;
                return -1;
            }
        }
    }
    cout << "Determinant: " << determinant << endl;
    
    cout << endl << "#################################################" << endl;
    cout         << "############ END OF THE MAIN FUNCTION ###########" << endl;
    cout         << "#################################################" << endl << endl;

    return 0;
}