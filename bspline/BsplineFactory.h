#ifndef BSPL
#define BSPL 1
#include "bspline.h"
#include <array>
using namespace std;
using namespace bspline;
class BsplineFactory
{
private:
	BsplineFactory(){}
	BsplineFactory(const BsplineFactory& otherfac){}
	const BsplineFactory& operator=(const BsplineFactory&){}
	~BsplineFactory(){}
	static BsplineFactory _fac;
public:
	static BsplineFactory& getFactory(){ return _fac;}
	enum BsplineType
	{
		Uni,
		Quasi,
		Besier,
		Nuni
	};
//	const shared_ptr<BsplineBase>& createBspline(BsplineType type, const shared_ptr<BsplineBase>& model = nullptr);
	shared_ptr<BsplineBase> createBspline(BsplineType type, const int modelind = -1, const bool storedin = true);
	size_t curveNum() const;
	size_t surfaceNum() const;
	const shared_ptr<BsplineBase>& getCurve(size_t index);
	const shared_ptr<BsplineSurface>& getSurface(size_t index = 0);
	void changeCurveRes(const size_t index, const int res);
	void changeSufaceRes(const size_t index, const int res);
	void set3DPoints(const int bit, const double value, const double value2 = 0); //1.x; 2.y; 3.z
	vector<array<double, 3>> get3DPoints()const;
	void genSuface();

	void setTypeU(BsplineType type){_type_u = type;}
	void setTypeV(BsplineType type){_type_v = type;}
	void setKU(const int k){_k_u = k;}
	void setKV(const int k){_k_v = k;}
	void setResU(const int res){_res_u = res;}
	void setResV(const int res){_res_v = res;}
	void setCurveNumU(const int n){_num_u = n;}
	//const shared_ptr<BsplineBase>& createBspline(BsplineType type, const shared_ptr<BsplineBase>& model);
private:
	BsplineType _type_u;
	BsplineType _type_v;
	int _k_u;
	int _k_v;
	int _res_u;
	int _res_v;
	int _num_u;
	vector<pair<double, double>> _surface_xy;
	vector<double> _surface_z;
	vector<shared_ptr<BsplineBase>>_splines;
	vector<shared_ptr<BsplineSurface>>_surfaces;
};
#endif
