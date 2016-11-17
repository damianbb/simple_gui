#ifndef COMMANDEXECUTOR_H
#define COMMANDEXECUTOR_H

class commandExecutor
{
	void showMessage();
	void askQuestion();
	void showWarrning();
};

#endif // COMMANDEXECUTOR_H


/**
  communication protocol :

  using example
  initial message - starting peer :




	{
	"id": 1234,			- message id - number
	"cmd": "addpeer",	activity type 
	"topic: "addpeer"+id, 	-creating new topic of conversation to easer manage many conversations 
	"msg":{					the main 
					"type": "question",				-podjecia decyzji
					."text":"do i need to make a coffe?",
					"ans":"yes/no/info";,
						parametry komendy
		}
	}
//info msg ://////////////////
{
	"id":1,
	"cmd":"info",
	"topic":"info1",
	"msg":{
		"text":"show first info"
	}
}
ans:
{
	"id":2,
	"cmd":"ans1",
	"topic":"info1",
}
//question/////////////////////////
{
	"id":3,
	"cmd":"ask",
	"topic":"ask3",
	"msg":{
		"text":"do a coffe?",
		"pans":"yes/no"
	}
}
ans:
{
	"id":4
	"cmd":"ans3"
	"topic":"ask3"
	"msg":{
		"ans":"yes"
	}
}



//////////////////////////////////
	answer:

	{
		"id":1235,
		"cmd":"ans"+1234,	 		-odpowiedz na rozkaz 1234
		"topic":"addpeer1234",		-temat rozmowy
		"err":"0",						-ewentualnie blad wkonania
		"msg":{					-ewentualne parametry odpowiedzi- z mozliwoscia wprasowania koniecznosci
					"type": "ans",				-podjecia decyzji"
					"ans":"yes"
		}
	}

  */


