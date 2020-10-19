#include "BsplineFactory.h"
#include <algorithm>
BsplineFactory BsplineFactory::_fac;
//
//const std::shared_ptr<bspline::BsplineBase>& BsplineFactory::createBspline(BsplineType type, const shared_ptr<BsplineBase>& model/* = nullptr*/)
//{
//	switch (type)
//	{
//	case Uni:
//		_splines.push_back(make_shared<UniBspline>());
//		break;
//	case Quasi:
//		_splines.push_back(make_shared<QuasiBspline>());
//		break;
//	case Besier:
//		_splines.push_back(make_shared<BezierBspline>());
//		break;
//	case Nuni:
//		_splines.push_back(make_shared<NuniBspline>());
//		break;
//	default:
//		break;
//	}
//	if (_splines.size() != 0)
//	{
//		if (model)
//		{
//			for (int i = 0; i < model->_P.size(); i++)
//			{
//				_splines[_splines.size() - 1]->setPi(model->_P[i]);
//			}
//		}
//		return _splines[_splines.size() - 1];
//	}
//	else
//		return nullptr;
//}

std::shared_ptr<bspline::BsplineBase> BsplineFactory::createBspline(BsplineType type, const int modelind /*= -1*/, const bool storedin /*= true*/)
{
	shared_ptr<BsplineBase> curve;
	switch (type)
	{
	case Uni:
		curve = (make_shared<UniBspline>());
		break;
	case Quasi:
		curve = (make_shared<QuasiBspline>());
		break;
	case Besier:
		curve = (make_shared<BezierBspline>());
		break;
	case Nuni:
		curve = (make_shared<NuniBspline>());
		break;
	default:
		break;
	}
	if (modelind >= 0 && modelind <= _splines.size() - 1)
	{
		for (int i = 0; i < (_splines[modelind]->_P).size(); i++)
		{
			curve->setPi(_splines[modelind]->_P[i]);
		}
	}
	if (storedin && curve)
	{
		_splines.push_back(curve);
	}
	return curve;

}

size_t BsplineFactory::curveNum() const
{
	return _splines.size();
}

size_t BsplineFactory::surfaceNum() const
{
	return _surfaces.size();
}

const std::shared_ptr<bspline::BsplineBase>& BsplineFactory::getCurve(size_t index)
{
	if (index < _splines.size())
		return _splines[index];
	else
		return nullptr;
}

const std::shared_ptr<bspline::BsplineSurface>& BsplineFactory::getSurface(size_t index /*= 0*/)
{
	return _surfaces[index];
}

void BsplineFactory::changeCurveRes(const size_t index, const int res)
{
	_splines[index]->changeCurveRes(res);
}

void BsplineFactory::changeSufaceRes(const size_t index, const int res)
{

}

void BsplineFactory::set3DPoints(const int bit, const double value, const double value2 /*= 0*/)
{
	switch(bit)
	{
	case 1:
		_surface_xy.push_back(make_pair(value, value2));
		break;
	case 3:
		_surface_z.push_back(value);
		break;
	default:
		break;
	}
}

std::vector<std::array<double, 3>> BsplineFactory::get3DPoints() const
{
	vector<array<double, 3>> points;
	for (int i = 0; i < _surface_xy.size(); i++)
	{
		points.push_back(array<double, 3>());
		points[i][0] = _surface_xy[i].first;
		points[i][1] = _surface_xy[i].second;
	}
	for (int i = 0; i < _surface_z.size() && i < points.size(); i++)
	{
		points[i][2] = _surface_z[i];
	}
	return points;
}

bool sortFun(const pair<double, double>& p1, const pair<double, double>& p2)
{
	return p1.first < p2.first;//ÉýÐòÅÅÁÐ  
}

void BsplineFactory::genSuface()
{
	shared_ptr<BsplineSurface> sf = make_shared<BsplineSurface>();
	BsplineSurface& surface = *sf;
	if (_surface_xy.size() % _num_u || _surface_xy.size() != _surface_z.size())
	{
		return;
	}
	sort(_surface_xy.begin(), _surface_xy.end(), sortFun);
	const int pointsnum_per_u = _surface_xy.size() / _num_u;
	for (int i = 0; i < _num_u; i++)
	{
		shared_ptr<BsplineBase>curveu = createBspline(_type_u, -1, false);
		curveu->setK(_k_u);
		curveu->changeCurveRes(_res_u);
		for (int j = 0; j < pointsnum_per_u; j++)
		{
			curveu->setPi(_surface_xy[i * pointsnum_per_u + j].first, _surface_xy[i * pointsnum_per_u + j].second, _surface_z[i * pointsnum_per_u + j]);
		}
		surface.setU(curveu);
	}	
	const int _num_v = surface.getMiddlePointsNum();
	if (!_num_v)
	{
		cout << "need more control points at U direction" << endl;
		return;
	}
	for (int i = 0; i < _num_v; i++)
	{
		shared_ptr<BsplineBase>curvev = createBspline(_type_v, -1, false);
		curvev->setK(_k_v);	
		curvev->changeCurveRes(_res_v);
		surface.setV(curvev);
	}
	surface.updateSurface();
	_surfaces.push_back(sf);
}
