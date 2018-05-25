#ifndef FENETRE_H
#define FENETRE_H

#include <QMainWindow>
#include "httprequesthandler.h"

class ModelHeaderRequestData;

namespace Ui {
class Fenetre;
}

class Fenetre : public QMainWindow
{
    Q_OBJECT

public:
    explicit Fenetre(QWidget *parent = 0);
    ~Fenetre();

private slots:
    void onClicked_pushButtonAddRawHeader();
    void onClicked_pushButtonRemoveRawHeader();
    void onClicked_pushButtonSendRequest();
    void onCurrentIndexChanged_ComboBoxRequestMethod();

private:
    Ui::Fenetre *ui;
    ModelHeaderRequestData* m_modelRawsHeader;
    HttpRequestHandler request;
};

#endif // FENETRE_H
