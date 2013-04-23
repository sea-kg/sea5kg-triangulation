//---------------------------------------------------------------------------

#ifndef fromMainH
#define fromMainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.ImgList.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Buttons.hpp>
#include "_tr_logger.h"
#include "_tr_painter.h"
#include <Vcl.Dialogs.hpp>

//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel2;
	TMemo *memLog;
	TMainMenu *MainMenu1;
	TImageList *ImageList1;
	TScrollBox *ScrollBox1;
	TImage *Image1;
	TStatusBar *StatusBar1;
	TSplitter *Splitter1;
	TPopupMenu *PopupMenu1;
	TMenuItem *est11;
	TMenuItem *N2delone1;
	TActionList *ActionList1;
	TAction *actClearAllAreas;
	TAction *actApply;
	TAction *actAddNewArea;
	TAction *actCancel;
	TPanel *Panel1;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TBitBtn *BitBtn3;
	TBitBtn *BitBtn4;
	TBitBtn *BitBtn5;
	TAction *actTriangulate;
	TAction *actSaveAreas;
	TMenuItem *Areas1;
	TMenuItem *SaveAreas1;
	TMenuItem *Load3;
	TMenuItem *AddNewArea1;
	TMenuItem *Apply1;
	TMenuItem *Cancel1;
	TMenuItem *ClearAllAreas1;
	TSaveDialog *SaveDialog1;
	TAction *actLoadAreas;
	TOpenDialog *OpenDialog1;
	TMenuItem *Picture1;
	TMenuItem *Save1;
	TSaveDialog *SaveDialog2;
	TListBox *lbxTriangles;
	TSplitter *Splitter2;
	TBitBtn *BitBtn6;
	TAction *actRefresh;
	void __fastcall Close1Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Image1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall Image1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall actClearAllAreasExecute(TObject *Sender);
	void __fastcall actApplyExecute(TObject *Sender);
	void __fastcall actAddNewAreaExecute(TObject *Sender);
	void __fastcall actCancelExecute(TObject *Sender);
	void __fastcall actAddNewAreaUpdate(TObject *Sender);
	void __fastcall actTriangulateDivisionUpdate(TObject *Sender);
	void __fastcall actApplyUpdate(TObject *Sender);
	void __fastcall actCancelUpdate(TObject *Sender);
	void __fastcall actClearAllAreasUpdate(TObject *Sender);
	void __fastcall actTriangulateUpdate(TObject *Sender);
	void __fastcall actSaveAreasExecute(TObject *Sender);
	void __fastcall actSaveAreasUpdate(TObject *Sender);
	void __fastcall actTriangulateExecute(TObject *Sender);
	void __fastcall actLoadAreasUpdate(TObject *Sender);
	void __fastcall actLoadAreasExecute(TObject *Sender);
	void __fastcall Save1Click(TObject *Sender);
	void __fastcall lbxTrianglesClick(TObject *Sender);
	void __fastcall actRefreshExecute(TObject *Sender);

private:	// User declarations
	void refresh();
	triangulation::logger *m_pLogger;
	painter *m_pPaiter;
	UnicodeString strCaption;

public:		// User declarations
	__fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
