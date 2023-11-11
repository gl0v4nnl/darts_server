#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include "../sdk.h"

#include "round.h"

namespace darts {

class Game
{
    int id;
    std::string date;
    std::string time;
    int target;
    std::vector<int> players_id;
    std::vector<Round> rounds;
    int winner_id = 0;
public:
    Game(int id, std::string date, std::string time, int target) : 
        id(id), date(date), time(time), target(target), players_id() {};
    
    int GetId() const {
        return id;
    }

    std::string GetDate() const {
        return date;
    }

    std::string GetTime() const {
        return time;
    }

    int GetTarget() const {
        return target;
    }

    std::vector<int> GetPlayersId() const {
        return players_id;
    }

    int GetWinnerId() const {
        return winner_id;
    }

    void SetWinnerId(int winner_id) {
        this->winner_id = winner_id;
    }

    void AddPlayerId(int player_id) {
        this->players_id.push_back(player_id);
    }

    bool RemovePlayerId(int player_id) {
        bool ret = false;
        auto it = std::find(players_id.begin(), players_id.end(), player_id);

        if (it != players_id.end()) {
            players_id.erase(it);
            ret = true;
        }

        return ret;
    }

    bool ClearPlayersId() {
        bool ret = false;

        if (!players_id.empty()) {
            players_id.clear();
            ret = true;
        }
        
        return ret;
    }

    void ChangeDate(std::string date) {
        this->date = date;
    }

    void ChangeTime(std::string time) {
        this->time = time;
    }

    void ChangeTarget(int target) {
        this->target = target;
    }

    int Play(); // main logic of the game
};

}