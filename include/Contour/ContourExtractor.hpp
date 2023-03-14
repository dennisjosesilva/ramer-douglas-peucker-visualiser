#pragma once

#include <QVector>
#include <QPoint>
#include <QVector2D>

class ContourExtractor
{
public:
  ContourExtractor(const QVector<bool> &bimg, int width, int height);
  QVector<float> compute() const;
  QVector<float> operator()() const { return compute(); }

private:
  struct PointDir 
  {
    QPoint p;
    int dir;
  };

  // ================================================================
  // AUX PRIVATE METHODS
  // ================================================================
  QVector<bool> computeContourImg() const;
  QVector<float> traceContour(const QVector<bool> &contourImg, 
    const QPoint &p) const;
  
  PointDir findNextPoint(const QVector<bool> &contourImg,
    const QPoint &curPoint, int dir) const;


  bool imgContains(const QPoint &p) const;
  bool background(const QPoint &p) const { return !foreground(p); }
  bool foreground(const QPoint &p) const;

  bool background(const QVector<bool> &contourImg, const QPoint &p) const
    { return !foreground(contourImg, p); }
  bool foreground(const QVector<bool> &contourImg, const QPoint &p) const;


  int index(const QPoint &p) const;
  QPoint point(int idx) const;


  // =================================================================
  // ATTRIBUTES
  // =================================================================
  int width_;
  int height_;
  const QVector<bool> &bimg_;

  static const QVector<QPoint> AdjF;
  static const QVector<QPoint> AdjE;
};

QVector<float> extractContour(const QVector<bool> &bimg, 
  int width, int height);