/* $Id: Matrix3x3.cc,v 1.3 2000/04/12 21:28:08 vinod Exp $ */

// Non-inline source code for Matrix3x3 class

#include "Matrix3x3.hh"

   // Invert the matrix, using elementary row operations
void Matrix3x3 :: invert(void)
{
  Vector3d inv[3];
  int i,j,swaprow;

  for (i=0; i < 3; ++i)
     inv[i][i] = 1.0;
         
     // inv will be identity initially and will become the inverse at the end
  for (i=0; i < 3; ++i)
     {
          // i is column
          // Find row in this column which has largest element (magnitude)
       swaprow = i;
       for (j=i+1; j < 3; ++j)
          if ( fabs(row[j][i]) > fabs(row[i][i]) ) swaprow = j;

       if ( swaprow != i )
          {
               // Swap the two rows to get largest element in main diagonal
               // Do this for the RHS also
            swap(row[i],row[swaprow]); swap(inv[i],inv[swaprow]);
          }

          // Check if pivot is non-zero
       if ( !isNonZero(row[i][i]) )
          {
            cerr << "Matrix3x3 inverse(const Matrix3x3&) : Singular matrix!" << endl;
               // Return original matrix without change
            return;
          }

          // Divide matrix by main diagonal element to make it 1.0
       double fact = row[i][i];
       for (j=0; j < 3; ++j)
          {
            row[j] /= fact;
            inv[j] /= fact;
          }

          // Make non-main-diagonal elements in this column 0 using main-diagonal row
       for (j=0; j < 3; ++j)
          {
            if ( j != i )
               {
                 double temp = row[j][i];
                 row[j] -= row[i]*temp;
                 inv[j] -= inv[i]*temp;
               }
          }
     }

     // Main-diagonal elements on LHS may not be 1.0 now. Divide to make LHS identity
     // Last row will be 1.0
  for (i=0; i < 2; ++i)
     {
       double pivot = row[i][i];
       row[i] /= pivot;
       inv[i] /= pivot;
     }
  for (i=0; i < 3; ++i)
     row[i] = inv[i];
}

/*
  The following functions are defined outside the class so that they use the
  friend versions of member functions instead of the member functions themselves
*/
     
   // Multiplication of 2 matrices - outer product
Matrix3x3 operator * (const Matrix3x3& mat1, const Matrix3x3& mat2)
{
  Matrix3x3 prod, trans;

     // Find the transpose of the 2nd matrix
  trans = transpose(mat2);

     // The columns of mat2 are now the rows of trans
     // Multiply appropriate rows and columns to get the product
  prod.row[0].set( mat1.row[0]*trans.row[0],
                   mat1.row[0]*trans.row[1],
                   mat1.row[0]*trans.row[2] );
  prod.row[1].set( mat1.row[1]*trans.row[0],
                   mat1.row[1]*trans.row[1],
                   mat1.row[1]*trans.row[2] );
  prod.row[2].set( mat1.row[2]*trans.row[0],
                   mat1.row[2]*trans.row[1],
                   mat1.row[2]*trans.row[2] );
  return prod;
}

   // Pre-multiplication by a Vector3d. Vector is assumed to be a row vector
Vector3d operator * (const Vector3d& vec, const Matrix3x3& mat)
{
  return ( transpose(mat) * vec );
}


/*
  $Log: Matrix3x3.cc,v $
  Revision 1.3  2000/04/12 21:28:08  vinod
  Bug fix: invert() function, vectors on the RHS (for row operations) weren't
  being initialized to be the identity matrix.

  Revision 1.2  2000/02/09 22:15:06  vinod
  Code modifications to take care of file/class renames

  Revision 1.1  2000/02/09 22:01:22  vinod
  Renamed

  Revision 1.1  2000/02/09 21:21:42  vinod
  Moved into VecMat directory

  Revision 1.1  2000/02/09 07:03:28  vinod
  Moved invert fns, etc. and friend fns defined outside class to source file

*/
