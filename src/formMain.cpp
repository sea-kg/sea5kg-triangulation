//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "formMain.h"
#include "_tr_triangulation.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "src/*.dfm"
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
	StatusBar1->Panels->Items[0]->Text = "X = " + IntToStr(X) + "; Y= " + IntToStr(Y);
}
//---------------------------------------------------------------------------



void __fastcall TfrmMain::Image1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
	if(astate_.bAddArea && Button == mbLeft)
	{
		m_pPaiter->addToBufferArea(X,Y);
		m_pPaiter->refresh();
	};
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


  /*//
  triangulation::point p1(50,120);
  triangulation::point p2(150,150);

  triangulation::point p3_1(120,50);
  triangulation::point p3_2(50,200);

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

  if(L1.hasPoint(p4_1))
  {
	ShowMessage("it has point");
  }

  Image1->Canvas->Pen->Color = clRed;
  L2_1.paint(Image1);

  Image1->Canvas->Pen->Color = clBlue;
  L2_2.paint(Image1);
  */
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
		Image1->Picture->SaveToFile(SaveDialog2->FileName);
	};
}
//---------------------------------------------------------------------------

