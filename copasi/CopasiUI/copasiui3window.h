#include <qmainwindow.h>
#include <qtoolbar.h>

#include "copasi.h"
#include "listviews.h" 
//#include "model/model.h"

class QToolButton;
class CopasiUI3Window : public QMainWindow
  {
    Q_OBJECT

  public:
    CopasiUI3Window();

  protected:

    DataModel<Folder>* dataModel; // to keep track of the data model..
    QSplitter *splitter; // to hold different views...
    ListViews *listViews; // to create different list views...

  protected slots:
    void slotFileOpen();
    void slotFileSave();
    void slotFileSaveAs();
    void newDoc();
    void slotFilePrint();
    void slotImportSBML();
    void slotExportSBML();
    void slotObjectBrowser();
    void about();
    void aboutQt();

  private:
    QString gpsFile;
    QToolButton * msave_button;
    QPopupMenu * file;
    int nsave_menu_id;
    int nsaveas_menu_id;
    int nexport_menu_SBML;
    void createToolBar();
    void createMenuBar();
  };
