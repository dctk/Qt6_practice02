#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "yubininfotablemodel.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPalette>
#include <QJsonValue>
#include <QJsonArray>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QAbstractTableModel *model = new YubinInfoTableModel;
    ui->tableView_detail->setModel(model);
    ui->tableView_detail->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_detail->setColumnWidth(1,450);
    ui->label_message->clear();

    manager = new QNetworkAccessManager(this);
    connect(manager,&QNetworkAccessManager::finished,
            this,&MainWindow::on_networkreplay_finished);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_search_clicked()
{
    QString yubinNo = ui->lineEdit_yubinNo->text();
    QRegularExpression reg("[0-9]{7}");
    if (!reg.match(yubinNo).hasMatch()){
        setErrorMessage("郵便番号は７桁の数値を入力してください。");
        return;
    }

    dynamic_cast<YubinInfoTableModel*>(ui->tableView_detail->model())->clearYubinInfo();

    QString url = QString("https://zipcloud.ibsnet.co.jp/api/search?zipcode=%1&limit=1").arg(yubinNo);
    QNetworkRequest req = QNetworkRequest(QUrl(url));

    ui->pushButton_search->setEnabled(false);
    setMessage(QString("%1を検索中しています。").arg(yubinNo));
    manager->get(req);
}

void MainWindow::on_networkreplay_finished(QNetworkReply *rep)
{
    if (rep->error() == QNetworkReply::NoError){
        QJsonDocument doc(QJsonDocument::fromJson(rep->readAll()));
        QJsonObject obj(doc.object());

        int status = obj.value("status").toInt();
        if (status == 200){
            QJsonValue results = obj.value("results");
            if (results.type() != QJsonValue::Null){
                setMessage("検索が完了しました。");
                QJsonObject result = results.toArray().at(0).toObject();

                YubinInfoTableModel *model = dynamic_cast<YubinInfoTableModel*>(ui->tableView_detail->model());
                model->setYubinInfo(
                            result.value("zipcode").toString(),
                            result.value("prefcode").toString(),
                            result.value("address1").toString(),
                            result.value("address2").toString(),
                            result.value("address3").toString(),
                            result.value("kana1").toString(),
                            result.value("kana2").toString(),
                            result.value("kana3").toString()
                            );
            }
            else{
                setErrorMessage("郵便番号に一致する情報が見つかりませんでした。");
            }
        }
        else if (status == 400 || status == 500){
            setErrorMessage(obj.value("message").toString());
        }
        else{
            setErrorMessage("未定義のエラーが発生しました。");
        }
    }
    else{
        setErrorMessage("APIの実行に失敗しました。");
    }

    ui->pushButton_search->setEnabled(true);
}

void MainWindow::setMessage(const QString &message)
{
    ui->label_message->setText(message);
    QPalette palette = ui->label_message->palette();
    palette.setColor(QPalette::WindowText,"#000000");
    ui->label_message->setPalette(palette);
}

void MainWindow::setErrorMessage(const QString &message)
{
    ui->label_message->setText(message);
    QPalette palette = ui->label_message->palette();
    palette.setColor(QPalette::WindowText,"#ff0000");
    ui->label_message->setPalette(palette);
}

