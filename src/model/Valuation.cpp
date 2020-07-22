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

void Valuation::mergeDaySeq(const std::vector<std::pair<int, int>> cons, const int day, const int service, const int agent, const ConstraintDaysSeq* constraint,const int iCons)
{
	bool found = false;
	int i = 0;
	//Update la valuation
	mutexDaySeq.lock();
	for (auto value : daySeq[service][agent][iCons]) {
		//Si la contrainte est dans l'intervalle, on vérifie qu'elle est toujours active
		if (value.first >= day - (int)constraint->getSequenceAtt().size() && value.second <= day + (int)constraint->getSequenceAtt().size()) {
			found = false;
			for (auto e : cons) {
				if (value.first == e.first && value.second == e.second) {
					found = true;
				}
			}
			//Si on le trouve pas, c'est qu'on a résolu la contrainte
			if (!found) {
				daySeq[service][agent][iCons].erase(daySeq[service][agent][iCons].begin() + i);
				i--;
				mutexScore.lock();
				score += constraint->getPriority();
				mutexScore.unlock();
			}
		}

		i++;
	}
	//Ajout des nouveaux éléments
	for (auto e : cons) {
		bool isIn = false;
		for (auto value : daySeq[service][agent][iCons]) {
			if (value.first == e.first && value.second == e.second) {
				isIn = true;
			}
		}

		if (!isIn) {
			daySeq[service][agent][iCons].push_back(e);
			mutexScore.lock();
			score -= constraint->getPriority();
			mutexScore.unlock();
		}
	}
	mutexDaySeq.unlock();
}

void Valuation::mergeInvolved(const std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> cons, const int day, const int service, const int agent, const ConstraintInvolved* constraint, const int iCons)
{
	bool found;
	//Update la valuation
	int i = 0;
	mutexInvolved.lock();
	for (auto value : involved[service][agent][iCons]) {
		//Si la contrainte est dans l'intervalle, on vérifie qu'elle est toujours active
		if (value.first.first >= day - (int)constraint->getFirstSeqAtt().size() - (int)constraint->getLastSeqAtt().size() + 1 && value.second.second <= day + (int)constraint->getFirstSeqAtt().size() + (int)constraint->getLastSeqAtt().size() - 1) {
			found = false;
			for (auto e : cons) {
				if (value.first.first == e.first.first && value.second.second == e.second.second &&
					value.first.second == e.first.second && value.second.first == e.second.first) {
					found = true;
				}
			}
			//Si on ne la trouve pas, c'est qu'on a résolu la contrainte
			if (!found) {
				involved[service][agent][iCons].erase(involved[service][agent][iCons].begin() + i);
				i--;
				mutexScore.lock();
				score += constraint->getPriority();
				mutexScore.unlock();
			}
		}
		i++;
	}
	//Ajout des nouveaux éléments
	for (auto e : cons) {
		bool isIn = false;
		for (auto value : involved[service][agent][iCons]) {
			if (value.first.first == e.first.first && value.second.second == e.second.second &&
				value.first.second == e.first.second && value.second.first == e.second.first) {
				isIn = true;
			}
		}

		if (!isIn) {
			involved[service][agent][iCons].push_back(e);
			mutexScore.lock();
			score -= constraint->getPriority();
			mutexScore.unlock();
		}
	}
	mutexInvolved.unlock();
}

void Valuation::mergeSeqMinMax(const std::vector<std::pair<int, int>> cons, const int day, const int service, const int agent, const ConstraintSeqMinMax* constraint, const int iCons)
{
	bool found;
	mutexseqMinMax.lock();
	int nbSeq = seqMinMax[service][agent][iCons].size();
	//Update la valuation
	int i = 0;
	for (auto value : seqMinMax[service][agent][iCons]) {
		//Si la contrainte est dans l'intervalle, on vérifie qu'elle est toujours active
		if (value.first >= day - (int)constraint->getSequenceAtt().size() && value.second <= day + (int)constraint->getSequenceAtt().size()) {
			found = false;
			for (auto e : cons) {
				if (value.first == e.first && value.second == e.second) {
					found = true;
				}
			}
			if (!found) {
				seqMinMax[service][agent][iCons].erase(seqMinMax[service][agent][iCons].begin() + i);
				i--;
			}
		}
		i++;
	}
	//Ajout des nouveaux éléments
	for (auto e : cons) {
		bool isIn = false;
		for (auto value : seqMinMax[service][agent][iCons]) {
			if (value.first == e.first && value.second == e.second) {
				isIn = true;
			}
		}

		if (!isIn) {
			seqMinMax[service][agent][iCons].push_back(e);
		}
	}

	//Ajuste le score
	if (constraint->getType() == Min) {

		int scoreA = 0;
		int scoreB = 0;
		if (nbSeq < constraint->getNumber())
			scoreA -= (constraint->getNumber() - nbSeq) * constraint->getPriority();
		if ((int)seqMinMax[service][agent][iCons].size() < constraint->getNumber())
			scoreB -= (constraint->getNumber() - seqMinMax[service][agent][iCons].size()) * constraint->getPriority();
		if (scoreA - scoreB != 0) {
			mutexScore.lock();
			score -= (scoreA - scoreB);
			mutexScore.unlock();
		}
	}
	else {
		int scoreA = 0;
		int scoreB = 0;
		if (nbSeq > constraint->getNumber())
			scoreA -= (nbSeq - constraint->getNumber()) * constraint->getPriority();
		if ((int)seqMinMax[service][agent][iCons].size() > constraint->getNumber())
			scoreB -= (seqMinMax[service][agent][iCons].size() - constraint->getNumber()) * constraint->getPriority();
		mutexScore.lock();
		score -= (scoreA - scoreB);
		mutexScore.unlock();
	}
	mutexseqMinMax.unlock();
}

void Valuation::mergeHoursWeekSlide(const std::vector<std::pair<int, int>> cons, const int day, const int service, const int agent)
{
	//Update la valuation
	bool found = false;
	int i = 0;
	mutexHoursWeekSlide.lock();
	for (auto value : hoursWeekSlide[service][agent]) {
		//Si la contrainte est dans l'intervalle, on vérifie qu'elle est toujours active
		if (value.first >= day - 6 && value.second <= day + 7) {
			found = false;
			for (auto e : cons) {
				if (value.first == e.first && value.second == e.second) {
					found = true;
				}
			}
			//Si on ne la trouve pas, c'est qu'on a résolu la contrainte
			if (!found) {
				hoursWeekSlide[service][agent].erase(hoursWeekSlide[service][agent].begin() + i);
				i--;
				mutexScore.lock();
				score += 1;
				mutexScore.unlock();
			}
		}
		i++;
	}
	//Ajout des nouveaux éléments
	for (auto e : cons) {
		bool isIn = false;
		for (auto value : hoursWeekSlide[service][agent]) {
			if (value.first == e.first && value.second == e.second) {
				isIn = true;
			}
		}

		if (!isIn) {
			hoursWeekSlide[service][agent].push_back(e);
			mutexScore.lock();
			score -= 1;
			mutexScore.unlock();
		}
	}
	mutexHoursWeekSlide.unlock();
}

void Valuation::mergeImpossiblePosts(const bool fail, const int day, const int service, const int agent)
{
	bool found = false;
	int i = 0;
	mutexImpossiblePosts.lock();
	for (auto e : impossiblePosts[service][agent]) {
		if (e == day) {
			found = true;
			if (!fail) {
				impossiblePosts[service][agent].erase(impossiblePosts[service][agent].begin() + i);
				i--;
				mutexScore.lock();
				score += 10;
				mutexScore.unlock();
				break;
			}
		}
		i++;
	}
	if (!found && fail) {
		impossiblePosts[service][agent].push_back(day);
		score -= 10;
	}
	mutexImpossiblePosts.unlock();
}

void Valuation::mergeHoursMonth(const float dif, const int service, const int agent, const float nbHoursMonth)
{
	mutexHoursMonth.lock();
	if (hoursMonth[service][agent] > nbHoursMonth &&
		hoursMonth[service][agent] + dif <= nbHoursMonth) {

		mutexScore.lock();
		score += 100;
		mutexScore.unlock();
	}
	else if (hoursMonth[service][agent] <=nbHoursMonth &&
		hoursMonth[service][agent] + dif > nbHoursMonth) {

		mutexScore.lock();
		score -= 100;
		mutexScore.unlock();
	}

	hoursMonth[service][agent] += dif;
	mutexHoursMonth.unlock();
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
