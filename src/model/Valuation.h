#pragma once
#include <array>
#include <vector>
#include <iostream>
#include "constraint/ConstraintDaysSeq.h"
#include "constraint/ConstraintInvolved.h"
#include "constraint/ConstraintSeqMinMax.h"

/*! \class Valuation
 *  This class is used to register data about violated constraints at every check in order to optimize the checker
 */

class Valuation
{
public:

	Valuation();
	Valuation(const Valuation& obj);
	virtual ~Valuation();

	Valuation& operator = (const Valuation& obj);

	const float& getScore();
	void setScore(const float score);

	const std::vector<std::vector<float>>& getHoursMonth();
	void setHoursMonth(const std::vector<std::vector<float>> h);

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

	const std::vector<std::array<int, 31>> getPostsRequirement();
	void setPostsRequirement(std::vector<std::array<int, 31>> postsRequirement);

	void mergeDaySeq(const std::vector<std::pair<int, int>> cons, const int day,
		const int service, const int agent, const ConstraintDaysSeq* constraint, const int iCons);
	void mergeInvolved(const std::vector <std::pair<std::pair<int, int>, std::pair<int, int>>> cons, const int day,
		const int service, const int agent, const ConstraintInvolved * constraint, const int iCons);
	void mergeSeqMinMax(const std::vector<std::pair<int, int>> cons, const int day,
		const int service, const int agent, const ConstraintSeqMinMax* constraint, const int iCons);
	void mergeHoursWeekSlide(const std::vector<std::pair<int, int>> cons, const int day,
		const int service, const int agent, const int priority);
	void mergeImpossiblePosts(const bool fail, const int day, const int service, const int agent, const int priority);
	void mergeHoursMonth(const float dif, const int service, const int agent, const float nbHoursMonth, const int priority);
	void mergePostRequirement(const int service, const int day, const int nbFail, const int priority);

	void print();
private:
	float score;

	std::vector<std::vector<float>> hoursMonth;
	std::vector<std::vector<std::array<int, 6>>> hoursWeeks;
	std::vector<std::vector<std::vector<std::pair<int, int>>>> hoursWeekSlide;
	std::vector<std::vector<std::vector<int>>> impossiblePosts;
	std::vector<std::vector<std::vector<std::vector<std::pair<int, int>>>>> daySeq;
	std::vector<std::vector<std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>>>> involved;
	std::vector<std::vector<std::vector<std::vector<std::pair<int, int>>>>> seqMinMax;
	std::vector<std::array<int, 31>> postsRequirement;
};

