#include "data_frame.h"
#include "iostream"

int main()
{
  std::array<std::string, 10> cols{"col1", "col2", "col3", "col4", "col5", "col6", "col2", "col3", "col4", "col5"};
  std::array<std::string, 10> rows{"row1", "row2", "row3", "row4", "row5", "row1", "row2", "row3", "row4", "row5"};

  DataFrame<int, 100, 10, 10> df(cols, rows);

  df[9, 9] = 666;
  df[5, 5] = 55;
  std::cout << df[9 * 9] << "\n";

  df.print();

  return 0;
}