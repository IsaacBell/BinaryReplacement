///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.9.0 Feb 14 2021)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/button.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/filepicker.h>
#include <wx/gbsizer.h>
#include <wx/frame.h>

class MyFrame2 : public wxFrame
{
	private:
    void OnGenerateButtonClick( wxCommandEvent& event );// { event.Skip(); }
		void OnFileSelected( wxFileDirPickerEvent& event ); // { event.Skip(); }

	protected:
		wxButton* m_button1;
		wxFilePickerCtrl* m_filePicker1;

		// Virtual event handlers, override them in your derived class
		// virtual void OnGenerateButtonClick( wxCommandEvent& event ) { event.Skip(); }
		// virtual void OnFileSelected( wxFileDirPickerEvent& event ) { event.Skip(); }


	public:
		MyFrame2(
      wxWindow* parent,
      wxWindowID id = wxID_ANY,
      const wxString& title = wxEmptyString,
      const wxPoint& pos = wxDefaultPosition, 
      const wxSize& size = wxSize( 500,300 ),
      long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL
    );

		~MyFrame2();

};

