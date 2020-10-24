#pragma once
#include <list>
#include <iostream>
#include <memory>
#include <vector>
#include <newmat.h>
#include <newmatio.h>

using namespace std;

namespace bspline
{
	//typedef double Real;


	class BsplineBase
	{
	public:
		virtual ~BsplineBase();
		BsplineBase();
	//protected:
		virtual void setPi(NEWMAT::Real x, NEWMAT::Real y, NEWMAT::Real z);
		virtual void setPi(NEWMAT::Real x, NEWMAT::Real y);
		virtual void setPi(const NEWMAT::ColumnVector& pi);
		virtual shared_ptr<vector<NEWMAT::ColumnVector>> getPoints();
		virtual int getPi(NEWMAT::Real x, NEWMAT::Real y);
		virtual int getPi(NEWMAT::Real x, NEWMAT::Real y, NEWMAT::Real z);
		virtual int changePi(NEWMAT::Real x, NEWMAT::Real y, int i = -1);
		virtual int changePi(NEWMAT::Real x, NEWMAT::Real y, NEWMAT::Real z, int i);
		virtual void changeCurveRes(int num_of_points = 5);
		virtual void updateCurve() = 0;
		virtual void changeOrder(int k);
		int getCurveRes() const;
		virtual void setK(const int k);
		virtual int getK() const;
		virtual int getN() const;
		virtual NEWMAT::ReturnMatrix getCurve();
		virtual NEWMAT::ReturnMatrix getBase(const int res = 10); 
		virtual double getNiku(const int i, const int k, const double u);

		//double getNik(int i, int k, double t);
	protected:
		NEWMAT::Matrix _curvepoints;
		NEWMAT::Matrix _basepoints;
	protected:
		int _k;
		int _n;	
		int _curveres;
		NEWMAT::RowVector _u;
	public:
		vector<NEWMAT::ColumnVector> _P;
	};

	class UniBspline : public BsplineBase
	{
	public:
		UniBspline(int k = 2, int n = -1);
		UniBspline(int k, int n, const NEWMAT::Real max, const NEWMAT::Real min);
		virtual void setPi(NEWMAT::Real x, NEWMAT::Real y) override;
		virtual void setPi(NEWMAT::Real x, NEWMAT::Real y, NEWMAT::Real z) override;
		virtual void setK(const int k) override;
		virtual NEWMAT::ReturnMatrix getCurve() override;
		NEWMAT::ReturnMatrix getSit(int i, NEWMAT::Real t);
		virtual void updateCurve() override;
	private:
		//list<NEWMAT::Real> _t;
		void genM();
	public:
		NEWMAT::SquareMatrix M;
	};

	class QuasiBspline : public BsplineBase
	{
		virtual void setPi(NEWMAT::Real x, NEWMAT::Real y) override;
		virtual void setPi(NEWMAT::Real x, NEWMAT::Real y, NEWMAT::Real z) override;
		virtual void setK(const int k) override;
		NEWMAT::ReturnMatrix getcit(const int i, const double t);
		NEWMAT::ReturnMatrix getMni(const int i) const;
		virtual void updateCurve() override;
	};

	class BernsteinBase
	{
	public:
		static double getBjnt(const int j, const int  n, const double t);
	};

	class BezierCurve : public BsplineBase
	{
	public:
		virtual ~BezierCurve(){}
		NEWMAT::ReturnMatrix getbjk(const int j, const int k, const double t);
		NEWMAT::ReturnMatrix getPt(const double t);
		virtual void updateCurve();
	};

	class BezierBspline : public BsplineBase
	{
	public:
		virtual ~BezierBspline(){}
		BezierBspline(const int k = 1);
		virtual void setK(const int k) override;
		virtual void setPi(NEWMAT::Real x, NEWMAT::Real y) override;
		virtual void setPi(NEWMAT::Real x, NEWMAT::Real y, NEWMAT::Real z) override;
		virtual int changePi(NEWMAT::Real x, NEWMAT::Real y, int i = -1) override;
		virtual NEWMAT::ReturnMatrix getCurve() override;
		virtual void changeCurveRes(int num_of_points = 5) override;
		virtual void updateCurveSeg(int segind, int num_of_points);
		virtual double getNiku(const int i, const int k, const double u) override;
		virtual void updateCurve()override;
	private:
		vector<shared_ptr<BezierCurve>> _segments;
	};
	
	class NuniBspline : public BsplineBase
	{
	public:
		//NuniBspline();
		NuniBspline(int k = 3);
		virtual void setPi(NEWMAT::Real x, NEWMAT::Real y) override;
		virtual void setPi(NEWMAT::Real x, NEWMAT::Real y, NEWMAT::Real z) override;
		virtual NEWMAT::ReturnMatrix getPu(NEWMAT::Real u);
		NEWMAT::ReturnMatrix getDlj(int l, int j, NEWMAT::Real u);
		virtual NEWMAT::ReturnMatrix getCurve() override;
		virtual void changeOrder(int k) override;
		virtual void changeCurveRes(int num_of_points = 5) override;
		virtual void updateCurve() override;
		virtual int changePi(NEWMAT::Real x, NEWMAT::Real y, int i = -1) override;
		virtual void recalculate();
		virtual double getNiku(const int i, const int k, const double u) override;
		void setParametricMethod(bool hartleyjudd);
		bool getParametricMethod() const;
		void reset();

	private:
		void updateL();
		void paramRiesenfeld();
		void paramHartleyJudd();	
		void updateN();
	private:
		NEWMAT::ColumnVector _l;
		bool _parametric_method;
	};
	class BsplineSurface
	{
	public:
		void setU(const shared_ptr<BsplineBase>& u){_u.push_back(u);}
		void setV(const shared_ptr<BsplineBase>& v){_v.push_back(v);}
		NEWMAT::ReturnMatrix getSurface();
		void updateSurface();
		int getMiddlePointsNum();
		int getUSize(){return _u.size();}
		const shared_ptr<BsplineBase>& getUi(size_t index) const;
	private:
		vector<shared_ptr<BsplineBase>> _u;
		vector<shared_ptr<BsplineBase>> _v;
		NEWMAT::Matrix _surfacepoints;
	};
}
