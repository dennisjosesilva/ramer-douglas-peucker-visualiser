#include "Contour/ContourExtractor.hpp"

#include <limits>

const QVector<QPoint> ContourExtractor::AdjF = { {-1, 0}, { 0, 1}, { 1, 0}, { 0,-1}};
const QVector<QPoint> ContourExtractor::AdjE = { { 1, 0}, { 1, 1}, { 0, 1}, {-1, 1},
                                                 {-1, 0}, {-1,-1}, { 0,-1}, { 1,-1}};

ContourExtractor::ContourExtractor(const QVector<bool> &bimg, int width, int height)
  :width_{width}, height_{height}, bimg_{bimg}
{}

bool ContourExtractor::imgContains(const QPoint &p) const
{
  return 0 <= p.x() && p.x() < width_ && 0 <= p.y() && p.y() < height_;
}

QVector<float> ContourExtractor::compute() const 
{
  QVector<bool> contourImg = computeContourImg();
  
  QPoint p;
  for (p.ry() = 0; p.y() < height_; p.ry()++) {
    for (p.rx() = 0; p.x() < width_; p.rx()++) {
      if (contourImg[index(p)]) 
        return traceContour(contourImg, p);
    }
  }

  return QVector<float>();
}

QVector<bool> ContourExtractor::computeContourImg() const
{
  QVector<bool> contours_(bimg_.size(), false);

  QPoint p;
  for (p.ry() = 0; p.y() < height_; p.ry()++) {
    for (p.rx() = 0; p.x() < width_; p.rx()++) {
      int pidx = index(p);
      if (bimg_[pidx]) {       
         for (const QPoint &o : AdjF) {
            QPoint q = p + o;
            int qidx = index(q);
            if (!imgContains(q) || !bimg_[qidx]) {
              contours_[pidx] = true;
              break;
            }
         }
      }
    }
  }

  return contours_;
}

QVector<float> ContourExtractor::traceContour(
  const QVector<bool> &contourImg,
  const QPoint &p) const 
{
  QVector<float> contour;
  PointDir next = findNextPoint(contourImg, p, 0);
  contour.append(static_cast<float>(next.p.x()) + 0.5f);
  contour.append(static_cast<float>(next.p.y()) + 0.5f);

  QPoint start = p;
  QPoint end = next.p;

  QPoint prev = p;
  QPoint cur = next.p;
  bool done = (prev == cur);
  while (!done) {
    int dsearch = (next.dir + 6) % 8;
    next = findNextPoint(contourImg, cur, dsearch);
    prev = cur;
    cur = next.p;
    done = (prev == start && cur == end);
    if (!done) {
      contour.append(static_cast<float>(cur.x()) + 0.5f);
      contour.append(static_cast<float>(cur.y()) + 0.5f);
    }
  }

  return contour;
}

ContourExtractor::PointDir 
ContourExtractor::findNextPoint(const QVector<bool> &contourImg,
  const QPoint &curPoint, int dir) const
{
  for (int i = 0; i < 7; i++) {
    QPoint q = curPoint + AdjE[dir];
    if (background(contourImg, q)) 
      dir = (dir + 1) % 8;
    else
      return {q, dir};
  }
  return {curPoint, dir};
}

int ContourExtractor::index(const QPoint &p) const
{
  if (imgContains(p))
    return p.y() * width_ + p.x();

  return std::numeric_limits<int>::min();
}

QPoint ContourExtractor::point(int idx) const
{
  return { idx % width_, idx / width_ };
}

bool ContourExtractor::foreground(const QPoint &p) const
{
  if (imgContains(p)) 
    return bimg_[index(p)];
  
  return false;
}

bool ContourExtractor::foreground(const QVector<bool> &contourImg, 
  const QPoint &p) const
{
  if (imgContains(p)) 
    return contourImg[index(p)];

  return false;
}

// =======================================================================
// SHORTCUT FUNCTION
// =======================================================================
QVector<float> extractContour(const QVector<bool> &bimg, 
  int width, int height)
{
  return ContourExtractor(bimg, width, height)();
}