#include "data_frame.h"
#include "iostream"

int main()
{
  std::array<std::string, 10> cols{"col1", "col2", "col3", "col4", "col5", "col6", "col2", "col3", "col4", "col5"};
  std::array<std::string, 10> rows{"row1", "row2", "row3", "row4", "row5", "row1", "row2", "row3", "row4", "row5"};

  DataFrame<int, 100, 10, 10> df(cols, rows);

  df.iloc[1, 2] = 123123;
  df.iloc[3, 4] = 123123;

  std::cout << df.iloc[9, 9] << "\n";
  std::cout << df.iloc[5, 5] << "\n";

  df.loc["col1", "row1"] = 1000;
  df.loc["col3", "row2"] = 1000;

  df[9 * 9] = 999;

  std::cout << df[9 * 9] << "\n";

  df.print();

  return 0;
}