#include <iostream>
#include <string>
#include <cctype>
#include <cassert>
#include <algorithm> // DON'T KNOW IF I CAN USE THIS LIBRARY

using namespace std;

// Checks if stateCode (to uppercase) is within the states string
bool isValidStateCode(string stateCode) {
	string test;
	string states = "AL.AK.AZ.AR.CA.CO.CT.DE.FL.GA.HI.ID.IL.IN.IA.KS.KY.LA.ME.MA.MD.MI.MN.MS.MO.MT.NE.NV.NH.NJ.NM.NY.NC.ND.OH.OK.OR.PA.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";

	test += toupper(stateCode.at(0));
	test += toupper(stateCode.at(1));

	if (states.find(test) != -1) {
		//cout << test << " is a valid state code" << endl;
		return true;
	}
	else {
		//cout << test << " is not a valid state code" << endl;
		return false;
	}
}

// Does not allow multiple party inputs next to each other (ex. CT4RR20d3l)
bool isValidPartyResult(string partyResult) {
	char party;
	int digits = 0;
	if (!isdigit(partyResult.at(0))) { 
		//cout << partyResult << " is not a valid party result (error 1)" << endl;
		return false;
	}
	for (int i = 0; i < partyResult.length(); i++) { // as long as each character is either a digit or alpha, it checks if the number and party is correct
		if ((!isdigit(partyResult.at(i))) && (!isalpha(partyResult.at(i)))) {
			//cout << partyResult << " is not a valid party result (error 2)" << endl;
			return false;
		}
		else {
			if (isdigit(partyResult.at(i))) {
				digits++;
				if (digits > 2) {
					//cout << "TOO HIGH OF A NUMBER: " << digits << endl;
					return false;
				}
			}
			if (isalpha(partyResult.at(i))) {
				party = toupper(partyResult.at(i));
				if ((party == 'D') || (party == 'R') || (party == 'I') || (party == 'L') || (party == 'G')) {
					//cout << partyResult << " is a valid party result" << endl;
					return true;
				}
				else {
					//cout << partyResult << " is not a valid party result (error 3)" << endl;
					return false;
				}
			}
		}
	}
	return true;
}

// Checks above functions to determine if state code and individual party results are valid
bool isValidStateForecast(string stateForecast) {
	char lastChar = stateForecast.at(stateForecast.length() - 1);
	if (isValidStateCode(stateForecast.substr(0, 2)) && (isalpha(lastChar) || lastChar == ',')) { // if first 2 letters are a valid state code and the last char is alpha or comma
		string temp;
		for (int i = 2; i < stateForecast.length(); i++) { // starting at the character after the state code, check if each party result is valid
			if (isalpha(stateForecast.at(i)) || isdigit(stateForecast.at(i)) || (stateForecast.at(i) == ',')) { // checks for if any characters are something other than digit, alpha, or comma
				if (!(isalpha(stateForecast.at(i)) && isdigit(stateForecast.at(i - 1)))) {
					if (isalpha(stateForecast.at(i)) && isalpha(stateForecast.at(i - 1))) { // ensures 2 party characters can't be next to each other
						return false;
					}
					else {
						temp += stateForecast.at(i);
					}

				}
				else {
					temp += stateForecast.at(i);
					if (isValidPartyResult(temp)) {
						temp = "";
						continue;
					}
				}
			}
			else {
				return false;
			}
		}
	}
	else {
		//cout << stateForecast << " is not a valid state forecast" << endl;
		return false;
	}
	//cout << stateForecast << " is a valid state forecast" << endl;
	return true;
}

// ensures no white spaces and ignores commas at the beginning of poll data; checks above functions for poll data to determine if valid
bool isValidPollString(string pollData) {
	string stateForecast;
	int containsSpace(pollData.find(' '));
	int i = 0;

	if ((containsSpace != -1) || (pollData.at(i) == ',')) { // ensures there is no white spaces or comma at index 0
		//cout << pollData << " is not valid poll data (error 1)" << endl;
		return false;
	}

	for (i; i < pollData.length(); i++) { // separates state forecasts to check them individually
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
				//cout << pollData << " is not valid poll data (error 2)" << endl;
				return false;
			}
		}
	}
	return true;
	//cout << pollData << " is valid poll data" << endl;
}

int countSeats(string pollData, char party, int& seatCount) {
	//cout << "STARTING SEAT COUNT: " << seatCount << endl;
	party = toupper(party);
	string temp = "";

	if (isValidPollString(pollData)) {
		if (isalpha(party)) {
			seatCount = 0;
			for (int i = 0; i < pollData.length(); i++) { // counts seats by iterating backwards and creating strings of the results after passing the correct party
				if (toupper(pollData.at(i)) == party) {   // then reverses the string, converts it to integer, and adds it to seatCount
					for (int j = i - 1; j > 1; j--) {
						if (!isalpha(pollData.at(j))) {
							//cout << "temp: " << temp << endl;
							temp += pollData.at(j);
						}
						else {
							break;
						}
					}
					reverse(temp.begin(), temp.end());
					seatCount += stoi(temp);
					temp = "";
					//cout << "Current seat count for " << party << " is " << seatCount << endl;
				}
			}
		}
		else {
			return 2;
		}
	}
	else {
		return 1;
	}
	return 0;
}


int main() {

	assert(isValidPollString("CT5D,NY9R16D1I,VT,ne3r00D"));
	assert(!isValidPollString("ZT5D,NY9R16D1I,VT,ne3r00D"));
	int seats;
	seats = -999;    // so we can detect whether countSeats sets seats
	assert(countSeats("CT5D,NY9R16D1I,VT,ne3r00D", 'd', seats) == 0 && seats == 21);
	seats = -999;    // so we can detect whether countSeats changes seats
	assert(countSeats("CT5D,ny9r16d1I,VT,ne3r00D", '%', seats) == 2 && seats == -999);
	seats = -999;    // so we can detect whether countSeats changes seats
	assert(countSeats("CT5D,ny9r16d1I,VT,ne3rR00D", 'r', seats) == 1 && seats == -999);
		
	cerr << "All tests succeeded" << endl;

	/*int seats = -999;

	string pollString;
	char party;

	cout << "Enter a poll string: ";
	getline(cin, pollString);

	cout << "Enter a party to count: ";
	cin >> party;

	cout << "Return code: " << countSeats(pollString, party, seats) << endl;
	cout << "Number of seats for " << party << ": " << seats << endl;*/
	// way to test any specific data through user input

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


	/*cout << countSeats("NY09R16D1I,Vt,NJ3d5r4D,KS4R", 'd', seats) << endl;
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
	seats = 0;*/
	// forgot to set it so that when reading the poll string, the code sets the character being read to uppercase

}
