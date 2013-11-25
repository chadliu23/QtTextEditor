#ifndef EDITOR_H
#define EDITOR_H

#include <QMainWindow>
class QUndoStack;

class QAction;
class QMenu;
class QPlainTextEdit;

class Editor : public QMainWindow
{
     Q_OBJECT

public:
     Editor();

protected:
     void closeEvent(QCloseEvent *event);

private slots:
     void newFile();
     void open();
     bool save();
     bool saveAs();
     void documentWasModified();

private:
     void createActions();
     void createMenus();
     bool maybeSave();
     void loadFile(const QString &fileName);
     bool saveFile(const QString &fileName);
     void setCurrentFile(const QString &fileName);

     QPlainTextEdit *textEdit;
     QString curFile;
     QUndoStack *undoStack;

     QMenu *fileMenu;
     QMenu *editMenu;
     QAction *newAct;
     QAction *openAct;
     QAction *closeAct;
     QAction *cutAct;
     QAction *copyAct;
     QAction *pasteAct;
     QAction *undoAct;
};

#endif
