/* Begin CVS Header
   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/UI/SliderDialog.h,v $
   $Revision: 1.15 $
   $Name:  $
   $Author: gauges $ 
   $Date: 2005/02/16 14:10:27 $
   End CVS Header */

#ifndef SLIDER_DIALOG_H__
#define SLIDER_DIALOG_H__

#include "qdialog.h"
#include "copasi.h"
#include <vector>
#include <map>

class QScrollView;
class QHBox;
class QVBox;
class QListBox;
class QCheckBox;
class QPushButton;
class QString;
class QSlider;
class CCopasiObject;
class QLabel;
class QPopupMenu;
class DataModelGUI;
class CopasiSlider;
class CCopasiTask;

class SliderDialog: public QDialog
  {
    Q_OBJECT

  public:
    SliderDialog(QWidget* parent, DataModelGUI* dataModel);
    virtual ~SliderDialog();
    void addSlider(CopasiSlider* slider, C_INT32 folderId);
    void setCurrentFolderId(C_INT32 id);

  protected:
    QPushButton* runTaskButton;
    QPushButton* newSliderButton;
    QCheckBox* autoRunCheckBox;
    QCheckBox* mpAutoModifyRangesCheckBox;
    QScrollView* scrollView;
    QVBox* sliderBox;
    QPopupMenu* contextMenu;
    CopasiSlider* currSlider;
    std::map<C_INT32 , std::vector< CopasiSlider* > > sliderMap;
    std::map < C_INT32 , void(SliderDialog::*)() > taskMap;
    C_INT32 currentFolderId;

    C_INT32 mapFolderId2EntryId(C_INT32 folderId) const;

    DataModelGUI* mpDataModel;

    bool mSliderValueChanged;
    bool mSliderPressed;
    QLabel* mpDeactivatedLabel;

    void init();

    static C_INT32 numMappings;
    static C_INT32 folderMappings[][2];
    static C_INT32 knownTaskIDs[];
    static char* knownTaskNames[];
    static C_INT32 numKnownTasks;

    virtual void contextMenuEvent(QContextMenuEvent* e);

    virtual void runTimeCourse();
    virtual void closeEvent(QCloseEvent* e);

    virtual void setDataModel(DataModelGUI* dataModel);
    virtual void addSliderToTask(CopasiSlider* slider);
    virtual void deleteSliderFromTask(CopasiSlider* slider);
    virtual CCopasiTask* getTaskForFolderId(C_INT32 folderId);
    virtual void updateAllSliders();

  protected slots:
    void removeSlider();
    void editSlider();
    void createNewSlider();
    void runTask();
    void sliderValueChanged();
    void sliderReleased();
    void sliderPressed();
  };

#endif
