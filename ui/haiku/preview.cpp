#include "preview.hpp"

#include <Layout.h>
#include <GroupLayout.h>

#include "../gl/gl1.hpp"

class PreviewWindow : public BWindow{
public:
	BGLView* canvas;
	GlPreviewer* gl;
	PreviewWindow() : BWindow(BRect(400,400,800,800),"Kovel - OpenGL preview",B_TITLED_WINDOW,0)
	{
		gl=new GlPreviewer();
		canvas=new BGLView(Bounds(),"GL",B_FOLLOW_ALL_SIDES,0,BGL_RGB | BGL_ALPHA | BGL_DOUBLE | BGL_DEPTH);
		AddChild(canvas);
	}
	void Render(float zoom, float rotation){
		Core* core=Core::Instance();
		BRect frame=Bounds();
		canvas->LockGL();
		gl->Render(frame.Size().Width(),frame.Size().Height(),core->geo,core->mat,zoom,rotation);
		canvas->UnlockGL();
		canvas->SwapBuffers();
	}
};

PreviewWindow* prev;

PreviewPanel::PreviewPanel() : BView(NULL,B_WILL_DRAW | B_PULSE_NEEDED)
{
	SetViewColor(220,220,220);
	BGroupLayout* box=new BGroupLayout(B_HORIZONTAL);
	BGroupLayout* control=new BGroupLayout(B_VERTICAL);
	BGroupLayout* zoomButton=new BGroupLayout(B_HORIZONTAL);
	BGroupLayout* rotationButton=new BGroupLayout(B_HORIZONTAL);
	
	SetLayout(box);
	
	zoomLevel=new BStringView(NULL,"");
	rotationLevel=new BStringView(NULL,"");
	BButton* zoomMore=new BButton("Zoom in",NULL);
	BButton* zoomLess=new BButton("Zoom out",NULL);
	BButton* rotationLeft=new BButton("Rotate left",NULL);
	BButton* rotationRight=new BButton("Rotate right",NULL);
	
	
	//canvas=new BGLView(Bounds(),"GL",B_FOLLOW_ALL_SIDES,0,BGL_RGB | BGL_ALPHA | BGL_DOUBLE | BGL_DEPTH);
	
	//box->AddView(canvas,1);
	box->AddItem(control,1);

	control->AddView(zoomLevel);
	control->AddView(rotationLevel);
	control->AddItem(zoomButton);
	control->AddItem(rotationButton);
	zoomButton->AddView(zoomMore);
	zoomButton->AddView(zoomLess);
	rotationButton->AddView(rotationLeft);
	rotationButton->AddView(rotationRight);	
	
	/* Pulse for refreshing */
	
	prev=new PreviewWindow();
	prev->Show();
}

void
PreviewPanel::Render()
{

}

void
PreviewPanel::Pulse()
{
	prev->Render(zoom,rotation);
}
