#ifndef IMAGEUTIL_H
#define IMAGEUTIL_H


#include "color.h"
#include "qcolor.h"
#include "qimage.h"

class ImageUtil
{
public:
    /// Returns the dominant color of an image from the given colors based on the given algorithm.
    static Color dominantColorFrom(QString imagePath, const std::vector<Color>& colors, QString algorithm);

private:
    /////////////////////////////////
    /// DOMINANT COLOR ALGORITHMS ///
    /////////////////////////////////

    /// Determines the dominant color based on the histogram approach.
    static QColor dominantColorHistogram(const QImage& image);
    /// Determines the dominant color based on the K-means clustering algorithm.
    static QColor dominantColorKMeans(const QImage& image);

    //////////////////////////////
    /// K-MEANS HELPER METHODS ///
    //////////////////////////////

    /// Initializes the centroids randomly.
    static void initializeCentroidsRandomly(const std::vector<QColor>& colors, std::vector<QColor>& centroids, int clustersCount);

    /// Calculates the Euclidean distance between two colors.
    static double colorDistance(const QColor& firstColor, const QColor& secondColor);

    /// Assigns each color to the closest cluster centroid.
    static void assignColorsToCentroids(const std::vector<QColor>& colors, const std::vector<QColor>& centroids, std::vector<int>& assignments);

    /// Calculates the new centroids based on the assigned colors.
    static void calculateNewCentroids(const std::vector<QColor>& colors, const std::vector<int>& assignments, std::vector<QColor>& centroids);
};


#endif // IMAGEUTIL_H
