#pragma once
#include <array>
#include <vector>
#include <iostream>

class Valuation
{
public:

	Valuation();
	Valuation(const Valuation& obj);
	virtual ~Valuation();

	Valuation& operator = (const Valuation& obj);

	const int& getScore();
	void setScore(const int score);

	const std::vector<std::vector<int>>& getHoursMonth();
	void setHoursMonth(const std::vector<std::vector<int>> h);

	const std::vector<std::vector<std::array<int, 6>>>& getHoursWeeks();
	void setHoursWeeks(const std::vector<std::vector<std::array<int, 6>>> hoursWeeks);

	const std::vector<std::vector<std::vector<std::vector<std::pair<int, int>>>>>& getDaySeq();
	void setDaySeq(const std::vector<std::vector<std::vector<std::vector<std::pair<int, int>>>>> daySeq);

	const std::vector<std::vector<std::vector<std::pair<int, int>>>>& gethoursWeekSlide();
	void sethoursWeekSlide(const std::vector<std::vector<std::vector<std::pair<int, int>>>> daySeq);

	const std::vector<std::vector<std::vector<int>>>& getImpossiblePosts();
	void setImpossiblePosts(const std::vector<std::vector<std::vector<int>>> impossiblePosts);

	const std::vector<std::vector<std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>>>>& getInvolved();
	void setInvolved(const std::vector<std::vector<std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>>>> involved);

	const std::vector<std::vector<std::vector<std::vector<std::pair<int, int>>>>>& getseqMinMax();
	void setSeqMinMax(const std::vector<std::vector<std::vector<std::vector<std::pair<int, int>>>>> seqMinMax);

	void print();
private:
	int score;

	std::vector<std::vector<int>> hoursMonth;
	std::vector<std::vector<std::array<int, 6>>> hoursWeeks;
	std::vector<std::vector<std::vector<std::pair<int, int>>>> hoursWeekSlide;
	std::vector<std::vector<std::vector<int>>> impossiblePosts;

	std::vector<std::vector<std::vector<std::vector<std::pair<int, int>>>>> daySeq;

	std::vector<std::vector<std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>>>> involved;

	std::vector<std::vector<std::vector<std::vector<std::pair<int, int>>>>> seqMinMax;
};

