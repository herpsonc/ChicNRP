#include "Valuation.h"

Valuation::Valuation()
{
	score = 0;
}

Valuation::Valuation(const Valuation& obj)
{
	this->score = obj.score;
	this->hoursMonth = obj.hoursMonth;
	this->hoursWeeks = obj.hoursWeeks;
	this->hoursWeekSlide = obj.hoursWeekSlide;
	this->impossiblePosts = obj.impossiblePosts;
	this->daySeq = obj.daySeq;
	this->involved = obj.involved;
	this->seqMinMax = obj.seqMinMax;
}

Valuation::~Valuation()
{
}

Valuation& Valuation::operator=(const Valuation& obj)
{
	if (this != NULL) {
		score = obj.score;
		hoursMonth = obj.hoursMonth;
		hoursWeeks = obj.hoursWeeks;
		hoursWeekSlide = obj.hoursWeekSlide;
		impossiblePosts = obj.impossiblePosts;
		daySeq = obj.daySeq;
		involved = obj.involved;
		seqMinMax = obj.seqMinMax;
	}
	return Valuation(obj);

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

const std::vector<std::vector<std::vector<std::vector<std::pair<int, int>>>>>& Valuation::getDaySeq()
{
	return daySeq;
}

void Valuation::setDaySeq(const std::vector<std::vector<std::vector<std::vector<std::pair<int, int>>>>> daySeq)
{
	this->daySeq = daySeq;
}

const std::vector<std::vector<std::vector<std::pair<int, int>>>>& Valuation::gethoursWeekSlide()
{
	return hoursWeekSlide;
}

void Valuation::sethoursWeekSlide(const std::vector<std::vector<std::vector<std::pair<int, int>>>> hoursWeekSlide)
{
	this->hoursWeekSlide = hoursWeekSlide;
}

const std::vector<std::vector<std::vector<int>>>& Valuation::getImpossiblePosts()
{
	return impossiblePosts;
}

void Valuation::setImpossiblePosts(const std::vector<std::vector<std::vector<int>>> impossiblePosts)
{
	this->impossiblePosts = impossiblePosts;
}

const std::vector<std::vector<std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>>>>& Valuation::getInvolved()
{
	return involved;
}

void Valuation::setInvolved(const std::vector<std::vector<std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>>>> involved)
{
	this->involved = involved;
}

const std::vector<std::vector<std::vector<std::vector<std::pair<int, int>>>>>& Valuation::getseqMinMax()
{
	return seqMinMax;
}

void Valuation::setSeqMinMax(const std::vector<std::vector<std::vector<std::vector<std::pair<int, int>>>>> seqMinMax)
{
	this->seqMinMax = seqMinMax;
}

void Valuation::print()
{

	for (int i = 0; i < daySeq.size(); i++) {
		for (int j = 0; j < daySeq[i].size(); j++) {
			for (int k = 0; k < daySeq[i][j].size(); k++) {
				for (int l = 0; l < daySeq[i][j][k].size(); l++) {
					std::cout << "ConstraintDaySeq: Service " << i << " Agent " << j << " constraint " << k << " Jours "
						<< daySeq[i][j][k][l].first << " " << daySeq[i][j][k][l].second << std::endl;
				}
			}
		}
	}

	for (int i = 0; i < involved.size(); i++) {
		for (int j = 0; j < involved[i].size(); j++) {
			for (int k = 0; k < involved[i][j].size(); k++) {
				for (int l = 0; l < involved[i][j][k].size(); l++) {
					std::cout << "ConstraintInvolved: Service " << i << " Agent " << j << " constraint " << k << " Jours "
						<< involved[i][j][k][l].first.first << "-" << involved[i][j][k][l].first.second
						<< " "<< involved[i][j][k][l].second.first << "-" << involved[i][j][k][l].second.second << std::endl;
				}
			}
		}
	}

	for (int i = 0; i < seqMinMax.size(); i++) {
		for (int j = 0; j < seqMinMax[i].size(); j++) {
			for (int k = 0; k < seqMinMax[i][j].size(); k++) {
				for (int l = 0; l < seqMinMax[i][j][k].size(); l++) {
					std::cout << "ConstraintseqMinMax: Service " << i << " Agent " << j << " constraint " << k << " Jours "
						<< seqMinMax[i][j][k][l].first << " " << seqMinMax[i][j][k][l].second << std::endl;
				}
			}
		}
	}
}
