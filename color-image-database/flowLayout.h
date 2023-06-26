#ifndef FLOWLAYOUT_H
#define FLOWLAYOUT_H

#include <QLayout>
#include <QRect>
#include <QStyle>

class FlowLayout : public QLayout
{
public:
    /*!
     * \brief Constructor Creates new flow layout object
     * \param QWidget parent widget
     * \param int margin
     * \param int Horizontal spacing
     * \param int Vertical spacing
     */
    explicit FlowLayout(QWidget *parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);
    /*!
     * \brief Constructor Creates new flow layout object
     * \param int margin
     * \param int Horizontal spacing
     * \param int Vertical spacing
     */
    explicit FlowLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);
    /*!
     * \brief Destructor Deallocates memory
    */
    ~FlowLayout();

    /*!
     * \brief Adds a new QLayoutItem object to the layout
     * \param QLayoutItem item to be added to the layout
     */
    void addItem(QLayoutItem *item) override;
    /*!
     * \brief Determines the horizontal spacing between the widgets inside the layout
     * \return int The spacing value
    */
    int horizontalSpacing() const;
    /*!
     * \brief Determines the vertical spacing between the widgets inside the layout
     * \return int The spacing value
    */
    int verticalSpacing() const;
    /*!
     * \brief Determines the Qt::Orientations in which the layout can make use of more space than its sizeHint()
     * \return Qt::Orientations The orientations a layout can use
    */
    Qt::Orientations expandingDirections() const override;
    /*!
     * \brief Determines if layout's preferred height depends on its width
     * \return bool True if the preferred height depends on the width, otherwise - false
    */
    bool hasHeightForWidth() const override;
    /*!
     * \brief Determines the preferred height for a layout item given the width.
     * \return int The preferred height
    */
    int heightForWidth(int) const override;
    /*!
     * \brief Returns the number of items in the layout
     * \return int The number of items
     */
    int count() const override;
    /*!
     * \brief Returns the item at a given index
     * \param int Item's index
     * \return QLayoutItem The corresponding item
     */
    QLayoutItem *itemAt(int index) const override;
    /*!
     * \brief Returns the minimum size of an item
     * \return QSize Item's minimum size
    */
    QSize minimumSize() const override;
    /*!
     * \brief Calculate the geometry of the layout's items
     * \param QRect The layout rect
     */
    void setGeometry(const QRect &rect) override;
    /*!
     * \brief Returns the preferred size of the layout
     * \return QSize The preferred size
     */
    QSize sizeHint() const override;
    /*!
     * \brief Removes and returns the item at a given index
     * \param int Item's index
     * \return QLayoutItem The item
     */
    QLayoutItem *takeAt(int index) override;
    /*!
     * \brief Removes all items from the layout
     */
    void clearLayout();

private:
    /*!
     * \brief Handles the calculations of the available space if horizontalSpacing() or verticalSpacing() don't return the default value
     * \param rect Layout's rect
     * \param bool Determiens if the item's geometry should be updated
     * \return int The bounds in which the items are laid out
     */
    int doLayout(const QRect &rect, bool testOnly) const;
    /*!
     * \brief Gets the default spacing for either the top-level layouts or the sublayouts
     * \param QStyle::PixelMetric A style dependent size represented by a single pixel value
     * \return int The value of the spacing
     */
    int smartSpacing(QStyle::PixelMetric pm) const;

    QList<QLayoutItem *> itemList;
    int m_hSpace;
    int m_vSpace;
};

#endif // FLOWLAYOUT_H
