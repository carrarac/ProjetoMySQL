#include "Application.h"


bool Application::OnInit() {
	Frame* MyFrame = new Frame("Janela wxWidgets + MySQL");	
	MyFrame->Show(true);
	return true;
}
wxIMPLEMENT_APP(Application);
