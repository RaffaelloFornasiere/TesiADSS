#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <math.h>
#include <iomanip>

int main()
{
    for(size_t j = 0; j < 7; j++)
    {
        std::vector<std::pair<size_t, int>> fitVect(pow(10, j));
        for(size_t i = 0; i < fitVect.size(); i++)
            fitVect[i] = std::make_pair(rand()%100, i);

        time_t start = clock();
        std::sort(fitVect.begin(), fitVect.end(),
                  [](std::pair<size_t, int> p1, std::pair<size_t, int> p2)
        {
            return p1.first > p2.first;
        });
        std::cout << "N: " << std::setw(20) << pow(4, j) << " time: " <<clock() - start << std::endl;
    }
    return 0;
}
