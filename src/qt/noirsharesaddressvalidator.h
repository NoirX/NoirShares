// Copyright (c) 2011-2014 The NoirShares Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef NOIRSHARES_QT_NOIRSHARESADDRESSVALIDATOR_H
#define NOIRSHARES_QT_NOIRSHARESADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class NoirSharesAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit NoirSharesAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** NoirShares address widget validator, checks for a valid noirshares address.
 */
class NoirSharesAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit NoirSharesAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // NOIRSHARES_QT_NOIRSHARESADDRESSVALIDATOR_H
