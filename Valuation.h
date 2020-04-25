#pragma once
#include <array>
#include <vector>

class Valuation
{
public:

	Valuation();
	virtual ~Valuation();

	const int& getScore();
	void setScore(const int score);

	const std::vector<std::vector<int>>& getHoursMonth();
	void setHoursMonth(const std::vector<std::vector<int>> h);

	const std::vector<std::vector<std::array<int, 6>>>& getHoursWeeks();
	void setHoursWeeks(const std::vector<std::vector<std::array<int, 6>>> hoursWeeks);

	const std::vector<std::vector<std::vector<std::pair<int, int>>>>& getDaySeq();
	void setDaySeq(const std::vector<std::vector<std::vector<std::pair<int, int>>>> daySeq);

	const std::vector<std::vector<std::vector<std::pair<int, int>>>>& gethoursWeekSlide();
	void sethoursWeekSlide(const std::vector<std::vector<std::vector<std::pair<int, int>>>> hoursWeekSlide);

	const std::vector<std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>>>& getInvolved();
	void setInvolved(const std::vector<std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>>> involved);

	const std::vector<std::vector<std::vector<std::pair<int, int>>>>& getseqMinMax();
	void setSeqMinMax(const std::vector<std::vector<std::vector<std::pair<int, int>>>> seqMinMax);

private:
	int score;

	std::vector<std::vector<int>> hoursMonth;
	std::vector<std::vector<std::array<int, 6>>> hoursWeeks;
	std::vector<std::vector<std::vector<std::pair<int, int>>>> hoursWeekSlide;

	std::vector<std::vector<std::vector<std::pair<int, int>>>> daySeq;

	std::vector<std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>>> involved;

	std::vector<std::vector<std::vector<std::pair<int, int>>>> seqMinMax;
};

