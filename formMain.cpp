//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "formMain.h"
#include "_tr_triangulation.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;

struct stState
{
	bool bAddArea;
} astate_;

//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner),
	strCaption("Triangulation")
{
	astate_.bAddArea = false;
	m_pLogger = new triangulation::logger(memLog);
	m_pPaiter = new painter(Image1, m_pLogger);
	Caption = strCaption;

}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::Close1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
	m_pPaiter->refresh();
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::Image1MouseMove(TObject *Sender, TShiftState Shift, int X,
		  int Y)
{
  	// std::vector<triangulation::area> &painter::getAreas()
	UnicodeString str = "";
	triangulation::point p(X, Y);
	for(unsigned int i = 0; i < m_pPaiter->getAreas().size(); i++)
	{
		if(m_pPaiter->getAreas()[i].hasPoint(p))
		{
			str += "{" + IntToStr((int)i) + "}";
			// p.paint(Image1);
		};
	};            

	StatusBar1->Panels->Items[0]->Text = "X = " + IntToStr(X) + "; Y= " + IntToStr(Y) + "; areas: " + str;
}
//---------------------------------------------------------------------------



void __fastcall TfrmMain::Image1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
	if(astate_.bAddArea && Button == mbLeft)
	{
		m_pPaiter->addToBufferArea(X,Y);
		m_pPaiter->refresh();
	}
	else if(Button == mbLeft)
	{
		triangulation::point p(X,Y);
		for(unsigned int i=0; i < m_pPaiter->getTriangles().size(); i++)
		{
			if(m_pPaiter->getTriangles()[i].hasPoint(p))
			{
				lbxTriangles->Selected[i] = true;
				lbxTrianglesClick(Sender);
				return;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormDestroy(TObject *Sender)
{
	delete m_pPaiter;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actClearAllAreasExecute(TObject *Sender)
{
	Caption = strCaption;
	m_pPaiter->clear();
	m_pPaiter->refresh();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actApplyExecute(TObject *Sender)
{
    m_pPaiter->applyBufferArea();
	astate_.bAddArea = false;
	m_pPaiter->refresh();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actAddNewAreaExecute(TObject *Sender)
{
	astate_.bAddArea = true;
	Image1->Cursor = crHandPoint;
	m_pPaiter->refresh();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actCancelExecute(TObject *Sender)
{
	astate_.bAddArea = false;
	Image1->Cursor = crDefault;
	m_pPaiter->clear_buffer();
	m_pPaiter->refresh();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actAddNewAreaUpdate(TObject *Sender)
{
  TAction *act = dynamic_cast<TAction *>(Sender);
  if(act != NULL) act->Enabled = !astate_.bAddArea;
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::actTriangulateDivisionUpdate(TObject *Sender)
{
  TAction *act = dynamic_cast<TAction *>(Sender);
  if(act != NULL) act->Enabled = m_pPaiter->hasAreas();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actApplyUpdate(TObject *Sender)
{
  TAction *act = dynamic_cast<TAction *>(Sender);
  if(act != NULL) act->Enabled = astate_.bAddArea;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actCancelUpdate(TObject *Sender)
{
  TAction *act = dynamic_cast<TAction *>(Sender);
  if(act != NULL) act->Enabled = astate_.bAddArea;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actClearAllAreasUpdate(TObject *Sender)
{
  TAction *act = dynamic_cast<TAction *>(Sender);
  if(act != NULL) act->Enabled = m_pPaiter->hasAreas();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actTriangulateUpdate(TObject *Sender)
{
  TAction *act = dynamic_cast<TAction *>(Sender);
  if(act != NULL) act->Enabled = m_pPaiter->hasAreas() && !astate_.bAddArea;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actSaveAreasExecute(TObject *Sender)
{
	if(SaveDialog1->Execute())
	{
		m_pPaiter->SaveAreas(SaveDialog1->FileName);
		Caption = strCaption + "[" + SaveDialog1->FileName + "]";
	};
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::actSaveAreasUpdate(TObject *Sender)
{
/*  TAction *act = dynamic_cast<TAction *>(Sender);
  if(act != NULL) act->Enabled = m_pPaiter->hasAreas();*/
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actTriangulateExecute(TObject *Sender)
{
  m_pPaiter->triangulate();
  m_pPaiter->refresh();
  lbxTriangles->Clear();

  for(unsigned int i=0; i < m_pPaiter->getTriangles().size(); i++)
	lbxTriangles->Items->Add(m_pPaiter->getTriangles()[i].toString());
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::actLoadAreasUpdate(TObject *Sender)
{
  TAction *act = dynamic_cast<TAction *>(Sender);
  if(act != NULL) act->Enabled = !m_pPaiter->hasAreas();
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::actLoadAreasExecute(TObject *Sender)
{
  //
  if(OpenDialog1->Execute())
  {

	m_pPaiter->LoadAreas(OpenDialog1->FileName);
	Caption = strCaption + "[" + OpenDialog1->FileName + "]";
  };
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Save1Click(TObject *Sender)
{
	if(SaveDialog2->Execute())
	{
		int maxX, minX, maxY, minY;
		if(m_pPaiter->getMinMaxXY(maxX,minX,maxY,minY))
		{
			maxX += 20;
			minX -= 20;
			maxY += 20;
			minY -= 20;

			TRect r_src(minX, minY, maxX, maxY);
			TRect r_dst(0, 0, maxX - minX, maxY - minY);

			TImage *pImg = new TImage(NULL);
			pImg->Picture->Bitmap->Height = maxY - minY;
			pImg->Picture->Bitmap->Width = maxX - minX;
			TPngImage *pngImg = new TPngImage();
			pngImg->Assign(pImg->Picture->Bitmap);
			pngImg->SetSize(maxX - minX, maxY - minY);
            pngImg->Canvas->CopyRect(r_dst, Image1->Canvas, r_src);
            pngImg->SaveToFile(SaveDialog2->FileName);
			delete pngImg;
			delete pImg;
		};
	};
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::lbxTrianglesClick(TObject *Sender)
{
  for(int i = 0; i <= lbxTriangles->Count; i++)
  {
	if(lbxTriangles->Selected[i] == true)
	{
		triangulation::triangle tr;
		tr.fromString(lbxTriangles->Items->Strings[i]);

		TColor nPenColor = Image1->Canvas->Pen->Color;
		TColor nBrushColor = Image1->Canvas->Brush->Color;
		int nWidth = Image1->Canvas->Pen->Width;

		Image1->Canvas->Pen->Color = clYellow;
		Image1->Canvas->Pen->Width = 2;
		Image1->Canvas->Brush->Color = clBlue;

		TPoint points[3];
		points[0] = Point(tr.p1.X,tr.p1.Y);
		points[1] = Point(tr.p2.X,tr.p2.Y);
		points[2] = Point(tr.p3.X,tr.p3.Y);
		Image1->Canvas->Polygon(points, 2);

		Image1->Canvas->Pen->Color = nPenColor;
		Image1->Canvas->Brush->Color = nBrushColor;
		Image1->Canvas->Pen->Width = nWidth;

		tr.p1.paint(Image1);
		tr.p2.paint(Image1);
		tr.p3.paint(Image1);

		return;
    };
  };


}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actRefreshExecute(TObject *Sender)
{
	m_pPaiter->refresh();
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::triangleisequals1Click(TObject *Sender)
{
	triangulation::point p1(50,120);
	triangulation::point p2(150,150);
	triangulation::point p3(120,50);

	triangulation::triangle tr[6] = {
		triangulation::triangle(p1,p2,p3),
		triangulation::triangle(p1,p3,p2),
		triangulation::triangle(p2,p1,p3),
		triangulation::triangle(p2,p3,p1),
		triangulation::triangle(p3,p2,p1),
		triangulation::triangle(p3,p1,p2)
	};

	UnicodeString str;

	for(int x = 0; x < 6; x++)
	{
		for(int y = 0; y < 6; y++)
		{
			str += "{" + IntToStr(x) + "," + IntToStr(y) + ",";
			str += tr[x].isEqual(tr[y]) ? "true" : "false";
			str += "}";
		}
		str += "\r\n";
	}

	ShowMessage(str);
	return;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::AreaHasIntersection1Click(TObject *Sender)
{
	int maxX = Image1->Width;
	int maxY = Image1->Height;

	for(unsigned int x = 0; x < maxX; x = x + 1)
		for(unsigned int y = 0; y < maxY; y = y + 1)
		{
			triangulation::point p(x, y);
			for(unsigned int i = 0; i < m_pPaiter->getAreas().size(); i++)
			{
				if(m_pPaiter->getAreas()[i].hasPoint(p))
					p.paint(Image1);
			};
		};
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::perpendicular1Click(TObject *Sender)
{
	std::vector<triangulation::point> points;
	
	points.push_back(triangulation::point(50,120));
	points.push_back(triangulation::point(150,150));
	points.push_back(triangulation::point(120,50));
	points.push_back(triangulation::point(50,200));


	randomize();
	for(int i = 0; i < 10; i++)
		points.push_back(triangulation::point(random(500) + 200,random(500) + 200));

	TColor nColor = Image1->Canvas->Pen->Color;
	int nWidth = Image1->Canvas->Pen->Width;

	for(int i = 2; i < points.size(); i++)
	{
		triangulation::point p1 = points[i];
		triangulation::point p2 = points[i-1];
		triangulation::point p3 = points[i-2];
		
                triangulation::line L1(p1,p2);
                

		Image1->Canvas->Pen->Color = clBlack;
		Image1->Canvas->Pen->Width = 2;
		L1.paint(Image1);

		triangulation::point p4;
                double len1 = L1.getPerpendicularToLine(p3, p4);

                triangulation::line L2(p3,p4);

		Image1->Canvas->Pen->Color = clRed;
		Image1->Canvas->Pen->Width = 2;
		L2.paint(Image1);

		if(!L1.hasPoint(p4))
		{
			triangulation::line L3;
			if(p4.length(p1) < p4.length(p2))
				L3 = triangulation::line(p1,p4);
			else
				L3 = triangulation::line(p2,p4);
			
			Image1->Canvas->Pen->Color = clBlue;
			Image1->Canvas->Pen->Width = 2;
			L3.paint(Image1);
			ShowMessage("line1 has not point");
		}
		else
			ShowMessage("line1 has point");			

	}

	Image1->Canvas->Pen->Color = nColor;
	Image1->Canvas->Pen->Width = nWidth;


	/*
  triangulation::point p1();
  triangulation::point p2();

  triangulation::point p3_1();
  triangulation::point p3_2();

  triangulation::line L1(p1,p2);

  Image1->Canvas->Pen->Color = clBlack;
  Image1->Canvas->Pen->Width = 2;

  L1.paint(Image1);

  triangulation::point p4_1, p4_2;

  double len1 = L1.getPerpendicularToLine(p3_1, p4_1);
  double len2 = L1.getPerpendicularToLine(p3_2, p4_2);

  triangulation::line L2_1(p3_1,p4_1);
  triangulation::line L2_1_(p4_1,p3_1);

  triangulation::line L2_2(p3_2,p4_2);

//  if(L1.hasPoint(p4_1))
//  {
//	ShowMessage("it has point");
//  }


  Image1->Canvas->Pen->Color = clBlue;
  L2_2.paint(Image1);
  */
}
//---------------------------------------------------------------------------

