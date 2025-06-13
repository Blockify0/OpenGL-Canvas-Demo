#pragma once

#include <wx/glcanvas.h>
#include <wx/timer.h>
#include <memory>

class MainFrame;

class GLCanvas : public wxGLCanvas
{
public:
    GLCanvas(wxWindow* parent, MainFrame* frame);
    ~GLCanvas();

    // Event handlers
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnMouse(wxMouseEvent& event);
    void OnEraseBackground(wxEraseEvent& event);

    // Public methods
    void SetRotation(float angle);
    void SetShowShape(bool show);
    bool IsOverlayButtonClicked(int x, int y) const;

private:
    void InitGL();
    void Render();
    void DrawScene();
    void DrawOverlayButton();
    void LoadTexture();

    MainFrame* m_frame;
    wxGLContext* m_glContext;
    bool m_initialized;
    float m_rotation;
    bool m_showShape;
    GLuint m_textureId;
    bool m_textureLoaded;

    // Overlay button properties
    struct {
        int x, y;
        int width, height;
    } m_overlayButton;

    DECLARE_EVENT_TABLE()
}; 