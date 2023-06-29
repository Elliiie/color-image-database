#include "imageutil.h"

QColor ImageUtil::dominantColorHistogram(const QImage& image)
{
    std::vector<int> histogram(UIConstants().HUE_HISTOGRAM_SIZE);

    // Loop through the rows and count hue values.
    for (int y = 0; y < image.height(); ++y) {
        const QRgb* row = reinterpret_cast<const QRgb*>(image.constScanLine(y));
        const QRgb* end = row + image.width();

        while (row < end) {
            QColor pixelColor = QColor::fromRgb(*row++);
            QColor hsvColor = pixelColor.toHsv();
            int hue = hsvColor.hue();

            if (hue == -1) {
                hue = 360; // Replace -1 with 360. (achromatic color)
            }

            histogram[hue]++;
        }
    }

    // Find hue value with the highest count.
    int dominantHue = std::distance(histogram.begin(), std::max_element(histogram.begin(), histogram.end()));

    if (dominantHue == 360) {
        dominantHue = -1; // Switch back to -1, in case the dominant hue is achromatic.
    }

    return QColor::fromHsv(dominantHue, UIConstants().HSV_SATURATION, UIConstants().HSV_VALUE);
}

void ImageUtil::initializeCentroidsRandomly(const std::vector<QColor>& colors, std::vector<QColor>& centroids, int clustersCount) {
    std::srand(time(nullptr));

    // A vector to keep track of which colors have already been selected for centroids.
    std::vector<bool> selected(colors.size());


    // Select random cluster centroids.
    for (int i = 0; i < clustersCount; ++i) {
        int randomIndex = std::rand() % colors.size();
        while (selected[randomIndex]) {
            randomIndex = std::rand() % colors.size();
        }

        selected[randomIndex] = true;
        centroids[i] = colors[randomIndex];
    }
}

double ImageUtil::colorDistance(const QColor& firstColor, const QColor& secondColor) {
    int redDiff = firstColor.red() - secondColor.red();
    int greenDiff = firstColor.green() - secondColor.green();
    int blueDiff = firstColor.blue() - secondColor.blue();

    return std::sqrt(redDiff * redDiff + greenDiff * greenDiff + blueDiff * blueDiff);
}

void ImageUtil::assignColorsToCentroids(const std::vector<QColor>& colors, const std::vector<QColor>& centroids, std::vector<int>& assignments) {
    for (int i = 0; i < colors.size(); ++i) {
        double minDistance = std::numeric_limits<double>::max();
        int closestCentroidIndex = -1;

        for (int j = 0; j < centroids.size(); ++j) {
            double distance = colorDistance(colors[i], centroids[j]);

            if (distance < minDistance) {
                minDistance = distance;
                closestCentroidIndex = j;
            }
        }

        assignments[i] = closestCentroidIndex;
    }
}

void ImageUtil::calculateNewCentroids(const std::vector<QColor>& colors, std::vector<QColor>& centroids, const std::vector<int>& assignments) {
    std::vector<int> clusterSizes(centroids.size());
    std::vector<int> redSums(centroids.size());
    std::vector<int> greenSums(centroids.size());
    std::vector<int> blueSums(centroids.size());

    for (int i = 0; i < colors.size(); ++i) {
        int clusterIndex = assignments[i];

        clusterSizes[clusterIndex]++;
        redSums[clusterIndex] += colors[i].red();
        greenSums[clusterIndex] += colors[i].green();
        blueSums[clusterIndex] += colors[i].blue();
    }

    // Assign the new centroids to be the average colors of all colors' values inside.
    for (int i = 0; i < centroids.size(); ++i) {
        if (clusterSizes[i] > 0) {
            int redMean = redSums[i] / clusterSizes[i];
            int greenMean = greenSums[i] / clusterSizes[i];
            int blueMean = blueSums[i] / clusterSizes[i];

            centroids[i] = QColor(redMean, greenMean, blueMean);
        }
    }
}

QColor ImageUtil::dominantColorKMeans(const QImage& image) {
    std::vector<QColor> colors;

    for (int y = 0; y < image.height(); ++y) {
        const QRgb* row = reinterpret_cast<const QRgb*>(image.constScanLine(y));
        const QRgb* end = row + image.width();

        while (row < end) {
            QColor pixelColor = QColor::fromRgb(*row++);
            colors.push_back(pixelColor);
        }
    }

    // Initialize the centroids randomly.
    std::vector<QColor> centroids(2);
    initializeCentroidsRandomly(colors, centroids, 2);

    // Keep track of which color is assigned to which centroid.
    std::vector<int> assignments(colors.size());

    // Run the K-means algorithm.
    assignColorsToCentroids(colors, centroids, assignments);
    calculateNewCentroids(colors, centroids, assignments);

    // Find the cluster with the most assigned colors.
    int maxCount = 0;
    QColor dominantColor;

    for (int i = 0; i < centroids.size(); ++i) {
        int count = std::count(assignments.begin(), assignments.end(), i);
        if (count > maxCount) {
            maxCount = count;
            dominantColor = centroids[i];
        }
    }

    return dominantColor;
}

Color ImageUtil::dominantColorFrom(QString imagePath, const std::vector<Color>& colors, Algorithm algorithm) {
    QImage image(imagePath);

    QColor dominantColor;

    switch (algorithm) {
    case HISTOGRAM:
        dominantColor = dominantColorHistogram(image);
        break;
    case KMEANS:
        dominantColor = dominantColorKMeans(image);
        break;
    default:
        dominantColor = QColor(0, 0, 0);
        break;
    }

    Color closestColor;
    int closestHue = 360;

    for(Color const& color : colors) {
        QColor currentColor = color.toQColor().toHsv();

        int hueDifference = abs(dominantColor.hue() - currentColor.hue());
        if (hueDifference < closestHue) {
            closestHue = hueDifference;
            closestColor = color;
        }
    }

    return closestColor;
}
