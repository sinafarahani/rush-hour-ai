#pragma once

#include <iostream>
#include <vector>

class parking {
public:
	class Car
	{
	public:
		bool operator==(Car& c) {
			return (c.row[0] == this->row[0] && c.row[1] == this->row[1] && c.col[0] == this->col[0] && c.col[1] == this->col[1] && c.h == this->h);
		}bool operator==(const Car& c) const {
			return (c.row[0] == this->row[0] && c.row[1] == this->row[1] && c.col[0] == this->col[0] && c.col[1] == this->col[1] && c.h == this->h);
		}
		Car& operator--() {
			if (this->h == 'h') {
				this->col[0]--;
				this->col[1]--;
			}
			else if (this->h == 'v') {
				this->row[0]--;
				this->row[1]--;
			}
			return *this;
		}Car& operator++() {
			if (this->h == 'h') {
				this->col[0]++;
				this->col[1]++;
			}
			else if (this->h == 'v') {
				this->row[0]++;
				this->row[1]++;
			}
			return *this;
		}
		int getFixedPos(){
			if (this->h == 'h') {
				return row[0];
			}
			else if (this->h == 'v') {
				return col[0];
			}
		}
		std::pair<int, int> getVarPos() {
			if (this->h == 'h') {
				return std::make_pair(col[0],col[1]);
			}
			else if (this->h == 'v') {
				return std::make_pair(row[0], row[1]);
			}
		}int getFixedPos() const{
			if (this->h == 'h') {
				return row[0];
			}
			else if (this->h == 'v') {
				return col[0];
			}
		}
		std::pair<int, int> getVarPos() const{
			if (this->h == 'h') {
				return std::make_pair(col[0], col[1]);
			}
			else if (this->h == 'v') {
				return std::make_pair(row[0], row[1]);
			}
		}
		int row[2];
		int col[2];
		char h;
	};

	parking(int M, int N, int V);

	int getM() const;
	int getN() const;
	int getV() const;

	std::vector<Car> cars;
private:
	int M;
	int N;
	int V;
	bool** boardPoints;
	std::vector<bool> brdpts;
	std::vector<std::vector<bool>> checkedbrdpts;
};