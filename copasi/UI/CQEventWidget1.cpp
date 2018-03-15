// Copyright (C) 2017 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and University of
// of Connecticut School of Medicine.
// All rights reserved.

// Copyright (C) 2010 - 2016 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

// Copyright (C) 2008 - 2009 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., EML Research, gGmbH, University of Heidelberg,
// and The University of Manchester.
// All rights reserved.

// Copyright (C) 2007 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

#include "CQEventWidget1.h"

#include <utility>
#include <vector>
#include <string.h>

#include "UI/qtUtilities.h"

#include "CQMessageBox.h"
#include "CCopasiSelectionDialog.h"
#include "resourcesUI/CQIconResource.h"

#include "CopasiDataModel/CDataModel.h"
#include "model/CModel.h"

#include "model/CEvent.h"
#include "report/CKeyFactory.h"
#include "utilities/CCopasiMessage.h"
#include "copasi/core/CRootContainer.h"

//UNDO framework classes
#include "undoFramework/DeleteEventCommand.h"
#include "undoFramework/CreateNewEventCommand.h"
#include "undoFramework/UndoEventData.h"
#include "undoFramework/UndoEventAssignmentData.h"
#include <copasi/undoFramework/EventChangeCommand.h>
#include "copasiui3window.h"

/*
 *  Constructs a CQEventWidget1 which is a child of 'parent', with the
 *  name 'name'.'
 */
CQEventWidget1::CQEventWidget1(QWidget * parent, const char * name)
  : CopasiWidget(parent, name)
  , mExpressionTriggerValid(false)
  , mExpressionDelayValid(false)
  , mExpressionEAValid(false)
  , mEAName("")
  , mChanged(false)
  , mpEvent(NULL)
  , mAssignmentKey("")
  , mCurrentTarget(C_INVALID_INDEX)
  , mAssignments()
  , mObjectCNToCopy()
{
  setupUi(this);

  init();

  CopasiUI3Window *  pWindow = dynamic_cast<CopasiUI3Window * >(parent->parent());
  setUndoStack(pWindow->getUndoStack());
}

/*
 *  Destroys the object and frees any allocated resources
 */
CQEventWidget1::~CQEventWidget1()
{
  // no need to delete child widgets, Qt does it all for us
}

/*! Slot to delete the active event widget */
void CQEventWidget1::slotBtnDelete()
{
  mpUndoStack->push(new DeleteEventCommand(this));
}

/// Slot to create a new event; activated whenever the New button is clicked
void CQEventWidget1::slotBtnNew()
{
  mpUndoStack->push(new CreateNewEventCommand(this));
}

void CQEventWidget1::slotBtnCopy()
{
  mObjectCNToCopy = mObjectCN;
}

/*! */
void CQEventWidget1::init()
{
  showDelayExpression(false);

  // SIGNAL-SLOT connections
  connect(mpComboBoxDelay, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChooseDelay(int)));
  connect(mpLBTarget, SIGNAL(currentRowChanged(int)), this, SLOT(slotActualizeAssignmentExpression(int)));

//  mpExpressionTrigger->mpExpressionWidget->setBoolean(true);
  mpExpressionTrigger->mpExpressionWidget->setBoolean(true);
  mExpressionDelayValid = true;
  mpExpressionDelay->mpExpressionWidget->setExpressionType(CQExpressionWidget::TransientExpression);

  mExpressionTriggerValid = false;
  //  mpExpressionTrigger->mpExpressionWidget->setExpressionType(CQSimpleSelectionTree::TRANSIENT_EXPRESSION);

  mExpressionEAValid = false;
  mpExpressionEA->mpExpressionWidget->setExpressionType(CQExpressionWidget::TransientExpression);

  // ----- correlated to GUI layout of the event assignment ----

  // hide the label and widget with respect to expression
  mpLabelEA->setEnabled(false);
  mpExpressionEA->setEnabled(false);

  // Set the icons for the buttons
  mpBtnSelectObject->setIcon(CQIconResource::icon(CQIconResource::copasi));
  mpBtnAddTarget->setIcon(CQIconResource::icon(CQIconResource::editAdd));
  mpBtnDeleteTarget->setIcon(CQIconResource::icon(CQIconResource::editDelete));
}

/*! Slot to add a new target without object target nor its expression
 *  Impact only on the appearance.
 */
void CQEventWidget1::slotAddTarget()
{
  CQSimpleSelectionTree::ObjectClasses Classes =
    CQSimpleSelectionTree::EventTarget;

  const CDataObject * pObject =
    CCopasiSelectionDialog::getObjectSingle(this, Classes);

  if (pObject == NULL) return;

  const CModelEntity * pME = dynamic_cast< const CModelEntity * >(pObject->getObjectParent());

  if (pME == NULL) return;

  mAssignments.add(new CEventAssignment(pME->getKey()), true);
  mpLBTarget->addItem(FROM_UTF8(pME->getObjectDisplayName()));

  mpLBTarget->setCurrentRow((int)(mAssignments.size() - 1));
}

/*! Slot to remove the active target from the appearance
 * Impact only on the appearance.
 */
void CQEventWidget1::slotDeleteTarget()
{
  if (mCurrentTarget > mAssignments.size() - 1 ||
      mAssignments.size() == 0) return;

  size_t ToBeDeleted = mCurrentTarget;

  QListWidgetItem * pItem = mpLBTarget->takeItem((int) ToBeDeleted);
  pdelete(pItem);

  mAssignments.remove(ToBeDeleted);

  mCurrentTarget = mpLBTarget->currentIndex().row();
}

int CQEventWidget1::getDelayTypeIndex(CEvent* event)
{
  int index = 0;

  if (event->getDelayExpression() == "")
    {
      index = 0; // Set Delay to "None"
    }
  else if (event->getDelayAssignment())
    {
      index = 2; // Assignment only
    }
  else
    {
      index = 1; // Calculation and Assignment
    }

  return index;
}

bool
CQEventWidget1::loadFromEvent()
{
  if (mpEvent == NULL) return false;

  const CModel * pModel =
    dynamic_cast< const CModel * >(mpEvent->getObjectAncestor("Model"));

  if (pModel == NULL) return false;

  // *** Expression of Trigger
  mpExpressionTrigger->mpExpressionWidget->setExpression(mpEvent->getTriggerExpression());
  mpExpressionTrigger->updateWidget();    // bring into view mode

  // *** Expression of Priority
  mpExpressionPriority->mpExpressionWidget->setExpression(mpEvent->getPriorityExpression());
  mpExpressionPriority->updateWidget();    // bring into view mode

  // *** Expression of Delay
  mpExpressionDelay->mpExpressionWidget->setExpression(mpEvent->getDelayExpression());
  mpExpressionDelay->updateWidget();    // bring into view mode

  // ** delay type
  mpComboBoxDelay->setCurrentIndex(getDelayTypeIndex(mpEvent));

  mpFireAtInitialTime->setChecked(mpEvent->getFireAtInitialTime());
  mpTriggerPersistent->setChecked(!mpEvent->getPersistentTrigger());

  // copy assignment from event
  CDataVectorN< CEventAssignment >::const_iterator it = mpEvent->getAssignments().begin();
  CDataVectorN< CEventAssignment >::const_iterator end = mpEvent->getAssignments().end();

  mIgnoreUpdates = true;
  mAssignments.clear();
  QStringList Targets;

  int ijk = 0;

  for (; it != end; ++it, ijk++)
    {
      const CModelEntity * pEntity = dynamic_cast< const CModelEntity * >(CObjectInterface::DataObject(mpDataModel->getObject(it->getTargetCN())));

      if (pEntity != NULL)
        {
          Targets.append(FROM_UTF8(pEntity->getObjectDisplayName()));
          mAssignments.add(*it);
        }
    }

  // fill the list box and the expression widget with correct assignments
  mpLBTarget->clear();
  mpLBTarget->insertItems(0, Targets);
  mIgnoreUpdates = false;

  size_t NewTarget = mCurrentTarget;

  if (mCurrentTarget == C_INVALID_INDEX &&
      mAssignments.size() > 0)
    {
      NewTarget = 0;
    }

  mpLBTarget->setCurrentRow((int) NewTarget);

  if (mAssignments.empty())
    {
      mpExpressionEA->mpExpressionWidget->clear();
      mpExpressionEA->updateWidget();
    }

  mChanged = false;

  return true;
}

/*! The slot to save all current values of the active event widget */
void CQEventWidget1::saveToEvent()
{
  // need to update object reference
  mpEvent = dynamic_cast<CEvent*>(mpObject);

  if (mpEvent == NULL) return;

  const CModel * pModel =
    dynamic_cast< const CModel * >(mpEvent->getObjectAncestor("Model"));

  if (pModel == NULL) return;

  mIgnoreUpdates = true;

  if (mpEvent->getTriggerExpression() != mpExpressionTrigger->mpExpressionWidget->getExpression())
    {
      mpUndoStack->push(
        new EventChangeCommand(
          CCopasiUndoCommand::EVENT_TRIGGER_EXPRESSION_CHANGE,
          FROM_UTF8(mpEvent->getTriggerExpression()),
          FROM_UTF8(mpExpressionTrigger->mpExpressionWidget->getExpression()),
          mpEvent,
          this
        )
      );

      mChanged = true;
    }

  if (mpEvent->getPriorityExpression() != mpExpressionPriority->mpExpressionWidget->getExpression())
    {
      mpUndoStack->push(
        new EventChangeCommand(
          CCopasiUndoCommand::EVENT_PRIORITY_EXPRESSION_CHANGE,
          FROM_UTF8(mpEvent->getPriorityExpression()),
          FROM_UTF8(mpExpressionPriority->mpExpressionWidget->getExpression()),
          mpEvent,
          this
        )
      );

      mChanged = true;
    }

  if (mpEvent->getDelayExpression() != mpExpressionDelay->mpExpressionWidget->getExpression())
    {
      mpUndoStack->push(
        new EventChangeCommand(
          CCopasiUndoCommand::EVENT_DELAY_EXPRESSION_CHANGE,
          FROM_UTF8(mpEvent->getDelayExpression()),
          FROM_UTF8(mpExpressionDelay->mpExpressionWidget->getExpression()),
          mpEvent,
          this
        )
      );

      mChanged = true;
    }

  if (mpComboBoxDelay->currentIndex() != getDelayTypeIndex(mpEvent))
    {
      mpUndoStack->push(
        new EventChangeCommand(
          CCopasiUndoCommand::EVENT_DELAY_TYPE_CHANGE,
          getDelayTypeIndex(mpEvent),
          mpComboBoxDelay->currentIndex(),
          mpEvent,
          this,
          mpEvent->getDelayExpression(),
          mpExpressionDelay->mpExpressionWidget->getExpression()
        )
      );
      mChanged = true;
    }

  if (mpEvent->getFireAtInitialTime() !=  mpFireAtInitialTime->isChecked())
    {
      mpUndoStack->push(
        new EventChangeCommand(
          CCopasiUndoCommand::EVENT_TRIGGER_INITIALTIME_CHANGE,
          mpEvent->getFireAtInitialTime(),
          mpFireAtInitialTime->isChecked(),
          mpEvent,
          this
        )
      );

      mChanged = true;
    }

  if (mpEvent->getPersistentTrigger() ==  mpTriggerPersistent->isChecked())
    {
      mpUndoStack->push(
        new EventChangeCommand(
          CCopasiUndoCommand::EVENT_TRIGGER_PERSISTENT_CHANGE,
          mpEvent->getPersistentTrigger(),
          !mpTriggerPersistent->isChecked(),
          mpEvent,
          this
        )
      );

      mChanged = true;
    }

  if (mCurrentTarget != C_INVALID_INDEX && mAssignments.size() > mCurrentTarget)
    {
      mAssignments[mCurrentTarget].setExpression(mpExpressionEA->mpExpressionWidget->getExpression());
    }

  // event assignments
  CDataVector< CEventAssignment >::const_iterator it = mAssignments.begin();
  CDataVector< CEventAssignment >::const_iterator end = mAssignments.end();

  CDataVectorN< CEventAssignment > & OldAssignments = mpEvent->getAssignments();
  size_t Found;

  // We first update all assignments.
  for (; it != end; ++it)
    {
      Found = OldAssignments.getIndex(it->getTargetCN());

      if (Found == C_INVALID_INDEX)
        {
          mpUndoStack->push(
            new EventChangeCommand(
              CCopasiUndoCommand::EVENT_ASSIGNMENT_ADDED,
              "",
              FROM_UTF8(it->getTargetCN()),
              mpEvent,
              this,
              it->getTargetCN(),
              it->getExpression()
            )
          );

          mChanged = true;
        }
      else if (OldAssignments[Found].getExpression() != it->getExpression())
        {
          mpUndoStack->push(
            new EventChangeCommand(
              CCopasiUndoCommand::EVENT_ASSIGNMENT_EXPRESSION_CHANGE,
              FROM_UTF8(OldAssignments[Found].getExpression()),
              FROM_UTF8(it->getExpression()),
              mpEvent,
              this,
              OldAssignments[Found].getObjectName(),
              OldAssignments[Found].getObjectName()
            )
          );

          mChanged = true;
        }
    }

  // Find the deleted assignments and mark them.
  CDataVectorN< CEventAssignment >::const_iterator itOld = OldAssignments.begin();
  CDataVectorN< CEventAssignment >::const_iterator endOld = OldAssignments.end();

  size_t DeleteCount = 0;

  if (OldAssignments.size() > mAssignments.size())
    {
      DeleteCount = OldAssignments.size() - mAssignments.size();
    }

  std::vector< std::pair < std::string, std::string > > ToBeDeleted;

  for (; itOld != endOld && DeleteCount > 0; ++itOld)
    {
      const std::string & CN = itOld->getTargetCN();

      for (it = mAssignments.begin(); it != end; ++it)
        {
          if (CN == it->getTargetCN()) break;
        }

      if (it == end)
        {
          ToBeDeleted.push_back(std::make_pair(CN, itOld->getExpression()));
          DeleteCount--;
          mChanged = true;
        }
    }

  // Delete the assignments marked to be deleted.
  std::vector< std::pair < std::string, std::string > >::const_iterator itDelete = ToBeDeleted.begin();
  std::vector< std::pair < std::string, std::string > >::const_iterator endDelete = ToBeDeleted.end();

  for (; itDelete != endDelete; ++itDelete)
    {
      mpUndoStack->push(new EventChangeCommand(
                          CCopasiUndoCommand::EVENT_ASSIGNMENT_REMOVED,
                          FROM_UTF8((*itDelete).first), // target key
                          "",
                          mpEvent,
                          this,
                          (*itDelete).second,  // expression
                          (*itDelete).first    // key
                        ));
    }

  mIgnoreUpdates = false;

  if (mChanged)
    {
      assert(mpDataModel != NULL);
      mpDataModel->changed();
      protectedNotify(ListViews::EVENT, ListViews::CHANGE, mObjectCN);
    }

  mChanged = false;
}

/*! The slot to update the active event widget */
bool CQEventWidget1::updateProtected(ListViews::ObjectType objectType, ListViews::Action action, const CCommonName & cn)
{

  switch (objectType)
    {
      case ListViews::MODEL:

        // For a new model we need to remove references to no longer existing metabolites
        if (action != ListViews::CHANGE)
          {
            mObjectCN.clear();
            mpObject = NULL;
            mpEvent = NULL;
          }

        break;

      case ListViews::EVENT:

        // If the currently displayed metabolite is deleted we need to remove its references.
        if (action == ListViews::DELETE && mObjectCN == cn)
          {
            mObjectCN.clear();
            mpObject = NULL;
            mpEvent = NULL;
          }

        break;

      case ListViews::STATE:
        break;

      default:
        return true;
        break;
    }

  if (isVisible() && !mIgnoreUpdates)
    enterProtected();

  return true;
}

/*! Function to interact with an object of class CEvent */
bool CQEventWidget1::enterProtected()
{
  bool success = true;

  if (mObjectCNToCopy != "")
    {
      CObjectInterface::ContainerList List;
      List.push_back(mpDataModel);

      // This will check the current data model and the root container for the object;
      mpEvent = dynamic_cast<CEvent *>(const_cast< CDataObject * >(CObjectInterface::DataObject(CObjectInterface::GetObjectFromCN(List, mObjectCNToCopy))));
      mObjectCNToCopy.clear();
    }
  else
    {
      mpEvent = dynamic_cast<CEvent*>(mpObject);
    }

  mCurrentTarget = C_INVALID_INDEX;

  if (mpEvent)
    {
      success =  loadFromEvent();
    }

  mpEvent = dynamic_cast<CEvent*>(mpObject);

  if (!success)
    {
      mpListView->switchToOtherWidget(116, std::string()); //TODO
    }

  return success;
}

/*! The slot to be done before leaving the active event widget */
bool CQEventWidget1::leave()
{
  // no saving if the dialog is not visible right now
  // (the changes would already have been saved the last time
  // it was visible and the pane was left)
  if (!isVisible()) return true;

  saveToEvent();

  return true;
}

/// Slot to select an object from the existing ones -only- for target.
void CQEventWidget1::slotSelectObject()
{
  if (mCurrentTarget == C_INVALID_INDEX)
    return slotAddTarget();

  CQSimpleSelectionTree::ObjectClasses Classes =
    CQSimpleSelectionTree::EventTarget;

  const CDataObject * pObject =
    CCopasiSelectionDialog::getObjectSingle(this, Classes);

  if (pObject == NULL) return;

  const CModelEntity * pME = dynamic_cast< const CModelEntity * >(pObject->getObjectParent());

  if (pME == NULL) return;

  if (mAssignments[mCurrentTarget].setTargetCN(pME->getCN()))
    {
      // If the target key change was successful we need to update the label.
      mpLBTarget->item((int) mCurrentTarget)->setText(FROM_UTF8(pME->getObjectDisplayName()));
    }
}

/// Slot to actualize the assignment expression widget of event assignment according to the target
void CQEventWidget1::slotActualizeAssignmentExpression(int index)
{
  if (mIgnoreUpdates) return;

  size_t NewTarget = (size_t) index;

  if (NewTarget != C_INVALID_INDEX &&
      NewTarget >= mAssignments.size())
    {
      NewTarget = mAssignments.size() - 1;
    }

  // Save the current assignment
  if (NewTarget != mCurrentTarget &&
      mCurrentTarget < mAssignments.size())
    {
      mAssignments[mCurrentTarget].setExpression(mpExpressionEA->mpExpressionWidget->getExpression());
    }

  mCurrentTarget = NewTarget;

  if (mCurrentTarget == C_INVALID_INDEX) // no target anymore
    {
      // hide the label and widget with respect to expression
      mpLabelEA->setEnabled(false);
      mpExpressionEA->setEnabled(false);

      return;
    }
  else
    {
      // show the label and widget with respect to expression
      mpLabelEA->setEnabled(true);
      mpExpressionEA->setEnabled(true);

      mpExpressionEA->mpExpressionWidget->setExpression(mAssignments[mCurrentTarget].getExpression());
      mpExpressionEA->updateWidget();
    }
}

/*! Function to apply the Delay Expression Widget */
void CQEventWidget1::showDelayExpression(bool show)
{
  if (show)
    {
      mpLabelDelayExpression->show();
      mpExpressionDelay->show();
    }
  else
    {
      mpLabelDelayExpression->hide();
      mpExpressionDelay->hide();
    }
}

//Slot to handle delay options
void CQEventWidget1::slotChooseDelay(int choice)
{
  if (choice != 0) // Not "None"
    {
      showDelayExpression(true);
    }
  else
    {
      showDelayExpression(false);
    }
}

//Undo methods
void CQEventWidget1::createNewEvent()
{

  // save the current setting values
  saveToEvent();

  // standard name
  std::string name = "event_1";

  // if the standard name already exists then creating the new event will fail
  // thus, a growing index will automatically be added to the standard name
  int i = 1;

  assert(mpDataModel != NULL);

  while (!mpDataModel->getModel()->createEvent(name))
    {
      i++;
      name = "event_";
      name += TO_UTF8(QString::number(i));
    }

  CCommonName CN = mpDataModel->getModel()->getEvents()[name].getCN();
  protectedNotify(ListViews::EVENT, ListViews::ADD, CN);
  mpListView->switchToOtherWidget(C_INVALID_INDEX, CN);
}

void CQEventWidget1::deleteEvent()
{
  mpListView->switchToOtherWidget(CCopasiUndoCommand::EVENTS, std::string());

  assert(mpDataModel != NULL);
  CModel * pModel = mpDataModel->getModel();
  assert(pModel != NULL);

  pModel->removeEvent(mpEvent);

  mpEvent = NULL;

  protectedNotify(ListViews::EVENT, ListViews::DELETE, mObjectCN);
}

void CQEventWidget1::deleteEvent(UndoEventData *pEventData)
{
  assert(mpDataModel != NULL);
  CModel * pModel = mpDataModel->getModel();
  assert(pModel != NULL);

  CEvent* pEvent = &pModel->getEvents()[pEventData->getName()];

  if (pEvent == NULL)
    return;

  std::string key = pEvent->getKey();
  pModel->removeEvent(key);
  mpEvent = NULL;

#undef DELETE
  protectedNotify(ListViews::EVENT, ListViews::DELETE, mObjectCN);
  protectedNotify(ListViews::EVENT, ListViews::DELETE, std::string());//Refresh all as there may be dependencies.

  switchToWidget(CCopasiUndoCommand::EVENTS);
}

void CQEventWidget1::addEvent(UndoEventData *pData)
{
  assert(mpDataModel != NULL);
  CModel * pModel = mpDataModel->getModel();
  assert(pModel != NULL);

  CDataObject* pEvent = pData->restoreObjectIn(pModel);

  if (pEvent == NULL)
    return;

  pData->setKey(pEvent->getKey());
  protectedNotify(ListViews::EVENT, ListViews::ADD, pEvent->getCN());

  mpListView->switchToOtherWidget(C_INVALID_INDEX, pEvent->getCN());
}

bool
CQEventWidget1::changeValue(const std::string &key,
                            CCopasiUndoCommand::Type type,
                            const QVariant &newValue,
                            const std::string &expression)
{
  /*
  if (!mIgnoreUpdates)
    {
      mKey = key;
      mpObject = CRootContainer::getKeyFactory()->get(key);
      mpEvent = dynamic_cast<CEvent*>(mpObject);
      loadFromEvent();

      mpListView->switchToOtherWidget(C_INVALID_INDEX, mKey);
    }

  switch (type)
    {
      case CCopasiUndoCommand::EVENT_DELAY_EXPRESSION_CHANGE:
        mpEvent->setDelayExpression(TO_UTF8(newValue.toString()));
        break;

      case CCopasiUndoCommand::EVENT_DELAY_TYPE_CHANGE:
        switch (newValue.toInt())
          {
            case 0:
              mpEvent->setDelayExpression("");
              mpEvent->setDelayAssignment(false);
              break;

            case 1:
              mpEvent->setDelayExpression(expression);
              mpEvent->setDelayAssignment(false);
              break;

            case 2:
              mpEvent->setDelayExpression(expression);
              mpEvent->setDelayAssignment(true);
              break;

            default:
              return false;
          }

        break;

      case CCopasiUndoCommand::EVENT_TRIGGER_EXPRESSION_CHANGE:
        mpEvent->setTriggerExpression(TO_UTF8(newValue.toString()));
        break;

      case CCopasiUndoCommand::EVENT_PRIORITY_EXPRESSION_CHANGE:
        mpEvent->setPriorityExpression(TO_UTF8(newValue.toString()));
        break;

      case CCopasiUndoCommand::EVENT_TRIGGER_INITIALTIME_CHANGE:
        mpEvent->setFireAtInitialTime(newValue.toBool());
        break;

      case CCopasiUndoCommand::EVENT_TRIGGER_PERSISTENT_CHANGE:
        mpEvent->setPersistentTrigger(newValue.toBool());
        break;

      case CCopasiUndoCommand::EVENT_ASSIGNMENT_ADDED:
      case CCopasiUndoCommand::EVENT_ASSIGNMENT_REMOVED:
      {
        // if newValue is empty, this means, that we want to remove the event
        // that targets the key in expression,
        const CDataObject * pTargetObject = dynamic_cast<const CDataObject *>(mpDataModel->getObject(expression));

        if (newValue.toString().isEmpty()
            && pTargetObject != NULL
            && mpEvent->getAssignments().getIndex(pTargetObject->getKey()) != C_INVALID_INDEX)
          {
            mpEvent->getAssignments().remove(pTargetObject->getKey());
          }
        else
          {
            std::string targetCN = TO_UTF8(newValue.toString());
            pTargetObject = dynamic_cast<const CDataObject *>(mpDataModel->getObject(targetCN));

            if (pTargetObject == NULL)
              return false;

            std::string targetKey = pTargetObject->getKey();
            CEventAssignment *assignment =
              new CEventAssignment(targetKey, mpEvent->getObjectParent());
            assignment->setExpression(expression);
            mpEvent->getAssignments().add(assignment, true);
          }

        break;
      }

      case CCopasiUndoCommand::EVENT_ASSIGNMENT_EXPRESSION_CHANGE:
      {
        std::string targetCN = expression;
        const CDataObject * pObject = dynamic_cast<const CDataObject *>(mpDataModel->getObject(targetCN));

        if (pObject == NULL)
          return false;

        std::string targetName = pObject->getObjectName();
        CDataVectorN< CEventAssignment > &assignments = mpEvent->getAssignments();
        CDataVectorN< CEventAssignment >::iterator it = assignments.begin();

        for (; it != assignments.end(); ++it)
          {
            const CDataObject* target = it->getTargetObject();

            if (target->getObjectParent()->getObjectName() != targetName)
              continue;

            it->setExpression(TO_UTF8(newValue.toString()));
            break;
          }

        break;
      }

      default:
        return false;
        break;
    }

  if (mIgnoreUpdates) return true;

  assert(mpDataModel != NULL);
  mpDataModel->changed();
  protectedNotify(ListViews::EVENT, ListViews::CHANGE, mKey);

  loadFromEvent();
  */
  return true;
}
