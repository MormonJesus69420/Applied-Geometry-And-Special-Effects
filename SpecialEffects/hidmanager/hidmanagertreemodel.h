#ifndef HIDBINDINGSMODEL_H
#define HIDBINDINGSMODEL_H


class TreeItem;

// local
#include "hidmanager.h"
#include "hidaction.h"

// qt
#include <QAbstractItemModel>

class HidManagerTreeModel : public QAbstractTableModel {
  Q_OBJECT
public:
  explicit HidManagerTreeModel( HidManager* hidmanager, QObject* parent = 0x0 );
  ~HidManagerTreeModel();

  QModelIndex     index(int row, int column, const QModelIndex &parent) const override;
  QModelIndex     parent(const QModelIndex &child) const override;
  int             rowCount(const QModelIndex &parent) const override;
  int             columnCount(const QModelIndex &parent) const override;

  QVariant        data(const QModelIndex &index, int role = Qt::DisplayRole ) const override;

  Qt::ItemFlags   flags(const QModelIndex &index) const override;

  QVariant        headerData(int section, Qt::Orientation orientation, int role) const override;

  void            update( const HidManager::HidActions& action, const HidManager::HidBindings& bindings );

  QHash< int, QByteArray >    roleNames() const override;

private:
  typedef QList<QString>                    HidEntryInfoList;
  typedef QMap<QString, HidEntryInfoList>   HidEntryMap;
  typedef QMap<QString, HidEntryMap>        HidGroupMap;

  HidManager                    *_hm;
  HidManager::HidActions        _hal;
  HidManager::HidBindings       _hb;

  HidGroupMap                   _data;


  TreeItem                      *_root;

}; // END class HidBindingsModel

#endif // HIDBINDINGSMODEL_H
