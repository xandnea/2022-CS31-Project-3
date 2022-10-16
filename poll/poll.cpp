#include <iostream>
#include <string>
#include <cctype>
#include <algorithm> // DON'T KNOW IF I CAN USE THIS LIBRARY

using namespace std;

bool isValidStateCode(string stateCode) {
	string test;
	string states = "AL,AK,AZ,AR,CA,CO,CT,DE,FL,GA,HI,ID,IL,IN,IA,KS,KY,LA,ME,MA,MD,MI,MN,MS,MO,MT,NE,NV,NH,NJ,NM,NY,NC,ND,OH,OK,OR,PA,SC,SD,TN,TX,UT,VT,VA,WA,WV,WI,WY";

	test += toupper(stateCode.at(0));
	test += toupper(stateCode.at(1));

	if (states.find(test) != -1) {
		return true;
	} else {
		return false;
	}
}

bool isValidPartyResult(string partyResult) {
	char party;
	if (!isdigit(partyResult.at(0))) {
		return false;
	}
	for (int i = 1; i < partyResult.length(); i++) {
		if ((!isdigit(partyResult.at(i))) && (!isalpha(partyResult.at(i)))) {
			return false;
		} else {
			if (isalpha(partyResult.at(i))) {
				party = toupper(partyResult.at(i));
				if ((party == 'D') || (party == 'R') || (party == 'I') || (party == 'L') || (party == 'G')) {
					return true;
				} else {
					return false;
				}
			}
		}
	}
	return true;
}

bool isValidStateForecast(string stateForecast) {                         
	char lastChar = stateForecast.at(stateForecast.length() - 1);
	if (isValidStateCode(stateForecast.substr(0, 2)) && (isalpha(lastChar) || lastChar == ',')) { 
		string temp; 
		for (int i = 2; i < stateForecast.length(); i++) { 
			if (!(isalpha(stateForecast.at(i)) && isdigit(stateForecast.at(i-1)))) { 
				temp += stateForecast.at(i);	
			} else {
				temp += stateForecast.at(i);
				if (isValidPartyResult(temp)) { 
					temp = "";
					continue;
				}
			}
		}
	}
	else {
		return false;
	}
	return true;
}

bool isValidPollString(string pollData) {
	string stateForecast;
	int containsSpace(pollData.find(' '));

	if (containsSpace != -1) {
		return false;
	}

	for (int i = 0; i < pollData.length(); i++) {
		if ((pollData.at(i) != ',') && (pollData.length() != (i + 1))) {
			stateForecast += pollData.at(i);
		}
		else {
			stateForecast += pollData.at(i);
			if (isValidStateForecast(stateForecast)) {
				if ((stateForecast.at(stateForecast.length() - 1)) == ',') {
					stateForecast.pop_back();
				}
				stateForecast = "";
				continue;
			}
			else {
				cout << stateForecast << endl;
				return false;
			}
		}
	}
	return true;
}

int countSeats(string pollData, char party, int& seatCount) {
	party = toupper(party);
	string temp = "";

	if (isValidPollString(pollData)) {
		if (isalpha(party)) {
			for (int i = 0; i < pollData.length(); i++) {
				if (toupper(pollData.at(i)) == party) {
					for (int j = i-1; j > 1; j--) {
						if (!isalpha(pollData.at(j))) {
							temp += pollData.at(j);
						} else {
							break;
						}
					}
					reverse(temp.begin(), temp.end());
					seatCount += stoi(temp);
					temp = "";
				}
			}
		} else {
			return 2;
		}
	} else {
		return 1;
	}
	return seatCount;
}


int main() {
	int seats = 0;

	/*cout << isValidStateCode("Ca") << endl;
	cout << isValidStateCode("nY") << endl;
	cout << isValidStateCode("ka") << endl;*/
	// couldn't figure out best way to efficiently check if it was a state code


	/*cout << isValidPartyResult("123R") << endl;
	cout << isValidPartyResult("09i") << endl;
	cout << isValidPartyResult("1K") << endl;
	cout << isValidPartyResult("12D0") << endl;*/
	// Had issues with letters not being the last character, other issues with numbers > 9

	/*isValidPollString("NY09R16D1I,Vt,NJ3d5r4D,KS4R");
	isValidPollString("ZO4R09D12i54l,CA00D"); 
	isValidPollString("NY9R16D1I");
	isValidPollString("ne3r00D");
    isValidPollString("KS 4R");
	isValidPollString("CT576D4r9l,CA3R");*/
	// stuck on how to separate the state from the party results, as well as separating state forecasts using commas
	// messed up with not having an if statement to catch any spaces
	// forgot to initialize stateForecast before sending it over to isValidStateForecast, causing crashes

	cout << countSeats("NY09R16D1I,Vt,NJ3d5r4D,KS4R", 'd', seats) << endl;
	seats = 0;
	cout << countSeats("NY09R16D1I,Vt,NJ3d5r4D,KS4R", 'i', seats) << endl;
	seats = 0;
	cout << countSeats("NY09R16D1I,Vt,NJ3d5r4D,KS4R", 'r', seats) << endl;
	seats = 0;

	cout << countSeats("KS 4R", 'i', seats) << endl;
	seats = 0;
	cout << countSeats("KS 4R", 'R', seats) << endl;
	seats = 0;

	cout << countSeats("KS4R", '/', seats) << endl;
	seats = 0;
	cout << countSeats("KS4R", '6', seats) << endl;
	seats = 0;

	cout << countSeats("CT576D4r9l,CA3R", 'i', seats) << endl;
	seats = 0;
	cout << countSeats("CT576D4r9l,CA3R", 'D', seats) << endl;
	seats = 0;
	// forgot to set it so that when reading the poll string, the code sets the character being read to uppercase

}