#include "_tr_painter.h"

//---------------------------------------------------------------------------

// painter::painter(TImage *img, triangulation::logger *pLogger)
// :
//     m_img(img),
//     m_pLogger(pLogger),
//     m_pTriangulator(new Sea5kgTriangulationTriangulator(m_pLogger))
// {

// };
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

std::vector<Sea5kgTriangulationArea> &painter::getAreas()
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

std::vector<Sea5kgTriangulationTriangle> &painter::getTriangles() {
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
            Sea5kgTriangulationPoint p = getAreas()[i].getPoint(i1);

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
