#pragma once
#include<iostream>
#include <newmat.h>

using namespace NEWMAT;
using namespace std;

namespace bspline
{
	static const double ZERO = 0.000000000001;
	int Cmn(int m, int n);
	int factorial(int n);
	NEWMAT::Real norm(const Matrix& c);
	NEWMAT::Real distance(const ColumnVector& p1, const ColumnVector& p2);
	bool isZero(NEWMAT::Real x);
}