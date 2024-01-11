#include <algorithm>
#include <bitset>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "raylib.h"

#define GRID_SIZE 10

#define ruleSetNo 200
struct coords {
    int x;
    int y;
};
struct celluwu {
    coords cellCoords;
    Rectangle cell = {cellCoords.x, cellCoords.y, GRID_SIZE, GRID_SIZE};
    Color color;
    int keyVal;
};

celluwu cellStateMod(const celluwu cell, const celluwu cell1, const celluwu cell2, std::vector<std::pair<int, int>> &ruleset) {
    celluwu newCell;
    int checkerKey = cell.keyVal * 4 + cell1.keyVal * 2 + cell2.keyVal;
    for (size_t i = 0; i < 7; i++) {
        if (checkerKey == ruleset[i].second) {
            newCell.keyVal = ruleset[i].first;
        }
    }
    newCell.cellCoords.x = cell.cellCoords.x;

    newCell.cellCoords.y = cell.cellCoords.y + GRID_SIZE;

    newCell.color = (newCell.keyVal == 1 ? BLUE : RED);
    return newCell;
}

int main(void) {
    // Initialization
    srand(static_cast<unsigned int>(time(0)));
    std::vector<std::pair<int, int>> ruleSet;
    if (ruleSetNo >= 0 && ruleSetNo <= 255) {
        std::bitset<8> binaryRepresentation(ruleSetNo);
        for (int i = 7; i >= 0; --i) {
            ruleSet.push_back({binaryRepresentation[i], i});
        };
    };

    std::vector<celluwu> initialCellArray;
    std::vector<celluwu> newinitialCellArray;
    std::vector<std::vector<celluwu>> screenCells;
    InitWindow(1000, 600, "window");
    for (int x = 0; x < GetScreenWidth(); x += GRID_SIZE) {
        celluwu rCell;
        rCell.cellCoords = {x, 0};
        rCell.keyVal = rand() % 2;
        rCell.color = (rCell.keyVal == 1 ? BLUE : RED);
        initialCellArray.push_back(rCell);
    }
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        for (const auto &rowarray : screenCells) {
            for (const auto &i : rowarray) {
                DrawRectangleRec(i.cell, i.color);
            }
        }
        ClearBackground(BLACK);
        EndDrawing();
        newinitialCellArray.clear();
        for (size_t i = 0; i < initialCellArray.size(); i++) {
            size_t a = i, b = (i + 1) % initialCellArray.size(), c = (i + 2) % initialCellArray.size();

            newinitialCellArray.push_back(cellStateMod(initialCellArray[a], initialCellArray[b], initialCellArray[c], ruleSet));
        }
        initialCellArray.clear();
        initialCellArray = std::move(newinitialCellArray);

        if (screenCells.size() > GetScreenHeight() / GRID_SIZE) {
            screenCells.erase(screenCells.begin());
            std::rotate(screenCells.begin(), screenCells.begin() + 1, screenCells.end());
            for (size_t i = 0; i < screenCells.front().size(); i++) {
                screenCells.front()[i].cellCoords.y = screenCells.back()[i].cellCoords.y - GRID_SIZE;
            }
        } else {
            screenCells.push_back(initialCellArray);
        }
    }

    CloseWindow();

    return 0;
}
