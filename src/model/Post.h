#include <string>
#include <vector>
#include <iostream>

using namespace std;

#ifndef MODEL_POST_H_
#define MODEL_POST_H_

/*! \class Post
 * 	This class represent a post with all his characteristics
 */

class Post {
public:
	Post();
	Post(string id, float time, bool accounted=true);
	virtual ~Post();

	const string& getId() const;
	void setId(const string &id);

	float getTime() const;
	void setTime(float time);

	bool getTimeAccounted() const;
	void setTimeAccounted(const bool timeAccounted);

	const vector<int>& getAttributs() const;
	void setAttributs(const vector<int> &attributs);
	void addAttribut(int attribut);

protected:
	string id;
	float time;
	bool timeAccounted;

	vector<int> attributs;
};

#endif /* MODEL_POST_H_ */
