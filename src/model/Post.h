/*
 * Post.h
 *
 *  Created on: 8 févr. 2020
 *      Author: Valars
 */


#include <string>
#include <vector>

using namespace std;

#ifndef MODEL_POST_H_
#define MODEL_POST_H_

class Post {
public:
	Post();
	Post(string id, float time);
	virtual ~Post();

	const string& getId() const;
	void setId(const string &id);

	float getTime() const;
	void setTime(float time);

	const vector<int>& getAttributs() const;
	void setAttributs(const vector<int> &attributs);
	void addAttribut(int attribut);

protected:
	string id;
	float time;

	vector<int> attributs;
};

#endif /* MODEL_POST_H_ */
