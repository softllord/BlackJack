#include "TableController.h"
#include "TableModel.h"
#include "TableView.h"
#include "DealerModel.h"
#include "PlayerModel.h"
#include "DeckModel.h"

TableController::TableController(TableModel* InTableModel, TableView* InTableView)
{
	tableModel = InTableModel;
	tableView = InTableView;
	gameState = GameState::PreMatch;
	
}

TableController::~TableController()
{
	delete tableModel;
	delete tableView;
}

void TableController::DrawTable() const
{
	auto dealer_hand = tableModel->GetDealer()->GetHand();
	std::string table = "Dealer: ";
	for (const auto card_in_hand : dealer_hand) {
		table += card_in_hand->getFullName() + " ";
	}

	table += "\tValue: " + std::to_string(tableModel->GetDealer()->GetHandValue());

	auto player_hand = tableModel->GetPlayers()[0]->GetHand();
	table += "\nPlayer: ";
	for (const auto card_in_hand : player_hand) {
		table += card_in_hand->getFullName() + " ";
	}

	table += "\tValue: " + std::to_string(tableModel->GetPlayers()[0]->GetHandValue()) + "\n";

	tableView->DrawTable(table);
}

void TableController::GiveCardToPlayer(const PlayerModel* InPlayer,DeckModel* InDeck)
{
	CardModel* card = InDeck->PopRandomCard();
	InPlayer->PushCardToHand(card);
}

void TableController::DrawMenu()
{
	std::string menu = "\n\n1.Take card\t2.Enough\n";
	tableView->DrawMenu(menu);
}

void TableController::GameLoop()
{
	CompetitionState competitionState = GetCompetitionState();
	while (true) {
		switch (gameState) {
			case GameState::PreMatch: {
				DrawTable();
				competitionState = GetCompetitionState();
				if (competitionState == CompetitionState::PlayerWins) {
					gameState = GameState::EndGame;
				}
				else {
					gameState = GameState::PlayerChoice;
				}
				break;
			}
			case GameState::PlayerChoice: {
				DrawMenu();
				int player_decision = tableView->GetInput();
				if (player_decision == 1) {
					GiveCardToPlayer(tableModel->GetPlayers()[0], tableModel->GetDealer()->GetDeck());
					DrawTable();
					competitionState = GetCompetitionState();
					if (competitionState == CompetitionState::DealerWins) {
						gameState = GameState::EndGame;
					}
				}
				else if (player_decision == 2) {
					gameState = GameState::DealerChoice;
				}
				break;
			}
			case GameState::DealerChoice: {
				int dealer_score = tableModel->GetDealer()->GetHandValue();
				if (dealer_score <= 17) {
					GiveCardToPlayer(tableModel->GetDealer(), tableModel->GetDealer()->GetDeck());
					DrawTable();
				}
				else {
					competitionState = GetCompetitionState();
					gameState = GameState::EndGame;
				}
				break;
			}
			case GameState::EndGame: {
				if (competitionState == CompetitionState::Draw) {
					tableView->DrawTable("\n\nDraw!");
				}
				else if (competitionState == CompetitionState::PlayerWins) {
					tableView->DrawTable("\n\nPlayer wins!");
				}
				else if (competitionState == CompetitionState::DealerWins) {
					tableView->DrawTable("\n\nDealer wins!");
				}
				return;
			}
		}
	}
}

CompetitionState TableController::GetCompetitionState() const
{
	int player_score = tableModel->GetPlayers()[0]->GetHandValue();
	int dealer_score = tableModel->GetDealer()->GetHandValue();

	switch (gameState) {
		case GameState::PreMatch:
			if (player_score == 21) {
				return CompetitionState::PlayerWins;
			}
			if (dealer_score == 21) {
				return CompetitionState::DealerWins;
			}
			break;
		case GameState::PlayerChoice:
			if (player_score > 21) {
				return CompetitionState::DealerWins;
			}
			break;
		case GameState::DealerChoice:
		case GameState::EndGame:
			if (dealer_score == 21 && player_score == 21) {
				return CompetitionState::Draw;
			}
			else if (dealer_score > player_score && dealer_score > 17 && dealer_score <= 21) {
				return CompetitionState::DealerWins;
			}
			else if (dealer_score < player_score && dealer_score > 17 && player_score <= 21) {
				return CompetitionState::PlayerWins;
			}
			else if (player_score > dealer_score && player_score > 21) {
				return CompetitionState::DealerWins;
			}
			else if (dealer_score > player_score && dealer_score > 21) {
				return CompetitionState::PlayerWins;
			}
			else if (dealer_score == player_score && dealer_score > 17) {
				return CompetitionState::Draw;
			}
			break;
			
	}
	return CompetitionState::InGame;
	
}

void TableController::StartGame()
{
	GiveCardToPlayer(tableModel->GetDealer(), tableModel->GetDealer()->GetDeck());
	GiveCardToPlayer(tableModel->GetDealer(), tableModel->GetDealer()->GetDeck());
	GiveCardToPlayer(tableModel->GetPlayers()[0], tableModel->GetDealer()->GetDeck());
	GiveCardToPlayer(tableModel->GetPlayers()[0], tableModel->GetDealer()->GetDeck());
	GameLoop();
}