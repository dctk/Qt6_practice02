#ifndef YUBININFOTABLEMODEL_H
#define YUBININFOTABLEMODEL_H

#include <QAbstractTableModel>
#include <qqml.h>

class YubinInfoTableModel : public QAbstractTableModel {
  Q_OBJECT
  QML_ELEMENT
  QML_ADDED_IN_MINOR_VERSION(1)

public:
  explicit YubinInfoTableModel(QObject *parent = nullptr);

  // QAbstractItemModel interface
public:
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QHash<int, QByteArray> roleNames() const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role) const override;
  void setYubinInfo(
          const QString &zipcode,
          const QString &prefcode,
          const QString &address1,
          const QString &address2,
          const QString &address3,
          const QString &kana1,
          const QString &kana2,
          const QString &kana3);
  void clearYubinInfo();

private:
  QString zipcode;
  QString prefcode;
  QString address1;
  QString address2;
  QString address3;
  QString kana1;
  QString kana2;
  QString kana3;
};

#endif // YUBININFOTABLEMODEL_H
