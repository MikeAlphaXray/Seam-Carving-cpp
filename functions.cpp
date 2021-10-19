#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

#define INFO(X) cout << "[INFO] ("<<__FUNCTION__<<":"<<__LINE__<<") " << #X << " = " << X << endl;

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;

Pixel** createImage(int width, int height) {
  cout << "Start createImage... " << endl;
  
  // Create a one dimensional array on the heap of pointers to Pixels 
  //    that has width elements (i.e. the number of columns)
  Pixel** image = new Pixel*[width];
  
  bool fail = false;
  
  for (int i=0; i < width; ++i) { // loop through each column
    // assign that column to a one dimensional array on the heap of Pixels
    //  that has height elements (i.e. the number of rows)
    image[i] = new Pixel[height];
    
    if (image[i] == nullptr) { // failed to allocate
      fail = true;
    }
  }
  
  if (fail) { // if any allocation fails, clean up and avoid memory leak
    // deallocate any arrays created in for loop
    for (int i=0; i < width; ++i) {
      delete [] image[i]; // deleting nullptr is not a problem
    }
    delete [] image; // dlete array of pointers
    return nullptr;
  }
  
  // initialize cells
  //cout << "Initializing cells..." << endl;
  for (int row=0; row<height; ++row) {
    for (int col=0; col<width; ++col) {
      //cout << "(" << col << ", " << row << ")" << endl;
      image[col][row] = { 0, 0, 0 };
    }
  }
  cout << "End createImage... " << endl;
  return image;
}

void deleteImage(Pixel** image, int width) {
  cout << "Start deleteImage..." << endl;
  // avoid memory leak by deleting the array
  for (int i=0; i<width; ++i) {
    delete [] image[i]; // delete each individual array placed on the heap
  }
  delete [] image;
  image = nullptr;
}

int* createSeam(int length) {
    // TODO(student): create a seam
  int *seam = new int[length];
  for(int i=0; i<length; i++){
    seam[i] = 0;
  }
  return seam;
}

void deleteSeam(int* seam) {
    // TODO(student): delete a seam
    delete[] seam;
}

bool loadImage(string filename, Pixel** image, int width, int height) {
  // TODO(student): load an image
  cout << "Loading image..." << endl;
  // declare/define and open input file stream
  ifstream ifs (filename);
  
  // check if input stream opened successfully
  if (!ifs.is_open()) {
    cout << "Error: failed to open input file - " << filename << endl;
    return false;
  }
  
  // get type from preamble
  char type[3];
  ifs >> type; // should be P3
/*   if(!isdigit(type[1])){
    cout << "Error: read non-integer value" << endl;
    return false;
  } */
  if ((toupper(type[0]) != 'P') || (type[1] != '3')) { // check that type is correct
    cout << "Error: type is " << type << " instead of P3" << endl;
    return false;
  }
  
  // get width (w) and height (h) from preamble
  int w = 0, h = 0;
  ifs >> w;
  if(ifs.fail()){
    cout << "Error: read non-integer value" << endl;
    return false;
  }
  if(w<=0){
    cout << "Error: width must be greater than 0. Read " << w << endl;
    return false;
  }
  if (w != width) { // check that width matches what was passed into the function
    cout << "Error: input width ("<< width <<") does not match value in file ("<< w <<")" << endl;
    return false;
  }
  
  ifs >> h;
  if(ifs.fail()){
    cout << "Error: read non-integer value" << endl;
    return false;
  }
  if(h<=0){
    cout << "Error: height must be greater than 0. Read " << h << endl;
    return false;
  }
  if (h != height) { // check that height matches what was passed into the function
    cout << "Error: input height ("<< height <<") does not match value in file ("<< h <<")" << endl;
    return false;
  }
  
  // get maximum value from preamble
  int colorMax = 0;
  ifs >> colorMax;
  if(ifs.fail()){
    cout << "Error: read non-integer value" << endl;
    return false;
  }
  if (colorMax != 255) {
    cout << "Error: file is not using RGB color values." << endl;
    return false;
  }

  // TODO(student): get RGB pixel values
  for(int row = 0; row < height; ++row){
    for(int col = 0; col < width; ++col){
      int r, g, b;
      ifs >> r;
      if(ifs.fail() && ifs.eof()){
        cout << "Error: not enough color values" << endl;
        return false;
      }
      if(ifs.fail()){
        cout << "Error: read non-integer value" << endl;
        return false;
      }  
      if(r>colorMax || r<0){
        cout << "Error: invalid color value " << r << endl;
        return false;
      }
      
      ifs >> g;
      if(ifs.fail() && ifs.eof()){
        cout << "Error: not enough color values" << endl;
        return false;
      }
      if(ifs.fail()){
        cout << "Error: read non-integer value" << endl;
        return false;
      }      
      if(g>colorMax || g<0){
        cout << "Error: invalid color value " << g << endl;
        return false;
      }      

      ifs >> b;
      if(ifs.fail() && ifs.eof()){
        cout << "Error: not enough color values" << endl;
        return false;
      }
      if(ifs.fail()){
        cout << "Error: read non-integer value" << endl;
        return false;
      }  
      if(b>colorMax || b<0){
        cout << "Error: invalid color value " << b << endl;
        return false;
      }
      image[col][row].r = r;
      image[col][row].g = g;
      image[col][row].b = b;
    }
  }

  int extra;

  ifs >> extra;

  if(!ifs.fail()){
     cout << "Error: too many color values" << endl;
    return false;
  }

  return true;
}

bool outputImage(string filename, Pixel** image, int width, int height) {
    // TODO(student): output an image
  cout << "Outputting image..." << endl;
  // TODO(student): declare/define and open output file stream
  ofstream ofs (filename);
  
  // TODO(student): check if output stream opened successfully
  if (!ofs.is_open()) {
    cout << "Error: failed to open ofstream " << filename << endl;
    return false;
  }
  
  // TODO(student): output preamble
  ofs << "P3" << endl;
  ofs << width << " " << height << endl;
  ofs << "255" << endl;

  // TODO(student): output pixels
  for(int row = 0; row < height; ++row){
    for(int col = 0; col < width; ++col){
      ofs << image[col][row].r << " ";
      ofs << image[col][row].g << " ";
      ofs << image[col][row].b << " ";
    }
    ofs << endl;
  }
  return true;
}

int energy(Pixel** image, int x, int y, int width, int height) { 
    // TODO(student): compute the energy of a pixel
/*     INFO(image);
    INFO(x);
    INFO(y);
    INFO(width);
    INFO(height); */

  int xEngy = 0;
  int yEngy = 0;
  
  if(width>1){
    if(x == 0){
      xEngy = pow((image[width - 1][y].r - image[x + 1][y].r), 2)
            + pow((image[width - 1][y].g - image[x + 1][y].g), 2)
            + pow((image[width - 1][y].b - image[x + 1][y].b), 2);
    }else if(x == width - 1){
      xEngy = pow((image[0][y].r - image[x - 1][y].r), 2)
            + pow((image[0][y].g - image[x - 1][y].g), 2)
            + pow((image[0][y].b - image[x - 1][y].b), 2);
    }else if(x > 0 && x < width - 1){
      xEngy = pow((image[x - 1][y].r - image[x + 1][y].r), 2)
            + pow((image[x - 1][y].g - image[x + 1][y].g), 2)
            + pow((image[x - 1][y].b - image[x + 1][y].b), 2);
    }
  }
  if(height>1){
    if(y == 0){
      yEngy = pow((image[x][height - 1].r - image[x][y + 1].r), 2)
            + pow((image[x][height - 1].g - image[x][y + 1].g), 2)
            + pow((image[x][height - 1].b - image[x][y + 1].b), 2);
    }else if(y == height - 1){
      yEngy = pow((image[x][y - 1].r - image[x][0].r), 2)
            + pow((image[x][y - 1].g - image[x][0].g), 2)
            + pow((image[x][y - 1].b - image[x][0].b), 2);
    }else if(y > 0 && y < height - 1){
      yEngy = pow((image[x][y - 1].r - image[x][y + 1].r), 2)
            + pow((image[x][y - 1].g - image[x][y + 1].g), 2)
            + pow((image[x][y - 1].b - image[x][y + 1].b), 2);
    }
  }

  return xEngy + yEngy;
}

int loadVerticalSeam(Pixel** image, int start_col, int width, int height, int* seam) {
    // TODO(student): load a vertical seam
/*     INFO(image);
    INFO(start_col);
    INFO(width);
    INFO(height);
    INFO(seam); */

  seam[0] = start_col;
  int col = start_col;
  int seamEngy = energy(image, col, 0, width, height);

  for(int i = 1; i < height; i++){
    if(width <= 1){
      int midEngy = energy(image, col, i, width, height);
      seam[i] = col;
      seamEngy += midEngy;
    }else{
      if(col == 0){
        int midEngy = energy(image, col, i, width, height);
        int rgtEngy = energy(image, col+1, i, width, height);
        if(midEngy <= rgtEngy){
          seam[i] = col;
          seamEngy += midEngy;
        }else{
          col++;
          seam[i] = col;
          seamEngy += rgtEngy;
        }

      }else if(col == width - 1){
        int midEngy = energy(image, col, i, width, height);
        int lftEngy = energy(image, col-1, i, width, height);
        if(midEngy <= lftEngy){
          seam[i] = col;
          seamEngy += midEngy;
        }else{
          col--;
          seam[i] = col;
          seamEngy += lftEngy;
        }

      }else{
        //order of preference
        int midEngy = energy(image, col, i, width, height);
        int rgtEngy = energy(image, col+1, i, width, height);
        int lftEngy = energy(image, col-1, i, width, height);

/*         INFO(midEngy);
        INFO(rgtEngy);
        INFO(lftEngy); */

        if(midEngy <= lftEngy && midEngy <= rgtEngy){
          seam[i] = col;
          seamEngy += midEngy;
        }else if(rgtEngy <= lftEngy){
          col++;
          seam[i] = col;
          seamEngy += rgtEngy;
        }else{
          col--;
          seam[i] = col;
          seamEngy += lftEngy;
        }
      }
    }
  }
  return seamEngy;
}

int loadHorizontalSeam(Pixel** image, int start_row, int width, int height, int* seam) {
    // TODO(student): load a horizontal seam
/*     INFO(image);
    INFO(start_row);
    INFO(width);
    INFO(height);
    INFO(seam); */

  seam[0] = start_row;
  int row = start_row;
  int seamEngy = energy(image, 0, row, width, height);

  for(int i = 1; i < width; i++){
    if(height <= 1){
      int midEngy = energy(image, i, row, width, height);
      seam[i] = row;
      seamEngy += midEngy;
    }else{
      if(row == 0){
        int midEngy = energy(image, i, row, width, height);
        int dwnEngy = energy(image, i, row+1, width, height);
        if(midEngy <= dwnEngy){
          seam[i] = row;
          seamEngy += midEngy;
        }else{
          row++;
          seam[i] = row;
          seamEngy += dwnEngy;
        }

      }else if(row == height - 1){
        int midEngy = energy(image, i, row, width, height);
        int topEngy = energy(image, i, row-1, width, height);
        if(midEngy <= topEngy){
          seam[i] = row;
          seamEngy += midEngy;
        }else{
          row--;
          seam[i] = row;
          seamEngy += topEngy;
        }

      }else{
        //order of preference
        int midEngy = energy(image, i, row, width, height);
        int topEngy = energy(image, i, row-1, width, height);
        int dwnEngy = energy(image, i, row+1, width, height);

/*         INFO(midEngy);
        INFO(dwnEngy);
        INFO(topEngy); */

        if(midEngy <= topEngy && midEngy <= dwnEngy){
          seam[i] = row;
          seamEngy += midEngy;
        }else if(topEngy <= dwnEngy){
          row--;
          seam[i] = row;
          seamEngy += topEngy;
        }else{
          row++;
          seam[i] = row;
          seamEngy += dwnEngy;
        }
      }
    }
  }
  return seamEngy;
}

int* findMinVerticalSeam(Pixel** image, int width, int height) {
    // TODO(student): find min vertical seam
/*     INFO(image);
    INFO(width);
    INFO(height); */
  int *seam = new int[height];
  int *minSeam = new int[height];
  int minEngy = loadVerticalSeam(image, 0, width, height, minSeam);

  for(int i = 1; i < width; i++){
    int seamEngy = loadVerticalSeam(image, i, width, height, seam);
    
    if(seamEngy < minEngy){
      minEngy = seamEngy;
      minSeam = seam;
    }
  }

  deleteSeam(seam);
  return minSeam;
}

int* findMinHorizontalSeam(Pixel** image, int width, int height) {
    // TODO(student): find min horizontal seam
  int *seam = new int[width];
  int *minSeam = new int[width];
  int minEngy = loadHorizontalSeam(image, 0, width, height, minSeam);

  for(int i = 1; i < height; i++){
    int seamEngy = loadHorizontalSeam(image, i, width, height, seam);
    
    if(seamEngy < minEngy){
      minEngy = seamEngy;
      loadHorizontalSeam(image, i, width, height, minSeam);
    }
  }

  deleteSeam(seam);
  return minSeam;
}

void removeVerticalSeam(Pixel** image, int width, int height, int* verticalSeam) {
    // TODO(student): remove a vertical seam
/*     INFO(image);
    INFO(width);
    INFO(height);
    INFO(verticalSeam); */
  
  for(int i = 0; i <= height - 1; i++){
    for(int j = verticalSeam[i]; j <= width - 2; j++){
      image[j][i] = image[j+1][i];
    }
  }
}

void removeHorizontalSeam(Pixel** image, int width, int height, int* horizontalSeam) {
    // TODO(student): remove a horizontal seam
/*     INFO(image);
    INFO(width);
    INFO(height);
    INFO(horizontalSeam); */
  
  for(int i = 0; i <= width - 1; i++){
    for(int j = horizontalSeam[i]; j <= height - 2; j++){
      image[i][j] = image[i][j+1];
    }
  }
}
