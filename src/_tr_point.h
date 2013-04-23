//---------------------------------------------------------------------------

#ifndef _tr_pointH
#define _tr_pointH
//---------------------------------------------------------------------------

#include <vcl.h>

namespace triangulation {

class point
{
	public:
		point();
		point(double X, double Y);
		point(const triangulation::point &p);
		void incCounter();
		int getCounterValue();

		void paint(TImage *img);

		double length(const triangulation::point &p);

		void operator += (const triangulation::point &p);
		const triangulation::point operator / (const int n);
		void operator = (const triangulation::point &p);
		bool operator == (const triangulation::point &p);
		bool operator != (const triangulation::point &p);

		double X,Y;
	private:
		int m_nCounter;
};

}; // triangulation
#endif
