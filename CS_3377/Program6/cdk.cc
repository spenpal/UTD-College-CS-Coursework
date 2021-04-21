/*
 * Filename:    cdk.cc
 * Date:        04/26/2020
 * Author:      Sanjeev Penupala
 * Email:       sxp170022@utdallas.edu
 * Course:      CS 3377.501 Spring 2020
 * Version:     1.0
 * Copyright    2020, All Rights Reserved
 *
 * Description:
 *
 *	Display Header Record(s) and Data Record(s) In CDK Matrix
 *
 */

#include "program6.h"

void display(string matrix[MATRIX_HEIGHT + 1][MATRIX_WIDTH + 1])
{
  WINDOW        *window;
  CDKSCREEN     *cdkscreen;
  CDKMATRIX     *myMatrix;           	// CDK Screen Matrix

  const char    *rowTitles[MATRIX_HEIGHT+1] =   {"x", "a", "b", "c", "d", "e"};
  const char    *columnTitles[MATRIX_WIDTH+1] = {"x", "a", "b", "c"};
  int           boxWidths[MATRIX_WIDTH+1] =     {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int           boxTypes[MATRIX_WIDTH+1] =      {vMIXED, vMIXED, vMIXED, vMIXED};

  //Initialize the CDK screen.
  window = initscr();
  cdkscreen = initCDKScreen(window);

  // Start CDK Colors
  initCDKColor();

  // Create the matrix.  Need to manually cast (const char**) to (char **)
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
                          MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
			  boxTypes, 1, 1, ' ', ROW, true, true, false);

  if(myMatrix == NULL)
  {
    printf("Error creating Matrix\n");
    exit(1);
  }

  // Display the Matrix
  drawCDKMatrix(myMatrix, true);

  // Dipslay a message
  for(int r = 1; r < MATRIX_HEIGHT + 1; r++)
  {
    for(int c = 1; c < MATRIX_WIDTH + 1; c++)
    {
      setCDKMatrixCell(myMatrix, r, c, matrix[r][c].c_str());
    }
  }

  drawCDKMatrix(myMatrix, true);        // REQUIRED

  // If Key Is Pressed, Exit CDK Matrix
  if(getchar())
    endCDK();     			// Cleanup Screen
}
