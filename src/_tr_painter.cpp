//---------------------------------------------------------------------------

#pragma hdrstop

#include "_tr_painter.h"

//---------------------------------------------------------------------------

painter::painter(TImage *img, triangulation::logger *pLogger)
:
	m_img(img),
	m_pLogger(pLogger),
	m_pTriangulator(new triangulation::triangulator(m_pLogger))
{

};
//---------------------------------------------------------------------------

void painter::SaveAreas(UnicodeString filename)
{
	TStringList *pList = new TStringList();

	for(unsigned int i = 0; i < m_pTriangulator->getAreas().size(); i++)
	{
		UnicodeString str = "area:[";

		triangulation::area ar = m_pTriangulator->getAreas()[i];

		str += IntToStr((int)ar.getCountTriangles()) + ",";

		for(unsigned int pi = 0; pi < ar.count(); pi++)
		{
			str += IntToStr((int)ar.getPoint(pi).X) + ",";
			str += IntToStr((int)ar.getPoint(pi).Y);
			if(pi + 1 < ar.count())
				str += ",";
		};
		str += "];";
		pList->Add(str);
	};

	pList->SaveToFile(filename);
};

//---------------------------------------------------------------------------


void split(TStringList* lout, wchar_t* str, const wchar_t* separator) {
	  for(wchar_t* tok = wcstok(str, separator); tok; tok = wcstok(NULL, separator))
		  lout->Add(tok);
}

//---------------------------------------------------------------------------

void painter::LoadAreas(UnicodeString filename)
{
	clear();

	TStringList *pList = new TStringList();
	pList->LoadFromFile(filename);

	for(int i = 0; i < pList->Count; i++)
	{
		UnicodeString str = pList->Strings[i];

		if(str.Pos("area:[") == 1)
		{
			str.Delete(1, str.Pos("["));
			str = str.SubString(1,str.Pos("]")-1);
			TStringList *pList2 = new TStringList();
			split( pList2, str.c_str(), L",");

			triangulation::area ar;
			ar.setCountTriangles(StrToInt(pList2->Strings[0]));

			for(int si = 1; si < pList2->Count; si += 2)
				ar.addPoint(StrToInt(pList2->Strings[si]), StrToInt(pList2->Strings[si+1]));
			m_pTriangulator->addArea(ar);
		};
	};
	refresh();
};

//---------------------------------------------------------------------------

void painter::clear()
{
	bufferArea.clear();
	m_pTriangulator->clear_triangles();
	m_pTriangulator->clear_areas();
};

//---------------------------------------------------------------------------

void painter::clear_buffer()
{
	bufferArea.clear();
};

//---------------------------------------------------------------------------

bool painter::hasAreas()
{
	return m_pTriangulator->getAreas().size() > 0;
};
//---------------------------------------------------------------------------

void painter::addToBufferArea(int X, int Y)
{
  bufferArea.addPoint(X,Y);
};
//---------------------------------------------------------------------------

void painter::applyBufferArea()
{
	if( bufferArea.count() < 4 )
	{
	   ShowMessage("Area must have points > 3");
	   return;
	};

	int nValue = 1;
	UnicodeString strValue;
	bool bCorrect = false;
	while(!bCorrect)
	{
		bCorrect = true;
		if(!InputQuery("Input Count Triangles", "Value", strValue))
			return;

		if(!TryStrToInt(strValue, nValue))
		{
			ShowMessage("It's not integer. Try again.");
			bCorrect = false;
			continue;
		};

		if(nValue < bufferArea.count())
		{
			ShowMessage("Value must be >= " + IntToStr(bufferArea.count()));
			bCorrect = false;
			continue;
		};
	}

	m_img->Cursor = crDefault;
	bufferArea.setCountTriangles(nValue);
	m_pTriangulator->addArea(bufferArea);
//	addArea(bufferArea);
	bufferArea.clear();
};

//---------------------------------------------------------------------------

void painter::refresh()
{
	m_img->Canvas->Rectangle( 0, 0, m_img->Width, m_img->Height );


	for(unsigned int i=0; i < m_pTriangulator->getAreas().size(); i++)
	   m_pTriangulator->getAreas()[i].paint(m_img);

	for(unsigned int i=0; i < m_pTriangulator->getTriangles().size(); i++)
	   m_pTriangulator->getTriangles()[i].paint(m_img);

	bufferArea.paint(m_img);

};
//---------------------------------------------------------------------------

void painter::triangulate()
{
	m_pLogger->info("begin triangulate");
	m_pTriangulator->triangulate();
	m_pLogger->info("end triangulate");
};
//---------------------------------------------------------------------------


/*
bool painter::addToTriangles(std::vector<triangulation::triangle> &vTriangles, triangulation::point &p1, triangulation::point &p2, triangulation::point &p3, double r)
{
    double k = 0.1;
	triangulation::point p1_alternate = findNearPoint(vTriangles, p1, r);
	if(p1_alternate.length(p2) > k && p1_alternate.length(p3) > k)
		p1 = p1_alternate;

	triangulation::point p2_alternate = findNearPoint(vTriangles, p2, r);
	if(p2_alternate.length(p3) > k && p2_alternate.length(p1) > k)
		p2 = p2_alternate;

	triangulation::point p3_alternate = findNearPoint(vTriangles, p3, r);
	if(p3_alternate.length(p2) > k && p3_alternate.length(p1) > k)
		p3 = p3_alternate;
	else if(findNearPointWithAreaSide(p3, p3_alternate, r))
	{
		if(p3_alternate.length(p2) > k && p3_alternate.length(p1) > k)
			p3 = p3_alternate;

//		triangulation::point p =
//		p3 = findNearPoint(vTriangles, p3_alternate, r);
//		p3 = p3_alternate;
    }
	else if(findNearPointWithAreaSide(p2, p3_alternate, r))
	{
		if(p3_alternate.length(p2) > k && p3_alternate.length(p1) > k)
			p3 = p3_alternate;
	}
	else if(findNearPointWithAreaSide(p1, p3_alternate, r))
	{
		if(p3_alternate.length(p2) > k && p3_alternate.length(p1) > k)
			p3 = p3_alternate;
	}

	if(	p1.length(p2) > k
		&& p2.length(p3) > k
		&& p3.length(p1) > k
		&& !findTriangle(vTriangles, p1, p2, p3)
		&& !hasIntersection(vTriangles, p1, p2, p3)
	)
	{
		vTriangles.push_back(triangulation::triangle(p1,p2,p3));
		return true;
	};

	return false;
};

//---------------------------------------------------------------------------
*/

//---------------------------------------------------------------------------
/*
bool painter::addNewTriangle(
	std::vector<triangulation::triangle> &vTriangles,
	triangulation::point &p1_buff,
	const triangulation::point &p2,
	triangulation::point &p4_buff,
	triangulation::point &p5_buff,
	double r
)
{
	// if( recurs > 5) return false;
	triangulation::line l1(p4_buff, p5_buff);
	triangulation::line l2(p4_buff,p2);

	if(!l2.hasPoint(p4_buff)) return false;

	if(l2.length() <= r/2) p4_buff = p2;

	if(  l1.length() <= r  )
	{
		triangulation::point p1_buff_copy = p1_buff;
		triangulation::point p4_buff_copy = p4_buff;
		triangulation::point p5_buff_copy = p5_buff;

		if(!addToTriangles(vTriangles, p1_buff_copy, p4_buff_copy, p5_buff_copy, r))
			return false;

		p1_buff = p1_buff_copy;
		p4_buff = p4_buff_copy;
		p5_buff = p5_buff_copy;

		p1_buff = p4_buff;
		p4_buff = triangulation::getNextPoint(p1_buff, p2, r);

		//paint(vTriangles);
		//ShowMessage("1");

		return true;
	}
	else if( l1.length() > r )
	{
		triangulation::point middle_p = l1.getMiddlePoint();

		triangulation::point p6 = triangulation::getNextPoint(p1_buff, middle_p, r);

		triangulation::line l2(p4_buff, p6);
		triangulation::line l3(p6, p5_buff);

		{
			triangulation::point p1_buff_copy = p1_buff;
			triangulation::point p4_buff_copy = p4_buff;
			triangulation::point p6_copy = p6;

			if(!addToTriangles(vTriangles, p1_buff, p4_buff, p6, r))
				return false;

			p1_buff = p1_buff_copy;
			p4_buff = p4_buff_copy;
			//p6 = p6_copy;
		}

		{
			triangulation::point p1_buff_copy = p1_buff;
			triangulation::point p6_copy = p6;
			triangulation::point p5_buff_copy = p5_buff;

			if(!addToTriangles(vTriangles, p1_buff, p6, p5_buff, r))
				return false;

			p1_buff = p1_buff_copy;
			//p6 = p6_copy;
			p5_buff = p5_buff_copy;
		}

		//vTriangles.push_back(triangulation::triangle(p1_buff, p4_buff, p6));
		//	vTriangles.push_back(triangulation::triangle(p1_buff, p6, p5_buff));

		p1_buff = p4_buff;
		p5_buff = p6;
		p4_buff = triangulation::getNextPoint(p1_buff, p2, r);
		return true;
*/
	/*
		triangulation::point middle_p = l1.getMiddlePoint();

		triangulation::point p6 = triangulation::getNextPoint(p1_buff, middle_p, r);

		triangulation::point p1_buff_copy = p1_buff;
		triangulation::point p4_buff_copy = p4_buff;
		triangulation::point p5_buff_copy = p5_buff;
		triangulation::point p6_copy = p6;


		if( !addNewTriangle(vTriangles, p1_buff_copy, p2, p6_copy, p5_buff_copy, r))
			return false;

		p1_buff_copy = p1_buff;
		p4_buff_copy = p4_buff;
		p5_buff_copy = p5_buff;
		p6_copy = p6;

		if( !addNewTriangle(vTriangles, p1_buff_copy, p2, p4_buff_copy, p6_copy, r))
			return false;

		p1_buff = p1_buff_copy;
		p4_buff = p4_buff_copy;
		p5_buff = p6;
		p4_buff = triangulation::getNextPoint(p1_buff, p2, r);

		return true;
		*/
		/*
	}
	else
	{
		m_pLogger->info("_");
	};

	return false;
};
//---------------------------------------------------------------------------
*/

#pragma package(smart_init)
