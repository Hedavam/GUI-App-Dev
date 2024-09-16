#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QVBoxLayout;
class QGridLayout;
class QButtonGroup;
class QPushButton;
class QAbstractButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

/*Constructor & Destructor */
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

/* private members (default) accessible by all other methods in class - good to be private so outside classes and stuff derived from them can't modify them */

    /* layout stuff */
    QVBoxLayout *mainLayout;
    QGridLayout *grid;
    QButtonGroup *buttonGroup; /*idea: add buttons to this and manage accordignly; replacement of ordered pairs to check winning sequences, etc.
                               w/ just a set of ids */
    QPushButton *resetBtn;

    /* Game logic stuff - want private member so it persists across turns */
    int movesCtr;
    bool isP1Turn;

    /* logic to determine who won - not sure whether did stuff should be on heap or stack*/
    QSet<int> p1Seq;
    QSet<int> p2Seq;
    QSet<QSet<int>> winningSeqs;
    void whoWon(const QSet<int> &winningSeq, const QString player); /* pass the complex set DS by reference but the simple int by value;
                                                                     * const b/cuz we don't need the func to modify the values */

public slots: /* accessible from outside the class - public so that QObject's connect() could work */
    void tileToggled(QAbstractButton *btn, bool checked); /* int specifiying the ID of the toggled tile - needs to match signature of toggled signal */
    void resetBtnClicked();
};


#endif // MAINWINDOW_H
