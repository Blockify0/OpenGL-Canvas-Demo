#pragma once

#include <wx/wx.h>
#include <wx/slider.h>
#include <wx/checkbox.h>
#include "GLCanvas.h"

class MainFrame : public wxFrame
{
public:
    MainFrame();

private:
    // Event handlers
    void OnSliderChange(wxCommandEvent& event);
    void OnCheckBoxChange(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);

    // UI elements
    GLCanvas* m_canvas;
    wxPanel* m_controlPanel;
    wxSlider* m_rotationSlider;
    wxCheckBox* m_showShapeCheckBox;

    // Panel visibility
    bool m_controlPanelVisible;

    DECLARE_EVENT_TABLE()
}; 