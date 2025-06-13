#include "GLCanvas.h"
#include "MainFrame.h"
#include <wx/wx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <wx/image.h>

BEGIN_EVENT_TABLE(GLCanvas, wxGLCanvas)
    EVT_PAINT(GLCanvas::OnPaint)
    EVT_SIZE(GLCanvas::OnSize)
    EVT_MOUSE_EVENTS(GLCanvas::OnMouse)
    EVT_ERASE_BACKGROUND(GLCanvas::OnEraseBackground)
END_EVENT_TABLE()

GLCanvas::GLCanvas(wxWindow* parent, MainFrame* frame)
    : wxGLCanvas(parent, wxID_ANY, nullptr, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
    , m_frame(frame)
    , m_initialized(false)
    , m_rotation(0.0f)
    , m_showShape(true)
    , m_textureId(0)
    , m_textureLoaded(false)
{
    // Set up overlay button properties
    m_overlayButton.width = 50;
    m_overlayButton.height = 50;
    m_overlayButton.x = 10;
    m_overlayButton.y = 10;

    // Create OpenGL context
    m_glContext = new wxGLContext(this);
}

GLCanvas::~GLCanvas()
{
    if (m_textureLoaded)
    {
        glDeleteTextures(1, &m_textureId);
    }
    delete m_glContext;
}

void GLCanvas::InitGL()
{
    if (!m_initialized)
    {
        SetCurrent(*m_glContext);
        
        // Initialize OpenGL
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Load texture
        LoadTexture();

        m_initialized = true;
    }
}

void GLCanvas::LoadTexture()
{
    if (!m_textureLoaded)
    {
        wxImage image;
        if (image.LoadFile("button.png"))
        {
            glGenTextures(1, &m_textureId);
            glBindTexture(GL_TEXTURE_2D, m_textureId);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            unsigned char* data = image.GetData();
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.GetWidth(), image.GetHeight(),
                0, GL_RGB, GL_UNSIGNED_BYTE, data);

            m_textureLoaded = true;
        }
    }
}

void GLCanvas::OnPaint(wxPaintEvent& event)
{
    wxPaintDC(this);
    Render();
}

void GLCanvas::OnSize(wxSizeEvent& event)
{
    if (m_initialized)
    {
        SetCurrent(*m_glContext);
        wxSize size = GetSize();
        glViewport(0, 0, size.GetWidth(), size.GetHeight());
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, size.GetWidth(), size.GetHeight(), 0);
        glMatrixMode(GL_MODELVIEW);
    }
    event.Skip();
}

void GLCanvas::OnMouse(wxMouseEvent& event)
{
    if (event.LeftDown())
    {
        wxPoint pos = event.GetPosition();
        if (IsOverlayButtonClicked(pos.x, pos.y))
        {
            // Toggle control panel visibility
            wxWindow* controlPanel = m_frame->FindWindowByName("ControlPanel");
            if (controlPanel)
            {
                controlPanel->Show(!controlPanel->IsShown());
                m_frame->Layout();
            }
        }
    }
    event.Skip();
}

void GLCanvas::OnEraseBackground(wxEraseEvent& event)
{
    // Do nothing to avoid flickering
}

void GLCanvas::Render()
{
    if (!m_initialized)
        InitGL();

    SetCurrent(*m_glContext);
    glClear(GL_COLOR_BUFFER_BIT);

    DrawScene();
    DrawOverlayButton();

    SwapBuffers();
}

void GLCanvas::DrawScene()
{
    if (!m_showShape)
        return;

    glPushMatrix();
    glTranslatef(GetSize().GetWidth() / 2.0f, GetSize().GetHeight() / 2.0f, 0.0f);
    glRotatef(m_rotation, 0.0f, 0.0f, 1.0f);

    // Draw a triangle
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-50.0f, -50.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(50.0f, -50.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(0.0f, 50.0f);
    glEnd();

    glPopMatrix();
}

void GLCanvas::DrawOverlayButton()
{
    if (!m_textureLoaded)
        return;

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(m_overlayButton.x, m_overlayButton.y);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(m_overlayButton.x + m_overlayButton.width, m_overlayButton.y);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(m_overlayButton.x + m_overlayButton.width, m_overlayButton.y + m_overlayButton.height);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(m_overlayButton.x, m_overlayButton.y + m_overlayButton.height);
    glEnd();

    glPopMatrix();
}

bool GLCanvas::IsOverlayButtonClicked(int x, int y) const
{
    return x >= m_overlayButton.x && x <= m_overlayButton.x + m_overlayButton.width &&
           y >= m_overlayButton.y && y <= m_overlayButton.y + m_overlayButton.height;
}

void GLCanvas::SetRotation(float angle)
{
    m_rotation = angle;
}

void GLCanvas::SetShowShape(bool show)
{
    m_showShape = show;
} 