#include "Valuation.h"

Valuation::Valuation()
{
}

Valuation::~Valuation()
{
}

const int& Valuation::getScore()
{
	return score;
}

void Valuation::setScore(const int score)
{
	this->score = score;
}

const std::vector<std::vector<int>>& Valuation::getHoursMonth()
{
	return hoursMonth;
}

void Valuation::setHoursMonth(const std::vector<std::vector<int>> h)
{
	hoursMonth = h;
}

const std::vector<std::vector<std::array<int, 6>>>& Valuation::getHoursWeeks()
{
	return hoursWeeks;
}

void Valuation::setHoursWeeks(const std::vector<std::vector<std::array<int, 6>>> hoursWeeks)
{
	this->hoursWeeks = hoursWeeks;
}

const std::vector<std::vector<std::vector<std::pair<int, int>>>>& Valuation::getDaySeq()
{
	return daySeq;
}

void Valuation::setDaySeq(const std::vector<std::vector<std::vector<std::pair<int, int>>>> daySeq)
{
	this->daySeq = daySeq;
}

const std::vector<std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>>>& Valuation::getInvolved()
{
	return involved;
}

void Valuation::setInvolved(const std::vector<std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>>> involved)
{
	this->involved = involved;
}

const std::vector<std::vector<std::vector<std::pair<int, int>>>>& Valuation::getseqMinMax()
{
	return seqMinMax;
}

void Valuation::setSeqMinMax(const std::vector<std::vector<std::vector<std::pair<int, int>>>> seqMinMax)
{
	this->seqMinMax = seqMinMax;
}
