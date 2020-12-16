#include <iostream>
#include <chrono>

using namespace std;

void optimized() {
	int AD[2][2];

	for (int j = 500000000; j > 0; j--)
	{
		AD[0][0] += 2;
	}

	AD[1][1] = AD[0][0];

	cout << AD[0][0];
}

int main1() {
	std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();
	optimized();

	cout << "\nExecution time: " << ((chrono::duration<double>) (std::chrono::high_resolution_clock::now() - start)).count() << endl;
	system("pause");
	return 0;
}