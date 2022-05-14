#include "yubininfotablemodel.h"

YubinInfoTableModel::YubinInfoTableModel(QObject *parent)
    : QAbstractTableModel{parent} {}

int YubinInfoTableModel::rowCount(const QModelIndex &parent) const { return 8; }

int YubinInfoTableModel::columnCount(const QModelIndex &parent) const {
  return 2;
}

QHash<int, QByteArray> YubinInfoTableModel::roleNames() const {
  return {{Qt::DisplayRole, "display"}};
}

QVariant YubinInfoTableModel::headerData(int section,
                                         Qt::Orientation orientation,
                                         int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();

  if (Qt::Orientation::Horizontal == orientation) {
    switch (section) {
    case 0:
      return QString("項目");
    case 1:
      return QString("値");
    }
  }

  return QVariant();
}

void YubinInfoTableModel::setYubinInfo(const QString &zipcode, const QString &prefcode,
                                       const QString &address1, const QString &address2,
                                       const QString &address3, const QString &kana1,
                                       const QString &kana2, const QString &kana3) {
    this->zipcode = zipcode;
    this->prefcode = prefcode;
    this->address1 = address1;
    this->address2 = address2;
    this->address3 = address3;
    this->kana1 = kana1;
    this->kana2 = kana2;
    this->kana3 = kana3;
    emit dataChanged(index(0,0),index(0,7));
}

void YubinInfoTableModel::clearYubinInfo() {
    setYubinInfo("","","","","","","","");
}

QVariant YubinInfoTableModel::data(const QModelIndex &index, int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();

  const QString column0[] = {
      "郵便番号", "都道府県コード", "都道府県名",     "市区町村名",
      "町域名",   "都道府県名カナ", "市区町村名カナ", "町域名カナ"};

  int row = index.row();
  int col = index.column();

  if (col == 0) {
    if (row >= 0 && row < 8) {
      return column0[row];
    }
  } else if (col == 1) {
    switch (row) {
    case 0:
      return zipcode;
    case 1:
      return prefcode;
    case 2:
      return address1;
    case 3:
      return address2;
    case 4:
      return address3;
    case 5:
      return kana1;
    case 6:
      return kana2;
    case 7:
      return kana3;
    default:
      return QVariant();
    }
  }

  return QVariant();
}
