#include <QtGui>
#include "editor.h"

 Editor::Editor()
 {
     textEdit = new QPlainTextEdit;

     undoStack = new QUndoStack( this );
     setCentralWidget(textEdit);

     createActions();
     createMenus();


     connect(textEdit->document(), SIGNAL(contentsChanged()),
             this, SLOT(documentWasModified()));

     setCurrentFile("");
     setUnifiedTitleAndToolBarOnMac(true);
 }

 void Editor::closeEvent(QCloseEvent *event)
 {
     if (maybeSave()) {
         event->accept();
     } else {
         event->ignore();
     }
 }

 void Editor::newFile()
 {
     if (maybeSave()) {
         textEdit->clear();
         setCurrentFile("");
     }
 }

 void Editor::open()
 {
     if (maybeSave()) {
         QString fileName = QFileDialog::getOpenFileName(this);
         if (!fileName.isEmpty())
             loadFile(fileName);
     }  
 }

 bool Editor::save()
 {
     if (curFile.isEmpty()) {
         return saveAs();
     } else {
         return saveFile(curFile);
     }
 }

 bool Editor::saveAs()
 {
     QString fileName = QFileDialog::getSaveFileName(this);
     if (fileName.isEmpty())
         return false;

     return saveFile(fileName);
 }


 void Editor::documentWasModified()
 {
     setWindowModified(textEdit->document()->isModified());
 }

 void Editor::createActions()
 {
     newAct = new QAction(tr("&New"), this);
     newAct->setShortcuts(QKeySequence::New);
     connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

     openAct = new QAction( tr("&Open..."), this);
     openAct->setShortcuts(QKeySequence::Open);
     connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

     closeAct = new QAction(tr("Close"), this);
     closeAct->setShortcuts(QKeySequence::Quit);
     connect(closeAct, SIGNAL(triggered()), this, SLOT(close()));

     cutAct = new QAction(tr("Cu&t"), this);
     cutAct->setShortcuts(QKeySequence::Cut);
     connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));

     copyAct = new QAction( tr("&Copy"), this);
     copyAct->setShortcuts(QKeySequence::Copy);
     connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));

     pasteAct = new QAction( tr("&Paste"), this);
     pasteAct->setShortcuts(QKeySequence::Paste);
     connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));

     cutAct->setEnabled(false);
     copyAct->setEnabled(false);
     connect(textEdit, SIGNAL(copyAvailable(bool)),
             cutAct, SLOT(setEnabled(bool)));
     connect(textEdit, SIGNAL(copyAvailable(bool)),
             copyAct, SLOT(setEnabled(bool)));

     undoAct = undoStack->createUndoAction(this, tr("Undo"));
     undoAct->setShortcut(QKeySequence::Undo);
     connect(undoAct, SIGNAL(triggered()), textEdit, SLOT(undo()));
     undoAct->setEnabled(false);
     connect(textEdit, SIGNAL(undoAvailable(bool)),
             undoAct, SLOT(setEnabled(bool)));

 }

 void Editor::createMenus()
 {
     fileMenu = menuBar()->addMenu(tr("&File"));
     fileMenu->addAction(newAct);
     fileMenu->addAction(openAct);
     fileMenu->addAction(closeAct);

     editMenu = menuBar()->addMenu(tr("&Edit"));
     editMenu->addAction(cutAct);
     editMenu->addAction(copyAct);
     editMenu->addAction(pasteAct);
     editMenu->addAction(undoAct);


 }


 bool Editor::maybeSave()
 {
     if (textEdit->document()->isModified()) {
         QMessageBox::StandardButton ret;
         ret = QMessageBox::warning(this, tr("Editor"),
                      tr("The document has been modified.\n"
                         "Do you want to save your changes?"),
                      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
         if (ret == QMessageBox::Save)
             return save();
         else if (ret == QMessageBox::Cancel)
             return false;
     }
     return true;
 }

 void Editor::loadFile(const QString &fileName)
 {
     QFile file(fileName);
     if (!file.open(QFile::ReadOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("Editor"),
                              tr("Cannot read file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return;
     }

     QTextStream in(&file);
     textEdit->setPlainText(in.readAll());

     setCurrentFile(fileName);
}
 bool Editor::saveFile(const QString &fileName)
 {
     QFile file(fileName);
     if (!file.open(QFile::WriteOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("Editor"),
                              tr("Cannot write file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return false;
     }

     QTextStream out(&file);
     out << textEdit->toPlainText();

     setCurrentFile(fileName);
     return true;
 }

 void Editor::setCurrentFile(const QString &fileName)
 {
     curFile = fileName;
     textEdit->document()->setModified(false);
     setWindowModified(false);

     QString shownName = curFile;
     if (curFile.isEmpty())
         shownName = "untitled.txt";
     setWindowFilePath(shownName);
 }

