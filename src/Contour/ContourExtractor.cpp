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
  QVector<bool> visited(contourImg.size(), false);
  QVector<float> contour;
  visited[index(p)] = true;
  
  QPoint cp = p;
  bool stop = false;
  while(!stop) {
    stop = true;
    contour.append(static_cast<float>(cp.x()) + 0.5f);
    contour.append(static_cast<float>(cp.y()) + 0.5f);
    int cpidx = index(cp);
    visited[cpidx] = true;

    for (const QPoint &o : AdjE) {
      QPoint q = cp + o;
      int qidx = index(q);
      if (imgContains(q) && contourImg[qidx] && !visited[qidx]) {
        stop = false;
        cp = q;
        break;
      }
    }
  }
  return contour;
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

// =======================================================================
// SHORTCUT FUNCTION
// =======================================================================
QVector<float> extractContour(const QVector<bool> &bimg, 
  int width, int height)
{
  return ContourExtractor(bimg, width, height)();
}