#include <QtTest/QtTest>
#include "../dataeater.hpp"
#include "../netclient.hpp"
#include "../json.hpp"

using nlohmann::json;

class Test_dataeater: public QObject {
    Q_OBJECT

private slots:
	void eat_simple_packet() {
		json j_msg  {
							{"cmd", "ping"},
							{"msg", "pong"}
						};
		std::string j_parsed = j_msg.dump();

		dataeater dataeater_test;
		dataeater_test.eat(netClient::serialize_msg(j_parsed).data());
		dataeater_test.process();

		std::string processed = dataeater_test.getLastCommand();

		std::cout << j_parsed << " =? " << processed << std::endl;
		QVERIFY(processed == j_parsed);
    }
};
