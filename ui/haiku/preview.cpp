#include "preview.hpp"

#include <Layout.h>
#include <GroupLayout.h>

#include "../gl/gl1.hpp"

class PreviewWindow : public BWindow {
public:
  BGLView* canvas;
  GlPreviewer* gl;
  PreviewWindow()
      : BWindow(BRect(400, 400, 800, 800), "Kovel - OpenGL preview",
                B_TITLED_WINDOW, 0) {
	gl = new GlPreviewer();
	canvas = new BGLView(Bounds(), "GL", B_FOLLOW_ALL_SIDES, 0,
	                     BGL_RGB | BGL_ALPHA | BGL_DOUBLE);
	AddChild(canvas);
  }
  void Render(float zoom, float rotation) {
	Core* core = Core::Instance();
	BRect frame = Bounds();
	canvas->LockGL();
	gl->Render(frame.Size().Width(), frame.Size().Height(), core->geo,
	           core->mat, zoom, rotation);
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
	  printf("OpenGL ERROR %s\n", error);
	}
	canvas->SwapBuffers();
	canvas->UnlockGL();
  }
};

PreviewWindow* prev;

PreviewPanel::PreviewPanel() : BView(NULL, B_WILL_DRAW | B_PULSE_NEEDED) {
  SetViewColor(220, 220, 220);
  BGroupLayout* box = new BGroupLayout(B_HORIZONTAL);
  BGroupLayout* control = new BGroupLayout(B_VERTICAL);
  BGroupLayout* zoomButton = new BGroupLayout(B_HORIZONTAL);
  BGroupLayout* rotationButton = new BGroupLayout(B_HORIZONTAL);

  SetLayout(box);

  zoomLevel = new BStringView(NULL, "");
  rotationLevel = new BStringView(NULL, "");
  BColorControl* color =
      new BColorControl(BPoint(0, 0), B_CELLS_32x8, 5.0, NULL, new BMessage(6));
  BButton* zoomMore = new BButton("Zoom in", new BMessage(11));
  BButton* zoomLess = new BButton("Zoom out", new BMessage(12));
  BButton* rotationLeft = new BButton("Rotate left", new BMessage(9));
  BButton* rotationRight = new BButton("Rotate right", new BMessage(10));

  box->AddItem(control, 1);

  control->AddView(zoomLevel);
  control->AddView(rotationLevel);
  control->AddView(color);
  control->AddItem(zoomButton);
  control->AddItem(rotationButton);
  zoomButton->AddView(zoomMore);
  zoomButton->AddView(zoomLess);
  rotationButton->AddView(rotationLeft);
  rotationButton->AddView(rotationRight);

  /* Pulse for refreshing */

  prev = new PreviewWindow();
  prev->Show();
}

void PreviewPanel::Render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  Pulse();
}

void PreviewPanel::Pulse() {
  prev->Render(zoom, rotation);
}
