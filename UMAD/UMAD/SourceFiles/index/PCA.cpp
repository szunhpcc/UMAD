#include "../../HeaderFiles/index/PCA.h"

#include <stdio.h>

#include "../../SourceFiles/util/GSL/include/gsl/gsl_rng.h"

#include "../../SourceFiles/util/GSL/include/gsl/gsl_matrix_double.h"

#include "../../SourceFiles/util/GSL/include/gsl/gsl_vector_double.h"

#include "../../SourceFiles/util/GSL/include/gsl/gsl_blas.h"

#include "../../SourceFiles/util/GSL/include/gsl/gsl_eigen.h"

#include "../../SourceFiles/util/GSL/include/gsl/gsl_linalg.h"

#include "../../SourceFiles/util/GSL/include/gsl/gsl_statistics.h"

#include <math.h>

#include <time.h>

#include <string.h>

//testtest

/** used to get the inverse matrix mi = (m)'*/

void CPCA::matrix_inverse(gsl_matrix *m,gsl_matrix *mi)

{

    int n1=m->size1;

    int n2=m->size2;

    gsl_permutation * p = gsl_permutation_alloc(n1);

    int sign = 1;

    gsl_linalg_LU_decomp(m, p, &sign);

    gsl_linalg_LU_invert(m, p, mi);



    gsl_permutation_free(p);

}



/** used to get a transpose matrix, ct = (m)T */

void CPCA::matrix_transpose(gsl_matrix *m,gsl_matrix *ct)

{

    // just change the positon of parameters

    gsl_matrix_transpose_memcpy(ct,m);

}



/** used to get product matrix, m3 =  m1 * m2 */

void CPCA::matrix_mul(gsl_matrix *m1,gsl_matrix *m2,gsl_matrix *m3)

{

    gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,1.0, m1, m2,0.0, m3);

}



/** used to get a covariance matrix, m = cov(r)*/

void CPCA::cov_calculate(gsl_matrix *r, gsl_matrix *m)

{

    size_t i, j;

    gsl_vector *a,*b;

    a=gsl_vector_calloc(m->size1);

    b=gsl_vector_calloc(m->size1);

    for (i =0; i <m->size2; ++i) {

        for (j = 0; j <m->size2; ++j) {

            double v;

            gsl_matrix_get_col (a,m,i);

            gsl_matrix_get_col (b,m,j);

            v = gsl_stats_covariance (a->data, a->stride,b->data, b->stride, a->size);

            gsl_matrix_set (r, i, j, v);



        }

    }



    gsl_vector_free(a);

    gsl_vector_free(b);

}



/** used to centerize the  matrix */

void CPCA::centerize(gsl_matrix* matrix)

{

    const int size = matrix->size1;//row

    const int dim = matrix->size2;//col



    double* columnMean = new double [dim];

    for (int i=0; i< dim; ++i)

    {

        columnMean[i] = 0;

        for(int j=0; j<size; ++j)

            columnMean[i] += gsl_matrix_get(matrix,j,i);//matrix.get(j,i);

        columnMean[i] /= size;

    }



    for (int i=0; i< dim; ++i)

        for (int j=0;j<size; ++j)

            gsl_matrix_set(matrix,j,i,gsl_matrix_get(matrix,j,i) - columnMean[i]);//matrix.set(j, i, matrix.get(j,i) - columnMean[i]);



    delete(columnMean);

}





/** vector1 mult vector2 to double, result = sum(vec1[i] * vec2[i]) */

double CPCA::vecMult(const gsl_vector* vec1, const gsl_vector* vec2)

{

    double result = 0;

    int size = vec1->size < vec2->size ? vec1->size : vec2->size;

    for (int i = 0; i < size; ++i)

        result += vec1->data[i] * vec2->data[i];

    return result;

}



/** do the iteration part in EM, iterNum is the time of iteration */

void CPCA::iteration (gsl_matrix *data,gsl_matrix *C,int iterNum)

{

    gsl_matrix *x=gsl_matrix_calloc(C->size2,data->size2);

    gsl_matrix *c=gsl_matrix_calloc(C->size2,C->size2);

    gsl_matrix *cT=gsl_matrix_calloc(C->size2,C->size1);

    gsl_matrix *cT1=NULL;

    gsl_matrix *cI=gsl_matrix_calloc(C->size2,C->size2);

    gsl_matrix *cM=gsl_matrix_calloc(C->size2,C->size2);

    gsl_matrix *cM1=NULL;

    gsl_matrix *cM2=NULL;



    //===========================================

    for(int i=0;i<iterNum;++i)

    {

        gsl_matrix_transpose_memcpy(cT,C);  



        matrix_mul(cT,C,cM); 

        matrix_inverse(cM,cI); 



        cM1=gsl_matrix_calloc(C->size2,C->size1);

        matrix_mul(cI,cT,cM1);   



        matrix_mul(cM1,data,x); 

        //STEP 1 END



        cT1=gsl_matrix_calloc(data->size2,C->size2);

        gsl_matrix_transpose_memcpy(cT1,x); 



        matrix_mul(x,cT1,cM);  

        matrix_inverse(cM,cI); 





        cM2=gsl_matrix_calloc(data->size1,C->size2);

        matrix_mul(data,cT1,cM2); 

        matrix_mul(cM2,cI,C); 

        //STEP 2 END





    }

    gsl_matrix_free(x);

    gsl_matrix_free(c);

    gsl_matrix_free(cT);

    gsl_matrix_free(cT1);

    gsl_matrix_free(cI);

    gsl_matrix_free(cM);

    gsl_matrix_free(cM1);

    gsl_matrix_free(cM2);

}



/* 

  out[ outBeginRow : outBeginRow+rows, outBeginCol : outBeginCol+cols]

  = in[ inBeginRow : inBeginRow+rows, inBeginCol : inBeginCol+cols] 

*/

void CPCA::viewPartCopyWithABS(int inBeginRow, int inBeginCol, int rows, int cols, 

       const gsl_matrix *in, int outBeginRow, int outBeginCol, gsl_matrix *out)

{

    if(rows + inBeginRow <= in->size1 && rows + outBeginRow <= out->size1 &&

        cols + inBeginRow <= in->size2 && cols + outBeginRow <= in->size2)

    {

        for (int i = 0; i < rows; ++i)

        {

            for (int j = 0; j < cols; ++j)

            {

                gsl_matrix_set(out, outBeginRow + i, outBeginRow + j,

                    gsl_matrix_get(in, inBeginRow + i, inBeginCol + j));

            }

        }

    }

}



/* the same as viewPartCopy but all the value is absolute.*/

void CPCA::viewPartCopy(int inBeginRow, int inBeginCol, int rows, int cols, 

       const gsl_matrix *in, int outBeginRow, int outBeginCol, gsl_matrix *out)

{

    if(rows + inBeginRow <= in->size1 && rows + outBeginRow <= out->size1 &&

        cols + inBeginRow <= in->size2 && cols + outBeginRow <= in->size2)

    {

        for (int i = 0; i < rows; ++i)

        {

            for (int j = 0; j < cols; ++j)

            {

                gsl_matrix_set(out, outBeginRow + i, outBeginRow + j,

                    abs(gsl_matrix_get(in, inBeginRow + i, inBeginCol + j)));

                /* the diference between this and viewPartCopy */

            }

        }

    }

}



/* out[ outBeginRow : outBeginRow+rows, outBeginCol : outBeginCol+cols]

   = in[ 0 : rows, 0 : cols] 

*/

void CPCA::viewPartCopy(int outBeginRow, int outBeginCol, int rows, int cols, 

         const gsl_matrix *in, gsl_matrix *out)

{

    viewPartCopy(0, 0, rows, cols, in, outBeginRow, outBeginCol, out);

}



/** swap(m[:, i], m[:, n - i] */

void CPCA::columnFilp(gsl_matrix *m)

{

    for(int i=0;i<m->size2/2;++i)

    {

        gsl_matrix_swap_columns(m,i,m->size2-1-i);

    }

}



/** get the rank of the matrix*/

int CPCA::getRank(const gsl_matrix* matrix)

{

    /* 

    the SVD in the GSL lib will change the input matirx ,

    to keep the origin matrix, copy the matrix to A below

    */

    gsl_matrix* A = gsl_matrix_calloc(matrix->size1, matrix->size2);

    gsl_matrix_memcpy(A, matrix);



    gsl_matrix* V = gsl_matrix_alloc(A->size2,A->size2);

    gsl_vector* S = gsl_vector_alloc(A->size2);



    /* contribute the SVD of matrix */

    gsl_linalg_SV_decomp_jacobi(A,V,S);



    /* using the S of SVD to contribute rank */

    int n = A->size1 > A->size2? A->size1: A->size2;

    double maxS = gsl_stats_max(S->data,1,S->size);

    double eps = pow(2.0,-52.0); // eps is the min double non-zero number  

    double tol =n * eps * S->data[0]; // tol is the number see as zero

    int rank = 0;

    for (int i = 0; i < S->size; ++i)

    {

        if(gsl_vector_get(S,i) > tol) rank++;

    }



    gsl_matrix_free(A);

    gsl_matrix_free(V);

    gsl_vector_free(S);

    return rank;

}



/** orthonormalize the matrix*/

gsl_matrix* CPCA::orthonormalization(const gsl_matrix* matrix)

{

    const int rowNum = matrix->size1;

    const int colNum = matrix->size2;

    const int rank = getRank(matrix);



    gsl_matrix* result = gsl_matrix_alloc(rowNum, rank);



    gsl_vector* colVec = gsl_vector_alloc(rowNum);

    gsl_vector* iVec = gsl_vector_alloc(rowNum);

    gsl_matrix_get_col(colVec,matrix,0);

    gsl_matrix_set_col(result,0,colVec);



    for(int col=1; col<rank; col++)

    {

        //result.viewColumn(col).assign(matrix.viewColumn(col));

        gsl_matrix_get_col(colVec,matrix,col);

        gsl_matrix_set_col(result,col,colVec);



        for(int i=0; i<col; ++i)

        {

            /*result.viewColumn(col).assign(tempColumn.assign( alg.mult(matrix.viewColumn(col), 

            result.viewColumn(i))/alg.norm2(result.viewColumn(i))).assign(result.viewColumn(i), Functions.mult), Functions.minus);*/



            //alg.norm2(result.viewColumn(i)) : A

            gsl_matrix_get_col(iVec,result,i);

            double norm2 = gsl_blas_dnrm2(iVec);

            norm2*=norm2;



            //alg.mult(matrix.viewColumn(col), result.viewColumn(i)) : B

            gsl_matrix_get_col(colVec,matrix,col);

            double mult = vecMult(colVec,iVec);



            //tempColumn.assign(B / A)   :   C

            gsl_vector_set_all(colVec,mult/norm2);



            //C.assign(result.viewColumn(i), Functions.mult)  :  D

            gsl_matrix_get_col(iVec,result,i);

            gsl_vector_mul(iVec,colVec);



            //result.viewColumn(col).assign((D, Functions.minus)

            gsl_matrix_get_col(colVec,result,col);

            gsl_vector_sub(colVec,iVec);

            gsl_matrix_set_col(result,col,colVec);

        }

    }



    for(int col=0; col<rank;col++)

    {

        //result.viewColumn(col).assign(tempColumn.assign(Math.sqrt(alg.norm2(result.viewColumn(col)))), Functions.div);



        //alg.norm2(result.viewColumn(col))  :  A

        gsl_matrix_get_col(colVec,result,col);

        double norm2 = gsl_blas_dnrm2(colVec);



        //Math.sqrt(A)  :   B

        //norm2 = sqrt(norm2);



        //tempColumn.assign(B) :  C

        gsl_vector_set_all(iVec,norm2);

        // result.viewColumn(col).assign(C, Functions.div);

        gsl_vector_div(colVec,iVec);

        gsl_matrix_set_col(result,col,colVec);

    }



    gsl_vector_free(colVec);

    gsl_vector_free(iVec);

    return result;

}



gsl_matrix* CPCA::EMPCA(gsl_matrix* matrix, const int pcNum)

{

    const int iterNum = 20; /** the time to interate below */

    const int dim = matrix->size2; 



    /* step 1: centerize the matrix */

    centerize(matrix);



    gsl_matrix* data = gsl_matrix_alloc(matrix->size2, matrix->size1);

    gsl_matrix_transpose_memcpy(data,matrix);



    /* step 2: initial a random matrix */

    const gsl_rng_type *T;    

    gsl_rng *r;

    double u;

    gsl_rng_env_setup();

    T = gsl_rng_default;

    gsl_rng_default_seed = ((unsigned long)(time(NULL)));

    r = gsl_rng_alloc(T);    

    u = gsl_rng_uniform(r);    



    gsl_matrix* C = gsl_matrix_alloc(dim,pcNum);

    for(int i=0;i<dim;++i)

    {

        for(int j=0;j<pcNum;++j)

        {

            u = gsl_rng_uniform(r); //����һ��[0, 1)�����ϵ������

            gsl_matrix_set(C,i,j,u - 0.5);

        }

    }



    /* step 3: the iteration with the random matrix and centerize matrix, the part of EM */

    iteration(data,C,iterNum);



    /* step 4: orthonomalize the matrix */

    C = orthonormalization(C);



    /* step 5: contribute the covariance of the orthonormalized matrix */

    gsl_matrix* cMul = gsl_matrix_alloc(C->size2, data->size2);

    gsl_blas_dgemm (CblasTrans, CblasNoTrans,1.0, C, data, 0.0, cMul);

    gsl_matrix* cMulDense  = gsl_matrix_alloc(cMul->size2, cMul->size1);

    gsl_matrix_transpose_memcpy(cMulDense,cMul);



    gsl_matrix* cov = gsl_matrix_alloc(cMulDense->size2, cMulDense->size2);

    cov_calculate(cov, cMulDense);



    /* step 6: contribute the eigenValues and eigenMatrix of the cov */

    gsl_eigen_symmv_workspace* w = gsl_eigen_symmv_alloc(cov->size1 * 2);

    gsl_vector* eval = gsl_vector_calloc (cov->size1);

    gsl_matrix* evec = gsl_matrix_calloc(cov->size1,cov->size1);

    gsl_eigen_symmv(cov,eval,evec,w);



    /* step 7: assign the result matrix from row 1 and col 0

    result[ 1:rowSize, 0:colSize] = orthonormalized matrix * eigenMatrix

    */

    gsl_matrix* result = gsl_matrix_alloc(dim+1, pcNum);

    columnFilp(evec);

    gsl_matrix* tmp = gsl_matrix_calloc(C->size1, evec->size2);

    matrix_mul(C,evec,tmp);

    viewPartCopy(1,0,dim,pcNum,tmp,result);



    /* step 8: copy the eigenValues as the first row the result */

    for (int i = eval->size; i >0; --i)

    {

        gsl_matrix_set(result, 0, eval->size-i, gsl_vector_get(eval,i-1));

    }



    /* step 9: transpose the result as tResult */

    gsl_matrix* tResult = gsl_matrix_alloc(pcNum,dim+1);

    gsl_matrix_transpose_memcpy(tResult,result);



    /*--------------- EMPCA END ------------------*/



    /* the code below used to print the result of EMPCA */

    /*printf("============================\nresult\n");

    for (int i = 0; i < tResult->size1; ++i)

    {

    for (int j = 0; j < tResult->size2; ++j)

    {

    printf("%lf ", gsl_matrix_get(tResult,i,j));

    }printf("\n");

    }printf("\n");*/



    gsl_eigen_symmv_free(w);

    gsl_matrix_free(tmp);

    gsl_matrix_free(data);

    gsl_matrix_free(result);

    gsl_matrix_free(evec);

    gsl_vector_free(eval);

    gsl_matrix_free(C);

    gsl_matrix_free(cov);

    gsl_matrix_free(cMul);

    gsl_matrix_free(cMulDense);

    gsl_rng_free(r);

    return tResult;

    int * b = new int[5];

}



vector<int> CPCA::pivotSelectionByPCAResultAngle(const gsl_matrix* pcaResult, int numP)

{

    const int pcNum = pcaResult->size1;

    const int dim = pcaResult->size2 - 1;



    //gsl_matrix* PC = gsl_matrix_alloc(pcNum, dim);

    //viewPartCopyWithABS(0, 0, pcNum, dim, pcaResult, 1, 0, PC);

    

    double* PC = new double[pcNum * dim];



    for (int i = 0; i < pcNum; ++i)

    {

        double* pPC = PC + i * dim;

        for (int j = 0; j < dim; ++j)

        {

            pPC[j] = abs(gsl_matrix_get(pcaResult, i, 1+j));

        }

    }

    

    vector<int> result;

    bool* isAdded = new bool[dim];

    memset(isAdded, dim, 0);

    int maxValue, maxValuePos;

    for (int i = 0; (i < pcNum) && (result.size() < numP); i++)

    {

        maxValue = -1;

        double* pPC = PC + i*dim;

        for (int j = 0; (j < dim) && (result.size() < numP); j++)

        {

            if(maxValue < pPC[j])

            {

                maxValue = pPC[j];

                maxValuePos = j;

            }

        }



        if(!isAdded[maxValuePos])

        {

            result.push_back(maxValuePos);

            isAdded[maxValuePos] = true;

            pPC[maxValuePos] = -2;

        }

    }



    return result;

}





