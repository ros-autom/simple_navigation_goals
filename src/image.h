/*
 * image.h
 *
 *  Created on: Jan 29, 2017
 *      Author: gra
 */



#ifndef IMAGE_H

#include <iostream>
using namespace std;

#define IMAGE_H

class Image
{
      public:
             Image()
      	  	  	  /* Creates an Image 0x0 */
      	  	  	  {
            	 		N = 0;
            	 		M = 0;
            	 		Q = 0;

            	 		pixelVal = NULL;
      	  	  	  }
             Image(int numRows, int numCols, int grayLevels)
             /* Creates an Image of numRows x numCols and creates the arrays for it*/
             {

                 N = numRows;
                 M = numCols;
                 Q = grayLevels;

                 pixelVal = new int *[N];
                 for(int i = 0; i < N; i++)
                 {
                     pixelVal[i] = new int [M];
                     for(int j = 0; j < M; j++)
                         pixelVal[i][j] = 0;
                 }
             }
             ~Image()
             /*destroy image*/
             {
            	 N = 0;
            	 M = 0;
            	 Q = 0;

                 for(int i = 0; i < N; i++)
                     delete pixelVal [N];

                 delete pixelVal;
             }

             Image(const Image& oldImage)
             /*copies oldImage into new Image object*/
             {
            	 N = oldImage.N;
            	 M = oldImage.M;
            	 Q = oldImage.Q;

                 pixelVal = new int* [N];
                 for(int i = 0; i < N; i++)
                 {
                     pixelVal[i] = new int [M];
                     for(int j = 0; j < M; j++)
                         pixelVal[i][j] = oldImage.pixelVal[i][j];
                 }
             }


             void set_image_info(int numRows, int numCols, int maxVal)
             /*sets the number of rows, columns and graylevels*/
             {
            	 N = numRows;
            	 M = numCols;
            	 Q = maxVal;
             }
             void get_image_info(int &numRows, int &numCols, int &maxVal)

             /*returns the number of rows, columns and gray levels*/
             {
                 numRows = N;
                 numCols = M;
                 maxVal = Q;
             }

             int get_pixel_value(int row, int col)
             /*returns the gray value of a specific pixel*/
             {
                 return pixelVal[row][col];
             }

             void set_pixel_value(int row, int col, int value)
             /*sets the gray value of a specific pixel*/
             {
                 pixelVal[row][col] = value;
             }

             double white_pixels()
             {
            	 int white_pixel=0;
            	 for(int i=0;i<N;i++)
            	 {
            		 for(int j=0;j<M;j++)
            		 {
            			if(pixelVal[i][j]>240)
            				 white_pixel++;



            		 }

            	 }
            		 double cells=N*M;
            	      return ((white_pixel/cells)*100);

             }


      private:
              int N; // number of rows
              int M; // number of columns
              int Q; // number of gray levels
              int **pixelVal;
};

#endif
