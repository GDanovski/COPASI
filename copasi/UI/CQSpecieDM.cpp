// Copyright (C) 2010 - 2016 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

// Copyright (C) 2009 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., EML Research, gGmbH, University of Heidelberg,
// and The University of Manchester.
// All rights reserved.

#include <QtCore/QString>

#include "CopasiDataModel/CCopasiDataModel.h"
#include "report/CCopasiRootContainer.h"
#include "model/CChemEqInterface.h"
#include "function/CExpression.h"

#include "CQMessageBox.h"
#include "CQSpecieDM.h"
#include "qtUtilities.h"

#include "model/CReaction.h"
#include "model/CMetab.h"
#include "model/CReactionInterface.h"
#include "undoFramework/InsertSpeciesRowsCommand.h"
#include "undoFramework/RemoveSpeciesRowsCommand.h"
#include "undoFramework/RemoveAllSpeciesRowsCommand.h"
#include "undoFramework/SpeciesDataChangeCommand.h"
#include "undoFramework/UndoSpeciesData.h"
#include "undoFramework/UndoReactionData.h"
#include "undoFramework/UndoGlobalQuantityData.h"
#include "undoFramework/UndoEventData.h"
#include "undoFramework/UndoEventAssignmentData.h"

CQSpecieDM::CQSpecieDM(QObject *parent):
  CQBaseDataModel(parent),
  mFlagConc(true),
  mpSpecies(NULL),
  mNotify(true)
{
  mTypes.push_back(FROM_UTF8(CModelEntity::StatusName[CModelEntity::REACTIONS]));
  mTypes.push_back(FROM_UTF8(CModelEntity::StatusName[CModelEntity::FIXED]));
  mTypes.push_back(FROM_UTF8(CModelEntity::StatusName[CModelEntity::ASSIGNMENT]));
  mTypes.push_back(FROM_UTF8(CModelEntity::StatusName[CModelEntity::ODE]));

  mItemToType.push_back(CModelEntity::REACTIONS);
  mItemToType.push_back(CModelEntity::FIXED);
  mItemToType.push_back(CModelEntity::ASSIGNMENT);
  mItemToType.push_back(CModelEntity::ODE);
}

const QStringList& CQSpecieDM::getTypes()
{
  return mTypes;
}

void CQSpecieDM::setFlagConc(bool flag)
{
  mFlagConc = flag;
}

const std::vector< unsigned C_INT32 >& CQSpecieDM::getItemToType()
{
  return mItemToType;
}

int CQSpecieDM::rowCount(const QModelIndex&) const
{
  return CCopasiRootContainer::getDatamodelList()->operator[](0).getModel()->getMetabolites().size() + 1;
}

int CQSpecieDM::columnCount(const QModelIndex&) const
{
  return TOTAL_COLS_SPECIES;
}

Qt::ItemFlags CQSpecieDM::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  if (isDefaultRow(index))
    {
      if (index.column() == COL_NAME_SPECIES || index.column() == COL_COMPARTMENT ||
          index.column() == COL_TYPE_SPECIES || index.column() == COL_ICONCENTRATION ||
          index.column() == COL_INUMBER)
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
      else
        return QAbstractItemModel::flags(index);
    }

  if (index.column() == COL_NAME_SPECIES || index.column() == COL_COMPARTMENT ||
      index.column() == COL_TYPE_SPECIES)
    {
      return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    }
  else if (index.column() == COL_ICONCENTRATION)
    {
      mpSpecies = &CCopasiRootContainer::getDatamodelList()->operator[](0).getModel()->getMetabolites()[index.row()];

      if (this->index(index.row(), COL_TYPE_SPECIES).data().toString() == QString(FROM_UTF8(CModelEntity::StatusName[CModelEntity::ASSIGNMENT]))
          || !(this->index(index.row(), COL_IEXPRESSION_SPECIES).data().toString().isEmpty()))
        return QAbstractItemModel::flags(index) & ~Qt::ItemIsEnabled;
      else
        {
          if (mpSpecies->isInitialConcentrationChangeAllowed())
            return QAbstractItemModel::flags(index)  | Qt::ItemIsEditable | Qt::ItemIsEnabled;
          else
            return QAbstractItemModel::flags(index) & ~Qt::ItemIsEditable & ~Qt::ItemIsEnabled;
        }
    }
  else if (index.column() == COL_INUMBER)
    {
      if (this->index(index.row(), COL_TYPE_SPECIES).data() == QString(FROM_UTF8(CModelEntity::StatusName[CModelEntity::ASSIGNMENT]))
          || !(this->index(index.row(), COL_IEXPRESSION_SPECIES).data().toString().isEmpty()))
        return QAbstractItemModel::flags(index) & ~Qt::ItemIsEditable & ~Qt::ItemIsEnabled;
      else
        return QAbstractItemModel::flags(index)  | Qt::ItemIsEditable | Qt::ItemIsEnabled;
    }
  else
    return QAbstractItemModel::flags(index);
}

QVariant CQSpecieDM::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (index.row() >= rowCount())
    return QVariant();

  if (index.column() > 0 && role == Qt::ForegroundRole && !(flags(index) & Qt::ItemIsEditable))
    return QColor(Qt::darkGray);

  if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
      if (isDefaultRow(index))
        {
          switch (index.column())
            {
              case COL_ROW_NUMBER:
                return QVariant(QString(""));

              case COL_NAME_SPECIES:
                return QVariant(QString("New Species"));

              case COL_COMPARTMENT:
              {
                const CCopasiVector < CCompartment > & compartments =
                  CCopasiRootContainer::getDatamodelList()->operator[](0).getModel()->getCompartments();

                if (compartments.size())
                  return QVariant(QString(FROM_UTF8(compartments[0].getObjectName())));
                else
                  return QVariant(QString(""));
              }

              case COL_TYPE_SPECIES:
                return QVariant(QString(FROM_UTF8(CModelEntity::StatusName[mItemToType[0]])));

              case COL_ICONCENTRATION:
              {
                if (mFlagConc)
                  return QVariant(QString::number(1.0, 'g', 10));
                else
                  return QVariant(QString(""));
              }

              case COL_INUMBER:
              {
                if (mFlagConc)
                  return QVariant(QString(""));
                else
                  return QVariant(QString::number(100.0, 'g', 10));
              }

              default:
                return QVariant(QString(""));
            }
        }
      else
        {
          mpSpecies = &CCopasiRootContainer::getDatamodelList()->operator[](0).getModel()->getMetabolites()[index.row()];

          switch (index.column())
            {
              case COL_ROW_NUMBER:
                return QVariant(index.row() + 1);

              case COL_NAME_SPECIES:
                return QVariant(QString(FROM_UTF8(mpSpecies->getObjectName())));

              case COL_COMPARTMENT:
                return QVariant(QString(FROM_UTF8(mpSpecies->getCompartment()->getObjectName())));

              case COL_TYPE_SPECIES:
                return QVariant(QString(FROM_UTF8(CModelEntity::StatusName[mpSpecies->getStatus()])));

              case COL_ICONCENTRATION:
              {
                if (role == Qt::EditRole)
                  return QVariant(QString::number(mpSpecies->getInitialConcentration(), 'g', 10));
                else
                  return QVariant(mpSpecies->getInitialConcentration());
              }

              case COL_INUMBER:
              {
                if (role == Qt::EditRole)
                  return QVariant(QString::number(mpSpecies->getInitialValue(), 'g', 10));
                else
                  return QVariant(mpSpecies->getInitialValue());
              }

              case COL_CONCENTRATION:
                return QVariant(mpSpecies->getConcentration());

              case COL_NUMBER:
                return QVariant(mpSpecies->getValue());

              case COL_CRATE:
                return QVariant(mpSpecies->getConcentrationRate());

              case COL_NRATE:
                return QVariant(mpSpecies->getRate());

              case COL_IEXPRESSION_SPECIES:
              {
                const CExpression * pExpression = NULL;

                if (mpSpecies->getInitialExpression() != "")
                  {
                    pExpression = mpSpecies->getInitialExpressionPtr();

                    if (pExpression != NULL)
                      return QVariant(QString(FROM_UTF8(pExpression->getDisplayString())));
                  }

                return QVariant(QString(""));
              }

              case COL_EXPRESSION_SPECIES:
              {
                const CExpression * pExpression = mpSpecies->getExpressionPtr();

                if (pExpression != NULL)
                  return QVariant(QString(FROM_UTF8(pExpression->getDisplayString())));
                else
                  return QVariant(QString(""));
              }
            }
        }
    }

  return QVariant();
}

QVariant CQSpecieDM::headerData(int section, Qt::Orientation orientation,
                                int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal)
    {
      const CModel * pModel = CCopasiRootContainer::getDatamodelList()->operator[](0).getModel();

      if (pModel == NULL) return QVariant();

      std::string ValueUnit, RateUnit, FrequencyUnit, ExpressionUnit;
      QString ValueUnits, RateUnits, FrequencyUnits, ExpressionUnits;

      ValueUnit = (pModel != NULL) ? CUnit::prettyPrint(pModel->getQuantityUnit() + "/(" + pModel->getVolumeUnit() + ")") : "?";
      ValueUnits = "\n[" + FROM_UTF8(ValueUnit) + "]";

      RateUnit = (pModel != NULL) ? CUnit::prettyPrint(pModel->getQuantityUnit() + "/(" + pModel->getVolumeUnit() + "*" + pModel->getTimeUnit() + ")") : "?";
      RateUnits = "\n[" + FROM_UTF8(RateUnit) + "]";

      FrequencyUnit = (pModel != NULL) ? CUnit::prettyPrint("1/(" + pModel->getTimeUnit() + ")") : "?";
      FrequencyUnits = "\n[" + FROM_UTF8(FrequencyUnit) + "]";

      if (ValueUnit == RateUnit)
        ExpressionUnits = ValueUnits;
      else
        ExpressionUnits = "\n[" + FROM_UTF8(ValueUnit) + "] or [" + FROM_UTF8(RateUnit) + "]";

      switch (section)
        {
          case COL_ROW_NUMBER:
            return QVariant(QString("#"));

          case COL_NAME_SPECIES:
            return QVariant(QString("Name"));

          case COL_COMPARTMENT:
            return QVariant(QString("Compartment"));

          case COL_TYPE_SPECIES:
            return QVariant(QString("     Type     "));

          case COL_ICONCENTRATION:
            return QVariant("Initial Concentration" + ValueUnits);

          case COL_INUMBER:
            return QVariant(QString("Initial Number"));

          case COL_CONCENTRATION:
            return QVariant("Concentration" + ValueUnits);

          case COL_NUMBER:
            return QVariant(QString("Number"));

          case COL_CRATE:
            return QVariant("Rate" + RateUnits);

          case COL_NRATE:
            return QVariant("Number Rate" + FrequencyUnits);

          case COL_IEXPRESSION_SPECIES:
            return QVariant("Initial Expression" + ValueUnits);

          case COL_EXPRESSION_SPECIES:
            return QVariant("Expression" + ExpressionUnits);

          default:
            return QVariant();
        }
    }
  else
    return QString("%1").arg(section + 1);
}

bool CQSpecieDM::setData(const QModelIndex &index, const QVariant &value,
                         int role)
{
  //change is only accepted if the new value is different from the old value and also the old value is not equal to "New Species" for the 'name' column
  // in that case no new species will be created!
  if (index.data() == value)
    return false;

  if (index.column() == COL_TYPE_SPECIES &&
      index.data().toString() == QString(FROM_UTF8(CModelEntity::StatusName[mItemToType[value.toInt()]])))
    return false;

  if (index.column() == COL_COMPARTMENT && value == "")
    return false;

  bool defaultRow = isDefaultRow(index);

  if (defaultRow)
    {
      mpUndoStack->push(new InsertSpecieRowsCommand(rowCount(), 1, this, index, value));
    }
  else
    {
      mpUndoStack->push(new SpecieDataChangeCommand(mpSpecies, value, index.data(), index.column(), this));
    }

  return true;
}

bool CQSpecieDM::insertRows(int position, int rows, const QModelIndex&)
{
  mpUndoStack->push(new InsertSpecieRowsCommand(position, rows, this));

  return true;
}

bool CQSpecieDM::removeRows(int position, int rows)
{
  if (rows <= 0)
    return true;

  beginRemoveRows(QModelIndex(), position, position + rows - 1);

  CModel * pModel = CCopasiRootContainer::getDatamodelList()->operator[](0).getModel();

  std::vector< std::string > DeletedKeys;
  DeletedKeys.resize(rows);

  std::vector< std::string >::iterator itDeletedKey;
  std::vector< std::string >::iterator endDeletedKey = DeletedKeys.end();

  CCopasiVector< CMetab >::const_iterator itRow = pModel->getMetabolites().begin() + position;

  for (itDeletedKey = DeletedKeys.begin(); itDeletedKey != endDeletedKey; ++itDeletedKey, ++itRow)
    {
      *itDeletedKey = itRow->getKey();
    }

  for (itDeletedKey = DeletedKeys.begin(); itDeletedKey != endDeletedKey; ++itDeletedKey)
    {
      pModel->removeMetabolite(*itDeletedKey);
      emit notifyGUI(ListViews::METABOLITE, ListViews::DELETE, *itDeletedKey);
      emit notifyGUI(ListViews::METABOLITE, ListViews::DELETE, ""); //Refresh all as there may be dependencies.
    }

  endRemoveRows();

  return true;
}

bool CQSpecieDM::removeRows(QModelIndexList rows, const QModelIndex&)
{
  mpUndoStack->push(new RemoveSpecieRowsCommand(rows, this, QModelIndex()));

  return true;
}

bool CQSpecieDM::specieDataChange(
  UndoSpeciesData *pUndoSpeciesData,
  const QVariant &value,
  int column)
{
  switchToWidget(CCopasiUndoCommand::SPECIES);

  GET_MODEL_OR(pModel, return false);

  mpSpecies =
    dynamic_cast<CMetab*>(pUndoSpeciesData->getObject(pModel));

  if (mpSpecies == NULL)
    return false;

  const CCompartment * pCompartment = NULL;

  if (column == COL_COMPARTMENT ||
      column == COL_ICONCENTRATION ||
      column == COL_INUMBER)
    {
      try
        {
          pCompartment = mpSpecies->getCompartment();
        }
      catch (...) {}
    }

  if (column == COL_NAME_SPECIES)
    {
      mpSpecies->setObjectName(TO_UTF8(value.toString()));
      pUndoSpeciesData->setCN(mpSpecies->getCN());
    }
  else if (column == COL_COMPARTMENT)
    {
      // This must be set first for setInitialConcentration and
      // setInitialNumber to work correctly.
      std::string Compartment(TO_UTF8(value.toString()));

      if (Compartment != pCompartment->getObjectName())
        {
          std::string CompartmentToRemove = mpSpecies->getCompartment()->getObjectName();

          if (!(pModel->getCompartments()[Compartment].addMetabolite(mpSpecies)))
            {
              QString msg;
              msg = "Unable to move species '" + FROM_UTF8(mpSpecies->getObjectName()) + "'\n"
                    + "from compartment '" + FROM_UTF8(CompartmentToRemove) + "' to compartment '" + FROM_UTF8(Compartment) + "'\n"
                    + "since a species with that name already exist in the target compartment.";

              CQMessageBox::information(NULL,
                                        "Unable to move Species",
                                        msg,
                                        QMessageBox::Ok, QMessageBox::Ok);
              return false;
            }
          else
            {
              pModel->getCompartments()[CompartmentToRemove].getMetabolites().remove(mpSpecies->getObjectName());
              pModel->setCompileFlag();
              pModel->initializeMetabolites();

              if (mpSpecies && pCompartment)
                {
                  C_FLOAT64 Factor = 1.0 / pCompartment->getInitialValue();
                  Factor *= pCompartment->getInitialValue();

                  mpSpecies->setInitialValue(Factor * pUndoSpeciesData->getINumber());
                  mpSpecies->setValue(Factor * mpSpecies->getValue());
                }

              emit notifyGUI(ListViews::METABOLITE, ListViews::CHANGE, mpSpecies->getKey());
              emit notifyGUI(ListViews::COMPARTMENT, ListViews::CHANGE, pCompartment->getKey());
            }
        }
    }
  else if (column == COL_TYPE_SPECIES)
    mpSpecies->setStatus((CModelEntity::Status) mItemToType[value.toInt()]);
  else if (column == COL_ICONCENTRATION)
    {
      if (mFlagConc)
        mpSpecies->setInitialConcentration(value.toDouble());

      if (mpSpecies && pCompartment)
        {
          const C_FLOAT64 initialValue =
            CMetab::convertToNumber(pUndoSpeciesData->getIConc(),
                                    *pCompartment,
                                    *pModel);
          mpSpecies->setInitialValue(initialValue);
        }
    }
  else if (column == COL_INUMBER)
    {
      if (!mFlagConc)
        mpSpecies->setInitialValue(value.toDouble());

      if (mpSpecies && pCompartment)
        {
          mpSpecies->setInitialConcentration(
            CMetab::convertToConcentration(pUndoSpeciesData->getINumber(),
                                           *pCompartment,
                                           *pModel)
          );
        }
    }

  //Save Key
  std::string key = mpSpecies->getKey();

  // ask for refresh this may change the key!
  QModelIndex index = getIndexFor(mpSpecies, column);
  emit dataChanged(index, index);

  if (column == COL_NAME_SPECIES)
    {
      emit notifyGUI(ListViews::METABOLITE, ListViews::RENAME, key);
    }
  else
    {
      emit notifyGUI(ListViews::METABOLITE, ListViews::CHANGE, key);
    }

  return true;
}

QList <UndoSpeciesData *> CQSpecieDM::insertNewSpecieRow(int position, int rows, const QModelIndex&index, const QVariant& value)
{
  QList <UndoSpeciesData *> result;
  GET_MODEL_OR(pModel, return result);

  bool createdCompartment = false;

  if (pModel->getCompartments().size() == 0)
    {
      CCompartment* pComp = pModel->createCompartment("compartment");
      createdCompartment = true;

      if (mNotify)
        {
          emit notifyGUI(ListViews::COMPARTMENT, ListViews::ADD, pComp->getKey());
        }
    }

  beginInsertRows(QModelIndex(), position, position + rows - 1);

  int column = index.column();

  for (int row = 0; row < rows; ++row)
    {
      QString name = createNewName(index.isValid() && column == COL_NAME_SPECIES ? value.toString() : "species", COL_NAME_SPECIES);

      QString compartment = index.isValid() && column == COL_COMPARTMENT ? value.toString() : "";

      double initial = index.isValid() && column == COL_ICONCENTRATION ? value.toDouble() : 1.0;

      CModelEntity::Status status = index.isValid() && column == COL_TYPE_SPECIES ?
                                    (CModelEntity::Status) mItemToType[value.toInt()] : CModelEntity::REACTIONS;

      mpSpecies =
        pModel->createMetabolite(TO_UTF8(name), TO_UTF8(compartment), initial, status);

      if (mpSpecies == NULL)
        continue;

      if (mNotify)
        {
          emit notifyGUI(ListViews::METABOLITE, ListViews::ADD, mpSpecies->getKey());
        }

      UndoSpeciesData* data = new UndoSpeciesData(mpSpecies);
      data->setCreatedCompartment(row == 0 && createdCompartment);
      result.append(data);
    }

  endInsertRows();
  return result;
}

void CQSpecieDM::deleteSpecieRow(UndoSpeciesData *pSpecieData)
{
  GET_MODEL_OR_RETURN(pModel);

  switchToWidget(CCopasiUndoCommand::SPECIES);

  CMetab * pSpecies = dynamic_cast< CMetab * >(pSpecieData->getObject(pModel));

  if (pSpecies == NULL) return;

  size_t Index = pModel->getMetabolites().getIndex(pSpecies);

  removeRow((int) Index);

  if (!pSpecieData->getCreatedCompartment()) return;

  Index = pModel->getCompartments().getIndex(pSpecieData->getCompartment());

  if (Index == C_INVALID_INDEX) return;

  CCompartment* pComp = &pModel->getCompartments()[Index];

  if (pComp == NULL) return;

  std::string key = pComp->getKey();
  pModel->removeCompartment(Index);
  emit notifyGUI(ListViews::COMPARTMENT, ListViews::DELETE, key);
}

void CQSpecieDM::addSpecieRow(UndoSpeciesData *pSpecieData)
{
  GET_MODEL_OR_RETURN(pModel);

  switchToWidget(CCopasiUndoCommand::SPECIES);

  CMetab *species =  pSpecieData->restoreObjectIn(pModel);

  if (species == NULL)
    return;

  beginInsertRows(QModelIndex(), 1, 1);
  emit notifyGUI(ListViews::METABOLITE, ListViews::ADD, species->getKey());
  endInsertRows();
}

bool CQSpecieDM::removeSpecieRows(QModelIndexList rows, const QModelIndex&)
{
  if (rows.isEmpty())
    return false;

  GET_MODEL_OR(pModel, return false);

  switchToWidget(CCopasiUndoCommand::SPECIES);

  //Build the list of pointers to items to be deleted
  //before actually deleting any item.
  QList <CMetab *> pSpecies;
  QModelIndexList::const_iterator i;

  for (i = rows.begin(); i != rows.end(); ++i)
    {
      if (!isDefaultRow(*i) && &pModel->getMetabolites()[i->row()])
        pSpecies.append(&pModel->getMetabolites()[i->row()]);
    }

  QList <CMetab *>::const_iterator j;

  for (j = pSpecies.begin(); j != pSpecies.end(); ++j)
    {
      CMetab * pSpecie = *j;

      size_t delRow =
        pModel->getMetabolites().CCopasiVector< CMetab >::getIndex(pSpecie);

      if (delRow == C_INVALID_INDEX)
        continue;

      QMessageBox::StandardButton choice =
        CQMessageBox::confirmDelete(NULL, "species",
                                    FROM_UTF8(pSpecie->getObjectName()),
                                    pSpecie->getDeletedObjects());

      if (choice == QMessageBox::Ok)
        removeRow((int) delRow);
    }

  return true;
}

bool CQSpecieDM::insertSpecieRows(QList <UndoSpeciesData *>& pData)
{
  GET_MODEL_OR(pModel, return false);

  //reinsert all the species
  QList <UndoSpeciesData *>::const_iterator i;

  for (i = pData.begin(); i != pData.end(); ++i)
    {
      UndoSpeciesData * data = *i;
      CMetab *pSpecies = data->restoreObjectIn(pModel);

      if (pSpecies == NULL)
        continue;

      beginInsertRows(QModelIndex(), 1, 1);
      emit notifyGUI(ListViews::METABOLITE, ListViews::ADD, pSpecies->getKey());
      endInsertRows();
    }

  switchToWidget(CCopasiUndoCommand::SPECIES);

  return true;
}

void CQSpecieDM::deleteSpecieRows(QList <UndoSpeciesData *>& pData)
{
  GET_MODEL_OR_RETURN(pModel);

  switchToWidget(CCopasiUndoCommand::SPECIES);

  QList <UndoSpeciesData *>::const_iterator j;

  for (j = pData.begin(); j != pData.end(); ++j)
    {
      deleteSpecieRow(*j);
    }
}

bool CQSpecieDM::clear()
{
  mpUndoStack->push(new RemoveAllSpecieRowsCommand(this, QModelIndex()));
  return true;
}

QModelIndex CQSpecieDM::getIndexFor(const CMetab *pMetab, int column) const
{
  int max = rowCount();

  CCopasiVector< CMetab >::const_iterator it = CCopasiRootContainer::getDatamodelList()->operator[](0).getModel()->getMetabolites().begin();

  for (int i = 0; i < max; ++i, ++it)
    {
      if (&*it == pMetab)
        return index(i, column);
    }

  return QModelIndex();
}

bool CQSpecieDM::removeAllSpecieRows()
{
  return removeRows(0, rowCount() - 1);
}
