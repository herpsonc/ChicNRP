/*
 * Post.h
 *
 *  Created on: 8 févr. 2020
 *      Author: Valars
 */


#include <string>

using namespace std;

#ifndef MODEL_POST_H_
#define MODEL_POST_H_

class Post {
public:
	Post();
	virtual ~Post();
	const string& getId() const;
	void setId(const string &id);
	int getTime() const;
	void setTime(int time);

protected:
	string id;
	int time;
};

#endif /* MODEL_POST_H_ */
