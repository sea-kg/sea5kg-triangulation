//---------------------------------------------------------------------------

#pragma hdrstop

#include "_tr_painter.h"

//---------------------------------------------------------------------------

// painter::painter(TImage *img, triangulation::logger *pLogger)
// :
//     m_img(img),
//     m_pLogger(pLogger),
//     m_pTriangulator(new triangulation::triangulator(m_pLogger))
// {

// };
//---------------------------------------------------------------------------

// void painter::SaveAreas(UnicodeString filename)
// {
//     TStringList *pList = new TStringList();
// 
//     for(unsigned int i = 0; i < m_pTriangulator->getAreas().size(); i++)
//     {
//         UnicodeString str = "area:[";
// 
//         triangulation::area ar = m_pTriangulator->getAreas()[i];
// 
//         str += IntToStr((int)ar.getCountTriangles()) + ",";
// 
//         for(unsigned int pi = 0; pi < ar.count(); pi++)
//         {
//             str += IntToStr((int)ar.getPoint(pi).X) + ",";
//             str += IntToStr((int)ar.getPoint(pi).Y);
//             if(pi + 1 < ar.count())
//                 str += ",";
//         };
//         str += "];";
//         pList->Add(str);
//     };
// 
//     pList->SaveToFile(filename);
// };

//---------------------------------------------------------------------------


// void split(TStringList* lout, wchar_t* str, const wchar_t* separator) {
//       for(wchar_t* tok = wcstok(str, separator); tok; tok = wcstok(NULL, separator))
//           lout->Add(tok);
// }

//---------------------------------------------------------------------------

/*void painter::LoadAreas(UnicodeString filename)
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
*/

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

std::vector<triangulation::area> &painter::getAreas()
{
    return m_pTriangulator->getAreas();    
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
    // if( bufferArea.count() < 4 )
    // {
    //    ShowMessage("Area must have points > 3");
    //    return;
    // };

    int nValue = 1;
    // UnicodeString strValue;
    bool bCorrect = false;
    while(!bCorrect)
    {
        bCorrect = true;
        // if(!InputQuery("Input Count Triangles", "Value", strValue))
        //     return;

        // if(!TryStrToInt(strValue, nValue))
        // {
        //     // ShowMessage("It's not integer. Try again.");
        //     bCorrect = false;
        //     continue;
        // };

        if(nValue < bufferArea.count())
        {
            // ShowMessage("Value must be >= " + IntToStr(bufferArea.count()));
            bCorrect = false;
            continue;
        };
    }

    // m_img->Cursor = crDefault;
    bufferArea.setCountTriangles(nValue);
    m_pTriangulator->addArea(bufferArea);
//    addArea(bufferArea);
    bufferArea.clear();
};

//---------------------------------------------------------------------------

void painter::refresh()
{
    // m_img->Canvas->Rectangle( 0, 0, m_img->Width, m_img->Height );


    //for(unsigned int i=0; i < m_pTriangulator->getAreas().size(); i++)
    //   m_pTriangulator->getAreas()[i].paint(m_img);
//
    //for(unsigned int i=0; i < m_pTriangulator->getTriangles().size(); i++)
    //   m_pTriangulator->getTriangles()[i].paint(m_img);

    // bufferArea.paint(m_img);

};
//---------------------------------------------------------------------------

void painter::triangulate()
{
    // m_pLogger->info("begin triangulate");
    m_pTriangulator->triangulate();
    // m_pLogger->info("end triangulate");
};

//---------------------------------------------------------------------------

std::vector<triangulation::triangle> &painter::getTriangles()
{
    return m_pTriangulator->getTriangles();
};

//---------------------------------------------------------------------------

bool painter::getMinMaxXY(int &maxX, int &minX, int &maxY, int &minY)
{
    double _maxX, _minX, _maxY, _minY;

    bool bFirst = true;

    for(unsigned int i = 0; i < getAreas().size(); i++)
    {
        for(int i1 = 0; i1 < getAreas()[i].count(); i1++)
        {
            triangulation::point p = getAreas()[i].getPoint(i1);

            if(bFirst)
            {
                _maxX = p.X;
                _minX = p.X;
                _maxY = p.Y;
                _minY = p.Y;
                bFirst = false;
            };

            if(_maxX < p.X) _maxX = p.X;
            if(_minX > p.X) _minX = p.X;
            if(_maxY < p.Y) _maxY = p.Y;
            if(_minY > p.Y) _minY = p.Y;

        };
    };

    if((_maxX > _minX) && (_maxY > _minY))
    {
       maxX = _maxX;
       minX = _minX;
       maxY = _maxY;
       minY = _minY;
       return true;
    };
    return false;
};

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#pragma package(smart_init)
