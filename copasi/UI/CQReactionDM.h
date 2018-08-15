// Copyright (C) 2017 - 2018 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and University of
// of Connecticut School of Medicine.
// All rights reserved.

// Copyright (C) 2010 - 2016 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

// Copyright (C) 2009 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., EML Research, gGmbH, University of Heidelberg,
// and The University of Manchester.
// All rights reserved.

#ifndef CQReactionDM_H
#define CQReactionDM_H

#include <vector>
#include <string>

#include "CQBaseDataModel.h"

#define COL_NAME_REACTIONS   1
#define COL_EQUATION         2
#define COL_RATE_LAW         3
#define COL_FLUX             4
#define COL_PARTICLE_FLUX    5
#ifdef WITH_SDE_SUPPORT
# define COL_NOISE_EXPRESSION 6
# define TOTAL_COLS_REACTIONS 7
#else
# define TOTAL_COLS_REACTIONS 6
#endif

class CReactionInterface;
class UndoReactionData;
class InsertReactionRowsCommand;
class CReaction;

class CQReactionDM : public CQBaseDataModel
{
  Q_OBJECT

  friend class ReactionDataChangeCommand;

public:
  CQReactionDM(QObject *parent, CDataModel * pDataModel);
  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole);
  bool removeRows(QModelIndexList rows, const QModelIndex &index = QModelIndex());

  //TODO Undo
  bool reactionDataChange(const std::string & key,
                          const QVariant &value,
                          int role,
                          QString &funcName,
                          std::vector<std::string>& createdObjects);

  void insertNewReactionRow(InsertReactionRowsCommand* command);
  void addReactionRow(CReaction *pReaction);
  void addReactionRow(UndoReactionData* pData);
  void deleteReactionRow(CReaction *pReaction);
  void deleteReactionRow(UndoReactionData* pData);
  bool updateReactionWithFunctionName(CReaction *pRea, QString &funcName);
  bool removeReactionRows(QModelIndexList rows, const QModelIndex&);
  bool insertReactionRows(QList <UndoReactionData *>& pReaData);
  void deleteReactionRows(QList <UndoReactionData *>& pReaData);
  bool removeAllReactionRows();
  bool clear();

protected:
  virtual bool insertRows(int position, int rows, const QModelIndex & source);
  virtual bool removeRows(int position, int rows);

private:
  void setEquation(const CReaction *pRea, const QVariant &value);

  QString mNewEquation;
  std::vector<std::string> mCreatedKeys;
};

#endif //CQReactionDM_H
