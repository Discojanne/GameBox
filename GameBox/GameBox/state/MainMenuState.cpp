#include "MainMenuState.h"
#include "../Game.h"
#include <fstream>

MainMenuState::MainMenuState(Game* pGame) : State(States::MainMenu, pGame) {

	m_logoTexture.loadFromFile("../Resources/DiscoNDungeons.png");
	m_logoSprite.setTexture(m_logoTexture);

	m_logoSprite.setPosition(-10, 0);

	float posX = 50;
	float posY = 150;

	m_buttons.push_back(Button("Start Game", pGame->GetFont(), [&]() {printf("MainMenu: \"Start Game\" Button Pressed!\n"); m_game->SetState(States::Game); }));
	m_buttons.back().setPosition(posX, posY);
	posY += 50;

	m_buttons.push_back(Button("Multiplayer", pGame->GetFont(), [&]() {
		m_bIsServerListOpen = true;
		}));
	m_buttons.back().setPosition(posX, posY);
	posY += 50;

	m_buttons.push_back(Button("Settings (not implemented)", pGame->GetFont(), [&]() {printf("MainMenu: \"Settings\" Button Pressed!\n"); }));
	m_buttons.back().setPosition(posX, posY);
	posY += 50;

	m_buttons.push_back(Button("About (not implemented)", pGame->GetFont(), [&]() {printf("MainMenu: \"About\" Button Pressed!\n"); }));
	m_buttons.back().setPosition(posX, posY);
	posY += 50;

	m_buttons.push_back(Button("Quit", pGame->GetFont(), [&]() {printf("MainMenu: \"Exit\" Button Pressed!\n"); m_game->getWindow()->close(); }));
	m_buttons.back().setPosition(posX, posY);
	posY += 50;

	m_buttons[m_selectedButton].SetIsSelected(true);

	//Load Saved Server IPs
	loadIPs();
}

MainMenuState::~MainMenuState() {

}

void MainMenuState::update(float dt) {
	sf::RenderWindow* window = m_game->getWindow();
	window->draw(m_logoSprite);

	for (auto& i : m_buttons) {
		window->draw(i);
	}
}

void MainMenuState::processInput(float dt) {

}

void MainMenuState::handleWindowEvent(const sf::Event& windowEvent) {

	// switch statement to handle different types of window events
	switch (windowEvent.type) {

		// case for when a key is pressed
	case sf::Event::EventType::KeyPressed:
		// if the Enter key is pressed, trigger the currently selected button
		if (windowEvent.key.code == sf::Keyboard::Escape) {
			m_bIsServerListOpen = false;
		}

		if (m_bIsServerListOpen) {
			//Skip keyboard events if the ServerListWindow is open. ServerList input is handled by IMGUI.
			return;
		}

		// if the Enter key is pressed, trigger the currently selected button
		if (windowEvent.key.code == sf::Keyboard::Enter) {
			m_buttons[m_selectedButton].Trigger();
		}

		// if the W or Up arrow key is pressed, move the selection up to the previous button if it exists
		if (windowEvent.key.code == sf::Keyboard::W || windowEvent.key.code == sf::Keyboard::Up) {
			if (m_selectedButton > 0) {
				m_buttons[m_selectedButton--].SetIsSelected(false);
				m_buttons[m_selectedButton].SetIsSelected(true);
			}
		}

		// if the S or Down arrow key is pressed, move the selection down to the next button if it exists
		if (windowEvent.key.code == sf::Keyboard::S || windowEvent.key.code == sf::Keyboard::Down) {
			if (m_selectedButton < m_buttons.size() - 1) {
				m_buttons[m_selectedButton++].SetIsSelected(false);
				m_buttons[m_selectedButton].SetIsSelected(true);
			}
		}
		break;

		// case for when a mouse button is pressed
	case sf::Event::MouseButtonPressed:
	{
		// get the ID of the button currently being hovered over
		size_t buttonID = getHoveredButtonID();

		// if a button is being hovered over, trigger it
		if (buttonID != -1) {
			m_buttons[buttonID].Trigger();
		}
		break;
	}
	// case for when the mouse is moved
	case sf::Event::EventType::MouseMoved:
	{
		// get the ID of the button currently being hovered over
		size_t buttonID = getHoveredButtonID();

		// if a button is being hovered over, set it as the selected button
		if (buttonID != -1) {
			m_buttons[m_selectedButton].SetIsSelected(false);
			m_buttons[buttonID].SetIsSelected(true);
			m_selectedButton = buttonID;
		}
		break;
	}
	// default case for handling any other event types not covered in the switch statement
	default:
		break;
	}

}

size_t MainMenuState::getHoveredButtonID() {

	size_t id = 0;
	for (auto& i : m_buttons) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(*m_game->getWindow());
		if (i.GetRectangle().contains(mousePos.x, mousePos.y)) {
			return id;
		}
		id++;
	}

	return -1;
}

void MainMenuState::renderGUI(float dt) {
	if (m_bIsServerListOpen) {
		renderServerList();
	}
}

void MainMenuState::joinServer(std::string ip, unsigned int port) {
	// Find the index of the first occurrence of ':' in the string 'ip'
	size_t seperatorIndex = ip.find_first_of(":");
	// If ':' was found in 'ip'
	if (seperatorIndex != std::string::npos) {
		// Extract the substring after ':' and convert it to an integer
		std::string port_string = ip.substr(seperatorIndex + 1);
		port = std::stoi(port_string);
		// Extract the substring before ':'
		ip = ip.substr(0, seperatorIndex);
	}

	if (m_game->m_networkConnection.Join()) {
		m_game->SetState(States::Lobby);
	} else {
		m_joinErrorMessage = "Could not join: " + ip + ":" + std::to_string(port);
	}
}

void MainMenuState::saveIPs() {

	std::filesystem::create_directories(m_serverListPath.parent_path());

	std::ofstream file(m_serverListPath);
	if (file.is_open()) {
		for (auto& i : m_savedIPs) {
			file << i << "\n";
		}
	}
}

void MainMenuState::loadIPs() {
	if (std::filesystem::exists(m_serverListPath)) {
		std::ifstream file(m_serverListPath);
		if (file.is_open()) {
			std::string line;
			while (std::getline(file, line)) {
				addServerToList(line);
			}
		}
	}
}

void MainMenuState::addServerToList(std::string serverAddress) {
	bool duplicate = false;
	for (auto& i : m_savedIPs) {
		if (i == serverAddress) {
			duplicate = true;
			break;
		}
	}
	if (!duplicate) {
		m_savedIPs.push_back(serverAddress);
	}
}

void MainMenuState::renderServerList() {
	sf::Vector2u windowSize = m_game->getWindow()->getSize();

	ImGui::SetNextWindowSize(ImVec2(500, windowSize.y));
	ImGui::SetNextWindowPos(ImVec2(windowSize.x - 500, 0));

	ImGui::Begin("Server List", &m_bIsServerListOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);

	if (ImGui::Button("Join")) {
		if (m_selectedServer > -1 && m_selectedServer < m_savedIPs.size()) {
			joinServer(m_savedIPs[m_selectedServer]);
		}
	}

	ImGui::SameLine();
	if (ImGui::Button("Host")) {
		if (m_game->m_networkConnection.Host()) {
			m_game->SetState(States::Lobby);
		} else {
			m_joinErrorMessage = "Could not host a server!";
		}
	}

	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing() * 2;
	if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), true)) {
		for (size_t i = 0; i < m_savedIPs.size(); i++) {
			if (ImGui::Selectable(m_savedIPs[i].c_str(), i == m_selectedServer, ImGuiSelectableFlags_AllowDoubleClick)) {
				m_selectedServer = i;
				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					joinServer(m_savedIPs[i]);
				}
			}
		}
		ImGui::EndChild();
	}

	static char buffer[256] = "127.0.0.1:1234";
	if (ImGui::Button("Join IP")) {
		joinServer(buffer);
	}
	ImGui::SameLine();
	if (ImGui::Button("Save")) {
		addServerToList(buffer);
		saveIPs();
	}
	ImGui::SameLine();
	if (ImGui::InputText("IP", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
		//Enter Was Pressed.
		joinServer(buffer);
	}
	if (m_joinErrorMessage != "") {
		ImGui::TextUnformatted(m_joinErrorMessage.c_str());
	}

	ImGui::End();
}
