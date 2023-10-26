#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <tchar.h>
#include <vector>
#include <format>
#include <future>
#include <iostream>
#include <thread>
#include "updserver.h"

struct Player {
	std::string name{""};
	sockaddr_in service;
	int id{};
};
struct Lobby {
	int id{};
	std::vector<std::string> messages;
	std::vector<Player> players;
};
bool addLobby(Player& host, std::vector<Lobby>& lobbies);
void printLobbies(std::vector<Lobby>& lobbies);
bool addPlayerToLobby(Player& player, int lobbyId, std::vector<Lobby>& lobbies);
bool removePlayerFromLobby(Player& player, int lobbyId, std::vector<Lobby>& lobbies);
bool removeLobby(int lobbyId, std::vector<Lobby>& lobbies);
void sayHi(bool& run);

int main() {
	/*std::future<bool> testing = std::async(std::launch::async, sayHi);*/
	std::vector<Lobby> lobbies;
	int lobbyId = 0;
	bool pressed = true;
	bool currentlyPolling = false;
	UDPServer server;
	server.startServer();

	while (true) {
		if (!currentlyPolling) {
			currentlyPolling = true;
			std::thread t1(sayHi, std::ref(currentlyPolling));
			t1.detach();
		}
		server.recieveData();
		/*Player playerOne{ "John" };
		Player playertwo{ "Atlas" };
		Player playerThree{"Chad" };
		Player playerFour{ "Wong" };
		addLobby(playerOne, lobbies);
		addLobby(playerThree, lobbies);
		addLobby(playertwo, lobbies);
		addPlayerToLobby(playerFour, 2, lobbies);
		addPlayerToLobby(playertwo, 1, lobbies);
		printLobbies(lobbies);
		removePlayerFromLobby(playertwo, 1, lobbies);
		removePlayerFromLobby(playerThree, 1, lobbies);
		removeLobby(1, lobbies);
		removeLobby(2, lobbies);*/
		if (GetKeyState(VK_UP) < 0 && pressed) {
			std::cout << "hit up";
			pressed = false;
		}
		else if(GetKeyState(VK_UP) >=0  && pressed == false) {
			pressed = true;

		}
		if (GetKeyState(VK_ESCAPE) < 0 && pressed) {
			server.closeServer();
			exit(EXIT_SUCCESS);
		}
	}
	return 0;
}

bool addLobby(Player& host, std::vector<Lobby>& lobbies) {
	try {
		Lobby createdLobby;
		createdLobby.players.push_back(host);
		if (lobbies.size() == 0) {
			createdLobby.id = 0;
		}

		else {
			createdLobby.id = lobbies.back().id + 1;
		}
		lobbies.push_back(createdLobby);
		return true;
	}
	catch (...) {
		std::cout << "Error adding lobby"<< std::endl;
		return false;
	}
}

void printLobbies(std::vector<Lobby>& lobbies) {
	for(auto& lobby : lobbies)
	{
		std::cout << std::format("Lobby Id: {}", lobby.id) << std::endl;
		std::cout << "Players: ";
		for (auto& player : lobby.players)
		{
			if(player.id == 0)
				std::cout << std::format("{}",  player.name);
			else
				std::cout << std::format(", {}", player.name);
		}
		std::cout << "\n";
	}
}

bool addPlayerToLobby(Player& player, int lobbyId, std::vector<Lobby>& lobbies){
	try {
		std::vector<Lobby>::iterator it = std::find_if(lobbies.begin(), lobbies.end(), 
			[lobbyId](const Lobby& l) -> bool {return l.id == lobbyId;});
		if (it - lobbies.begin() < lobbies.size()) {
			Lobby& lobby = lobbies[it - lobbies.begin()];
			player.id = lobby.players.size();
			lobby.players.push_back(player);
			return true;
		}
		else {
			throw ("Cannot find lobby");
		}
	}
	catch (...) {
		std::cout << "ERROR adding player to lobby" << std::endl;
		return false;
	}
}

bool removePlayerFromLobby(Player& player, int lobbyId, std::vector<Lobby>& lobbies) {
	try {
		//get lobby index
		std::vector<Lobby>::iterator it = std::find_if(lobbies.begin(), lobbies.end(),
			[lobbyId](const Lobby& l) -> bool {return l.id == lobbyId;});
		if (it - lobbies.begin() < lobbies.size()) {
			Lobby& lobby = lobbies[it - lobbies.begin()];
			//Get player index
			std::vector<Player>::iterator playerIt = std::find_if(lobby.players.begin(), lobby.players.end(),
				[player](const Player& p) -> bool {return p.id == player.id;});
			if (playerIt - lobby.players.begin() < lobby.players.size())
			{
				lobby.players.erase(playerIt);
				for (auto& eachPlayer : lobby.players)
				{
					if(eachPlayer.id != 0)
					eachPlayer.id = eachPlayer.id - 1;
				}
				return true;
			}
			else {
				throw ("ERROR finding player");
			}
		}
		else {
			throw ("ERROR finding lobby");
		}
	}
	catch (...) {
		std::cout << "ERROR removing player from to lobby" << std::endl;
		return false;
	}
}

bool removeLobby(int lobbyId, std::vector<Lobby>& lobbies) {
	try {
		std::vector<Lobby>::iterator it = std::find_if(lobbies.begin(), lobbies.end(),
			[lobbyId](const Lobby& l) -> bool {return l.id == lobbyId;});
		if (it - lobbies.begin() < lobbies.size()) {
			lobbies.erase(it);
			return true;
		}
		else {
			throw ("Cannot find lobby");
		}
	}
	catch (...) {
		std::cout << "ERROR removing lobby" << std::endl;
		return false;
	}
}

void sayHi(bool& run) {
	std::cout << "Currenty polling..." << std::endl;
	Sleep(10000);
	/*std::cout << "Hello again" << std::endl;*/
	run = false;
}

//strcpy_s(buffer, playerOne.name.c_str());
