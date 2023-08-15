#include "LevelGrid.h"
#include <iostream>
#include <glm/glm.hpp>

void LevelGrid::AddCell(const glm::vec2& topLeftPos, CellKind cellKind, int rowNr, int collNr)
{
	assert(collNr < m_MaxAmountOfCollumns && "the grid can only have 9 collumns");

	//calculate the middle
	glm::vec2 middlePos{ topLeftPos.x + m_CellSideLenght / 2.f, topLeftPos.y + m_CellSideLenght / 2.f };

	if (cellKind == CellKind::longEmpty || cellKind == CellKind::longFloor || cellKind == CellKind::longGoDown
		|| cellKind == CellKind::longGoUp || cellKind == CellKind::longGoUpAndDown || cellKind == CellKind::plate)
		middlePos.x += m_CellSideLenght / 2.f;

	m_Cells.push_back(std::make_unique<Cell>(middlePos, cellKind, rowNr, collNr));

	RecalculateGrid();
}

void LevelGrid::RecalculateGrid()
{
	for (auto& pCell : m_Cells)
	{
		for (auto& pOtherCell : m_Cells)
		{
			//make sure the same cell doesn't check himself
			if (pCell == pOtherCell)
				continue;

			float spaceBetweenCellCentersX{ m_CellSideLenght };

			if (pCell->cellKind == CellKind::longEmpty || pCell->cellKind == CellKind::longFloor || pCell->cellKind == CellKind::longGoDown || pCell->cellKind == CellKind::longGoUp || pCell->cellKind == CellKind::longGoUpAndDown || pCell->cellKind == CellKind::plate
				|| pOtherCell->cellKind == CellKind::longEmpty || pOtherCell->cellKind == CellKind::longFloor || pOtherCell->cellKind == CellKind::longGoDown || pOtherCell->cellKind == CellKind::longGoUp || pOtherCell->cellKind == CellKind::longGoUpAndDown || pOtherCell->cellKind == CellKind::plate)
			{
				spaceBetweenCellCentersX = 1.5f * m_CellSideLenght;
			}

			//check if pOtherCell is a top of pCell
			if (CheckForTopNeighbor(pCell.get(), pOtherCell.get()))
			{
				pCell->pTopNeighbor = pOtherCell.get();
				pOtherCell->pBottomNeighbor = pCell.get();
				continue;
			}

			//check if pOtherCell is a rightNeighbor of pCell
			if (CheckForRightNeighbor(pCell.get(), pOtherCell.get(), spaceBetweenCellCentersX))
			{
				pCell->pRightNeighbor = pOtherCell.get();
				pOtherCell->pLeftNeighbor = pCell.get();
			}
		}
	}
}

Cell* LevelGrid::GetCell(const glm::vec2& pos) const
{
	//loop over all the cells and check in which one the pos is in
	for (const auto& cell : m_Cells)
	{
		if (pos.y <= cell->middlePos.y + m_CellSideLenght / 2.f
			&& pos.y >= cell->middlePos.y - m_CellSideLenght / 2.f)
		{
			if (cell->cellKind == CellKind::longEmpty || cell->cellKind == CellKind::longFloor || cell->cellKind == CellKind::longGoDown || cell->cellKind == CellKind::longGoUp || cell->cellKind == CellKind::longGoUpAndDown || cell->cellKind == CellKind::plate)
			{
				if (pos.x <= cell->middlePos.x + m_CellSideLenght
					&& pos.x >= cell->middlePos.x - m_CellSideLenght)
					return cell.get();
			}
			else if (pos.x <= cell->middlePos.x + m_CellSideLenght / 2.f
					&& pos.x >= cell->middlePos.x - m_CellSideLenght / 2.f)
				return cell.get();
		}
	}

	return nullptr;
}

Cell* LevelGrid::GetCell(int colNr, int rowNr)
{
	const int cellIndex{ rowNr * m_MaxAmountOfCollumns + colNr };

	if (cellIndex > static_cast<int>(m_Cells.size()))
		return nullptr;

	return m_Cells[cellIndex].get();
}

Cell* LevelGrid::GetNearestCellOfKind(const glm::vec2& pos, CellKind cellKind)
{
	auto givenCell{ GetCell(pos) };

	if (givenCell == nullptr)
		return nullptr;

	Cell* currentClosestCell{};
	float distanceBetweenGivenAndCurrentClosestCell{ FLT_MAX };

	//loop over all the cells
	for (const auto& cell : m_Cells)
	{
		//if the current cell is the kind of cell that is being searched for calculate the distance between the givenCell and the current cell
		if(cell->cellKind == cellKind)
		{
			const float distanceBetweenCells{ glm::length(cell->middlePos - givenCell->middlePos) };

			//if this cell is closer set the currentClosestCell to it and set the distanceBetweenGivenAndCurrentClosestCell
			if (distanceBetweenCells < distanceBetweenGivenAndCurrentClosestCell)
			{
				currentClosestCell = cell.get();
				distanceBetweenGivenAndCurrentClosestCell = distanceBetweenCells;
			}
		}
	}

	return currentClosestCell;
}

const std::vector<Cell*> LevelGrid::GetCells() const
{
	std::vector<Cell*> cells{};

	for (const auto& cell : m_Cells)
		cells.push_back(cell.get());

	return cells;
}

bool LevelGrid::CheckForTopNeighbor(Cell* pCell, Cell* pOtherCell)
{
	if (abs(pCell->middlePos.x - pOtherCell->middlePos.x) < m_Epsilon
		&& abs(pCell->middlePos.y - pOtherCell->middlePos.y - m_CellSideLenght) < m_Epsilon)
	{
		return true;
	}

	return false;
}

bool LevelGrid::CheckForRightNeighbor(Cell* pCell, Cell* pOtherCell, float distanceBetweenCellCentersX)
{
	if (abs(pCell->middlePos.x - pOtherCell->middlePos.x + distanceBetweenCellCentersX) < m_Epsilon
		&& abs(pCell->middlePos.y - pOtherCell->middlePos.y) < m_Epsilon)
	{
		return true;
	}

	return false;
}

void LevelGrid::Reset()
{
	m_Cells.clear();
}

int LevelGrid::GetAmountOfRows() const
{
	return static_cast<int>(ceil(static_cast<float>(m_Cells.size()) / m_MaxAmountOfCollumns));
}