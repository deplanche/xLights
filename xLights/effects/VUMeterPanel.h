#ifndef VUMETERPANEL_H
#define VUMETERPANEL_H

//(*Headers(VUMeterPanel)
#include <wx/panel.h>
class wxBitmapButton;
class wxCheckBox;
class wxChoice;
class wxFlexGridSizer;
class wxSlider;
class wxStaticText;
class wxTextCtrl;
//*)

#include "../BulkEditControls.h"

class VUMeterPanel: public wxPanel
{
	public:

		VUMeterPanel(wxWindow* parent);
		virtual ~VUMeterPanel();

		//(*Declarations(VUMeterPanel)
		BulkEditCheckBox* CheckBox_VUMeter_SlowDownFalls;
		BulkEditChoice* Choice_VUMeter_Shape;
		BulkEditChoice* Choice_VUMeter_TimingTrack;
		BulkEditChoice* Choice_VUMeter_Type;
		BulkEditSlider* Slider_VUMeter_Bars;
		BulkEditSlider* Slider_VUMeter_EndNote;
		BulkEditSlider* Slider_VUMeter_Sensitivity;
		BulkEditSlider* Slider_VUMeter_StartNote;
		BulkEditSlider* Slider_VUMeter_XOffset;
		BulkEditSlider* Slider_VUMeter_YOffset;
		BulkEditTextCtrl* TextCtrl_VUMeter_Bars;
		BulkEditTextCtrl* TextCtrl_VUMeter_EndNote;
		BulkEditTextCtrl* TextCtrl_VUMeter_Sensitivity;
		BulkEditTextCtrl* TextCtrl_VUMeter_StartNote;
		BulkEditTextCtrl* TextCtrl_VUMeter_XOffset;
		BulkEditTextCtrl* TextCtrl_VUMeter_YOffset;
		BulkEditValueCurveButton* BitmapButton_VUMeter_YOffsetVC;
		wxBitmapButton* BitmapButton_VUMeter_Bars;
		wxBitmapButton* BitmapButton_VUMeter_EndNote;
		wxBitmapButton* BitmapButton_VUMeter_Sensitivity;
		wxBitmapButton* BitmapButton_VUMeter_Shape;
		wxBitmapButton* BitmapButton_VUMeter_SlowDownFalls;
		wxBitmapButton* BitmapButton_VUMeter_StartNote;
		wxBitmapButton* BitmapButton_VUMeter_TimingTrack;
		wxBitmapButton* BitmapButton_VUMeter_Type;
		wxBitmapButton* BitmapButton_VUMeter_XOffset;
		wxBitmapButton* BitmapButton_VUMeter_YOffset;
		wxStaticText* StaticText10;
		wxStaticText* StaticText11;
		wxStaticText* StaticText12;
		wxStaticText* StaticText13;
		wxStaticText* StaticText14;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		wxStaticText* StaticText8;
		wxStaticText* StaticText9;
		//*)

	protected:

		//(*Identifiers(VUMeterPanel)
		static const long ID_STATICTEXT_VUMeter_Bars;
		static const long ID_SLIDER_VUMeter_Bars;
		static const long IDD_TEXTCTRL_VUMeter_Bars;
		static const long ID_BITMAPBUTTON_SLIDER_VUMeter_Bars;
		static const long ID_STATICTEXT_VUMeter_Type;
		static const long ID_CHOICE_VUMeter_Type;
		static const long ID_STATICTEXT5;
		static const long ID_BITMAPBUTTON_CHOICE_VUMeter_Type;
		static const long ID_STATICTEXT_VUMeter_TimingTrack;
		static const long ID_CHOICE_VUMeter_TimingTrack;
		static const long ID_STATICTEXT4;
		static const long ID_BITMAPBUTTON_CHOICE_VUMeter_TimingTrack;
		static const long ID_STATICTEXT_VUMeter_Sensitivity;
		static const long ID_SLIDER_VUMeter_Sensitivity;
		static const long IDD_TEXTCTRL_VUMeter_Sensitivity;
		static const long ID_BITMAPBUTTON_SLIDER_VUMeter_Sensitivity;
		static const long ID_STATICTEXT_VUMeter_Shape;
		static const long ID_CHOICE_VUMeter_Shape;
		static const long ID_STATICTEXT8;
		static const long ID_BITMAPBUTTON_CHOICE_VUMeter_Shape;
		static const long ID_STATICTEXT9;
		static const long ID_CHECKBOX_VUMeter_SlowDownFalls;
		static const long ID_STATICTEXT10;
		static const long ID_BITMAPBUTTON_CHECKBOX_VUMeter_SlowDownFalls;
		static const long ID_STATICTEXT_VUMeter_StartNote;
		static const long ID_SLIDER_VUMeter_StartNote;
		static const long IDD_TEXTCTRL_VUMeter_StartNote;
		static const long ID_BITMAPBUTTON_SLIDER_VUMeter_StartNote;
		static const long ID_STATICTEXT_VUMeter_EndNote;
		static const long ID_SLIDER_VUMeter_EndNote;
		static const long IDD_TEXTCTRL_VUMeter_EndNote;
		static const long ID_BITMAPBUTTON_SLIDER_VUMeter_EndNote;
		static const long ID_STATICTEXT_VUMeter_XOffset;
		static const long ID_SLIDER_VUMeter_XOffset;
		static const long IDD_TEXTCTRL_VUMeter_XOffset;
		static const long ID_BITMAPBUTTON_SLIDER_VUMeter_XOffset;
		static const long ID_STATICTEXT_VUMeter_YOffset;
		static const long ID_SLIDER_VUMeter_YOffset;
		static const long ID_VALUECURVE_VUMeter_YOffset;
		static const long IDD_TEXTCTRL_VUMeter_YOffset;
		static const long ID_BITMAPBUTTON_SLIDER_VUMeter_YOffset;
		//*)

	public:

		//(*Handlers(VUMeterPanel)
		void OnLockButtonClick(wxCommandEvent& event);
        void OnChoice_VUMeter_TypeSelect(wxCommandEvent& event);
		void OnSlider_VUMeter_StartNoteCmdSliderUpdated(wxScrollEvent& event);
		void OnTextCtrl_VUMeter_StartNoteText(wxCommandEvent& event);
        void OnVCButtonClick(wxCommandEvent& event);
        void OnVCChanged(wxCommandEvent& event);
        //*)

		DECLARE_EVENT_TABLE()

	public:

		void ValidateWindow();
};

#endif
