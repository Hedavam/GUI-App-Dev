#include "mainwindow.h"

#include <QtCore/qforeach.h>
#include <QtWidgets> /* Gives us access to various Qt GUI elements */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* Game Logic Definitions */
    movesCtr = 1;

    /* Want to make a set of winning sets (sequences) */

    /* Define winning Sets */
    winningSeqs = {
        /* down */
        QSet<int>({1,2,3}),
        QSet<int>({4,5,6}),
        QSet<int>({7,8,9}),
        /* across */
        QSet<int>({1,4,7}),
        QSet<int>({2,5,8}),
        QSet<int>({3,6,9}),
        /* diagonal */
        QSet<int>({1,5,9}),
        QSet<int>({3,5,7})
    };

    /* Set starting size, min size for mainwindow */
    setMinimumSize(300,300);

    /* Create widget Set up as central widget */
    QWidget *center = new QWidget(this);
    setCentralWidget(center);

    /* Define mainlayout, grid & add grid to main Layout */
    mainLayout = new QVBoxLayout(center); /* sets center widget as parent */
    grid = new QGridLayout();
    mainLayout->addLayout(grid);

    /* define buttonGroup for later use*/
    buttonGroup = new QButtonGroup();
    buttonGroup->setExclusive(false); /* CRUCIAL to avoid behavior where another button needs to be clicked for checked to be updated since it ONLY allows one toggled button for the group*/


    /* Create button widgets & add to 3x3 grid layout */
    int iterationCounter = 0; /* will label buttons 1-9 starting top left, goint across to the right. Same for next 2 rows */
    for (int i = 0; i < 3; i++) { /* move down rows */
        for (int j = 0; j < 3; j++) { /* move across columns */
            QPushButton *tile = new QPushButton();

            /* Style - min size for buttons, text-size, and make sure they expand to fill up space */
            tile->setMinimumSize(30, 30);
            tile->setStyleSheet("font: bold;"
                                "font-size: 25px");
            tile->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            tile->setCheckable(true); /*CRUCIAL to make it toggable button */

            /* add button to grid layout, specifiying row, col */
            grid->addWidget(tile, i, j);

            /* Add button to buttonGroup for managament!! */
            iterationCounter++;
            buttonGroup->addButton(tile, iterationCounter);
        }
    }

    /* Connect buttonGroup to pogram; NOTE: The whole button group is the emitting object and you don't need to connect individual buttons in the loop */
    connect(buttonGroup, &QButtonGroup::buttonToggled, this, &MainWindow::tileToggled);

    /* Create reset button, make it of fixed size; add it to main layout, center it AND connect it */
    resetBtn = new QPushButton("Reset");
    resetBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mainLayout->addWidget(resetBtn);
    mainLayout->setAlignment(resetBtn, Qt::AlignHCenter);
    connect(resetBtn, &QPushButton::clicked, this, &MainWindow::resetBtnClicked);
}


MainWindow::~MainWindow() {}

/* One for filling in Xs and Os - should also disable the toggled button */

void MainWindow::tileToggled(QAbstractButton *btn, bool checked) {

    /* Handles UI Change */
    if (checked) { /*NOTE: not sure why this is necessary - I THINK IT'S CUZ BUTTON GROUP EMITS SIGNAL WHEN ANY OF ITS BUTTONS ARE TOGGLED */

        /* Determine if it's player 1's turn or player 2's turn */
        movesCtr++;

        isP1Turn = (movesCtr % 2 == 0); /* no need for if-else; even is p1, odd is p2 */

        /* Set Text to X or O & add move to player sequences */

        if (isP1Turn) {
            btn->setText("X");
            p1Seq.insert(buttonGroup->id(btn));

        } else {
            btn->setText("O");
            p2Seq.insert(buttonGroup->id(btn));
        }

        /* Disable it so it can't be changed */
        btn->setDisabled(true);
    }

    /* Game Logic */

    if(movesCtr >= 5 && isP1Turn) { /* p1 won? */
        whoWon(p1Seq, "X");
    } else if (movesCtr >= 5 && !isP1Turn) { /* p2 won? */
        whoWon(p2Seq, "O");
    }
}

/* One for resetting grid when rstBtn is clicked */

void MainWindow::resetBtnClicked() {

    //reset p1, p2 sequences & movesCtr
    p1Seq = QSet<int>{};
    p2Seq = QSet<int>{};
    movesCtr = 1;


    //set text -> "" &&  //enable all the buttons
    foreach(QAbstractButton *btn, buttonGroup->buttons()) {
        btn->setEnabled(true);
        btn->setChecked(false);
        btn->setText("");
    }
}

//TODO: Logic methods


/* One for determining who has won the game - display message, and enable rstBtn */
void MainWindow::whoWon(const QSet<int> &pSeq, const QString winner) {

    /* when we check for winner, loop through winningSeqs, see if the player sequence contains a winning one.
     * If so, display winning message and disable all buttons in group,
     * then break (no need to keep checking for winner... we found it */
    if(winner == "X" || winner == "O") {
        foreach(QSet<int> winningSeq, winningSeqs) {
            if(pSeq.contains(winningSeq)) {
                QMessageBox::information(this, "Game Over", QString("Player %1 won").arg(winner));
                foreach(QAbstractButton *btn, buttonGroup->buttons()) {
                        btn->setDisabled(true);
                }
                break;
            }
        }
    }

    /* if the board is full and there was no winner */
    if(movesCtr > 9) {
        QMessageBox::information(this, "Game Over", QString("Game was a Draw"));
    }
}







