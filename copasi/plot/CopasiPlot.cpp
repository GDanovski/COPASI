/* Begin CVS Header
   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/plot/Attic/CopasiPlot.cpp,v $
   $Revision: 1.11 $
   $Name:  $
   $Author: ssahle $ 
   $Date: 2004/06/20 19:57:36 $
   End CVS Header */

#include <qarray.h>
#include <qevent.h>
#include <qstring.h>
#include <qcolor.h>   //might need to go to the header file

#include <qpainter.h>
#include <qwt_plot_canvas.h>
#include <qwt_curve.h>
#include <qwt_scale.h>

#include "CopasiPlot.h"
#include "CPlotSpec.h"
#include "CopasiUI/qtUtilities.h"

//TODO: put all the stuff to locally store the data in a separate class!

void CopasiPlot::createIndices(const CPlotSpec* pspec)
{
  C_INT32 i, imax = pspec->getCurves().size();
  C_INT32 jj;
  C_INT32 index;
  std::vector<C_INT32>::iterator it; // iterator for indexTable
  C_INT32 iterindex;
  dataIndices.resize(imax);

  for (i = 0; i < imax; ++i) //all curves
    {
      dataIndices[i].resize(2);

      for (jj = 0; jj < 2; ++jj) //2D
        {
          index = pspec->getCurves()[i].mChannels[jj].index;
          for (it = indexTable.begin(), iterindex = 0; it != indexTable.end(); ++it, ++iterindex)
          {if (*it >= index) break;};
          if (it == indexTable.end()) //index is not yet in indexTable
            indexTable.insert(it, index);
          else if (*it != index)
            indexTable.insert(it, index);
          dataIndices[i][jj] = iterindex;
        }
    }
}

CopasiPlot::CopasiPlot(const CPlotSpec* plotspec, QWidget* parent)
    : ZoomPlot(parent), zoomOn(false)
{
  // set up legend
  enableLegend(TRUE);
  setAutoLegend(TRUE); //curves have to be inserted after this is set
  setLegendPos(Qwt::Bottom);

  initFromSpec(plotspec);

  // white background better for printing...
  setCanvasBackground(white);

  //  setTitle(FROM_UTF8(plotspec->getTitle()));
  setCanvasLineWidth(0);

  // signal and slot connections
  connect(this, SIGNAL(plotMousePressed(const QMouseEvent &)),
          SLOT(mousePressed(const QMouseEvent&)));
  connect(this, SIGNAL(plotMouseReleased(const QMouseEvent &)),
          SLOT(mouseReleased(const QMouseEvent&)));
  connect(this, SIGNAL(legendClicked(long)),
          SLOT(toggleCurve(long)));
}

bool CopasiPlot::initFromSpec(const CPlotSpec* plotspec)
{
  createIndices(plotspec);
  setTitle(FROM_UTF8(plotspec->getTitle()));

  removeCurves();

  while (data.size() > 0)
    {
      delete data[data.size() - 1];
      data.pop_back();
    }

  for (unsigned int i = 0; i < indexTable.size(); i++)
    {//TODO !!
      QMemArray<double>* v = new QMemArray<double>(500);  // initial size = 500
      data.push_back(v);
    }
  ndata = 0;

  QColor curveColours[6] = {red, yellow, blue, green, cyan, magenta};
  unsigned C_INT32 k;
  for (k = 0; k < plotspec->getCurves().size(); k++)
    {
      // set up the curve
      long crv = insertCurve(FROM_UTF8(plotspec->getCurves()[k].title));

      setCurvePen(crv, QPen(curveColours[k]));
      setCurveXAxis(crv, plotspec->getCurves()[k].xAxis);
      setCurveYAxis(crv, plotspec->getCurves()[k].yAxis);
    }
  return true; //TODO really check!
}

void CopasiPlot::takeData(const std::vector<C_FLOAT64> & dataVector)
{
  unsigned C_INT32 i;
  if (ndata >= data[0]->size())
    {
      unsigned C_INT32 newSize = data[0]->size() + 1000;
      for (i = 0; i < data.size(); i++)
        data[i]->resize(newSize);
    }

  for (i = 0; i < indexTable.size(); ++i)
    {
      data[i]->at(ndata) = dataVector[indexTable[i]];
    }
  ++ndata;
}

void CopasiPlot::updatePlot()
{
  // TODO: only do this once
  QMemArray<long> crvKeys = curveKeys();

  unsigned C_INT32 k;
  for (k = 0; k < crvKeys.size(); k++)
    {
      curve(crvKeys.at(k))->setRawData(data[dataIndices[k][0]]->data(),
                                       data[dataIndices[k][1]]->data(),
                                       ndata);
      //drawCurveInterval(crvKeys[k], 0, ndata-1);
    }
  replot();
}

//-----------------------------------------------------------------------------

/*void CopasiPlot::drawCurveInterval(long curveId, int from, int to)
{
  // taken from the realtime_plot example from Qwt library...
  QwtPlotCurve *curve = CopasiPlot::curve(curveId);
  if (curve == 0)
    return;
 
  QPainter p(canvas());
 
  p.setClipping(TRUE);
  p.setClipRect(canvas()->rect());
 
  curve->draw(&p,
              canvasMap(curve->xAxis()), canvasMap(curve->yAxis()),
              from, to);
}*/

//-----------------------------------------------------------------------------

void CopasiPlot::enableZoom(bool enabled)
{
  zoomOn = enabled;
}

//-----------------------------------------------------------------------------

void CopasiPlot::mousePressed(const QMouseEvent &e)
{
  if (zoomOn)
    {
      // zooming is now supported, so simply call the method in the base class
      ZoomPlot::mousePressed(e);
    }
  else
    {
      setOutlineStyle(Qwt::Cross);
      enableOutline(TRUE);
    }
}

//-----------------------------------------------------------------------------

void CopasiPlot::mouseReleased(const QMouseEvent &e)
{
  if (!zoomOn)
    return;

  ZoomPlot::mouseReleased(e);

  if (e.button() == RightButton)
    zoomOn = false;
}

//-----------------------------------------------------------------------------

void CopasiPlot::toggleCurve(long curveId)
{
  QwtPlotCurve *c = curve(curveId);
  if (c)
    {
      c->setEnabled(!c->enabled());
      replot();
    }
}

//-----------------------------------------------------------------------------

CopasiPlot::~CopasiPlot()
{
  removeCurves();
  //delete pointers sourcefile and plotSpec?

  while (data.size() > 0)
    {
      delete data[data.size() - 1];
      data.pop_back();
    }
}
