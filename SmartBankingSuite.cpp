#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;
const string ClientsFileName = "Clients.txt";
void GoBackToMainMenue();

struct stClient
{
	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0.0;
	bool MarkForDelete = false;
	bool MakeForUpdate = false;
};
enum enMainMenueOptions
{
	ShowClientsList = 1, eAddNewClient, DeleteClient,
	UpdateClientInformations, FindCliend, Exit
};

string ReadClientAccountNumber()
{
	string AccountNumber;
	cout << "Please enter Account Number ? ";
	cin >> AccountNumber;
	return AccountNumber;
}
bool ReadMore(string Message = "")
{
	char C;
	cout << Message;
	cin >> C;
	return (C == 'Y' || C == 'y');
}
bool ReadAnswer(string Message = "")
{
	char Answer;
	cout << Message;
	cin >> Answer;
	return (Answer == 'Y' || Answer == 'y');
}
vector <string> SplitString(string S1, string Delim = " ")
{
	short pos = 0;
	string Word = "";
	vector <string> vS1;

	while ((pos = S1.find(Delim)) != std::string::npos)
	{
		Word = S1.substr(0, pos);

		if (Word != "") vS1.push_back(Word);

		S1.erase(0, pos + Delim.length());
	}

	if (S1 != "") vS1.push_back(S1);

	return vS1;
}
string ConvertRecordToLine(stClient Client, string Seperator = "#//#")
{
	string stClientRecord = "";

	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.Phone + Seperator;
	stClientRecord += to_string(Client.AccountBalance);

	return stClientRecord;


}
stClient ConvertLineToRecord(string Line, string Seperator = "#//#")
{
	stClient Client;
	vector <string> vClientData = SplitString(Line, Seperator);

	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);

	return Client;
}

vector <string> GetAccountsNumbers(vector <stClient>&vClients)
{
	vector <string> AccountsNumbers;
	for (stClient Client : vClients)
	{
		AccountsNumbers.push_back(Client.AccountNumber);
	}
	return AccountsNumbers;
}
void ShowEndScreen()
{
	system("cls");
	cout << "\n___________________________________\n";
	cout << "\t Program Ends :-)";
	cout << "\n___________________________________\n\n";
}

void AddClientDataToFile(string FileName, string ClientDataLine)
{
	fstream MyFile;
	stClient Client = ConvertLineToRecord(ClientDataLine);
	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open() && Client.MarkForDelete == false)
	{
		MyFile << ClientDataLine << endl;
		MyFile.close();
	}

}
void AddAllClientsDataFromVToFile(string FileName, vector <stClient>& vClients)
{
	for (stClient Client : vClients)
	{
		if (Client.MarkForDelete == false)
			AddClientDataToFile(ClientsFileName, ConvertRecordToLine(Client));
	}
}
void ClearAllDataFile(string FileName)
{
	fstream File;

	File.open(FileName, ios::out);

	if (File.is_open())
	{
		File.clear();
		File.close();
	}
}

bool IsAccountNumberExists(string AccNum, vector <string>&AccountsNumbers)
{
	for (string ExistsAccNum : AccountsNumbers)
	{
		if (AccNum == ExistsAccNum)
		{
			return 1;
		}
	}
	return 0;
}
stClient ReadNewClientData(vector <string>&AccountsNumbers)
{
	stClient Client;
	string AccountNumber;

	cout << "Enter AccountNumber? ";
	getline(cin >> ws, AccountNumber);

	while (IsAccountNumberExists(AccountNumber, AccountsNumbers))
	{
		cout << "Client with [" << AccountNumber << "] is already exists,";
		cout << " Enter another Account Number? ";
		getline(cin >> ws, AccountNumber);

	}

	Client.AccountNumber = AccountNumber;
	cout << "Enter PinCode? ";
	getline(cin >> ws, Client.PinCode);
	cout << "Enter Name? ";
	getline(cin, Client.Name);
	cout << "Enter Phone? ";
	getline(cin, Client.Phone);
	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;

	return Client;
}
void AddNewClient(vector <string>& AccountsNumbers)
{
	
	stClient stClient;
	stClient = ReadNewClientData(AccountsNumbers);
	AddClientDataToFile(ClientsFileName, ConvertRecordToLine(stClient));

}
void AddClients(vector <stClient>& vClients)
{
	vector <string> AccountsNumbers = GetAccountsNumbers(vClients);
	do {
		cout << "\n___________________________________\n";
		cout << "\t Add New Clients Screen";
		cout << "\n___________________________________\n";
		AddNewClient(AccountsNumbers);
		cout << "Client Added Successfully, do you want to add more clients? ";
	} while (ReadMore());
}


vector <stClient> LoadClientsDataFromFileToV(string FileName)
{
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	vector <stClient> vClients;

	if (MyFile.is_open())
	{
		string Line;
		stClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);

			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return vClients;
}

void PrintClientRecord(stClient Client)
{
	cout << "| " << setw(15) << Client.AccountNumber;
	cout << "| " << setw(10) << Client.PinCode;
	cout << "| " << setw(40) << Client.Name;
	cout << "| " << setw(12) << Client.Phone;
	cout << "| " << setw(12) << Client.AccountBalance;

}
void PrintClientData(stClient Client)
{
	cout << "\nThe following are the client details:\n";
	cout << "\n___________________________________\n";
	cout << "AccountNumber  : " << Client.AccountNumber << endl;
	cout << "PinCode        : " << Client.PinCode << endl;
	cout << "Client         : " << Client.Name << endl;
	cout << "Phone          : " << Client.Phone << endl;
	cout << "AccountBalance : " << Client.AccountBalance << endl;
	cout << "\n___________________________________\n";
}


bool FindClientByAccountNumber(vector <stClient>& vClients, stClient& Client, string AccountNumber)
{
	for (stClient Clientt : vClients)
	{
		if (Clientt.AccountNumber == AccountNumber)
		{
			Client = Clientt;
			return 1;
		}
	}
	return 0;
}
void DeleteAccountFromClientsVector(vector <stClient>& vClients, string AccountNumber)
{
	string Message = "Are you sure you want to delete this account? Y/N : ";

	if (ReadAnswer(Message))
	{
		vector <stClient> vC;

		for (stClient& C : vClients)
		{
			if (C.AccountNumber != AccountNumber)
			{
				vC.push_back(C);
			}
			else
			{
				C.MarkForDelete = true;
			}
		}
		cout << "Client deleted successfully!\n";
		vClients = vC;
	}
	else
	{
		cout << "Good choice <3\n";
	}
}
void DeleteClientByAccountNumber(vector <stClient>& vClients, string AccountNumber)
{
	stClient Client;

	if (FindClientByAccountNumber(vClients, Client, AccountNumber))
	{
		PrintClientData(Client);
		DeleteAccountFromClientsVector(vClients, AccountNumber);
		ClearAllDataFile(ClientsFileName);
		AddAllClientsDataFromVToFile(ClientsFileName, vClients);
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!\n";
	}

}

void ReadNewInfoForClient(stClient& Client)
{

	cout << "Enter PinCode? ";
	getline(cin >> ws, Client.PinCode);
	cout << "Enter Name? ";
	getline(cin, Client.Name);
	cout << "Enter Phone? ";
	getline(cin, Client.Phone);
	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;


}
void UpdateMarkedAccounts(vector <stClient>& vClients)
{
	vector <stClient> vC;
	for (stClient& Client : vClients)
	{
		if (Client.MakeForUpdate)
		{
			ReadNewInfoForClient(Client);
		}
	}
}
void MakeAMarkForUpdateAccount(vector <stClient>& vClients, string AccountNumber)
{
	for (stClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
			C.MakeForUpdate = true;
		break;
	}

}
void UpdateClientByAccountNumber(vector <stClient>& vClients, string AccountNumber)
{
	stClient Client;

	if (FindClientByAccountNumber(vClients, Client, AccountNumber))
	{
		PrintClientData(Client);
		string Message = "Are you sure you want to update this account? Y/N : ";
		if (ReadAnswer(Message))
		{
			MakeAMarkForUpdateAccount(vClients, AccountNumber);
			UpdateMarkedAccounts(vClients);
			ClearAllDataFile(ClientsFileName);
			AddAllClientsDataFromVToFile(ClientsFileName, vClients);
			cout << "Client updated successfully!\n";
		}
		else
		{
			cout << "Account will not be updated\n";
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!\n";
	}

}

void PrintAllClientsData(vector <stClient> vClients)
{

	cout << "\n\t\t\t\t\tClients List (" << vClients.size() << ") Clients(s)." << endl;
	cout << "\n_________________________________________________________";
	cout << "__________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_________________________________________________________";
	cout << "__________________________________________\n" << endl;

	for (stClient Client : vClients)
	{
		PrintClientRecord(Client);
		cout << endl;
	}


}



void ShowClientList(vector <stClient>&vClients)
{
	PrintAllClientsData(vClients);
}
void AddNewClientScreen(vector <stClient>&vClients)
{
	AddClients(vClients);

}
void DeleteClientScreen(vector <stClient>&vClients)
{
	cout << "\n___________________________________\n";
	cout << "\t Delete Client Screen";
	cout << "\n___________________________________\n";
	string AccountNumber = ReadClientAccountNumber();
	DeleteClientByAccountNumber(vClients, AccountNumber);
}
void UpdateClientInfo(vector <stClient>& vClients)
{
	cout << "\n___________________________________\n";
	cout << "\t Update Client Screen";
	cout << "\n___________________________________\n";
	string AccountNumber = ReadClientAccountNumber();
	UpdateClientByAccountNumber(vClients, AccountNumber);
}
void FindAccount(vector <stClient>& vClients)
{
	string AccountNumber = ReadClientAccountNumber();
	stClient Client;
	if (FindClientByAccountNumber(vClients, Client, AccountNumber))
		PrintClientData(Client);
	else
		cout << "Client with Number (" << AccountNumber << ") is NOT found.\n";

}


enMainMenueOptions ReadMainMenueOption()
{
	short  Choice;
	do {
		cout << "Choose what do you want to do [1 to 6] : ";
		cin >> Choice;
	} while (Choice > 6 || Choice < 1);
	return (enMainMenueOptions)Choice;
}

void PerformMainMenuOption(enMainMenueOptions MainMenueOption)
{
	
	vector <stClient> vClients = LoadClientsDataFromFileToV(ClientsFileName);

	switch (MainMenueOption)
	{
	case ShowClientsList:
		system("cls");
		ShowClientList(vClients);
		GoBackToMainMenue();
		break;
	case eAddNewClient:
		system("cls");
		AddNewClientScreen(vClients);
		GoBackToMainMenue();
		break;
	case DeleteClient:
		system("cls");
		DeleteClientScreen(vClients);
		GoBackToMainMenue();
		break;
	case UpdateClientInformations :
		system("cls");
		UpdateClientInfo(vClients);
		GoBackToMainMenue();
		break;
	case FindCliend:
		FindAccount(vClients);
		GoBackToMainMenue();
		break;
	case Exit:
		system("cls");
		ShowEndScreen();
		break;
	}
}
void ShowMainMenueScreen()
{
	system("cls");
	cout << "====================================\n";
	cout << "\tMain Menue Screen\n";
	cout << "====================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Exit.\n";
	cout << "====================================\n";




}
void ShowMainMenue()
{
	ShowMainMenueScreen();
	PerformMainMenuOption(ReadMainMenueOption());
}
void GoBackToMainMenue()
{
	cout << "\nPress any key to go back to main menue...";
	system("pause");
	ShowMainMenue();
}
int main()
{
	ShowMainMenue();
	return 0;
}