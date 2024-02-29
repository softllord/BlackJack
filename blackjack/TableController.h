#pragma once
#include <string>

class TableModel;
class TableView;
class PlayerModel;
class DeckModel;

enum class GameState {
	PreMatch,
	PlayerChoice,
	DealerChoice,
	EndGame
};

enum class CompetitionState {
	InGame,
	PlayerWins,
	DealerWins,
	Draw
};

class TableController
{
private:
	GameState gameState;
	TableModel* tableModel = nullptr;
	TableView* tableView = nullptr;
	void DrawTable() const;
	void GiveCardToPlayer(const PlayerModel* InPlayer, DeckModel* InDeck);
	void DrawMenu();
	void GameLoop();
	CompetitionState GetCompetitionState() const;
public:
	TableController(TableModel* InTableModel, TableView* InTableView);
	~TableController();
	void StartGame();
};

