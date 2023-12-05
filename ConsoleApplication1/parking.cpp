#include "parking.h"

parking::parking(int M, int N, int V)
	:
	M(M),
	N(N),
	V(V)
{
	boardPoints = new bool* [M];
	for (int i = 0; i < M; ++i) {
		boardPoints[i] = new bool[N];
	}
	bool redcar = true;
	for (int i = 0; i < V; i++) {
		int r, c, l;
		char H;
		std::cin >> r >> c >> H >> l;
		Car car;

		car.row[0] = r;
		car.col[0] = c;
		car.h = H;

		if (H == 'v') {
			car.row[1] = r + l - 1;
			car.col[1] = c;
		}
		else if (H == 'h') {
			car.row[1] = r;
			car.col[1] = c + l - 1;
		}
		cars.push_back(car);
	}
}

int parking::getM() const
{
	return M;
}

int parking::getN() const
{
	return N;
}

int parking::getV() const
{
	return V;
}