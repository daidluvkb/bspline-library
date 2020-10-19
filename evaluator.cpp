#include "evaluator.h"
int bspline::Cmn(int m, int n)
{
	int result = 0;
	if (!n) result = 1;
	else if (m >= n)
	{
		result = 1;
		for (int i = m; i > n; i--)
		{
			result *= i;
		}
		for (int i = 1; i <= m - n; i++)
		{
			result /= i;
		}
	}
	return result;
}

int bspline::factorial(int n)
{
	if (!n) return 1;
	if (n > 0)
	{
		int result = 1;
		for (int i = 1; i <= n; i++)
		{
			result *= i;
		}
		return result;
	}
	return 0;
}

NEWMAT::Real bspline::norm(const Matrix& c)
{
	Real s = c.SumSquare();
	return sqrt(s);
}

NEWMAT::Real bspline::distance(const ColumnVector& p1, const ColumnVector& p2)
{
	if (p1.size() == p2.size())
	{
		return norm(p1 - p2);
	}
	else return -1;
}

bool bspline::isZero(NEWMAT::Real x)
{
	if(abs(x) < ZERO)
		return true;
	return false;
}

