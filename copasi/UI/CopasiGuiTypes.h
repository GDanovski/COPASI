// Copyright (C) 2017 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and University of
// of Connecticut School of Medicine.
// All rights reserved.

#ifndef COPASI_GUI_TYPES_H
#define COPASI_GUI_TYPES_H

#include <copasi/UI/AboutDialog.h>
#include <copasi/UI/CBrowseModelVersionDialog.h>
#include <copasi/UI/CColorScale.h>
#include <copasi/UI/CCopasiPlotSelectionDialog.h>
#include <copasi/UI/CCopasiSelectionDialog.h>
#include <copasi/UI/CCopasiSelectionWidget.h>
#include <copasi/UI/CCreateModelVersionDialog.h>
#include <copasi/UI/CEntityProvenanceDialog.h>
#include <copasi/UI/ChemEqValidator.h>
#include <copasi/UI/CLNAResultSubwidget.h>
#include <copasi/UI/CLNAResultWidget.h>
#include <copasi/UI/CMCAResultSubwidget.h>
#include <copasi/UI/CMCAResultWidget.h>
#include <copasi/UI/CopasiFileDialog.h>
#include <copasi/UI/CopasiGuiTypes.h>
#include <copasi/UI/CopasiSlider.h>
#include <copasi/UI/copasiui3window.h>
#include <copasi/UI/copasiWidget.h>
#include <copasi/UI/CProgressBar.h>
#include <copasi/UI/CProvenanceDialog.h>
#include <copasi/UI/CQAnalyticsWidget.h>
#include <copasi/UI/CQArrayAnnotationsWidget.h>
#include <copasi/UI/CQArrayAnnotationsWidgetDM.h>
#include <copasi/UI/CQBaseDataModel.h>
#include <copasi/UI/CQBrowserPane.h>
#include <copasi/UI/CQBrowserPaneDM.h>
#include <copasi/UI/CQCheckModelWindow.h>
#include <copasi/UI/CQComboDelegate.h>
#include <copasi/UI/CQCompartment.h>
#include <copasi/UI/CQCompartmentCopyOptions.h>
#include <copasi/UI/CQCompartmentDM.h>
#include <copasi/UI/CQCompartmentsWidget.h>
#include <copasi/UI/CQCopasiApplication.h>
#include <copasi/UI/CQCrossSectionTaskWidget.h>
#include <copasi/UI/CQDateTimeEditDelegate.h>
#include <copasi/UI/CQDependenciesWidget.h>
#include <copasi/UI/CQDependencyDialog.h>
#include <copasi/UI/CQDependencyWidget.h>
#include <copasi/UI/CQDifferentialEquations.h>
#include <copasi/UI/CQEFMListWidget.h>
#include <copasi/UI/CQEFMNetReactionDM.h>
#include <copasi/UI/CQEFMReactionDM.h>
#include <copasi/UI/CQEFMResultWidget.h>
#include <copasi/UI/CQEFMSpeciesDM.h>
#include <copasi/UI/CQEFMWidget.h>
#include <copasi/UI/CQEventDM.h>
#include <copasi/UI/CQEventsWidget.h>
#include <copasi/UI/CQEventWidget1.h>
#include <copasi/UI/CQExpandModelData.h>
#include <copasi/UI/CQExperimentData.h>
#include <copasi/UI/CQExperimentSelection.h>
#include <copasi/UI/CQExpressionMmlStackedWidget.h>
#include <copasi/UI/CQExpressionWidget.h>
#include <copasi/UI/CQFittingItemWidget.h>
#include <copasi/UI/CQFittingResult.h>
#include <copasi/UI/CQFittingResultTab1.h>
#include <copasi/UI/CQFittingWidget.h>
#include <copasi/UI/CQFluxModeDM.h>
#include <copasi/UI/CQFunctionDM.h>
#include <copasi/UI/CQFunctionsWidget.h>
#include <copasi/UI/CQGlobalQuantitiesWidget.h>
#include <copasi/UI/CQGlobalQuantityDM.h>
#include <copasi/UI/CQLayoutsDM.h>
#include <copasi/UI/CQLayoutsWidget.h>
#include <copasi/UI/CQLNAWidget.h>
#include <copasi/UI/CQLyapResultWidget.h>
#include <copasi/UI/CQLyapWidget.h>
#include <copasi/UI/CQMathMatrixWidget.h>
#include <copasi/UI/CQMatrixDialog.h>
#include <copasi/UI/CQMCAWidget.h>
#include <copasi/UI/CQMergingData.h>
#include <copasi/UI/CQMessageBox.h>
#include <copasi/UI/CQMmlScrollView.h>
#include <copasi/UI/CQModelValue.h>
#include <copasi/UI/CQModelWidget.h>
#include <copasi/UI/CQMoietiesTaskResult.h>
#include <copasi/UI/CQMoietiesTaskWidget.h>
#include <copasi/UI/CQNameSelectionDialog.h>
#include <copasi/UI/CQNotes.h>
#include <copasi/UI/CQOptimizationResult.h>
#include <copasi/UI/CQOptimizationWidget.h>
#include <copasi/UI/CQOptPopulation.h>
#include <copasi/UI/CQOscillationTaskWidget.h>
#include <copasi/UI/CQParameterEstimationResult.h>
#include <copasi/UI/CQParameterGroupDM.h>
#include <copasi/UI/CQParameterGroupView.h>
#include <copasi/UI/CQParameterOverviewDM.h>
#include <copasi/UI/CQParameterOverviewWidget.h>
#include <copasi/UI/CQParameterSetsDM.h>
#include <copasi/UI/CQParameterSetsWidget.h>
#include <copasi/UI/CQPlotDM.h>
#include <copasi/UI/CQPlotsWidget.h>
#include <copasi/UI/CQPreferenceDialog.h>
#include <copasi/UI/CQProgressDialog.h>
#include <copasi/UI/CQProgressItem.h>
#include <copasi/UI/CQProgressItemBar.h>
#include <copasi/UI/CQProgressItemText.h>
#include <copasi/UI/CQPushButtonDelegate.h>
#include <copasi/UI/CQReactionDM.h>
#include <copasi/UI/CQReactionsWidget.h>
#include <copasi/UI/CQReportDefinition.h>
#include <copasi/UI/CQReportDefinitionSelect.h>
#include <copasi/UI/CQReportDM.h>
#include <copasi/UI/CQReportListItem.h>
#include <copasi/UI/CQReportsWidget.h>
#include <copasi/UI/CQSBMLFileDialog.h>
#include <copasi/UI/CQSEDMLFileDialog.h>
#include <copasi/UI/CQSensResultWidget.h>
#include <copasi/UI/CQSimpleSelectionTree.h>
#include <copasi/UI/CQSortFilterProxyModel.h>
#include <copasi/UI/CQSpecieDM.h>
#include <copasi/UI/CQSpeciesDetail.h>
#include <copasi/UI/CQSpeciesWidget.h>
#include <copasi/UI/CQSpinBoxDelegate.h>
#include <copasi/UI/CQSplashWidget.h>
#include <copasi/UI/CQStartValueReset.h>
#include <copasi/UI/CQSteadyStateResult.h>
#include <copasi/UI/CQTableView.h>
#include <copasi/UI/CQTabWidget.h>
#include <copasi/UI/CQTaskBtnWidget.h>
#include <copasi/UI/CQTaskHeaderWidget.h>
#include <copasi/UI/CQTaskMethodWidget.h>
#include <copasi/UI/CQTaskThread.h>
#include <copasi/UI/CQTextDialog.h>
#include <copasi/UI/CQThread.h>
#include <copasi/UI/CQTimeSeriesDM.h>
#include <copasi/UI/CQTimeSeriesWidget.h>
#include <copasi/UI/CQTrajectoryWidget.h>
#include <copasi/UI/CQTSSAResultSubWidget.h>
#include <copasi/UI/CQTSSAResultWidget.h>
#include <copasi/UI/CQTSSATimeScaleWidget.h>
#include <copasi/UI/CQTSSAWidget.h>
#include <copasi/UI/CQUnitDetail.h>
#include <copasi/UI/CQUnitDM.h>
#include <copasi/UI/CQUnitsWidget.h>
#include <copasi/UI/CQUpdateAvogadro.h>
#include <copasi/UI/CQUpdatesWidget.h>
#include <copasi/UI/CQValidator.h>
#include <copasi/UI/CQValidatorUnit.h>
#include <copasi/UI/CScanContainerWidget.h>
#include <copasi/UI/CScanItemData.h>
#include <copasi/UI/CScanWidgetRandom.h>
#include <copasi/UI/CScanWidgetRepeat.h>
#include <copasi/UI/CScanWidgetScan.h>
#include <copasi/UI/CScanWidgetTask.h>
#include <copasi/UI/CUpDownSubwidget.h>
#include <copasi/UI/CWindowInterface.h>
#include <copasi/UI/DataModel.txt.h>
#include <copasi/UI/DataModelGUI.h>
#include <copasi/UI/DefaultplotDialog.h>
#include <copasi/UI/FunctionWidget1.h>
#include <copasi/UI/listviews.h>
#include <copasi/UI/MyLineEdit.h>
#include <copasi/UI/ObjectBrowserDialog.h>
#include <copasi/UI/ObjectBrowserItem.h>
#include <copasi/UI/ObjectBrowserWidget.h>
#include <copasi/UI/parametertable.h>
#include <copasi/UI/qtUtilities.h>
#include <copasi/UI/ReactionsWidget1.h>
#include <copasi/UI/ScanWidget.h>
#include <copasi/UI/SensitivitiesWidget.h>
#include <copasi/UI/SensWidgetComboBox.h>
#include <copasi/UI/SliderDialog.h>
#include <copasi/UI/SliderSettingsDialog.h>
#include <copasi/UI/StateSubwidget.h>
#include <copasi/UI/SteadyStateWidget.h>
#include <copasi/UI/TaskWidget.h>

#endif // COPASI_GUI_TYPES_H
