#include "MyFrame2.h"
#include "BinaryReplacement2.h"
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif

MyFrame2::MyFrame2(
  wxWindow* parent,
  wxWindowID id,
  const wxString& title,
  const wxPoint& pos,
  const wxSize& size,
  long style
) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxGridBagSizer* gbSizer1;
	gbSizer1 = new wxGridBagSizer( 0, 0 );
	gbSizer1->SetFlexibleDirection( wxBOTH );
	gbSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_button1 = new wxButton( this, wxID_ANY, wxT("Generate"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( m_button1, wxGBPosition( 5, 12 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	m_filePicker1 = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	gbSizer1->Add( m_filePicker1, wxGBPosition( 5, 9 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	this->SetSizer( gbSizer1 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame2::OnGenerateButtonClick ), NULL, this );
	m_filePicker1->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( MyFrame2::OnFileSelected ), NULL, this );
}

MyFrame2::~MyFrame2()
{
	// Disconnect Events
	m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame2::OnGenerateButtonClick ), NULL, this );
	m_filePicker1->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( MyFrame2::OnFileSelected ), NULL, this );
}

void MyFrame2::OnFileSelected(wxFileDirPickerEvent& event)
{
  event.Skip();
}

void MyFrame2::OnGenerateButtonClick(wxCommandEvent& event)
{
  wxString str = this->m_filePicker1->GetPath();
  std::cout<<"path: "<<str<<"\n";
  // wxLogMessage(str.mb_str());

  std::string std_string = "";
  const char* fileNameChar = str.mb_str();
  // std_string = str;
  std::cout<<"fileNameChar: "<<fileNameChar<<"\n";
  std_string.append(fileNameChar);
  // std::cout<<"std_str: "<<std_string<<"\n";
  // wxLogMessage(std_string);
  BinaryReplacement replacer;
  replacer.translate(std_string);
}

class MyApp : public wxApp
{
  public:
    virtual bool OnInit();
};

DECLARE_APP(MyApp)

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
  MyFrame2 *frame = new MyFrame2(
    (wxFrame*) NULL,
    -1,
    _T("Binary String Replacement")
  );
  frame->Show(true);
  return true;
}