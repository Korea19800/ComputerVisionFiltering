#include <bits/stdc++.h>
using namespace std;

int main() {
  vector<vector<unsigned char>> grid1; // grid for average
  vector<vector<unsigned char>> grid2; // grid for median
  vector<vector<unsigned char>> grid3; // grid for edge detection

  int w, h, intensity; // width,height,intensity
  string fname, fType; // file name and file type
  cout << "Enter a file name: ";
  cin >> fname;

  // reading file
  ifstream fp(fname);
  if (fp) { // when file exists
    cout << "Reading a file\n";
    while (fp) {
      fp >> fType >> w >> h >> intensity;
      cout << "file's type: " << fType << '\n'
           << "width : " << w << " height: " << h
           << " max intensity: " << intensity << '\n';
      vector<unsigned char> row1(w);  // row for grid1
      vector<unsigned char> row2(w);  // row for grid2
      vector<unsigned char> row3(w);  // row for grid3
      for (int i = 0; i < h; i++) {   // height is row
        for (int j = 0; j < w; j++) { // width is col
          fp >> row1[j];
          row3[j] = row2[j] = row1[j];
        }
        grid1.push_back(row1);
        grid2.push_back(row2);
        grid3.push_back(row3);
      }
    }
  } else { // no proper file
    cout << "There is no proper file\n";
    exit(1);
  }

  // arrays to check 8 directions
  int dx[] = {-1, 1, 0, 0, -1, 1, 1, -1}; // x is height direction(row)
  int dy[] = {0, 0, -1, 1, -1, 1, -1, 1}; // y is width direction(col)
  int maxMag = 0; // variable to find maximum magnitude among grid vector

  for (int i = 0; i < grid1.size(); i++) {
    for (int j = 0; j < grid1[0].size(); j++) { // four corners of the grid
      vector<int> square;                       // 3*3 square
      int DX = 0, DY = 0; // delta x and delta y for edge detection
      
      // first push (i,j) to the square.
      square.push_back((int)grid1[i][j]);
      // checking 8 directions from the cell and push the numbers to the square
      for (int k = 0; k < 8; k++) {
        int nx = i + dx[k]; // x is height direction(row)
        int ny = j + dy[k]; // y is width direction(col)
        // if (nx,ny) are out of the range, check next number
        if (nx < 0 || ny < 0 || nx >= h || ny >= w)
          continue;
        if (ny == j - 1) { // 3 cells left of (i,j)
          DX -= (int)grid1[nx][ny];
        }
        if (ny == j + 1) { // 3 cells right of (i,j)
          DX += (int)grid1[nx][ny];
        }
        if (nx == i - 1) { // 3 cells above (i,j)
          DY += (int)grid1[nx][ny];
        }
        if (nx == i + 1) { // 3 cells below (i,j)
          DY += (int)grid1[nx][ny];
        }
        square.push_back((int)grid1[nx][ny]);
      }

      // 1. average filtering
      int sum = 0;
      for (int x = 0; x < square.size(); x++) {
        sum += square[x];
      }
      int avg = sum / square.size();
      grid1[i][j] = (unsigned char)avg;

      // 2. median filtering
      sort(square.begin(), square.end()); // sort the square
      if (square.size() % 2) { // the number of numbers in square is odd
        grid2[i][j] = (unsigned char)square[square.size() / 2]; // put median
                                                                // num
      } else { // the number of numbers in square is even
        int evenMedian =
            (square[square.size() / 2] + square[square.size() / 2 - 1]) / 2;
        grid2[i][j] = (unsigned char)evenMedian;
      }
      
      // 3. calculating magnitude for edge detection
      int magnitude = sqrt((DX * DX) + (DY * DY));
      maxMag = max(maxMag, magnitude);
      grid3[i][j] = (unsigned char)magnitude;

      /*  if magnitude is over 900 it is an edge, and change pixel to 255
          otherwise change pixel to 0 */
      if (magnitude > 900)
        magnitude = 255;
      else
        magnitude = 0;
      grid3[i][j] = (unsigned char)magnitude;
    }
  }

  // writing to files
  ofstream ofp("average.pgm");
  ofp << "P5" << ' ' << 128 << ' ' << 128 << ' ' << 255 << ' ';
  for (int i = 0; i < grid1.size(); i++) {
    for (int j = 0; j < grid1[0].size(); j++) {
      ofp << grid1[i][j];
    }
  }

  ofstream ofp2("median.pgm");
  ofp2 << "P5" << ' ' << 128 << ' ' << 128 << ' ' << 255 << ' ';
  for (int i = 0; i < grid2.size(); i++) {
    for (int j = 0; j < grid2[0].size(); j++) {
      ofp2 << grid2[i][j];
    }
  }

  ofstream ofp3("edge.pgm");
  ofp3 << "P5" << ' ' << 128 << ' ' << 128 << ' ' << 255 << ' ';
  for (int i = 0; i < grid3.size(); i++) {
    for (int j = 0; j < grid3[0].size(); j++) {
      ofp3 << grid3[i][j];
    }
  }

  return 0;
}
