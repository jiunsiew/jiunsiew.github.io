#include <Rcpp.h>
using namespace Rcpp;

// This is a simple example of exporting a C++ function to R. You can
// source this function into an R session using the Rcpp::sourceCpp 
// function (or via the Source button on the editor toolbar). Learn
// more about Rcpp at:
//
//   http://www.rcpp.org/
//   http://adv-r.had.co.nz/Rcpp.html
//   http://gallery.rcpp.org/
//


// calculate the hamming distance between two binary matrices.  
// To be used for two behavioural matrices, the first one being the behavioural
// matrix of users, the second one being the behaviouarl matrix of a group of 
// users you want to match the closest to.
// 
// [[Rcpp::export]]
NumericMatrix hamming(NumericMatrix x, NumericMatrix y){
  
  // get the matrix dimensions
  int xNrow = x.nrow();
  int xNcol = x.ncol();
  int yNrow = y.nrow();
  int yNcol = y.ncol();
  
  // output--> rows correspond to rows in x, columns correspond to rows in y
  NumericMatrix output(xNrow, yNrow);
  
  for (int iX = 0; iX < xNrow; iX++){
    
    for (int iY = 0; iY < yNrow; iY++){
      output(iX, iY) = 0;
      
      for (int iC = 0; iC < xNcol; iC++){
        output(iX, iY) += abs(x(iX, iC) - y(iY, iC));
        
        //Rprintf("\nx: %d\ty: %d\toutput: %d", x(iX, iC), y(iY, iC), output(iX, iY));
      }
      
    }
    
  }
  return output;
}

// You can include R code blocks in C++ files processed with sourceCpp
// (useful for testing and development). The R code will be automatically 
// run after the compilation.
//

// /*** R
// nC <- 5
// nR.x <- 5
// nR.y <- 3
// 
// x.mat <- matrix(data = sample.int(2, size = nC*nR.x, replace = TRUE)-1, ncol = nC)
// y.mat <- matrix(data = sample.int(2, size = nC*nR.y, replace = TRUE)-1, ncol = nC)
// 
// print(x.mat)
// print(y.mat)
// 
// hamming(x.mat, y.mat)
// */
