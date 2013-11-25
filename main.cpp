#include <QApplication>
#include "Editor.h"
int main(int argc, char *argv[ ])
{
    QApplication app(argc, argv);
     app.setOrganizationName("Chad");
     app.setApplicationName("Text Editor");
    Editor editor;
	editor.show();
	return app.exec();
}
