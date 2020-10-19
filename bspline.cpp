#include "bspline.h"
#include "evaluator.h"
#include <cmath>

bspline::UniBspline::UniBspline(int k, int n, const NEWMAT::Real max, const NEWMAT::Real min)
{
	_n = n;
	_k = k;
	//_t.resize(n + k + 1);
	list<NEWMAT::Real>::iterator iter;
	NEWMAT::Real value = min;
	NEWMAT::Real inc = (max - min) / (n +  k + 1);

	genM();
}

void bspline::UniBspline::setPi(NEWMAT::Real x, NEWMAT::Real y)
{
	BsplineBase::setPi(x, y);
	if (getN() < getK())
	{
		return;
	}
	_u.ReSize(getK() + getN() + 2);
	double inc = 1.0 / (getK() + getN() + 1);
	_u[0] = 0.0;
	for (int i = 1; i < _u.size(); i++)
	{
		_u[i] = _u[i - 1] + inc;
	}
	updateCurve();
}

void bspline::UniBspline::setPi(NEWMAT::Real x, NEWMAT::Real y, NEWMAT::Real z)
{
	BsplineBase::setPi(x, y, z);
	if (getN() < getK())
	{
		return;
	}
	_u.ReSize(getK() + getN() + 2);
	double inc = 1.0 / (getK() + getN() + 1);
	_u[0] = 0.0;
	for (int i = 1; i < _u.size(); i++)
	{
		_u[i] = _u[i - 1] + inc;
	}
	updateCurve();
}

void bspline::UniBspline::setK(const int k)
{
	if (getK()==k)
	{
		return;
	}
	BsplineBase::setK(k);
	genM();
}

bspline::UniBspline::UniBspline(int k /*= 2*/, int n /*= 5*/)
{
	_n = n;
	_k = k;
	genM();
}

NEWMAT::ReturnMatrix bspline::UniBspline::getCurve()
{
	using namespace NEWMAT;
	if (_P.size() < _n + 1)
	{
		Matrix none(0, 0);
		none.release();
		return none;
	}
	return _curvepoints;
}

NEWMAT::ReturnMatrix bspline::UniBspline::getSit(int i, NEWMAT::Real t)
{
	using namespace NEWMAT;
	RowVector T(_k + 1);
	Matrix D(_k + 1, _P[0].size());
	for (int j = 0; j <= _k; j++)
	{
		T[j] = pow(t, j);
		D.row(j + 1) = _P[i - _k + j].t();
	}
	ColumnVector result = (T * M * D).t();
	result.release();
	return result;
}

void bspline::UniBspline::updateCurve()
{
	ColumnVector v(_P[0].size());
	Matrix& result = _curvepoints;
	_curvepoints.resize(_curveres * (getN() - getK()+ 1), _P[0].size());
	Real t = 0, delt = 1.0 / (_curveres - 1);
	int cnt  = 0;
	for (int i = _k; i <= _n; i++)
	{
		t = 0;
		for (int j = 0; j < _curveres; j++)
		{
			v = getSit(i, t);
			result.Row((i  - _k) * _curveres + j + 1) = v.AsRow();
			//result->push_back(v);
			t += delt;
			cnt++;
		}
	}
	//cout << result;
	return ;
}

void bspline::UniBspline::genM()
{
	M.ReSize(_k + 1);
	int K = _k + 1;
	for (int i = 0; i <= K - 1; i++)
	{
		for(int j = 0; j <= K - 1; j++)
		{
			int sum = 0;
			for (int s = j; s <= K - 1; s++)
			{
				sum += pow(-1, s - j) * Cmn(K, s - j) * pow(K - s - 1, K - 1 - i);
			}
			M[i][j] = Cmn(K - 1, K - 1 - i) * sum;
		}
	}
	//std::cout << M;
	M /= factorial(K - 1);
}

bspline::BsplineBase::~BsplineBase()
{

}

bspline::BsplineBase::BsplineBase() : _n(-1), _curvepoints(0, 0), _curveres(3), _basepoints(0, 0)
{

}

void bspline::BsplineBase::setPi(NEWMAT::Real x, NEWMAT::Real y, NEWMAT::Real z)
{
	NEWMAT::ColumnVector p(3);
	p << x << y << z;
	_P.push_back(p);
	_n = _P.size() - 1;
}

void bspline::BsplineBase::setPi(NEWMAT::Real x, NEWMAT::Real y)
{
	NEWMAT::ColumnVector p(2);
	p << x << y;
	_P.push_back(p);
	_n = _P.size() - 1;	
}

void bspline::BsplineBase::setPi(const NEWMAT::ColumnVector& pi)
{
	_P.push_back(pi);
	_n = _P.size() - 1;
}

std::shared_ptr<std::vector<NEWMAT::ColumnVector>> bspline::BsplineBase::getPoints()
{
	shared_ptr<vector<ColumnVector>> pointers = make_shared<vector<ColumnVector>>(_P);
	return pointers;
}

int bspline::BsplineBase::getPi(NEWMAT::Real x, NEWMAT::Real y)
{
	using namespace NEWMAT;
	int index = -1;
	Real d = 10, abs = 0;
	ColumnVector p(2);
	p << x << y;
	for (int i = 0; i < _P.size(); i++)
	{
		abs = norm(p - _P[i]);
		if (abs < d)
		{
			d = abs;
			index = i;
		}
	}
	return index;
}

int bspline::BsplineBase::getPi(NEWMAT::Real x, NEWMAT::Real y, NEWMAT::Real z)
{
	using namespace NEWMAT;
	int index = -1;
	Real d = 10, abs = 0;
	ColumnVector p(2);
	p << x << y << z;
	for (int i = 0; i < _P.size(); i++)
	{
		abs = norm(p - _P[i]);
		if (abs < d)
		{
			d = abs;
			index = i;
		}
	}
	return index;
}

int bspline::BsplineBase::changePi(NEWMAT::Real x, NEWMAT::Real y, int i /*= -1*/)
{
	if (i == -1)
	{
		int ind = getPi(x, y);
		if (ind >= 0 && ind < _P.size())
		{
			_P[ind] << x << y;
		}
		updateCurve();
		return ind;
	}
	if (i >= 0 && i < _P.size())
		_P[i] << x << y;
	updateCurve();
	return -1;
}

int bspline::BsplineBase::changePi(NEWMAT::Real x, NEWMAT::Real y, NEWMAT::Real z, int i /*= -1*/)
{
	if (i == -1)
	{
		int ind = getPi(x, y, z);
		if (ind >= 0 && ind < _P.size())
		{
			_P[ind] << x << y << z;
		}
		updateCurve();
		return ind;
	}
	if (i >= 0 && i < _P.size())
		_P[i] << x << y << z;
	updateCurve();
	return -1;
}

void bspline::BsplineBase::changeCurveRes(int num_of_points /*= 5*/)
{
	_curveres = num_of_points;
	//cout << "parent" << endl;
}


void bspline::BsplineBase::changeOrder(int k)
{

}

int bspline::BsplineBase::getCurveRes() const
{
	return _curveres;
}

void bspline::BsplineBase::setK(const int k)
{
	_k = k;
}

int bspline::BsplineBase::getK() const
{
	return _k;
}

int bspline::BsplineBase::getN() const
{
	return _n;
}

NEWMAT::ReturnMatrix bspline::BsplineBase::getCurve()
{
	return _curvepoints;
}

NEWMAT::ReturnMatrix bspline::BsplineBase::getBase(const int res /*= 10*/) 
{
	const int n = (getN() + 1);
	if (_basepoints.Nrows() == n * res)
	{
		return _basepoints;
	}
	_basepoints.resize(n * res, 2);
	int cnt = 0;
	for (int i = 0; i < n; i++)
	{
		const double inc = (_u[i + _k + 1] - _u[i]) / (res - 1);
		for (int j = 0; j < res; j++)
		{
			cnt++;
			/*if (cnt == _basepoints.nrows())
			{
			cout << ' ';
			}*/
			_basepoints[i * res + j][0] = _u[i] + inc * j;
			_basepoints[i * res + j][1] = getNiku(i, _k, _u[i] + inc * j);
			//cout << _basepoints<< endl;
		}
	}
	//cout << _basepoints;
	return _basepoints;
}


double bspline::BsplineBase::getNiku(const int i, const int k, const double u)
{	
	if (!_u.size())
	{
		return 0.0;
	}
	if (k == 0)
	{
		if (_u[i] <= u && u <= _u[i + 1])
		{
			return 1.0;
		}
		else
		{
			return 0.0;
		}
	}
	double front = 0.0, back = 0.0;
	double multiplier_i = 0.0, multiplier_ip1 = 0.0;
	if ((_u[i + k] - _u[i]) > ZERO)
	{
		multiplier_i = (u - _u[i]) / (_u[i + k] - _u[i]);
		front = getNiku(i, k - 1, u);
	}
	if ((_u[i + k + 1] - _u[i + 1]) > ZERO)
	{
		multiplier_ip1 = (_u[i + k + 1] - u) / (_u[i + k + 1] - _u[i + 1]);
		back = getNiku(i + 1, k - 1, u);
	}
	return front * multiplier_i + back * multiplier_ip1;
}


bspline::NuniBspline::NuniBspline(int k) : _parametric_method(false)
{
	_k = k;
}

void bspline::NuniBspline::updateL()
{
	_l.ReSize(getN());
	for (int i = 0; i < getN(); i++)
	{
		_l[i] = norm(_P[i + 1] - _P[i]);
	}
}

//
//void bspline::NuniBspline::paramRiesenfeld()
//{
//	_u.ReSize(_n - _k);
//	const Real L = _l.sum();
//	Real sum = 0;
//
//	int k = _k;
//	if (!(k % 2))//odd
//	{ 
//		int j = 1;
//		for (; j <= k / 2; j++)
//		{
//			sum += _l(j);
//		}
//		for (int i = 1; i <= _n - _k; i++, j++)
//		{
//			_u(i) = (sum + _l(j + 1) / 2) / L;
//			sum += _l(j + 1) / 2;
//		}
//	}
//	else
//	{
//		int j = 1;
//		for (; j <= k / 2; j++)
//		{
//			sum += _l(j);
//		}
//		for (int i = 1; i <= _n - _k; i++, j++)
//		{
//			_u(i) = (sum + _l(j + 1)) / L;			
//		}
//	}
//}

void bspline::NuniBspline::paramRiesenfeld()
{
	_u.ReSize(getN() + getK() + 2);
	const Real L = _l.sum();
	Real sum = 0;

	for (int i = 0; i < getK() + 1; i++)
	{
		_u[i] = 0.0;
		_u[getN() + getK() + 2 - 1 - i] = 1.0;
	}
	int k = _k;
	if (!(k % 2))//even
	{ 
		int j = 1;
		for (; j <= k / 2; j++)
		{
			sum += _l(j);
		}
		for (int i = 1; i <= _n - _k; i++, j++)
		{
			_u(i + getK() + 1) = (sum += _l(j + 1) / 2) / L;
			sum += _l(j + 1) / 2;
		}
	}
	else//odd
	{
		int j = 1;
		for (; j <= k / 2; j++)
		{
			sum += _l(j);
		}
		for (int i = 1; i <= _n - _k; i++, j++)
		{
			_u(i + getK() + 1) = (sum += _l(j + 1)) / L;		
		}
	}
}

void bspline::NuniBspline::paramHartleyJudd()
{
	_u.ReSize(getN() + getK() + 2);
	//cout << "_l: \n" << _l;
	Real numerator = 0.0;
	Real denominator = 0;
	
	for (int i = 1; i <= _n - _k; i++)
	{
		for (int j = i; j <= _k + i - 1; j++)
		{
			numerator += _l(j);
		}
		_u(i + getK() + 1) = numerator;
	}
	denominator = numerator;

	for (int j = _n - _k + 1; j <= _n; j++)
	{
		denominator += _l(j);
	}
	
	if (!isZero(denominator))
	{
		_u /= denominator;
	}
	for (int i = 0; i < getK() + 1; i++)
	{
		_u[i] = 0.0;
		_u[getN() + getK() + 2 - 1 - i] = 1.0;
	}
	//cout << "_u: \n" << _u;
	//cout << endl;
}


void bspline::NuniBspline::setPi(NEWMAT::Real x, NEWMAT::Real y)
{
	BsplineBase::setPi(x, y);
	if (_l.size() < _n)
	{
		_l.resize_keep(_n);
		_l(_n) = norm(_P[_n] - _P[_n - 1]);
	}
	updateCurve();
}

void bspline::NuniBspline::setPi(NEWMAT::Real x, NEWMAT::Real y, NEWMAT::Real z)
{
	BsplineBase::setPi(x, y, z);
	if (_l.size() < _n)
	{
		_l.resize_keep(_n);
		_l(_n) = norm(_P[_n] - _P[_n - 1]);
	}
	updateCurve();
}

NEWMAT::ReturnMatrix bspline::NuniBspline::getPu(Real u)
{
	int i;
	for (i = 1 + getK() + 1; i <= _u.size() - getK() - 1; i++)
	{
		if (_u(i) > u)
		{
			break;
		}
	}
	return getDlj(_k, i - 1 - getK() - 1, u);
}

NEWMAT::ReturnMatrix bspline::NuniBspline::getDlj(int l, int j, Real u)
{
	if (!l)
	{
		return _P[j];
	}
	//RowVector U(2 * (_k + 1) + _u.size());//n+k+2
	//U = 0;
	//for (int i = _k + 1; i <= _n; i++)
	//{
	//	U[i] = _u(i - _k);
	//}
	//for (int i = _n + 1; i <= _n + _k + 1; i++)
	//{
	//	U[i] = 1;
	//}
	Real alpha = 0;
	alpha = u - _u[j+l];
	alpha /= (_u[j + _k + 1] - _u[j + l]);
	ColumnVector result = _P[0];
	result = (1 - alpha) * getDlj(l - 1, j, u) + alpha * getDlj(l - 1, j + 1, u);
	result.release();
	return result;
}

NEWMAT::ReturnMatrix bspline::NuniBspline::getCurve()
{

	if (_P.size() < (_n + 1) || (int)_P.size() - 1 < _k + 1)
	{
		Matrix none(0, 0);
		none.release();
		return none;
	}

	Matrix results(_curvepoints);
	results.release();
	return results;

}

void bspline::NuniBspline::changeOrder(int k)
{

}

void bspline::NuniBspline::changeCurveRes(int num_of_points /*= 5*/)
{
	if (num_of_points == _curveres)
	{
		return;
	}
	BsplineBase::changeCurveRes(num_of_points);
	if (_P.size() < (_n + 1) || (int)_P.size() - 1 < _k + 1)
	{
		_curvepoints.ReSize(0, 0);
		return;
	}
	if (_parametric_method)
		paramHartleyJudd();
	else
		paramRiesenfeld();
	//cout << "u: " << _u << endl;
	Real left = 0, right = 0, delta = 0, u = 0;
	int resultn = (_u.size() + 1 - 2 * getK() - 2) * num_of_points + 1;
	Matrix& result = _curvepoints;
	result.ReSize(resultn, _P[0].Nrows());
	//cout << result;
	ColumnVector pu = _P[0];
	int rowcnt = 1;
	for (int i = 1; i <= _u.size() - 2 * getK() - 2; i++)
	{
		left = right;
		right = _u(i + getK() + 1);
		delta = (right - left) / num_of_points;
		u = left;
		for(int j = 1; j <= num_of_points; j++)
		{			
			pu = getPu(u);
			u += delta;
			result.Row(rowcnt++) = pu.AsRow();
		}
	}
	left = right;
	right = 1;
	delta = (right - left) / num_of_points;
	u = left;
	for(int j = 1; j <= num_of_points + 1; j++)//包含右端点
	{			
		pu = getPu(u);
		u += delta;
		result.Row(resultn - num_of_points - 1 + j) = pu.AsRow();
	}
}

void bspline::NuniBspline::updateCurve()
{
	if (_P.size() < (_n + 1) || (int)_P.size() - 1 < _k + 1)
	{
		_curvepoints.ReSize(0, 0);
		return;
	}
		//cout << "test"<<endl;
	if (_parametric_method)
		paramHartleyJudd();
	else
		paramRiesenfeld();
	//cout << "u: " << _u << endl;
	Real left = 0, right = 0, delta = 0, u = 0;
	int resultn = (_u.size() + 1 - 2 * getK() - 2) * _curveres + 1;
	Matrix& result = _curvepoints;
	result.ReSize(resultn, _P[0].Nrows());
	//cout << result;
	ColumnVector pu = _P[0];
	int rowcnt = 1;
	for (int i = 1; i <= _u.size() - 2 * getK() - 2; i++)
	{
		left = right;
		right = _u(i + getK() + 1);
		delta = (right - left) / _curveres;
		u = left;
		for(int j = 1; j <= _curveres; j++)
		{			
			pu = getPu(u);
			u += delta;
			result.Row(rowcnt++) = pu.AsRow();
		}
	}
	left = right;
	right = 1;
	delta = (right - left) / _curveres;
	u = left;
	for(int j = 1; j <= _curveres + 1; j++)//包含右端点
	{			
		pu = getPu(u);
		u += delta;
		result.Row(resultn - _curveres - 1 + j) = pu.AsRow();
	}
}

int bspline::NuniBspline::changePi(NEWMAT::Real x, NEWMAT::Real y, int i /*= -1*/)
{
	return BsplineBase::changePi(x, y, -1);
}

void bspline::NuniBspline::recalculate()
{
	if (_P.size() < (_n + 1) || (int)_P.size() - 1 < _k + 1)
	{
		_curvepoints.ReSize(0, 0);
		return;
	}
	if (_parametric_method)
		paramHartleyJudd();
	else
		paramRiesenfeld();
	//cout << "u: " << _u << endl;
	Real left = 0, right = 0, delta = 0, u = 0;
	int resultn = (_u.size() + 1 - 2 * getK() - 2) * _curveres + 1;
	Matrix& result = _curvepoints;
	result.ReSize(resultn, _P[0].Nrows());
	//cout << result;
	ColumnVector pu = _P[0];
	int rowcnt = 1;
	for (int i = 1; i <= _u.size() - 2 * getK() - 2; i++)
	{
		left = right;
		right = _u(i + getK() + 1);
		delta = (right - left) / _curveres;
		u = left;
		for(int j = 1; j <= _curveres; j++)
		{			
			pu = getPu(u);
			u += delta;
			result.Row(rowcnt++) = pu.AsRow();
		}
	}
	left = right;
	right = 1;
	delta = (right - left) / _curveres;
	u = left;
	for(int j = 1; j <= _curveres + 1; j++)//包含右端点
	{			
		pu = getPu(u);
		u += delta;
		result.Row(resultn - _curveres - 1 + j) = pu.AsRow();
	}
}

double bspline::NuniBspline::getNiku(const int i, const int k, const double u)
{
	if (!_u.size())
	{
		return 0.0;
	}
	if (k == 0)
	{
		if (_u[i] <= u && u <= _u[i + 1])
		{
			return 1.0;
		}
		else
		{
			return 0.0;
		}
	}
	double front = 0.0, back = 0.0;
	double multiplier_i = 0.0, multiplier_ip1 = 0.0;
	if ((_u[i + k] - _u[i]) > ZERO)
	{
		multiplier_i = (u - _u[i]) / (_u[i + k] - _u[i]);
		front = getNiku(i, k - 1, u);
	}
	if ((_u[i + k + 1] - _u[i + 1]) > ZERO)
	{
		multiplier_ip1 = (_u[i + k + 1] - u) / (_u[i + k + 1] - _u[i + 1]);
		back = getNiku(i + 1, k - 1, u);
	}
	return front * multiplier_i + back * multiplier_ip1;
}

void bspline::NuniBspline::setParametricMethod(bool hartleyjudd)
{
	_parametric_method = hartleyjudd;

}

bool bspline::NuniBspline::getParametricMethod() const
{
	return _parametric_method;
}

void bspline::NuniBspline::updateN()
{

}

double bspline::BernsteinBase::getBjnt(const int j, const int n, const double t)
{
	if (j >= 0 && j <= n)
		return Cmn(n, j) * pow(t, j) * pow(1 - t, n - j);
	else
		return 0.0;
}

NEWMAT::ReturnMatrix bspline::BezierCurve::getbjk(const int j, const int k, const double t)
{
	if (!k)
	{
		ColumnVector b(3);
		b = _P[j];
		b.Release();
		return b;
	}
	ColumnVector b(3);
	b = (1 - t) * getbjk(j, k - 1, t) + t * getbjk(j + 1, k - 1, t);
	b.Release();
	return b;
}

NEWMAT::ReturnMatrix bspline::BezierCurve::getPt(const double t)
{
	ColumnVector b(3);
	b = getbjk(0, getN(), t);
	b.Release();
	return b;
}

void bspline::BezierCurve::updateCurve()
{

}

bspline::BezierBspline::BezierBspline(const int k)
{
	setK(k);
	BezierCurve seg;
	seg.setK(_k);
	_segments.push_back(make_shared<BezierCurve>(seg));
}

void bspline::BezierBspline::setK(const int k)
{
	BsplineBase::setK(k);
	_u.resize(2 * k + 2);
	_u = 1.0;
	for (int i = 0; i < k + 1; i++)
	{
		_u[i] = 0.0;
	}
}

void bspline::BezierBspline::setPi(NEWMAT::Real x, NEWMAT::Real y)
{
	if ((_n % _k) == 0 && _P.size() >= 2)
	{
		shared_ptr<BezierCurve> seg = make_shared<BezierCurve>();
		seg->setK(_k);
		seg->setPi(_P[_P.size() - 1]);
		_segments.push_back(seg);
		seg->setPi(x, y);
	}
	else
	{
		_segments[_segments.size() - 1]->setPi(x, y);
	}
	BsplineBase::setPi(x, y);
	updateCurve();
}

void bspline::BezierBspline::setPi(NEWMAT::Real x, NEWMAT::Real y, NEWMAT::Real z)
{
	if ((_n % _k) == 0 && _P.size() >= 2)
	{
		shared_ptr<BezierCurve> seg = make_shared<BezierCurve>();
		seg->setK(_k);
		seg->setPi(_P[_P.size() - 1]);
		_segments.push_back(seg);
		seg->setPi(x, y, z);
	}
	else
	{
		_segments[_segments.size() - 1]->setPi(x, y, z);
	}
	BsplineBase::setPi(x, y, z);
	updateCurve();
}

int bspline::BezierBspline::changePi(NEWMAT::Real x, NEWMAT::Real y, int i /*= -1*/)
{
	int ind = BsplineBase::changePi(x, y);
	if (ind >= 0 && ind < _P.size())
	{
		int segind = ind / getK();
		int subind = ind % getK();
		if (segind < _segments.size())
		{
			_segments[segind]->changePi(x, y, subind);
		}
		if (subind == 0 && segind > 0)
		{
			_segments[segind - 1]->changePi(x, y, getK());
		}
	}
	return ind;
}

NEWMAT::ReturnMatrix bspline::BezierBspline::getCurve()
{		

	Matrix curve(_curvepoints);
	curve.release();
	return curve;

}

void bspline::BezierBspline::changeCurveRes(int num_of_points /*= 5*/)
{
	if (num_of_points == _curveres)
	{
		return;
	}
	BsplineBase::changeCurveRes(num_of_points);
	//cout << "bezierchangeres: " << num_of_points << endl;
	if (_P.size() < 2 || getN() % getK() )
	{
		//_curvepoints.ReSize(0, 0);		
		return ;
	}
	int N = _segments.size() * num_of_points;

	_curvepoints.ReSize(N + 1, _P[0].size());
	double delta = 1.0 / num_of_points;
	for (int i = 0; i < _segments.size(); i++)
	{
		for (int j = 0; j < num_of_points; j++)
		{
			_curvepoints.Row(i * num_of_points + j + 1) = _segments[i]->getPt(delta * j).AsRow();
		}
	}
	_curvepoints.Row(N + 1) = _P[_n].AsRow();
}

void bspline::BezierBspline::updateCurveSeg(int segind, int num_of_points)
{
	double delta = 1.0 / num_of_points;
	for (int j = 0; j < num_of_points; j++)
	{
		_curvepoints.Row(segind * num_of_points + j + 1) = _segments[segind]->getPt(delta * j).AsRow();
	}
}

double bspline::BezierBspline::getNiku(const int i, const int k, const double u)
{
	return BernsteinBase::getBjnt(i, k, u);
}

void bspline::BezierBspline::updateCurve()
{
	if (_P.size() < 2 || getN() % getK() )
	{
		//_curvepoints.ReSize(0, 0);		
		return ;
	}
	int N = _segments.size() * _curveres;

	_curvepoints.ReSize(N + 1, _P[0].size());
	double delta = 1.0 / _curveres;
	for (int i = 0; i < _segments.size(); i++)
	{
		for (int j = 0; j < _curveres; j++)
		{
			_curvepoints.Row(i * _curveres + j + 1) = _segments[i]->getPt(delta * j).AsRow();
		}
	}
	_curvepoints.Row(N + 1) = _P[_n].AsRow();
}

void bspline::QuasiBspline::setPi(NEWMAT::Real x, NEWMAT::Real y)
{
	BsplineBase::setPi(x, y);
	if (getN() < getK())
		return;
	_u.resize_keep(getK() + getN() + 2);
	double inc = 1.0 / (getN() + 1 - getK());
	for (int i = getK() + 1; i < getN() + 1; i++)
	{
		_u[i] = _u[i - 1] + inc;
	}
	for (int i = getN() + 1; i <= getN() + getK() + 1; i++)
	{
		_u[i] = 1.0;
	}
	updateCurve();
}

void bspline::QuasiBspline::setPi(NEWMAT::Real x, NEWMAT::Real y, NEWMAT::Real z)
{
	BsplineBase::setPi(x, y, z);
	if (getN() < getK())
		return;
	_u.resize_keep(getK() + getN() + 2);
	double inc = 1.0 / (getN() + 1 - getK());
	for (int i = getK() + 1; i < getN() + 1; i++)
	{
		_u[i] = _u[i - 1] + inc;
	}
	for (int i = getN() + 1; i <= getN() + getK() + 1; i++)
	{
		_u[i] = 1.0;
	}
	updateCurve();
}

void bspline::QuasiBspline::setK(const int k)
{
	if (k >= 3)
	{
		BsplineBase::setK(3);
	}
	else if (k <= 2)
	{
		BsplineBase::setK(2);
	}
}

NEWMAT::ReturnMatrix bspline::QuasiBspline::getcit(const int i, const double t)
{
	RowVector T (getK() + 1);
	for (int j = 0; j < T.size(); j++)
	{
		T(j + 1) = pow(t, j);
	}
	Matrix D(getK() + 1, _P[0].size());
	for (int j = 0 ; j < getK() + 1; j++)
	{
		D.Row(j + 1) = _P[i - getK() + j].AsRow();
	}
	RowVector v = (T * getMni(i) * D);
	v.release();
	return v;
}

NEWMAT::ReturnMatrix bspline::QuasiBspline::getMni(const int i) const
{
	Matrix M;
	if (getK() == 2)
	{
		M.ReSize(3, 3);
		if (getN() == 2)
		{
			M << 1.0 << 0.0 << 0.0
				<< -2.0 << 2.0 << 0.0
				<< 1.0 << -2.0 << 1.0;
		}
		else
		{
			if (getN() > 3 && 3 <= i && i <= getN() - 1)
			{
				M << 0.5 << 0.5 << 0.0
					<< -1.0 << 1.0 << 0.0
					<< 0.5 << -1.0 << 0.5;
			}
			else if (getN() >= 3)
			{
				if (i == 2)
				{
					M << 1.0 << 0.0 << 0.0
						<< -2.0 << 2.0 << 0.0
						<< 1.0 << -1.5 << 0.5;
				}
				else if(i == getN())
				{
					M << 0.5 << 0.5 << 0.0
						<< -1.0 << 1.0 << 0.0
						<< 0.5 << -1.5 << 1.0;
				}
			}

		}
	}
	else if(getK() == 3)
	{
		M.ReSize(4, 4);
		if (getN() == 3)
		{
			if (i == 3)
				M << 1.0 << 0.0 << 0.0 << 0.0
				<< -3.0 << 3.0 << 0.0 << 0.0
				<< 3.0 << -6.0 << 3.0 << 0.0
				<< -1.0 << 3.0 << -3.0 << 1.0;
		}
		else if (getN() == 4)
		{
			if (i == 3)
			{
				M << 1.0 << 0.0 << 0.0 << 0.0
					<< -3.0 << 3.0 << 0.0 << 0.0
					<< 3.0 << -4.5 << 1.5 << 0.0
					<< -1.0 << 1.75 << -1.0 << 0.25;
			}
			else if (1 == 4)
			{
				M << 0.25 << 0.5 << 0.25 << 0.0
					<< -0.75 << 0.0 << 0.75 << 0.0
					<< 0.75 << -1.5 << 0.75 << 0.0
					<< -0.25 << 1.0 << -1.75 << 1.0;
			}
		}
		else if (getN() == 5 || (getN() >= 6 && (i == 3 || i == getN())))
		{
			if (i == 3)
			{
				M << 1.0 << 0.0 << 0.0 << 0.0
					<< -3.0 << 3.0 << 0.0 << 0.0
					<< 3.0 << -4.5 << 1.5 << 0.0
					<< -1.0 << 1.75 << -11.0 / 12 << 1.0 / 6;
			}
			else if(getN() == 5 && i == 4)
			{
				M << 0.25 << 7.0 / 12 << 1.0 / 6 << 0.0
					<< -0.75 << 0.25 << 0.5 << 0.0
					<< 0.75 << -1.25 << 0.5 << 0.0
					<< -1.0 << 7.0 / 12 << -7.0 / 12 << 0.25;
			}
			else if ((getN() == 5 && i == 5) || (getN() >= 6 && i == getN()))
			{
				M << 1.0 / 6 << 7.0 / 12 << 0.25 << 0.0
					<< -0.5 << -0.25 << 0.75 << 0.0
					<< 0.75 << -1.25 << 0.75 << 0.0
					<< -0.25 << 11.0 / 12 << -1.75 << 1.0;
			}
		}
		else if (getN() > 6 && 5 <= i && i <= getN() - 2)
		{
			M << 1.0 / 6 << 2.0 / 3 << 1.0 / 6 << 0.0
				<< -0.5 << 0.0 << 0.5 << 0.0
				<< 0.5 << -1.0 << 0.5 << 0.0
				<< -1.0 / 6 << 0.5 << -0.5 << 1.0 / 6;
		}
		else if (getN() >= 6)
		{
			if (i == 4)
				M << 1.0 / 6 << 2.0 / 3 << 1.0 / 6 << 0.0
				<< -0.5 << 0.0 << 0.5 << 0.0
				<< 0.5 << -1.0 << 0.5 << 0.0
				<< -0.25 << 0.5 << -7.0 / 12 << 0.25;
			else if (i == getN() - 1)
				M << 1.0 / 6 << 2.0 / 3 << 1.0 / 6 << 0.0
				<< -0.5 << 0.0 << 0.5 << 0.0
				<< 0.5 << -1.0 << 0.5 << 0.0
				<< -0.25 << 0.5 << -7.0 / 12 << 0.25;
		}
	}
	M.release();
	return M;
}

void bspline::QuasiBspline::updateCurve()
{
	const double inc = 1.0 / (_curveres - 1);
	_curvepoints.resize(_curveres * (getN() - getK() + 1), _P[0].size());
	for (int i = getK(); i <= getN(); i++)
	{
		double t = 0.0;
		for (int j = 0; j < _curveres; j++)
		{
			Matrix tst = getcit(i, t);
			_curvepoints.Row(((i - getK()) * _curveres) + j + 1) = getcit(i, t);
			t += inc;
		}
	}
}

NEWMAT::ReturnMatrix bspline::BsplineSurface::getSurface()
{
	return _surfacepoints;
}

void bspline::BsplineSurface::updateSurface()
{
	if (_u.empty())
	{
		return;
	}
	for (int i = 0 ; i < _v.size(); i++)
	{
		for (int j = 0; j < _u.size(); j++)
		{
			Matrix m = _u[j]->getCurve();
			RowVector cv = m.Row(i + 1);
			_v[i]->setPi(cv(1), cv(2), cv(3));
		}
	}
	Matrix tmp = _v[0]->getCurve();
	_surfacepoints.ReSize(tmp.nrows() * _v.size(), 3);
	for (int i = 0 ; i < _v.size(); i++)
	{
		_surfacepoints.Rows(i * tmp.nrows() + 1, (i + 1) * tmp.nrows()) = _v[i]->getCurve() ;
	}
	//cout << _surfacepoints;
}

int bspline::BsplineSurface::getMiddlePointsNum()
{
	Matrix m = _u[0]->getCurve();
	return m.nrows();
}

const std::shared_ptr<bspline::BsplineBase>& bspline::BsplineSurface::getUi(size_t index) const
{
	if (index >= _u.size())
	{
		return nullptr;
	}
	return _u[index];
}
