// Copyright (c) 2017-2018 The PIVX developers
// Copyright (c) 2020 The sbercoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ZsbercoinCONTROLDIALOG_H
#define ZsbercoinCONTROLDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "primitives/zerocoin.h"

class CZerocoinMint;
class WalletModel;

namespace Ui {
class ZsbercoinControlDialog;
}

class ZsbercoinControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ZsbercoinControlDialog(QWidget *parent);
    ~ZsbercoinControlDialog();

    void setModel(WalletModel* model);

    static std::set<std::string> setSelectedMints;
    static std::set<CMintMeta> setMints;
    static std::vector<CMintMeta> GetSelectedMints();

private:
    Ui::ZsbercoinControlDialog *ui;
    WalletModel* model;

    void updateList();
    void updateLabels();

    enum {
        COLUMN_CHECKBOX,
        COLUMN_DENOMINATION,
        COLUMN_PUBCOIN,
        COLUMN_VERSION,
        COLUMN_CONFIRMATIONS,
        COLUMN_ISSPENDABLE
    };

private slots:
    void updateSelection(QTreeWidgetItem* item, int column);
    void ButtonAllClicked();
};

#endif // ZsbercoinCONTROLDIALOG_H
