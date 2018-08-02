/********************************************************************************
** Form generated from reading UI file 'mini_search.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MINI_SEARCH_H
#define UI_MINI_SEARCH_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mini_searchClass
{
public:
    QAction *actionOpen;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_4;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QMainWindow *mini_searchClass)
    {
        if (mini_searchClass->objectName().isEmpty())
            mini_searchClass->setObjectName(QStringLiteral("mini_searchClass"));
        mini_searchClass->setEnabled(true);
        mini_searchClass->resize(622, 412);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(mini_searchClass->sizePolicy().hasHeightForWidth());
        mini_searchClass->setSizePolicy(sizePolicy);
        mini_searchClass->setStyleSheet(QStringLiteral("border-image: url(:/mini_search/images/back.jpg);"));
        actionOpen = new QAction(mini_searchClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        centralWidget = new QWidget(mini_searchClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalSpacer = new QSpacerItem(17, 125, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(155, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        groupBox->setStyleSheet(QStringLiteral("border-image: url(:/mini_search/images/back_2.png);"));
        radioButton = new QRadioButton(groupBox);
        radioButton->setObjectName(QStringLiteral("radioButton"));
        radioButton->setEnabled(true);
        radioButton->setGeometry(QRect(0, 10, 53, 21));
        sizePolicy.setHeightForWidth(radioButton->sizePolicy().hasHeightForWidth());
        radioButton->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamily(QStringLiteral("Comic Sans MS"));
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        font.setStyleStrategy(QFont::PreferDefault);
        radioButton->setFont(font);
        radioButton->setMouseTracking(true);
        radioButton->setAcceptDrops(false);
        radioButton->setAutoFillBackground(false);
        radioButton->setStyleSheet(QStringLiteral("border-image: url(:/mini_search/images/back_2.png);"));
        radioButton->setCheckable(true);
        radioButton->setChecked(false);
        radioButton->setAutoRepeat(false);
        radioButton->setAutoExclusive(false);
        radioButton_2 = new QRadioButton(groupBox);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));
        radioButton_2->setEnabled(true);
        radioButton_2->setGeometry(QRect(130, 10, 41, 21));
        sizePolicy.setHeightForWidth(radioButton_2->sizePolicy().hasHeightForWidth());
        radioButton_2->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setFamily(QStringLiteral("Comic Sans MS"));
        font1.setPointSize(11);
        font1.setBold(true);
        font1.setWeight(75);
        font1.setKerning(true);
        radioButton_2->setFont(font1);
        radioButton_2->setFocusPolicy(Qt::StrongFocus);
        radioButton_2->setContextMenuPolicy(Qt::DefaultContextMenu);
        radioButton_2->setLayoutDirection(Qt::LeftToRight);
        radioButton_2->setStyleSheet(QStringLiteral("border-image: url(:/mini_search/images/back_2.png);"));

        horizontalLayout->addWidget(groupBox);

        horizontalSpacer_2 = new QSpacerItem(170, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_4 = new QSpacerItem(75, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(20);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());
        lineEdit->setSizePolicy(sizePolicy2);
        QFont font2;
        font2.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font2.setPointSize(11);
        lineEdit->setFont(font2);
        lineEdit->setStyleSheet(QStringLiteral("border-image: url(:/mini_search/images/back_3.png);"));
        lineEdit->setFrame(true);
        lineEdit->setClearButtonEnabled(false);

        horizontalLayout_2->addWidget(lineEdit);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        sizePolicy1.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy1);
        QFont font3;
        font3.setFamily(QStringLiteral("Comic Sans MS"));
        font3.setPointSize(10);
        font3.setBold(true);
        font3.setWeight(75);
        pushButton->setFont(font3);
        pushButton->setStyleSheet(QLatin1String("border-image: url(:/mini_search/images/back.png);\n"
"pressed{border-image: url(:/mini_search/images/cloud.jpg)};"));

        horizontalLayout_2->addWidget(pushButton);

        horizontalSpacer_5 = new QSpacerItem(60, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(17, 165, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        mini_searchClass->setCentralWidget(centralWidget);
        groupBox->raise();
        QWidget::setTabOrder(radioButton, radioButton_2);

        retranslateUi(mini_searchClass);

        QMetaObject::connectSlotsByName(mini_searchClass);
    } // setupUi

    void retranslateUi(QMainWindow *mini_searchClass)
    {
        mini_searchClass->setWindowTitle(QApplication::translate("mini_searchClass", "cloud_search", 0));
        actionOpen->setText(QApplication::translate("mini_searchClass", "Open...", 0));
        groupBox->setTitle(QString());
#ifndef QT_NO_TOOLTIP
        radioButton->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        radioButton->setText(QApplication::translate("mini_searchClass", "AND", 0));
#ifndef QT_NO_WHATSTHIS
        radioButton_2->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        radioButton_2->setText(QApplication::translate("mini_searchClass", "OR", 0));
        pushButton->setText(QApplication::translate("mini_searchClass", "Search", 0));
    } // retranslateUi

};

namespace Ui {
    class mini_searchClass: public Ui_mini_searchClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MINI_SEARCH_H
