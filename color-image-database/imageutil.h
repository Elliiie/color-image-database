#ifndef IMAGEUTIL_H
#define IMAGEUTIL_H


#include "color.h"
#include "qcolor.h"
#include "qimage.h"
#include "constants.cpp"

class ImageUtil
{
public:
    /*!
     * \brief Determines the dominant color of an image from the given colors based on the given algorithm.
     * \param QString The path to the image
     * \param vector<Color> The colors to choose the dominant color from
     * \param Algorithm The algorithm to calculate the dominant color
     * \return Color The dominant color
    */
    static Color dominantColorFrom(QString imagePath, const std::vector<Color>& colors, Algorithm algorithm);

private:
    /////////////////////////////////
    /// DOMINANT COLOR ALGORITHMS ///
    /////////////////////////////////

    /*!
    * \brief Determines the dominant color based on the histogram approach.
    * \param Image The image to determine the dominant color of
    * \return QColor The image's dominant color
   */
    static QColor dominantColorHistogram(const QImage& image);

    /*!
    * \brief Determines the dominant color based on the K-means clustering algorithm.
    * \param Image The image to determine the dominant color of
    * \return QColor The image's dominant color
   */
    static QColor dominantColorKMeans(const QImage& image);

    //////////////////////////////
    /// K-MEANS HELPER METHODS ///
    //////////////////////////////

    /*!
    * \brief Initializes the centroids randomly.
    * \param vector<QColor> All the image's pixel colors
    * \param vector<QColor> The centroids' colors
    * \param int The cluster count
   */
    static void initializeCentroidsRandomly(const std::vector<QColor>& colors, std::vector<QColor>& centroids, int clustersCount);

    /*!
    * \brief Calculates the Euclidean distance between two colors.
    * \param QColor The first color
    * \param QColor The second color
    * \return double The color distance of the two colors
   */
    static double colorDistance(const QColor& firstColor, const QColor& secondColor);

    /*!
    * \brief Assigns each color to the closest cluster centroid.
    * \param vector<QColor> All the image's pixel colors
    * \param vector<QColor> The centroids' colors
    * \param vector<int> The assignments of each color to its corresponding centroid
   */
    static void assignColorsToCentroids(const std::vector<QColor>& colors, const std::vector<QColor>& centroids, std::vector<int>& assignments);

    /*!
    * \brief Calculates the new centroids based on the assigned colors.
    * \param vector<QColor> All the image's pixel colors
    * \param vector<QColor> The centroids' colors
    * \param vector<int> The assignments of each color to its corresponding centroid
   */
    static void calculateNewCentroids(const std::vector<QColor>& colors, std::vector<QColor>& centroids, const std::vector<int>& assignments);
};


#endif // IMAGEUTIL_H
