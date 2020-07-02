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
	// if (this != NULL) {
	if(this){
		score = obj.score;
		hoursMonth = obj.hoursMonth;
		hoursWeeks = obj.hoursWeeks;
		hoursWeekSlide = obj.hoursWeekSlide;
		impossiblePosts = obj.impossiblePosts;
		daySeq = obj.daySeq;
		involved = obj.involved;
		seqMinMax = obj.seqMinMax;
	}
	return *this;

}

//! \return score score of the Valuation
const int& Valuation::getScore()
{
	return score;
}

//! \param score score to set
void Valuation::setScore(const int score)
{
	this->score = score;
}

//! \return hoursMonth vector of worked hours in the month for each Agent
const std::vector<std::vector<float>>& Valuation::getHoursMonth()
{
	return hoursMonth;
}

//! \param h hoursMonth to set
void Valuation::setHoursMonth(const std::vector<std::vector<float>> h)
{
	hoursMonth = h;
}

//! \return daySeq vector with informations about where the ConstraintDaysSeq is broken
const std::vector<std::vector<std::vector<std::vector<std::pair<int, int>>>>>& Valuation::getDaySeq()
{
	return daySeq;
}

//! \param daySeq daySeq to set
void Valuation::setDaySeq(const std::vector<std::vector<std::vector<std::vector<std::pair<int, int>>>>> daySeq)
{
	this->daySeq = daySeq;
}

//! \return hoursWeekSlide vector with the intervals of days where the constraint about the week hours is broken
const std::vector<std::vector<std::vector<std::pair<int, int>>>>& Valuation::gethoursWeekSlide()
{
	return hoursWeekSlide;
}

//! \param hoursWeekSlide hoursWeekSlide to set
void Valuation::sethoursWeekSlide(const std::vector<std::vector<std::vector<std::pair<int, int>>>> hoursWeekSlide)
{
	this->hoursWeekSlide = hoursWeekSlide;
}

//! \return vector of which day the constraint about impossible posts is broken
const std::vector<std::vector<std::vector<int>>>& Valuation::getImpossiblePosts()
{
	return impossiblePosts;
}

//! \param impossiblePosts impossiblePosts to set
void Valuation::setImpossiblePosts(const std::vector<std::vector<std::vector<int>>> impossiblePosts)
{
	this->impossiblePosts = impossiblePosts;
}

//! \return daySeq vector with informations about where the ConstraintInvolved is broken
const std::vector<std::vector<std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>>>>& Valuation::getInvolved()
{
	return involved;
}

//! \param involved involved to set
void Valuation::setInvolved(const std::vector<std::vector<std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>>>> involved)
{
	this->involved = involved;
}

//! \return daySeq vector with informations about where the ConstraintSeqMinMax is detected
const std::vector<std::vector<std::vector<std::vector<std::pair<int, int>>>>>& Valuation::getseqMinMax()
{
	return seqMinMax;
}

//! \param seqMinMax seqMinMax to set
void Valuation::setSeqMinMax(const std::vector<std::vector<std::vector<std::vector<std::pair<int, int>>>>> seqMinMax)
{
	this->seqMinMax = seqMinMax;
}

//! Print all the informations about each constraints in the Valuation
void Valuation::print()
{

	for (int i = 0; i < (int)daySeq.size(); i++) {
		for (int j = 0; j < (int)daySeq[i].size(); j++) {
			for (int k = 0; k < (int)daySeq[i][j].size(); k++) {
				for (int l = 0; l < (int)daySeq[i][j][k].size(); l++) {
					std::cout << "ConstraintDaySeq: Service " << i << " Agent " << j << " constraint " << k << " Jours "
						<< daySeq[i][j][k][l].first << " " << daySeq[i][j][k][l].second << std::endl;
				}
			}
		}
	}

	for (int i = 0; i < (int)involved.size(); i++) {
		for (int j = 0; j < (int)involved[i].size(); j++) {
			for (int k = 0; k < (int)involved[i][j].size(); k++) {
				for (int l = 0; l < (int)involved[i][j][k].size(); l++) {
					std::cout << "ConstraintInvolved: Service " << i << " Agent " << j << " constraint " << k << " Jours "
						<< involved[i][j][k][l].first.first << "-" << involved[i][j][k][l].first.second
						<< " "<< involved[i][j][k][l].second.first << "-" << involved[i][j][k][l].second.second << std::endl;
				}
			}
		}
	}

	for (int i = 0; i < (int)seqMinMax.size(); i++) {
		for (int j = 0; j < (int)seqMinMax[i].size(); j++) {
			for (int k = 0; k < (int)seqMinMax[i][j].size(); k++) {
				for (int l = 0; l < (int)seqMinMax[i][j][k].size(); l++) {
					std::cout << "ConstraintseqMinMax: Service " << i << " Agent " << j << " constraint " << k << " Jours "
						<< seqMinMax[i][j][k][l].first << " " << seqMinMax[i][j][k][l].second << std::endl;
				}
			}
		}
	}
}
