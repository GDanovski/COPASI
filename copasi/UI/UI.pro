######################################################################
# $Revision: 1.19 $ $Author: ssahle $ $Date: 2003/10/29 15:22:05 $  
######################################################################

include(../common.pri)

TEMPLATE = app

DEPENDPATH += .. 
INCLUDEPATH += ..

contains(BUILD_OS, WIN32) {
  COPASI_LIBS += ../lib/commandline.lib \
          ../lib/copasiXML.lib \
          ../lib/elementaryFluxModes.lib \
          ../lib/function.lib \
          ../lib/mathmodel.lib \
          ../lib/model.lib \
          ../lib/optimization.lib \
          ../lib/output.lib \
          ../lib/randomGenerator.lib \
          ../lib/report.lib \
          ../lib/scan.lib \
          ../lib/steadystate.lib \
          ../lib/trajectory.lib \
          ../lib/utilities.lib \
          ../lib/plot.lib

  LIBS += $$COPASI_LIBS
  TARGETDEPS += $$COPASI_LIBS
} else {
  # The order of objects is important 
  OBJECTS +=  ../lib/libelementaryFluxModes.a \
              ../lib/libcopasiXML.a \
              ../lib/libmathmodel.a \
              ../lib/libmodel.a \
              ../lib/libfunction.a \
              ../lib/liboutput.a \
              ../lib/libreport.a \
              ../lib/liboptimization.a \
              ../lib/libscan.a \
              ../lib/libsteadystate.a \
              ../lib/libtrajectory.a \
              ../lib/librandomGenerator.a \
              ../lib/libutilities.a \
              ../lib/liboutput.a \
              ../lib/libreport.a \
              ../lib/libfunction.a \
              ../lib/libmodel.a \
              ../lib/libplot.a 

  LIBS += -lexpat \
          -lqwt
  
  TARGETDEPS += ../lib/libcommandline.a \
                ../lib/libcopasiXML.a \
                ../lib/libelementaryFluxModes.a \
                ../lib/libfunction.a \
                ../lib/libmathmodel.a \
                ../lib/libmodel.a \
                ../lib/liboptimization.a \
                ../lib/liboutput.a \
                ../lib/librandomGenerator.a \
                ../lib/libreport.a \
                ../lib/libscan.a \
                ../lib/libsteadystate.a \
                ../lib/libtrajectory.a \
                ../lib/libutilities.a \
                ../lib/libplot.a

}

contains(BUILD_OS, SunOS) {
  LIBS += -lICE -ldl
}  

# Input
HEADERS += CompartmentsWidget.h \
           CompartmentsWidget1.h \
           CompartmentSymbols.h \
           ConstantSymbols.h \
           copasiui3window.h \
           copasiWidget.h \
           CReactionInterface.h \
           DataModel.h \
           DifferentialEquations.h \
           FixedMetaboliteSymbols.h \
           FunctionSymbols.h \
           FunctionWidget.h \
           FunctionWidget1.h \
           listviews.h \
           MetabolitesWidget.h \
           MetabolitesWidget1.h \
           MetaboliteSymbols.h \
           ModelWidget.h \
           ModesWidget.h \
           MoietiesWidget.h \
           MoietyWidget.h \
           MoietyWidget1.h \
           MyLineEdit.h \
           MyTable.h \
           ObjectBrowser.h \
           ObjectBrowserItem.h \
           parametertable.h \
           PlotWidget.h \
           ReactionsWidget.h \
           ReactionsWidget1.h \
           ScanItemWidget.h \
           ScanWidget.h \
           OptimizationWidget.h \
           FunctionItemWidget.h \
           OptimizationItemWidget.h \
           SteadyStateWidget.h \
           StretchTable.h \
           TrajectoryWidget.h \
           Tree.h \
           TableDefinition.h \
           TableDefinition1.h \
           CReportDefinitionSelect.h \
           objectdebug.h

# INTERFACES += CompartmentsWidget.ui \
#               FunctionWidget1.ui \
#               MetabolitesWidget1.ui \
#               MoietyWidget1.ui \
#               ObjectBrowser.ui \
#               objectdebug.ui \
#               ReactionsWidget1.ui \
#               SteadyStateWidget.ui \
#               TrajectoryWidget.ui

SOURCES += CompartmentsWidget.cpp \
           CompartmentsWidget1.cpp \
           CompartmentSymbols.cpp \
           ConstantSymbols.cpp \
           copasiui3window.cpp \
           copasiWidget.cpp \
           CReactionInterface.cpp \
           DataModel.cpp \
           DifferentialEquations.cpp \
           FixedMetaboliteSymbols.cpp \
           FunctionSymbols.cpp \
           FunctionWidget.cpp \
           FunctionWidget1.cpp \
           listviews.cpp \
           main.cpp \
           MetabolitesWidget.cpp \
           MetabolitesWidget1.cpp \
           MetaboliteSymbols.cpp \
           ModelWidget.cpp \
           ModesWidget.cpp \
           MoietiesWidget.cpp \
           MoietyWidget.cpp \
           MoietyWidget1.cpp \
           MyLineEdit.cpp \
           MyTable.cpp \
           ObjectBrowser.cpp \
           ObjectBrowserItem.cpp \
           parametertable.cpp \
           PlotWidget.cpp \
           ReactionsWidget.cpp \
           ReactionsWidget1.cpp \
           ScanItemWidget.cpp \
           ScanWidget.cpp \
           OptimizationWidget.cpp \
           FunctionItemWidget.cpp \
           OptimizationItemWidget.cpp \
           SteadyStateWidget.cpp \
           StretchTable.cpp \
           TrajectoryWidget.cpp \
           TableDefinition.cpp \
           TableDefinition1.cpp \
           CReportDefinitionSelect.cpp \
           objectdebug.cpp

