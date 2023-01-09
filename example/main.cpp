#include "iostream"
#include <dataframe.h>

int main()
{
  std::array<std::string, 10> cols{"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
  std::array<std::string, 10> rows{"row1", "row2", "row3", "row4", "row5", "row6", "row7", "row8", "row9", "row10"};

  DataFrame<int, 100, 10, 10> df{cols, rows};

  std::cout << "df.begin: " << df.begin() << "\n";
  std::cout << "df.end: " << df.end() << "\n";

  for (int i = 0; i < 100; i++)
  {
    df[i] = i;
  }

  df.iloc[1, 2] = 111;
  df.iloc[3, 4] = 222;

  *df.begin() = 111;
  *df.end()   = 999;

  std::cout << "df.iloc[9, 9] " << df.iloc[9, 9] << "\n";
  std::cout << "df.iloc[5, 5] " << df.iloc[5, 5] << "\n";

  df.loc["9", "row1"] = 696969;
  df.loc["3", "row2"] = 1000;

  df[9 * 9] = 999;

  std::cout << df[9 * 9] << "\n";

  df.print();

  return 0;
}
