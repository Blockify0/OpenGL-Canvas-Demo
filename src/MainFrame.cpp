#include "MainFrame.h"
#include <wx/sizer.h>

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_SLIDER(wxID_ANY, MainFrame::OnSliderChange)
    EVT_CHECKBOX(wxID_ANY, MainFrame::OnCheckBoxChange)
    EVT_CLOSE(MainFrame::OnClose)
END_EVENT_TABLE()

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, "OpenGL Canvas Demo", wxDefaultPosition, wxSize(800, 600))
    , m_controlPanelVisible(false)
{
    // Create main sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    // Create OpenGL canvas
    m_canvas = new GLCanvas(this, this);
    mainSizer->Add(m_canvas, 1, wxEXPAND);

    // Create control panel
    m_controlPanel = new wxPanel(this, wxID_ANY);
    wxBoxSizer* controlSizer = new wxBoxSizer(wxVERTICAL);

    // Add rotation slider
    m_rotationSlider = new wxSlider(m_controlPanel, wxID_ANY, 0, 0, 360,
        wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL | wxSL_LABELS);
    controlSizer->Add(m_rotationSlider, 0, wxALL | wxEXPAND, 5);

    // Add show shape checkbox
    m_showShapeCheckBox = new wxCheckBox(m_controlPanel, wxID_ANY, "Show Shape");
    m_showShapeCheckBox->SetValue(true);
    controlSizer->Add(m_showShapeCheckBox, 0, wxALL, 5);

    m_controlPanel->SetSizer(controlSizer);
    mainSizer->Add(m_controlPanel, 0, wxEXPAND);

    // Initially hide the control panel
    m_controlPanel->Hide();

    SetSizer(mainSizer);
    Centre();
}

void MainFrame::OnSliderChange(wxCommandEvent& event)
{
    if (m_canvas)
    {
        m_canvas->SetRotation(m_rotationSlider->GetValue());
        m_canvas->Refresh();
    }
}

void MainFrame::OnCheckBoxChange(wxCommandEvent& event)
{
    if (m_canvas)
    {
        m_canvas->SetShowShape(m_showShapeCheckBox->GetValue());
        m_canvas->Refresh();
    }
}

void MainFrame::OnClose(wxCloseEvent& event)
{
    Destroy();
}

// Application class and entry point
class GLCanvasApp : public wxApp
{
public:
    virtual bool OnInit()
    {
        if (!wxApp::OnInit())
            return false;

        MainFrame* frame = new MainFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(GLCanvasApp); 