#include <QCoreApplication>


#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QStringList peers;
//	peers.push_back("aaaa");
//	peers.push_back("bbbb");
	QString my_ip = "192.177.23.1";


	QJsonObject peersLst;
	peersLst["aaaa"] = "2e34f:3424f:34ff";
	peersLst["bbbb"] = "ze4ff:3424f:34fg";


//	QJsonArray peersArray;
//	peersArray.push_back(QJsonArray(peers));

//	foreach (auto it ,peers) peersArray.push_back(it);
	QJsonObject doc_obj;
	doc_obj["myIp"] = my_ip;
	doc_obj["peerList"] = peersLst;

	QJsonDocument doc(doc_obj);
	qDebug()<<doc.toJson();
	QFile file("peers.json");
	file.open(QFile::WriteOnly);


	file.write(doc.toJson(),doc.toJson().size());
	file.close();



	QFile in_file("peers.json");
	in_file.open(QFile::ReadOnly);
	qDebug()<<"reading: ";

	QJsonDocument in_docu(QJsonDocument::fromJson((in_file.readAll())));
	qDebug()<< in_docu.object()["myIp"].toString();
	QJsonObject params_obj = in_docu.object()["peerList"].toObject();
	for (auto it:params_obj.keys()) qDebug()<< it <<" "<<params_obj[it].toString();


	file.close();

//	int16_t before = 1232;
	char tab[2];
	tab[0] = 1;
	tab[1] = 1;

	int16_t after = reinterpret_cast<int16_t&>(*tab);

	qDebug()<<after;

//	qDebug()<<doc.toBinaryData();

	return a.exec();
}
