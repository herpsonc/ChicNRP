/*
 * Post.h
 *
 *  Created on: 8 févr. 2020
 *      Author: Valars
 */


#include <string>
#include <vector>
#include "constraint\Constraint.h"

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

	const vector<Constraint*>& getConstraints() const;
	void setConstraints(const vector<Constraint*> &constraints);
	void addConstraint(Constraint* constraint);

	const vector<string>& getAttributs() const;
	void setAttributs(const vector<string> &attributs);
	void addAttribut(string attribut);

protected:
	string id;
	float time;

	vector<string> attributs;
	vector<Constraint*> constraints;
};

#endif /* MODEL_POST_H_ */
