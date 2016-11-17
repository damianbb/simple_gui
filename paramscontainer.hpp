#ifndef PARAMSCONTAINER_H
#define PARAMSCONTAINER_H

#include <QObject>
#include <QStringList>
#include <QMap>

class ParamsContainer : public QObject
{
	Q_OBJECT
public:
	explicit ParamsContainer(QObject *parent = 0);

signals:

public slots:
	bool readParams(QString file_name);
	bool writeParams(QString file_name);
	QString getIp();
	QMap<QString,QString> getPeerList();

	void setIp(QString my_ip);
	void setPeerList(QMap<QString,QString> peer_list);
private:
	bool m_was_readed;

	QMap<QString,QString>m_peers;
	QString m_my_ip;

};

#endif // PARAMSCONTAINER_H
