#include "fenetre.h"
#include "ui_fenetre.h"

#include "common.h"

#include "modelheaderrequestdata.h"

Fenetre::Fenetre(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Fenetre),
    m_modelRawsHeader(new ModelHeaderRequestData())
{
    ui->setupUi(this);

    ui->textEditBody->setEnabled(false);
    ui->tableViewHeader->setModel(m_modelRawsHeader);

    connect(ui->pushButtonAddRawHeader, &QPushButton::clicked, this, &Fenetre::onClicked_pushButtonAddRawHeader);
    connect(ui->pushButtonRemoveRawHeader, &QPushButton::clicked, this, &Fenetre::onClicked_pushButtonRemoveRawHeader);
    connect(ui->pushButtonSendRequest, &QPushButton::clicked, this, &Fenetre::onClicked_pushButtonSendRequest);
    connect(ui->comboBoxRequestMethod, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &Fenetre::onCurrentIndexChanged_ComboBoxRequestMethod);
}

Fenetre::~Fenetre()
{
    delete m_modelRawsHeader;
    delete ui;
}

void Fenetre::onClicked_pushButtonAddRawHeader()
{
    RawHeader raw = { ui->lineEditRawHeaderName->text().toLatin1(), ui->lineEditRawHeaderValue->text().toLatin1() };
    m_modelRawsHeader->addNewRawHeader(raw);
}

void Fenetre::onClicked_pushButtonRemoveRawHeader()
{
    m_modelRawsHeader->removeOneRawHeader(ui->tableViewHeader->currentIndex().row());
}

void Fenetre::onClicked_pushButtonSendRequest()
{
    ui->textEditReception->clear();

    request;
    QByteArray code;
    request.setRawsHeader(m_modelRawsHeader->rawsHeader());

    if(ui->comboBoxRequestMethod->currentIndex() == 0)  //GET
    {
        code = request.requestGet(ui->lineEditUrl->text());
    }
    else        //POST
    {
        code = request.requestPost(ui->lineEditUrl->text(), ui->textEditBody->toPlainText().toLatin1());
    }

    ui->textEditReception->setText(code);
    qDebug() << "Fin de la requête";
}

void Fenetre::onCurrentIndexChanged_ComboBoxRequestMethod()
{
    if(ui->comboBoxRequestMethod->currentIndex() == 0)  //GET
    {
        ui->textEditBody->setEnabled(false);
    }
    else        //POST
    {
        ui->textEditBody->setEnabled(true);
    }
}
